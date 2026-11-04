
#include "MinHeap.h"

#include <iostream>
#include <stdexcept>

void check(bool condition, const char* message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

//Test 1: a newly created heap should be empty
void testEmptyHeap() {
    MinHeap heap;

    check(heap.empty(), "New heap should be empty");
    check(heap.size() == 0, "New heap should have size 0");
}

//Test 2: insert one element and remove it
void testSingleElement() {
    MinHeap heap;

    heap.push(1, 100.0);

    check(!heap.empty(), "Heap should not be empty");
    check(heap.size() == 1, "Heap size should be 1");

    HeapNode result = heap.pop();

    check(result.airportID == 1, "Incorrect airport ID");
    check(result.priority == 100.0, "Incorrect priority");
    check(heap.empty(), "Heap should be empty after pop");
}

//Test 3: elements should be removed in priority order
void testPriorityOrder() {
    MinHeap heap;

    heap.push(1, 500.0);
    heap.push(2, 100.0);
    heap.push(3, 300.0);
    heap.push(4, 200.0);
    heap.push(5, 400.0);

    check(heap.pop().airportID == 2, "Expected airport 2");
    check(heap.pop().airportID == 4, "Expected airport 4");
    check(heap.pop().airportID == 3, "Expected airport 3");
    check(heap.pop().airportID == 5, "Expected airport 5");
    check(heap.pop().airportID == 1, "Expected airport 1");

    check(heap.empty(), "Heap should be empty");
}

//Test 4: peek should not remove an element
void testPeek() {
    MinHeap heap;

    heap.push(10, 250.0);
    heap.push(20, 50.0);

    HeapNode result = heap.peek();

    check(result.airportID == 20, "Peek returned wrong airport");
    check(heap.size() == 2, "Peek should not remove an element");
}

//Test 5: equal priorities should be allowed
void testDuplicatePriorities() {
    MinHeap heap;

    heap.push(1, 100.0);
    heap.push(2, 100.0);
    heap.push(3, 50.0);

    check(heap.pop().airportID == 3, "Minimum priority incorrect");
    check(heap.pop().priority == 100.0, "Expected priority 100");
    check(heap.pop().priority == 100.0, "Expected priority 100");
}

//Test 6: rhe same airport can have updated priorities
void testDuplicateAirports() {
    MinHeap heap;

    heap.push(5, 500.0);
    heap.push(5, 200.0);

    check(heap.size() == 2, "Both entries should exist");
    check(heap.pop().priority == 200.0, "Improved priority missing");
    check(heap.pop().priority == 500.0, "Original entry missing");
}

//Test 7: insertion should still work after removal
void testMixedOperations() {
    MinHeap heap;

    heap.push(1, 100.0);
    heap.push(2, 300.0);

    check(heap.pop().airportID == 1, "Expected airport 1");

    heap.push(3, 50.0);

    check(heap.pop().airportID == 3, "Expected airport 3");
    check(heap.pop().airportID == 2, "Expected airport 2");
}

//Test 8: empty heap operations should throw exceptions
void testEmptyExceptions() {
    MinHeap heap;
    bool popFailed = false;
    bool peekFailed = false;

    try {
        heap.pop();
    } catch (const std::out_of_range&) {
        popFailed = true;
    }

    try {
        heap.peek();
    } catch (const std::out_of_range&) {
        peekFailed = true;
    }

    check(popFailed, "Pop should throw on an empty heap");
    check(peekFailed, "Peek should throw on an empty heap");
}

//Test 9: check a larger number of elements
void testLargeHeap() {
    MinHeap heap;

    for (int i = 1000; i >= 1; i--) {
        heap.push(i, static_cast<double>(i));
    }

    check(heap.size() == 1000, "Expected 1000 elements");

    for (int i = 1; i <= 1000; i++) {
        HeapNode result = heap.pop();

        check(result.airportID == i, "Large heap order incorrect");
    }

    check(heap.empty(), "Large heap should be empty");
}

int main() {
    try {
        testEmptyHeap();
        testSingleElement();
        testPriorityOrder();
        testPeek();
        testDuplicatePriorities();
        testDuplicateAirports();
        testMixedOperations();
        testEmptyExceptions();
        testLargeHeap();

        std::cout << "All MinHeap tests passed!" << std::endl;
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "MinHeap test failed: " << error.what() << std::endl;
        return 1;
    }
}
