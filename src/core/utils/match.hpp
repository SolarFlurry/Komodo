#pragma once

#include <type_traits>
#include <variant>

template<typename T, typename Head, typename... Tail>
constexpr int type_index(int index = 0) {
    if constexpr (std::is_same_v<T, Head>) return index;
    else
        return type_index<T, Tail...>(index + 1);
}

template<typename... Types>
struct type_sequence {
    constexpr type_sequence(const std::variant<Types...>&) {}
    template<typename T>
    constexpr static int idx = type_index<T, Types...>();
};
template<typename ...Types> inline size_t index_of(std::variant<Types...>& node) { return node.index(); }
template<typename  T, typename ...Types> auto& get_elem(std::variant<Types...>& node) { return std::get<T>(node); }

#define match(v)                                                                                    \
{                                                                                                   \
    auto& v__ = v;                                                                                  \
    bool was_default = false;                                                                       \
    switch (auto t_seq__ = type_sequence(v); index_of(v)) {                                   

#define holds(T, name)                                                                              \
break;}                                                                                             \
    case t_seq__.idx<std::remove_pointer<std ::remove_cvref<T>::type>::type>: {                     \
    T name = get_elem<std::remove_pointer<std ::remove_cvref<T>::type>::type>(v__); 

#define _                                                                                           \
        break;                                                                                      \
    }                                                                                               \
        default:                                                                                    \
            was_default = true;                                                                     \
}                                                                                                   \
    if (was_default)
