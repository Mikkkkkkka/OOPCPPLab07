#ifndef RINGBUFFER_HPP
#define RINGBUFFER_HPP

#include <iterator>
#include <iostream>
#include <vector>


template<typename T, unsigned N>
class RingBuffer
{
    T* body;
    unsigned head, tail;
    unsigned _size;

    void simple_error() { std::cout << "AAA\n"; exit(-1); }

    unsigned fStep(unsigned I, const unsigned& memSize, const unsigned& step = 1) { return (I + memSize + step) % memSize; }
    unsigned bStep(unsigned I, const unsigned& memSize, const unsigned& step = 1) { return (I + memSize - step) % memSize; }

public:

    class Iterator
    {
        RingBuffer* buffer;
        unsigned index;

    public:

        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using difference_type = int;

        Iterator(unsigned index, RingBuffer* buffer)
        {
            this->buffer = buffer;
            this->index = buffer->fStep(0, N + 1, index);
        }

        reference operator*() const
        {
            return buffer->body[index];
        }
        pointer operator->() const
        {
            return buffer->body + index;
        }
        reference operator[](int i) const // FIXME
        {
            return buffer->body[(index + N + 1 + i) % (N + 1)];
        }

        Iterator& operator++()
        {
            index = buffer->fStep(index, N + 1);
            return *this;
        }
        Iterator& operator--()
        {
            index = buffer->bStep(index, N + 1);
            return *this;
        }
        Iterator& operator++(int)
        {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }
        Iterator& operator--(int)
        {
            Iterator temp = *this;
            --(*this);
            return temp;
        }
        Iterator& operator+=(difference_type i)
        {
            index = buffer->fStep(index, N + 1, i);
            return *this;
        }
        Iterator& operator-=(difference_type i)
        {
            index = buffer->bStep(index, N + 1, i);
            return *this;
        }

        bool operator==(const Iterator& other) const
        {
            return index == other.index;
        }
        bool operator!=(const Iterator& other) const
        {
            return !(*this == other);
        }
        bool operator<(const Iterator& other) const
        {
            if (index == other.index) { return false; }
            if (index <= buffer->tail and buffer->tail < other.index) { return false; }
            if (other.index <= buffer->tail and buffer->tail < index) { return true; }
            return index < other.index;
        }
        bool operator>(const Iterator& other) const
        {
            if (index == other.index) { return false; }
            if (index >= buffer->tail and buffer->tail > other.index) { return false; }
            if (other.index >= buffer->tail and buffer->tail > index) { return true; }
            return index < other.index;
        }
        bool operator<=(const Iterator& other) const
        {
            return !(*this > other);
        }
        bool operator>=(const Iterator& other) const
        {
            return !(*this < other);
        }

        Iterator operator+(const Iterator& other)
        {
            return Iterator(buffer->fStep(index, N + 1, other.index), buffer);
        }
        Iterator operator+(const difference_type& i) const
        {
            return Iterator(buffer->fStep(index, N + 1, i), buffer);
        }
        Iterator operator-(const difference_type& i) const
        {
            return Iterator(buffer->bStep(index, N + 1, i), buffer);
        }
        difference_type operator-(const Iterator& other) const
        {
            if (index < buffer->head and buffer->head <= other.index) { return index + buffer->_size - other.index; }
            if (other.index < buffer->head and buffer->head <= index) { return index - other.index + buffer->_size; }
            return index - other.index;
        }
        friend Iterator operator+(const difference_type& i, const Iterator& it)
        {
            return Iterator(it.index + i, it.buffer);
        }
        friend Iterator operator-(const difference_type& i, const Iterator& it)
        {
            return Iterator(it.index - i, it.buffer);
        }
    };

    RingBuffer()
    {
        _size = 0;
        head = tail = 0;
        body = (T*)calloc(N + 1, sizeof(T)); // N + 1, потому что tail должен указывать на незанятую ячейку
        if (!body) { simple_error(); }
    }

    ~RingBuffer() { free(body); }

    Iterator begin()
    {
        return Iterator(head, this);
    }
    Iterator end()
    {
        return Iterator(tail, this);
    }

    unsigned size() const
    {
        return _size;
    }

    void insert(Iterator it, int data)
    {
        Iterator last = end();
        for (; it != last; it++)
            std::swap(*it, data);
        *it = data;
        tail = fStep(tail, N + 1);
        ++_size;
    }

    void erase(Iterator it)
    {
        Iterator last = end();
        if (it == last)
            return;
        for (; it != last; ++it)
            *it = *(it + 1);
        tail = bStep(tail, N + 1);
        --_size;
    }

    void print() // Отладка 
    {
        for (int i = 0; i < N + 1; i++)
            std::cout << body[i];
        std::cout << '\n' << hlppos() << '\n';
    }

    void push_back(T data)
    {
        if (head == fStep(tail, N + 1))
        {
            head = fStep(head, N + 1);
            --_size;
        }
        body[tail] = data;
        tail = fStep(tail, N + 1);
        ++_size;
    }
    void push_front(T data)
    {
        if (head == fStep(tail, N + 1))
        {
            tail = bStep(tail, N + 1);
            --_size;
        }
        body[head = bStep(head, N + 1)] = data;
        ++_size;
    }

    T back() const
    {
        if (_size == 0) { simple_error(); }
        if (tail == 0) { return body[N]; } // Последний элемент в массиве размера N + 1
        return body[tail - 1];
    }
    T front() const
    {
        if (_size == 0) { simple_error(); }
        return body[head];
    }

    void pop_back()
    {
        if (_size != 0)
        {
            tail = bStep(tail, N + 1);
            --_size;
        }
    }
    void pop_front()
    {
        if (_size != 0)
        {
            head = fStep(head, N + 1);
            --_size;
        }
    }

    T& operator[](unsigned index) const
    {
        if (index = fStep(0, N + 1, index) == tail)
            ++index;
        return body[index];
    }

    std::string hlppos()
    {
        std::string res(N + 1, '-');
        res[head] = 'h';
        res[tail] = 't';
        return res;
    }
};


#endif // RINGBUFFER_HPP
