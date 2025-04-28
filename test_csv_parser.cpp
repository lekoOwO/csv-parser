#include "csv_parser.hpp"
#include <cassert>
#include <vector>
#include <string_view>
#include <iostream>

void test_simple() {
    constexpr auto text = "a,b,c\n1,2,3\n";
    auto table = CSV::parse<std::vector>(text);

    assert(table.rows.size() == 2);
    assert(table.rows[0].size() == 3);
    assert(table.rows[0][0] == "a");
    assert(table.rows[0][1] == "b");
    assert(table.rows[0][2] == "c");
    assert(table.rows[1].size() == 3);
    assert(table.rows[1][0] == "1");
    assert(table.rows[1][1] == "2");
    assert(table.rows[1][2] == "3");
}

void test_quoted_commas() {
    constexpr auto text = "a,\"b,c\",d\n";
    auto table = CSV::parse<std::vector>(text);

    assert(table.rows.size() == 1);
    assert(table.rows[0].size() == 3);
    assert(table.rows[0][0] == "a");
    assert(table.rows[0][1] == "b,c");
    assert(table.rows[0][2] == "d");
}

void test_escaped_quotes() {
    constexpr auto text = "\"a\"\"b\",c\n";
    auto table = CSV::parse<std::vector>(text);

    assert(table.rows.size() == 1);
    assert(table.rows[0].size() == 2);
    assert(table.rows[0][0] == "a\"b");
    assert(table.rows[0][1] == "c");
}

void test_newline_in_quotes() {
    constexpr auto text = "a,\"b\nc\",d\n";
    auto table = CSV::parse<std::vector>(text);

    assert(table.rows.size() == 1);
    assert(table.rows[0].size() == 3);
    assert(table.rows[0][1] == "b\nc");
}

int main() {
    test_simple();
    test_quoted_commas();
    test_escaped_quotes();
    test_newline_in_quotes();
    std::cout << "All CSV parser tests passed.\n";
    return 0;
}
