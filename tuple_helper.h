#pragma once

#include <iostream>
#include <tuple>

template <typename T>
auto explode(T&& t, char)
{
    return std::forward_as_tuple(std::forward<T>(t));
}

template <typename T, std::size_t I = std::tuple_size<std::decay_t<T>>{}>
auto explode(T&& t, int);

template <typename T, std::size_t... Is>
auto explode(T&& t, std::index_sequence<Is...>)
{
    return std::tuple_cat(explode(std::get<Is>(std::forward<T>(t)), 0)...);
}

template <typename T, std::size_t I>
auto explode(T&& t, int)
{
    return explode(std::forward<T>(t), std::make_index_sequence<I>{});
}

template <typename T, std::size_t... Is>
auto decay_tuple(T&& t, std::index_sequence<Is...>)
{
    return std::make_tuple(std::get<Is>(std::forward<T>(t))...);
}

template <typename T>
auto decay_tuple(T&& t)
{
    return decay_tuple(std::forward<T>(t), std::make_index_sequence<std::tuple_size<std::decay_t<T>>{}>{});
}

template <typename T, std::size_t... Is>
auto merge_tuple(T&& t, std::index_sequence<Is...>)
{
    return decay_tuple(std::tuple_cat(explode(std::get<Is>(std::forward<T>(t)), 0)...));
}

template <typename T>
auto merge_tuple(T&& t)
{
    return merge_tuple(std::forward<T>(t), std::make_index_sequence<std::tuple_size<std::decay_t<T>>{}>{});
}
