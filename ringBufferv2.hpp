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

    static unsigned fStep(unsigned I, const unsigned& step = 1) { return (I + N + 1 + step) % (N + 1); }
    static unsigned bStep(unsigned I, const unsigned& step = 1) { return (I + N + 1 - step) % (N + 1); }

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
        using difference_type = unsigned;

        Iterator(unsigned _index, RingBuffer* _buffer)
        {
            buffer = _buffer;
            index = fStep(0, _index);
        }
        Iterator(const Iterator& other)
        {
            buffer = other.buffer;
            index = other.index;
        }

        // inline Iterator& operator+=(difference_type i)
        // {
        //     index = fStep(index, i);
        //     return *this;
        // }
        // inline Iterator& operator-=(difference_type i)
        // {
        //     index = bStep(index, i);
        //     return *this;
        // }
        inline reference operator*() const
        {
            return buffer->body[index];
        }
        inline pointer operator->() const
        {
            return buffer->body + index;
        }
        inline reference operator[](int i) const
        {
            return buffer->body[fStep(index, i)];
        }

        //  Инкремент / Декремент 
        inline Iterator& operator++()
        {
            index = fStep(index);
            return *this;
        }
        // inline Iterator& operator--()
        // {
        //     index = bStep(index);
        //     return *this;
        // }
        inline Iterator operator++(int)
        {
            Iterator temp(*this);
            ++(*this);
            return temp;
        }
        // inline Iterator operator--(int)
        // {
        //     Iterator temp(*this);
        //     --(*this);
        //     return temp;
        // }

        // Арифметика
        // inline Iterator operator+(const Iterator& other)
        // {
        //     return Iterator(fStep(index, other.index), buffer);
        // }
        // inline Iterator operator+(const difference_type& i) const
        // {
        //     return Iterator(fStep(index, i), buffer);
        // }
        // inline Iterator operator-(const difference_type& i) const
        // {
        //     return Iterator(bStep(index, i), buffer);
        // }
        // inline difference_type operator-(const Iterator& other) const
        // {
        //     if (index < buffer->head and buffer->head <= other.index) { return index + buffer->_size - other.index; }
        //     if (other.index < buffer->head and buffer->head <= index) { return index - other.index + buffer->_size; }
        //     return index - other.index;
        // }
        // friend inline Iterator operator+(const difference_type& i, const Iterator& it)
        // {
        //     return Iterator(it.index + i, it.buffer);
        // }
        // friend inline Iterator operator-(const difference_type& i, const Iterator& it)
        // {
        //     return Iterator(it.index - i, it.buffer);
        // }

        // Сравнение
        inline bool operator==(const Iterator& other) const
        {
            return index == other.index;
        }
        inline bool operator!=(const Iterator& other) const
        {
            return !(*this == other);
        }
        // bool operator<(const Iterator& other) const
        // {
        //     if (index == other.index) { return false; }
        //     if (index <= buffer->tail and buffer->tail < other.index) { return false; }
        //     if (other.index <= buffer->tail and buffer->tail < index) { return true; }
        //     return index < other.index;
        // }
        // bool operator>(const Iterator& other) const
        // {
        //     if (index == other.index) { return false; }
        //     if (index >= buffer->tail and buffer->tail > other.index) { return false; }
        //     if (other.index >= buffer->tail and buffer->tail > index) { return true; }
        //     return index < other.index;
        // }
        // bool operator<=(const Iterator& other) const
        // {
        //     return !(*this > other);
        // }
        // bool operator>=(const Iterator& other) const
        // {
        //     return !(*this < other);
        // }

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
        tail = fStep(tail);
        ++_size;
    }

    void erase(Iterator it)
    {
        Iterator last = end();
        if (it == last)
            return;
        for (; it != last; ++it)
            *it = *(it + 1);
        tail = bStep(tail);
        --_size;
    }

    void debugPrint() // Отладка 
    {
        for (int i = 0; i < N + 1; i++)
            std::cout << body[i];
        std::cout << '\n' << hlppos() << '\n';
    }

    void push_back(T data)
    {
        if (head == fStep(tail))
        {
            head = fStep(head);
            --_size;
        }
        body[tail] = data;
        tail = fStep(tail);
        ++_size;
    }
    void push_front(T data)
    {
        if (head == fStep(tail))
        {
            tail = bStep(tail);
            --_size;
        }
        body[head = bStep(head)] = data;
        ++_size;
    }

    T back() const
    {
        if (_size == 0) { simple_error(); }
        return body[bStep(tail)];
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
            tail = bStep(tail);
            --_size;
        }
    }
    void pop_front()
    {
        if (_size != 0)
        {
            head = fStep(head);
            --_size;
        }
    }

    T& operator[](unsigned index) const
    {
        index = fStep(0, index);
        if (index == tail)
            ++index;
        return body[index];
    }

    std::string hlppos() const
    {
        std::string res(N + 1, '-');
        res[head] = 'h';
        res[tail] = 't';
        return res;
    }
};


#endif // RINGBUFFER_HPP
