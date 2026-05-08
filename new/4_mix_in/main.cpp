#include <atomic>
#include <cassert>
#include <iostream>

template <class T> class less_then_comparable {
public:
  virtual bool operator<(T const &other) const = 0;
  bool operator==(const T &other) const {
    return !(*this < other) && !(other < static_cast<const T &>(*this));
  }
  bool operator!=(T const &other) const {
    return (*this < other) || (other < *this);
  }
  bool operator>(T const &other) const {
    return (other < static_cast<const T &>(*this));
  }
  bool operator>=(T const &other) const {
    return (other < static_cast<const T &>(*this)) ||
           (other == static_cast<const T &>(*this));
  }
  bool operator<=(T const &other) const {
    return (*this < other) || (other == static_cast<const T &>(*this));
  }
};

template <class T> class counter {
private:
  inline static std::atomic<size_t> count_ = 0;

public:
  counter() { count_++; }
  ~counter() { count_--; }

  static auto count() { return (size_t)count_; }
};

class Number : public less_then_comparable<Number>, public counter<Number> {
public:
  Number(int value) : m_value{value} {}
  int value() const { return m_value; }
  bool operator<(Number const &other) const { return m_value < other.m_value; }

private:
  int m_value;
};

int main(int argc, char *argv[]) {
  Number one{1};
  Number two{2};
  Number three{3};
  Number four{4};
  assert(one >= one);
  assert(three <= four);
  assert(two == two);
  assert(three > two);
  assert(one < two);
  std::cout << "Count: " << counter<Number>::count() << std::endl;
  return 0;
}
