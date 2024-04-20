#include "ringbufferv2.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

int main()
{
    RingBuffer<int> rb(6);

    for (int i = 0; i < 6; i++)
        rb.push_back(i);

    rb.debugPrint();

    rb.resize(4);
    rb.debugPrint();

    return 0;
}