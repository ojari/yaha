# GitHub Copilot Instructions for Advanced C++ Development

## General Guidelines
- Follow the C++17 standard or later.
- Prioritize readability and maintainability of the code.
- Use modern C++ features such as smart pointers, auto keyword, and range-based for loops.
- Avoid using raw pointers unless absolutely necessary.

## Code Style
- Use camelCase for variable and function names.
- Use PascalCase for class names.
- Indent with 4 spaces, no tabs.
- Place opening braces on the same line as the statement.
- Use "#pragma once" for header guards instead of include guards.

## Libraries and Frameworks
- Prefer using the Standard Template Library (STL) for data structures and algorithms.
- Avoid using Boost libraries.
- For unit testing, use the Catch2 framework.

## Error Handling
- Use exceptions for error handling.
- Ensure all exceptions are caught and handled appropriately.
- Avoid using `goto` statements.

## Performance
- Optimize for performance-critical sections of the code.
- Use profiling tools to identify bottlenecks.
- Avoid premature optimization; focus on clear and correct code first.

## Documentation
- Document all public functions and classes using Doxygen-style comments.
- Provide examples of usage where applicable.
- Ensure all comments are up-to-date with the code changes.

## Example Code
```cpp
#include <iostream>
#include <vector>
#include <algorithm>

// Example function to demonstrate guidelines
std::vector<int> filterAndSort(const std::vector<int>& input) {
    std::vector<int> result;
    std::copy_if(input.begin(), input.end(), std::back_inserter(result), [](inturn x > 0; });
    std::sort(result.begin(), result.end());
    return result;
}

int main() {
    std::vector<int> data = {3, -1, 2, 0, -5, 7};
    std::vector<int> filteredSortedData = filterAndSort(data);
    for (int num : filteredSortedData) {
        std::cout << num << " ";
    }
    return 0;
}
