
#pragma once

#include <vector>

//stores an airport and its priority in the heap
struct HeapNode {
    int airportID;
    double priority;
};

class MinHeap {
private:
    std::vector<HeapNode> heap;

    //restores heap order after inserting an element
    void heapifyUp(int index);

    //restores heap order after removing the minimum
    void heapifyDown(int index);

public:
    //straighforward implementations:
    void push(int airportID, double priority);
    HeapNode pop();
    HeapNode peek() const;
    bool empty() const;
    int size() const;
};
