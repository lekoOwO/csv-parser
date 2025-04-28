# csv-parser

[![CI](https://github.com/lekoOwO/csv-parser/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/lekoOwO/csv-parser/actions/workflows/ci.yml)

**A modern, header-only, zero-allocation, RFC‑4180–compliant CSV parser for C++20/23.**

This lightweight CSV parser uses `string_view` to provide a fast, elegant, zero-copy interface for parsing CSV input into any STL-compatible container.

- ✅ **Header-only**
- 🚀 **Zero allocations during parsing** (all fields reference a single owned buffer)
- 🧠 **RFC‑4180 quoting & escaping**
- 📐 **C++20/23 **constexpr**-friendly**
- 💡 **Custom container support** (e.g., `std::vector`, `std::list`)

---

## 📦 Installation

Just drop [`csv_parser.hpp`](./csv_parser.hpp) into your project and `#include` it. No other dependencies or build steps required.

---

## ⚡ Quick Start

```cpp
#include <iostream>
#include <vector>
#include "csv_parser.hpp"

int main() {
    std::string input = "\"Name\",Age,City\r\n"
                        "\"Alice\",30,New York\r\n"
                        "Bob,25,London\n";

    // Parse into Table<std::vector>
    auto table = CSV::parse<std::vector>(input);

    for (auto& row : table.rows) {
        for (auto& cell : row) {
            std::cout << '[' << cell << ']';
        }
        std::cout << '\n';
    }
}
```

**Output:**
```
[Name][Age][City]
[Alice][30][New York]
[Bob][25][London]
```

---

## 🧠 `constexpr` Usage

```cpp
constexpr std::string_view data = "a,b,c\r\n1,2,3\n";
constexpr auto table = CSV::parse<std::vector>(data);
static_assert(table.rows.size() == 2);
```

---

## 📄 API

```cpp
namespace CSV {
    template<template<typename> typename Container>
    struct Table {
        std::string data;               // Owned CSV buffer
        Container<Container<std::string_view>> rows;
    };

    template<template<typename> typename Container, typename S>
    requires (convertible_to<S, std::string_view>) && /* Container concept */
    Table<Container> parse(S&& input);
}
```

- **`Table<Container>`**
  - `data`: the owned CSV text
  - `rows`: a container of rows, each a container of `string_view` fields referencing `data`
- **`parse<Container>(input)`**
  - **Parameters:**
    - `Container`: any STL-like container template supporting `emplace_back` and construction from two iterators
    - `input`: `std::string`, `std::string_view`, or convertible-to-`string_view`
  - **Returns:** a `Table<Container>` holding parsed rows

---

## 🧪 Testing

We include a standalone test file [`test_csv_parser.cpp`](./test_csv_parser.cpp) covering:

1. Simple fields and basic delimiters
2. Quoted fields containing commas
3. Escaped double quotes (`""` → `"`)
4. Embedded newlines within quoted fields

Compile and run:

```sh
g++ -std=c++20 -I. test_csv_parser.cpp -o test_csv
./test_csv
# should print "CSV parser tests passed."
```

## 📜 License

MIT License