#include <iostream>
#include <cstdint>
#include <stdexcept>
#include <vector>

struct Token
{
    char kind;
    uint64_t value;
    std::string name;

  public:
    Token(char ch)
      : kind{ ch }
      , value{ 0 }
    {
    }

    Token(char ch, uint64_t val)
      : kind{ ch }
      , value{ val }
    {
    }

    Token(char ch, std::string name_)
      : kind{ ch }
      , name{ name_ }
    {
    }
};

class Token_stream
{
    bool full{ false };
    Token buffer{ '\0' };

  public:
    Token_stream() {}

    Token get();
    void putback(Token t);

    void ignore(char);
};

void
Token_stream::putback(Token t)
{
    if (full)
        std::cerr << "putback() into a full buffer" << std::endl;

    buffer = t;
    full   = true;
}

constexpr char quit   = 'q';
constexpr char print  = ';';
constexpr char number = '8';
constexpr char name   = 'a';
constexpr char unar   = '~';
constexpr char let    = 'L';

const std::string prompt  = "> ";
const std::string result  = "= ";
const std::string declkey = "let";

Token
Token_stream::get()
{
    if (full) {
        full = false;
        return buffer;
    }

    char ch;
    std::cin >> ch;
    if (!std::cin) {
        return { ch, quit };
    }

    switch (ch) {
        case quit: return Token{ ch, quit };
        case unar: return Token{ ch, unar };
        case '(':
        case ')':
        case '>':
        case '<':
        case '&':
        case '|':
        case ';':
        case '=': return Token{ ch };

        case '0':
        case '1': {
            std::cin.putback(ch);
            uint64_t tmp, val = 0, k = 1;
            std::cin >> tmp;
            while (tmp) {
                auto tmp2 = tmp % 10;
                if (tmp2 > 1)
                    throw std::runtime_error("Bad token");

                val += tmp2 * k;
                tmp /= 10;
                k *= 2;
            }
            return Token{ number, val };
        }

        default:
            if (isalpha(ch)) {
                std::string s;
                s += ch;
                while (std::cin.get(ch) && (isalpha(ch) || isdigit(ch)))
                    s += ch;
                std::cin.putback(ch);

                if (s == declkey)
                    return Token{ let };

                return Token{ name, s };
            }
            throw std::runtime_error("Bad token");
    }
}

void
Token_stream::ignore(char c)
{
    if (full && c == buffer.kind) {
        full = false;
        return;
    }
    full = false;

    char ch;
    while (std::cin >> ch)
        if (ch == c)
            return;
}

struct Variable
{
    std::string name;
    uint64_t value;

    Variable(std::string n, uint64_t v)
      : name{ n }
      , value{ v }
    {
    }
};

std::vector<Variable> var_table;

uint64_t
get_value(std::string s)
{
    for (int i = 0; i < var_table.size(); ++i)
        if (var_table[i].name == s)
            return var_table[i].value;

    throw std::runtime_error("get: undefined name of var");
}

void
set_value(std::string s, uint64_t d)
{
    for (int i = 0; i <= var_table.size(); ++i) {
        if (var_table[i].name == s) {
            var_table[i].value = d;
            return;
        }
    }

    throw std::runtime_error("set: undefined name of var");
}

bool
is_declared(std::string s)
{
    for (int i = 0; i < var_table.size(); ++i)
        if (var_table[i].name == s)
            return true;

    return false;
}

uint64_t
define_name(std::string var, uint64_t val)
{
    if (is_declared(var))
        throw std::runtime_error("var declared twice");

    var_table.push_back(Variable{ var, val });

    return val;
}

Token_stream ts;

uint64_t
expression();

uint64_t
primary()
{
    Token t = ts.get();
    switch (t.kind) {
        case '(': {
            uint64_t d = expression();
            t          = ts.get();
            if (t.kind != ')')
                throw std::runtime_error("'(' expected");
            return d;
        }

        case unar: return ~primary();

        case number: return t.value;

        case name: return get_value(t.name);

        default: throw std::runtime_error("primary expected");
    }
}

uint64_t
term()
{
    uint64_t left = primary();
    char tmp;
    while (true) {
        Token t = ts.get();

        switch (t.kind) {
            case '&': left &= primary(); break;
            case '<':
                tmp = std::cin.get();
                if (tmp == '<')
                    left = left << term();
                else {
                    ts.putback({ (char)tmp });
                    throw std::runtime_error("< expected after \"<\"");
                }
                break;
            case '>':
                tmp = std::cin.get();
                if (tmp == '>')
                    left = term() >> left;
                else {
                    ts.putback({ (char)tmp });
                    throw std::runtime_error("> expected after \">\"");
                }
                break;

            default: ts.putback(t); return left;
        }
    }
}

uint64_t
expression()
{
    uint64_t left = term();

    while (true) {
        Token t = ts.get();
        if (t.value == ')') {
            ts.putback(t);
            return left;
        }

        switch (t.kind) {
            case '^': left ^= term(); break;
            case '|': left |= term(); break;

            default: ts.putback(t); return left;
        }
    }
}

uint64_t
declaration()
{
    Token t = ts.get();
    if (t.kind != name)
        throw std::runtime_error("name expected in declaration");

    std::string var = t.name;
    if (is_declared(var))
        throw std::runtime_error("var declared twice");

    t = ts.get();
    if (t.kind != '=')
        throw std::runtime_error("'=' missing in declaration of var");

    return define_name(var, expression());
}

uint64_t
statement()
{
    Token t = ts.get();
    switch (t.kind) {
        case let: return declaration();
        default: ts.putback(t); return expression();
    }
}

void
clean_up_mess()
{
    ts.ignore(print);
}

void
calculate()
{
    while (true)
        try {
            if (!std::cin) {
                return;
            }
            std::cout << prompt;
            Token t = ts.get();
            while (t.kind == print)
                t = ts.get();
            if (t.kind == quit)
                return;

            ts.putback(t);
            auto tmp = statement();
            std::cout << result;
            std::vector<bool> buff;
            while (tmp) {
                buff.push_back(tmp % 2);
                tmp /= 2;
            }
            if (!buff.size())
                std::cout << 0;
            for (int i = buff.size() - 1; i >= 0; i--)
                std::cout << (buff[i] ? 1 : 0);
            std::cout << std::endl;
        } catch (std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
            clean_up_mess();
        }
}

int
main()
try {
    calculate();
} catch (std::exception& e) {
    std::cerr << "exception: " << e.what() << std::endl;
    return 1;
} catch (...) {
    std::cerr << "Oops, unknown exception" << std::endl;
    return 2;
}
