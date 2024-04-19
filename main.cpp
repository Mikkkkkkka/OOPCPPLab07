#include "ringbufferv2.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

void out(int x)
{
    std::cout << x << ' ';
}

int main()
{
    RingBuffer<int, 6> rb;

    for (int i = 0; i < 6; i++)
        rb.push_back(i);
    rb.debugPrint();
    std::cout << "7: " << *std::find(rb.begin(), rb.end(), 7) << '\n';
    std::for_each(rb.begin(), rb.end(), out);
    std::cout << '\n';

    return 0;
}