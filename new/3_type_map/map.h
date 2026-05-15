#ifndef __MAP_H__
#define __MAP_H__

#include <tuple>

#include "list.h"

template <typename... Ts> struct TypeMap {
private:
  using internallist = TypeList<Ts...>;
  std::tuple<Ts...> vals;
  bool inicialized[Length<internallist>] = {false};

public:
  template <class T> void AddValue(T val) {
    std::get<IndexOf_v<T, internallist>>(vals) = val;
    inicialized[IndexOf_v<T, internallist>] = true;
  }

  template <class T> T GetValue() {
    return std::get<IndexOf_v<T, internallist>>(vals);
  }

  template <class T> bool Contains() {
    return Contains_v<T, internallist> &&
           inicialized[IndexOf_v<T, internallist>];
  }

  template <class T> bool RemoveValue() {
    if (!Contains_v<T, internallist> ||
        !inicialized[IndexOf_v<T, internallist>]) {
      return false;
    }

    inicialized[IndexOf_v<T, internallist>] = false;

    return true;
  }
};

#endif // !__MAP_H__
