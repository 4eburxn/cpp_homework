#ifndef __LIST_H__
#define __LIST_H__

#include <cstddef>
#include <type_traits>

template <typename... Ts> struct TypeList {};

namespace LIST_details {
template <typename T> struct Size;
template <typename... Ts> struct Size<TypeList<Ts...>> {
  static constexpr std::size_t value = sizeof...(Ts);
};

template <std::size_t Index, typename T> struct At;
template <typename Head, typename... Tail>
struct At<0, TypeList<Head, Tail...>> {
  using type = Head;
};
template <std::size_t Index, typename Head, typename... Tail>
struct At<Index, TypeList<Head, Tail...>> {
  using type = typename At<Index - 1, TypeList<Tail...>>::type;
};

template <typename T, typename List> struct Contains;
template <typename T> struct Contains<T, TypeList<>> {
  static constexpr bool value = false;
};
template <typename T, typename Head, typename... Tail>
struct Contains<T, TypeList<Head, Tail...>> {
  static constexpr bool value =
      std::is_same_v<T, Head> || Contains<T, TypeList<Tail...>>::value;
};

template <typename T, typename List> struct IndexOf;
template <typename T, typename... Tail>
struct IndexOf<T, TypeList<T, Tail...>> {
  static constexpr std::size_t value = 0;
};
template <typename T, typename Head, typename... Tail>
struct IndexOf<T, TypeList<Head, Tail...>> {
  static constexpr std::size_t value = 1 + IndexOf<T, TypeList<Tail...>>::value;
};

template <typename T, typename List> struct PushFront;
template <typename T, typename... Ts> struct PushFront<T, TypeList<Ts...>> {
  using type = TypeList<T, Ts...>;
};

template <typename T, typename List> struct PushBack;
template <typename T, typename... Ts> struct PushBack<T, TypeList<Ts...>> {
  using type = TypeList<Ts..., T>;
};
} // namespace LIST_details

template <typename List>
inline constexpr std::size_t Length = LIST_details::Size<List>::value;

template <std::size_t Index, typename List>
using At_t = typename LIST_details::At<Index, List>::type;

template <typename T, typename List>
inline constexpr bool Contains_v = LIST_details::Contains<T, List>::value;

template <typename T, typename List>
inline constexpr std::size_t IndexOf_v = LIST_details::IndexOf<T, List>::value;

template <typename T, typename List>
using PushFront_t = typename LIST_details::PushFront<T, List>::type;

template <typename T, typename List>
using PushBack_t = typename LIST_details::PushBack<T, List>::type;

#endif // !__LIST_H__
