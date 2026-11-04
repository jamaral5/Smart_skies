
add_executable(test_MinHeap
        tests/test_MinHeap.cpp
        src/MinHeap.cpp
)

target_include_directories(test_MinHeap PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

enable_testing()
add_test(NAME MinHeapTests COMMAND test_MinHeap)
