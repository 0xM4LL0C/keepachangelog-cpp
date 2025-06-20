# keepachangelog-cpp

## Overview

`keepachangelog-cpp` is a lightweight C++ library designed to parse and generate
changelog files that adhere to the [Keep a Changelog](https://keepachangelog.com/)
standard

## Installation

### Prerequisites

To build and use `keepachangelog-cpp`, you need:
- **CMake**: Version 3.10 or higher.
- **Compiler**: A C++17-compliant compiler

### Building

1. Clone the repository:
  ```bash
  git clone git@github.com:0xM4LL0C/keepachangelog-cpp
  ```

2. Create a build directory:
  ```bash
  mkdir .build && cd .build
  ```

3. Configure the project with CMake:
  ```bash
  cmake ..
  ```

4. Build the library:
  ```bash
  cmake --build .
  ```

### CMake Integration

To use `keepachangelog-cpp` in your CMake project, include it as a subdirectory or install it and link
against the `keepachangelog` library:

```cmake
add_subdirectory(keepachangelog-cpp)
target_link_libraries(your_target PRIVATE keepachangelog)
```

Or, if installed:

```cmake
find_package(keepachangelog REQUIRED)
target_link_libraries(your_target PRIVATE keepachangelog)
```

Alternatively, you can use [CPM.cmake](https://github.com/cpm-cmake/CPM.cmake) to fetch and integrate
`keepachangelog-cpp` directly into your project:

```cmake
CPMAddPackage("gh:0xM4LL0C/keepachangelog-cpp#0.1.0")
target_link_libraries(your_target PRIVATE keepachangelog)
```
## Usage

Include the library header in your C++ code:
```cpp
#include <keepachangelog.hpp>
```

Here’s an example demonstrating parsing and generating a changelog:
```cpp
#include <keepachangelog.hpp>
#include <iostream>

int main() {
    std::string input = R"(
## [Unreleased]
### Added
- Feature 1
### Changed
- Feature 2
## [1.0.0] - 2025-06-20
### Added
- Initial release
)";

    keepachangelog::Changelog changelog = keepachangelog::Changelog::parse(input);
    std::string output = changelog.unparse();
    std::cout << output << std::endl;
    return 0;
}
```

This code parses a changelog string into a `Changelog` object and converts it
back to a formatted string, demonstrating the library’s core functionality.

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.

## Contributing

Contributions are welcome! Please submit pull requests or open issues on the
[GitHub repository](https://github.com/0xM4LL0C/keepachangelog-cpp).
