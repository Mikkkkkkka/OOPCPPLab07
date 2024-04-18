#include "ringbufferv2.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>


int main()
{
    RingBuffer<int, 8> rb;

    for (int i = 0; i < 8; ++i)
        rb.push_back(i);
    rb.print();
    for (int i = 0; i < 10; i++)
        std::cout << *std::find(rb.begin(), rb.end(), i) << ' ';
    std::cout << '\n';

    rb.push_back(8);
    rb.print();
    for (int i = 0; i < 10; i++)
        std::cout << *std::find(rb.begin(), rb.end(), i) << ' ';
    std::cout << '\n';

    rb.push_back(9);
    rb.print();
    for (int i = 0; i < 10; i++)
        std::cout << *std::find(rb.begin(), rb.end(), i) << ' ';
    std::cout << '\n';

    rb.push_back(9);
    rb.print();
    for (int i = 0; i < 10; i++)
        std::cout << *std::find(rb.begin(), rb.end(), i) << ' ';
    std::cout << '\n';

    return 0;
}