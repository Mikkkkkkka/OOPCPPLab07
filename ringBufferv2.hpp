#ifndef RINGBUFFER_HPP
#define RINGBUFFER_HPP

#include <iterator>
#include <iostream>
#include <string> // ДЛЯ ОТАЛДКИ

template<typename T>
class RingBuffer
{
    T* _body;
    unsigned _head, _tail;
    unsigned _size;
    unsigned _capacity;

    void simple_error() { std::cout << "AAA\n"; exit(-1); } // Надо бы сделать через exception, но лень

    unsigned fStep(unsigned I, const unsigned& step = 1) { return (I + _capacity + 1 + step) % (_capacity + 1); }
    unsigned bStep(unsigned I, const unsigned& step = 1) { return (I + _capacity + 1 - step) % (_capacity + 1); }

public:

    RingBuffer(unsigned capacity)
    {
        _size = 0;
        _capacity = capacity;
        _head = _tail = 0;
        _body = (T*)calloc(_capacity + 1, sizeof(T)); // _capacity + 1, потому что _tail должен указывать на незанятую ячейку
        if (!_body) { simple_error(); }
    }

    ~RingBuffer() { free(_body); }

    void resize(unsigned newSize)
    {
        T* temp = (T*)calloc(newSize + 1, sizeof(T));
        if (!temp) { simple_error(); }
        for (int i = 0; i < _size and i < newSize; i++)
            temp[i] = _body[fStep(_head, i)];
        free(_body);
        _body = temp;
        _capacity = newSize;
        _size = std::min(_size, newSize);
        _head = 0;
        _tail = _size;
    }

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

        Iterator(unsigned _index, RingBuffer* _buffer)
        {
            buffer = _buffer;
            index = buffer->fStep(0, _index);
        }

        Iterator& operator+=(difference_type i)
        {
            index = buffer->fStep(index, i);
            return *this;
        }
        Iterator& operator-=(difference_type i)
        {
            index = buffer->bStep(index, i);
            return *this;
        }
        reference operator*() const
        {
            return buffer->_body[index];
        }
        pointer operator->() const
        {
            return buffer->_body + index;
        }
        reference operator[](int i) const
        {
            return buffer->_body[buffer->fStep(index, i)];
        }

        // Арифметика
        Iterator& operator++()
        {
            index = buffer->fStep(index);
            return *this;
        }
        Iterator& operator--()
        {
            index = buffer->bStep(index);
            return *this;
        }
        Iterator operator++(int)
        {
            Iterator temp(*this);
            ++(*this);
            return temp;
        }
        Iterator operator--(int)
        {
            Iterator temp(*this);
            --(*this);
            return temp;
        }
        difference_type operator-(const Iterator& other) const
        {
            if (index == other.index) { return 0; }
            if (index <= buffer->_tail and buffer->_tail < other.index) { return index + buffer->_size - other.index + 1; }
            if (other.index <= buffer->_tail and buffer->_tail < index) { return -(other.index + buffer->_size - index + 1); }
            return index - other.index;
        }
        Iterator operator+(const difference_type& i) const
        {
            return Iterator(buffer->fStep(index, i), buffer);
        }
        Iterator operator-(const difference_type& i) const
        {
            return Iterator(buffer->bStep(index, i), buffer);
        }
        friend Iterator operator+(const difference_type& i, const Iterator& it)
        {
            return Iterator(it.buffer->fStep(it.index, i), it.buffer);
        }
        friend Iterator operator-(const difference_type& i, const Iterator& it)
        {
            return Iterator(it.buffer->bStep(it.index, i), it.buffer);
        }

        // Сравнение
        bool operator==(const Iterator& other) const
        {
            return index == other.index;
        }
        bool operator!=(const Iterator& other) const
        {
            return !(*this == other);
        }
        operator<(const Iterator& other) const
        {
            if (index == other.index) { return false; }
            if (index <= buffer->_tail and buffer->_tail < other.index) { return false; }
            if (other.index <= buffer->_tail and buffer->_tail < index) { return true; }
            return index < other.index;
        }
        bool operator>(const Iterator& other) const
        {
            if (index == other.index) { return false; }
            if (index >= buffer->_tail and buffer->_tail > other.index) { return false; }
            if (other.index >= buffer->_tail and buffer->_tail > index) { return true; }
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
    };

    Iterator begin()
    {
        return Iterator(_head, this);
    }
    Iterator end()
    {
        return Iterator(_tail, this);
    }

    unsigned size() const
    {
        return _size;
    }

    void insert(Iterator it, int data)
    {
        if (end() <= it and it < begin())
            return;
        if (std::abs(it - begin()) < std::abs(it - end()))
        {
            --it;
            for (; it != begin() - 1; --it)
                std::swap(*it, data);
            push_front(data);
        }
        else
        {
            for (; it != end(); ++it)
                std::swap(*it, data);
            push_back(data);
        }
    }

    void erase(Iterator it)
    {
        Iterator last = end();
        if (it == last)
            return;
        for (; it != last; ++it)
            *it = *(it + 1);
        _tail = bStep(_tail);
        --_size;
    }

    void push_back(T data)
    {
        if (_head == fStep(_tail))
        {
            _head = fStep(_head);
            --_size;
        }
        _body[_tail] = data;
        _tail = fStep(_tail);
        ++_size;
    }
    void push_front(T data)
    {
        if (_head == fStep(_tail))
        {
            _tail = bStep(_tail);
            --_size;
        }
        _body[_head = bStep(_head)] = data;
        ++_size;
    }

    T back() const
    {
        if (_size == 0) { simple_error(); }
        return _body[bStep(_tail)];
    }
    T front() const
    {
        if (_size == 0) { simple_error(); }
        return _body[_head];
    }

    void pop_back()
    {
        if (_size != 0)
        {
            _tail = bStep(_tail);
            --_size;
        }
    }
    void pop_front()
    {
        if (_size != 0)
        {
            _head = fStep(_head);
            --_size;
        }
    }

    T& operator[](unsigned index) const
    {
        index = fStep(0, index);
        if (index == _tail)
            ++index;
        return _body[index];
    }

    void debugPrint() // Отладка 
    {
        for (int i = 0; i < _capacity + 1; i++)
            std::cout << _body[i];
        std::string htppos(_capacity + 1, '-');
        htppos[_head] = 'h';
        htppos[_tail] = 't';
        std::cout << '\n' << htppos << '\n';
    }
};


#endif // RINGBUFFER_HPP
