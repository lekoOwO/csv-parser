#pragma once

#include <ranges>
#include <concepts>
#include <iterator>
#include <type_traits>
#include <string_view>
#include <utility>

namespace CSV {
    namespace ranges = std::ranges;
    namespace views = std::ranges::views;
    using std::string_view;

    template<typename T>
    concept ViewableString = std::convertible_to<T, string_view>;

    template<typename T, typename... U>
    concept IsAnyOf = (std::same_as<T, U> || ...);

    template<template<typename> typename ContainerType, typename E = int>
    concept IsContainer = requires(ContainerType<E> c, E e) {
        typename ContainerType<E>;
        typename ContainerType<E>::value_type;
        requires std::same_as<typename ContainerType<E>::value_type, E>;

        { c.emplace_back(e) } -> IsAnyOf<typename ContainerType<E>::reference, void>;
        requires std::constructible_from<ContainerType<E>,
                  decltype(ranges::begin(std::declval<ranges::empty_view<E>&>())),
                  decltype(ranges::end(std::declval<ranges::empty_view<E>&>()))>;
    };

    template <template<typename> typename C, std::ranges::range R>
    requires IsContainer<C, std::ranges::range_value_t<R>>
    constexpr auto to_container(R&& r) {
        return C<std::ranges::range_value_t<R>>(ranges::begin(r), ranges::end(r));
    }

    template<template<typename> typename C, ViewableString S>
    requires IsContainer<C, string_view>
    constexpr auto parse(S&& in_raw) {
        auto in_sv = string_view(std::forward<S>(in_raw));
        C<C<string_view>> result;

        for (auto&& line : in_sv | views::split(string_view("\n"))) {
            if (ranges::empty(line)) continue;

            auto cells = line
                | views::split(string_view(","))
                | views::transform([](auto&& x) {
                    return string_view(ranges::begin(x), ranges::end(x));
                });

            result.emplace_back(to_container<C>(cells));
        }

        return result;
    }

}
