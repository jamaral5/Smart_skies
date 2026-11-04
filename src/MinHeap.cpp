
#include "MinHeap.h"

#include <stdexcept>


void MinHeap::heapifyUp(int index) {
    //moves an element upward until the min-heap property is satisfied
    while (index > 0) {
        int parent = (index - 1) / 2;

        //stop if the parent already has a smaller priority
        if (heap[parent].priority <= heap[index].priority) {
            break;
        }

        //swap the current element with its parent
        HeapNode temp = heap[parent];
        heap[parent] = heap[index];
        heap[index] = temp;

        index = parent;
    }
}

void MinHeap::heapifyDown(int index) {
    //moves an element downward until the min-heap property is satisfied
    int count = size();

    while (true) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int smallest = index;

        //check whether the left child is smaller
        if (left < count &&
            heap[left].priority < heap[smallest].priority) {
            smallest = left;
        }

        //check whether the right child is smaller
        if (right < count &&
            heap[right].priority < heap[smallest].priority) {
            smallest = right;
        }

        //stop if the current element is already smallest
        if (smallest == index) {
            break;
        }

        //swap with the smaller child
        HeapNode temp = heap[index];
        heap[index] = heap[smallest];
        heap[smallest] = temp;

        index = smallest;
    }
}

void MinHeap::push(int airportID, double priority) {
    //inserts an airport into the heap
    HeapNode newNode = {airportID, priority};
    heap.push_back(newNode);
    heapifyUp(size() - 1);
}

HeapNode MinHeap::pop() {
    //removes and returns the min priority element
    if (empty()) {
        throw std::out_of_range("Cannot pop from an empty heap");
    }
    HeapNode minimum = heap[0];
    heap[0] = heap.back();
    heap.pop_back();

    if (!empty()) {
        heapifyDown(0);
    }

    return minimum;
}

HeapNode MinHeap::peek() const {
    //returns the minimum element without removing it
    if (empty()) {
        throw std::out_of_range("Cannot peek at an empty heap");
    }

    return heap[0];
}

bool MinHeap::empty() const {
    return heap.empty();
}

int MinHeap::size() const {
    return static_cast<int>(heap.size());
}
