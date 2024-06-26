// Use span

#include <collie/container/span.h>
#include <iostream>

#define span_DIMENSION_OF(a) ( sizeof(a) / sizeof(0[a]) )

using namespace collie;

int line = 0;

void bad(int *arr, size_t num) {
    std::cout << ++line << ": ";
    for (size_t i = 0; i != num; ++i) {
        std::cout << (i == 0 ? "[" : "") << arr[i] << (i != num - 1 ? ", " : "]\n");
    }
}

void good(span<int> arr) {
    std::cout << ++line << ": ";
    for (size_t i = 0; i != arr.size(); ++i) {
        std::cout << (i == 0 ? "[" : "") << arr[i] << (i != arr.size() - 1 ? ", " : "]\n");
    }
}

int main() {
    try {
        int arr[] = {1, 2, 3, 4, 5,};

        good(arr);                        // 1. Good: deduce length

        std::array<int, 6> ary = {1, 2, 3, 4, 5, 6,};
        std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7,};

        good(ary);                        // 2. Good: single function handles
        good(vec);                        // 3.   C-array, std::array and containers such as std::vector

        bad(arr, span_DIMENSION_OF(arr)); // 4. Avoid: specify elements and length separately
        bad(arr, 3);                      // 5. Avoid, but not wrong
        bad(arr, 7);                      // 6. Wrong, array length exceeded

        good({arr, 3});                 // 7. Avoid, but not wrong
        good({arr, 7});                 // 8. Wrong, array length exceeded

        span<int> s(arr);
        good(s.first(3));          //  9. Fine
        good(s.last(3));          // 10. Fine
        good(s.subspan(1));          // 11. Fine
        good(s.subspan(1, 3));          // 12. Fine
        good(s.subspan(1, 5));          // 13. Run-time error, terminate

    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

#if 0
cl -EHsc -I../include 02-span.cpp && 02-span.exe
g++ -std=c++11 -Wall -I../include -o 02-span.exe 02-span.cpp && 02-span.exe
#endif
