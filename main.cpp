#include "ringbufferv2.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

int main()
{
    RingBuffer<int, 6> rb;

    for (int i = 0; i < 6; i++)
        rb.push_back(i);
    rb.debugPrint();

    // std::cout << rb.begin() - rb.end() << '\n';

    rb.insert(rb.begin(), 9);
    rb.debugPrint();

    return 0;
}