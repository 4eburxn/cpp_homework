#include "list.h"
// --- ТЕСТЫ ---

using MyList = TypeList<int, double, char>;

// Тест Size
static_assert(Length<MyList> == 3);

// Тест At
static_assert(std::is_same_v<At_t<0, MyList>, int>);
static_assert(std::is_same_v<At_t<1, MyList>, double>);
// static_assert(std::is_same_v<At_t<5, MyList>, int>); // Ошибка компиляции!

// Тест Contains
static_assert(Contains_v<double, MyList> == true);
static_assert(Contains_v<float, MyList> == false);

// Тест IndexOf
static_assert(IndexOf_v<char, MyList> == 2);

// Тест PushFront/PushBack
using AfterFront = PushFront_t<float, MyList>; // float, int, double, char
static_assert(std::is_same_v<At_t<0, AfterFront>, float>);

using AfterBack = PushBack_t<long, MyList>; // int, double, char, long
static_assert(std::is_same_v<At_t<3, AfterBack>, long>);

int main(int argc, char *argv[]) { return 0; }
