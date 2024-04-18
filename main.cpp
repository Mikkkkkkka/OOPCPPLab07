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
    RingBuffer<int, 8> rb;


    for (int i = 0; i < 8; ++i)
        rb.push_back(i);
    rb.print();
    std::for_each(rb.begin(), rb.end(), out);
    std::cout << '\n' << *std::find(rb.begin(), rb.end(), 8) << '\n';

    rb.push_back(8);
    rb.print();
    std::for_each(rb.begin(), rb.end(), out);
    std::cout << '\n' << *std::find(rb.begin(), rb.end(), 8) << '\n';

    rb.push_back(9);
    rb.print();
    std::for_each(rb.begin(), rb.end(), out);
    std::cout << '\n' << *std::find(rb.begin(), rb.end(), 9) << '\n';

    return 0;
}