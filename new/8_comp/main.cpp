#include <iostream>
#include <limits>
#include <map>
#include <string>

class Expression {
public:
  int refCount = 0;
  virtual void print() const = 0;
  virtual double
  calculate(const std::map<std::string, double> &context) const = 0;
  virtual ~Expression() {}
};

class Constant : public Expression {
  double value;

public:
  Constant(double v) : value(v) {}
  void print() const override { std::cout << value; }
  double calculate(const std::map<std::string, double> &) const override {
    return value;
  }
};

class Variable : public Expression {
  std::string name;

public:
  Variable(std::string n) : name(n) {}
  void print() const override { std::cout << name; }
  double
  calculate(const std::map<std::string, double> &context) const override {
    return context.at(name);
  }
};

class BinaryOperation : public Expression {
protected:
  Expression *left, *right;

public:
  BinaryOperation(Expression *l, Expression *r) : left(l), right(r) {
    left->refCount++;
    right->refCount++;
  }
  virtual ~BinaryOperation() {
    if (--left->refCount <= 0)
      delete left;
    if (--right->refCount <= 0)
      delete right;
  }
};

class Addition : public BinaryOperation {
public:
  using BinaryOperation::BinaryOperation;
  void print() const override {
    std::cout << "(";
    left->print();
    std::cout << " + ";
    right->print();
    std::cout << ")";
  }
  double
  calculate(const std::map<std::string, double> &context) const override {
    return left->calculate(context) + right->calculate(context);
  }
};

class Multiplication : public BinaryOperation {
public:
  using BinaryOperation::BinaryOperation;
  void print() const override {
    std::cout << "(";
    left->print();
    std::cout << " * ";
    right->print();
    std::cout << ")";
  }
  double
  calculate(const std::map<std::string, double> &context) const override {
    return left->calculate(context) * right->calculate(context);
  }
};

class ExpressionFactory {
  std::map<double, Constant *> constants;
  std::map<std::string, Variable *> variables;

public:
  ExpressionFactory() {
    for (int i = -5; i <= 256; ++i) {
      constants[static_cast<double>(i)] = new Constant(i);
      constants[static_cast<double>(i)]->refCount =
          std::numeric_limits<int>::max();
    }
  }

  Constant *createConstant(double v) {
    if (constants.find(v) == constants.end()) {
      constants[v] = new Constant(v);
      constants[v]->refCount++;
    }
    return constants[v];
  }

  Variable *createVariable(const std::string &name) {
    if (variables.find(name) == variables.end()) {
      variables[name] = new Variable(name);
      variables[name]->refCount++;
    }
    return variables[name];
  }

  ~ExpressionFactory() {
    for (auto const &[val, ptr] : constants) {
      if (ptr->refCount >= 1000000)
        delete ptr;
    }
    for (auto const &[name, ptr] : variables)
      delete ptr;
  }
};

void run_test(const std::string &title, Expression *expr,
              std::map<std::string, double> &ctx) {
  std::cout << "--- Test: " << title << " ---\n";
  expr->print();
  std::cout << " = " << expr->calculate(ctx) << "\n\n";
}

int main() {
  ExpressionFactory factory;
  std::map<std::string, double> context = {{"x", 3.0}, {"y", 5.0}, {"z", 10.0}};

  // Нейротесты
  // Тест 1: Базовый (2 + x)
  Addition *t1 =
      new Addition(factory.createConstant(2), factory.createVariable("x"));
  run_test("Simple Addition", t1, context);

  // Тест 2: Использование Flyweight (x * x + x)
  // Переменная "x" — это один и тот же объект в памяти
  Variable *x = factory.createVariable("x");
  Addition *t2 = new Addition(new Multiplication(x, x), x);
  run_test("Reuse Variable (x*x + x)", t2, context);

  // Тест 3: Сложное выражение (x + y) * (z - 5)
  // Константа 5 берется из заранее созданного пула
  Addition *sum_xy =
      new Addition(factory.createVariable("x"), factory.createVariable("y"));
  // Для простоты используем Addition как суррогат вычитания (z + -5)
  Addition *diff_z5 =
      new Addition(factory.createVariable("z"), factory.createConstant(-5));
  Multiplication *t3 = new Multiplication(sum_xy, diff_z5);
  run_test("Complex Expression (x+y)*(z-5)", t3, context);

  // Очистка
  delete t1;
  delete t2;
  delete t3;

  return 0;
}
