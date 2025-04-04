# csv-parser

**A modern, header-only, zero-allocation CSV parser for C++20/23.**

This lightweight CSV parser uses `std::ranges`, `std::views`, and `string_view` to provide a fast and elegant interface for parsing CSV input into any STL-compatible container.

- ✅ **Header-only**
- 🚀 **Zero allocations during parsing**
- 🧠 **Modern C++20/23 ranges & concepts**
- 🧪 **`constexpr`-friendly**
- 💡 **Custom container support (e.g., `std::vector`, `std::list`)**

---

## 📦 Installation

Just drop [`csv_parser.hpp`](./csv_parser.hpp) into your project and `#include` it. No dependencies, no build system required.

---

## ⚡ Example

```cpp
#include <iostream>
#include <vector>
#include <string>
#include "csv_parser.hpp"

int main() {
    std::string input = "name,age,city\nAlice,30,New York\nBob,25,London\n";

    // Parse CSV into vector of vector of string_view
    auto result = csv::parse<std::vector>(input);

    for (const auto& row : result) {
        for (const auto& cell : row) {
            std::cout << "[" << cell << "]";
        }
        std::cout << "\n";
    }
}
```

**Output:**
```
[name][age][city]
[Alice][30][New York]
[Bob][25][London]
```

---

## 🧠 `constexpr` support

You can even use it in `constexpr` contexts:

```cpp
constexpr std::string_view data = "a,b\n1,2\n";
constexpr auto table = csv::parse<std::vector>(data);
static_assert(table.size() == 2);
```

---

## 🛠 Requirements

- C++20 or higher
- Standard Library with full `<ranges>` support (GCC 11+, Clang 13+, MSVC 2022+)

---

## 📄 API

### `csv::parse<C>(S&& input)`

| Parameter | Description |
|----------|-------------|
| `C`      | A template STL-compatible container (e.g. `std::vector`) |
| `input`  | Any `std::string`, `std::string_view`, or convertible-to-string_view type |

Returns: `C<C<std::string_view>>` — i.e., a container of rows, each containing a container of cells.

---

## 📎 Limitations

- Does **not** handle quoted fields or escaped commas (e.g., `a,"b,c"` is treated as 2 cells).
- Trims and Unicode normalization are **not included** (yet).
- Empty lines are skipped silently.

If you need RFC-4180 compliance or quoting support, consider extending the core logic.

---

## 🧩 Extensibility

This parser is container-agnostic. You can use `std::list`, `std::deque`, or even custom containers that:

- Support `emplace_back`
- Support construction from `(begin, end)` iterators

---

## 📜 License

MIT License

---

## 🤝 Contributions

Pull requests and issues are welcome! If you use this in your project, feel free to share it via a GitHub issue or star the repo.
