#pragma once

#include <concepts>
#include <iterator>
#include <string>
#include <string_view>
#include <utility>

namespace CSV {
    using std::string_view;

    template<typename T>
    concept ViewableString = std::convertible_to<T, string_view>;

    template<template<typename...> typename C>
    struct Table {
        std::string       data;
        C<C<string_view>> rows;
    };

    template<template<typename...> typename C, ViewableString S>
    Table<C> parse(S&& in_raw) {
        Table<C> table;
        table.data = std::string(in_raw);
        auto& data = table.data;

        using Row = C<string_view>;
        Row row;
        bool in_quotes = false;
        size_t field_start = 0;

        for (size_t i = 0; i < data.size(); ++i) {
            char c = data[i];

            if (!in_quotes) {
                if (c == '"') {
                    in_quotes = true;
                    field_start = i + 1;
                }
                else if (c == ','
                      || c == '\n') {
                    size_t len = i - field_start;
                    if (len > 0 && data[i - 1] == '"') {
                        --len;
                    }
                    row.emplace_back(data.data() + field_start, len);

                    if (c == '\n') {
                        if (!row.empty()) {
                            table.rows.push_back(row);
                        }
                        row.clear();
                    }
                    field_start = i + 1;
                }
                else if (c == '\r') {
                    if (field_start == i) {
                        field_start = i + 1;
                    }
                }
            }
            else {
                if (c == '"') {
                    if (i + 1 < data.size() && data[i + 1] == '"') {
                        data.erase(i, 1);
                    } else {
                        in_quotes = false;
                    }
                }
            }
        }

        if (!data.empty() && data.back() != '\n') {
            size_t len = data.size() - field_start;
            if (len > 0 && data.back() == '"') {
                --len;
            }
            row.emplace_back(data.data() + field_start, len);
            if (!row.empty()) {
                table.rows.push_back(row);
            }
        }

        return table;
    }

}
