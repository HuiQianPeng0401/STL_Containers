#ifndef _PHQ_VECTOR_H_
#define _PHQ_VECTOR_H_

#include <initializer_list>
#include "iterator.h"
#include "util.h"
#include "exceptdef.h"
#include "allocator.h"
#include <memory>
#include <algorithm>

//@ phq
//2024/1/31

namespace phq
{

template <class T>
class vector
{
public:
    typedef allocator<T> allocator_type;
    typedef allocator<T> data_allocator;
    typedef typename allocator_type::value_type value_type;
    typedef typename allocator_type::pointer pointer;
    typedef typename allocator_type::const_pointer const_pointer;
    typedef typename allocator_type::reference reference;
    typedef typename allocator_type::const_reference const_reference;
    typedef typename allocator_type::size_type size_type;
    typedef typename allocator_type::difference_type difference_type;
    typedef value_type *iterator;
    typedef const value_type *const_iterator;
    typedef phq::reverse_iterator<iterator> reverse_iterator;
    typedef phq::reverse_iterator<const_iterator> const_reverse_iterator;
    allocator_type get_allocator() { return data_allocator(); }

private:
    iterator _begin; //表示目前使用空间的头部
    iterator _end; // 表示目前使用空间的尾部
    iterator _cap; // 表示目前存储空间的尾部

public:
    vector() noexcept;
    explicit vector(size_type n);
    vector(size_type n, const value_type &value);
    template <class Iter, typename std::enable_if<phq::is_input_iterator<Iter>::value, int>::type = 0>
    vector(Iter first, Iter last);
    vector(const vector &rhs);
    vector(vector &&rhs) noexcept;
    vector(std::initializer_list<value_type> list);
    vector &operator=(const vector &rhs);
    vector &operator=(vector &&rhs) noexcept;
    vector &operator=(std::initializer_list<value_type> list);
    ~vector();

public:
    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    reverse_iterator rbegin() noexcept;
    const_reverse_iterator rbegin() const noexcept;
    reverse_iterator rend() noexcept;
    const_reverse_iterator rend() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;
    const_reverse_iterator crbegin() const noexcept;
    const_reverse_iterator crend() const noexcept;
    bool empty() const noexcept;
    size_type size() const noexcept;
    size_type max_size() const noexcept;
    size_type capacity() const noexcept;
    void reverse(size_type n);
    void shrink_to_fit();
    reference operator[](size_type n);
    const_reference operator[](size_type n) const;
    reference at(size_type n);
    const_reference at(size_type n) const;
    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;
    pointer data() noexcept;
    const_pointer data() const noexcept;
    void assign(size_type n, const value_type &value);
    template <class Iter, typename std::enable_if<phq::is_input_iterator<Iter>::value, int>::type = 0>
    void assign(Iter first, Iter last);
    void assign(std::initializer_list<value_type> list);
    template <class... Args>
    iterator emplace(const_iterator pos, Args &&...args);
    template <class... Args>
    void emplace_back(Args &&...args);
    void push_back(const value_type &value);
    void push_back(value_type &&value);
    void pop_back();
    iterator insert(const_iterator pos, const value_type &value);
    iterator insert(const_iterator pos, value_type &&value);
    iterator insert(const_iterator pos, size_type n, const value_type &value);
    template <class Iter, typename std::enable_if<phq::is_input_iterator<Iter>::value, int>::type = 0>
    void insert(const_iterator pos, Iter first, Iter last);
    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);
    void clear();
    void resize(size_type new_size);
    void resize(size_type new_size, const value_type &value);
    void reverse();
    void swap(vector &rhs);
    void reserve(size_type n);

private://helper functions
    //initialize and destroy
    void try_init() noexcept;
    void init_space(size_type size, size_type cap);
    void fill_init(size_type n, const value_type &value);
    template <class Iter>
    void range_init(Iter first, Iter last);
    void destroy_and_recover(iterator first, iterator last, size_type n);
    //calculate the growth size
    size_type get_new_cap(size_type add_size);
    //assign
    void fill_assign(size_type n, const value_type &value);
    template <class IIter>
    void copy_assign(IIter first, IIter last, input_iterator_tag);
    template <class FIter>
    void copy_assign(FIter first, FIter last, forward_iterator_tag);
    //reallocate
    template <class... Args>
    void reallocate_emplace(iterator pos, Args &&...args);
    void reallocate_insert(iterator pos, const value_type &value);
    //insert
    iterator fill_insert(iterator pos, size_type n, const value_type &value);
    template <class IIter>
    void copy_insert(iterator pos, IIter first, IIter last);
    //shrink
    void reinsert(size_type size);
};

template <class T>
void vector<T>::try_init() noexcept {
    try
    {
        _begin = data_allocator::allocate(16);
        _end = _begin;
        _cap = _begin + 16;
    }
    catch(...)
    {
        _begin = nullptr;
        _end = nullptr;
        _cap = nullptr;
    }   
}
template <class T>
void vector<T>::init_space(size_type size, size_type cap) {
    try
    {
        _begin = data_allocator::allocate(cap);
        _end = _begin + size;
        _cap = _begin + cap;
    }
    catch(...)
    {
        _begin = nullptr;
        _end = nullptr;
        _cap = nullptr;
        throw;
    }
}
template <class T>
void vector<T>::fill_init(size_type n, const value_type &value) {
    const size_type init_size = std::max(static_cast<size_type>(16), n);
    init_space(n, init_size);
    std::uninitialized_fill_n(_begin, n, value);
}
template <class T>
template <class Iter>
void vector<T>::range_init(Iter first, Iter last) {
    const size_type len = std::distance(first, last);
    const size_type init_size = std::max(len, static_cast<size_type>(16));
    init_space(len, init_size);
    std::uninitialized_copy(first, last, _begin);
}
template <class T>
void vector<T>::destroy_and_recover(iterator first, iterator last, size_type n) {
    data_allocator::destroy(first, last);
    data_allocator::deallocate(first, n);
}
template <class T>
typename vector<T>::size_type
vector<T>::get_new_cap(size_type add_size) { // 计算新容量，并不实际扩容
    const auto old_size = capacity();
    if (old_size > max_size() - old_size / 2) {
        return old_size + add_size > max_size() - 16 ? old_size + add_size : old_size + add_size + 16;
    }
    const size_type new_size = old_size == 0 ? std::max(add_size, static_cast<size_type>(16)) : std::max(old_size + old_size / 2, old_size + add_size);
    return new_size;
}
template <class T>
void vector<T>::fill_assign(size_type n, const value_type &value) {
    if (n > capacity()) {
        vector temp(n, value);
        swap(temp);
    }
    else if (n > size()) {
        std::fill(begin(), end(), value);
        _end = std::uninitialized_fill_n(_end, n - size(), value);
    } else {
        erase(std::fill_n(begin(), n, value), _end);
    }
}
template <class T>
template <class IIter>
void vector<T>::copy_assign(IIter first, IIter last, input_iterator_tag) {
    auto cur = _begin;
    for (; first != last && cur != _end; ++first, ++cur) {
        *cur = *first;
    }
    if (first == last) {
        erase(cur, _end);
    } else {
        insert(_end, first, last);
    }
}
template <class T>
template <class FIter>
void vector<T>::copy_assign(FIter first, FIter last, forward_iterator_tag) {
    const size_type len = std::distance(first, last);
    if (len > capacity()) {
        vector temp(first, last);
        swap(temp);
    }
    else if (size() >= len) {
        auto new_end = std::copy(first, last, _begin);
        data_allocator::destroy(new_end, _end);
        _end = new_end;
    } else {
        auto cur = first;
        phq::advance(cur, size());
        std::copy(first, cur, _begin);
        auto new_end = std::uninitialized_copy(cur, last, _end);
        _end = new_end;
    }
}
template <class T>
template <class... Args>
void vector<T>::reallocate_emplace(iterator pos, Args &&...args) {
    const auto new_size = get_new_cap(1);
    auto new_begin = data_allocator::allocate(new_size);
    auto new_end = new_begin;
    try {
        new_end = std::uninitialized_move(_begin, pos, new_begin);
        data_allocator::construct(std::addressof(*new_end), std::forward<Args>(args)...);
        ++new_end;
        new_end = std::uninitialized_move(pos, _end, new_end);
    } catch(...) {
        data_allocator::deallocate(new_begin, new_size);
        throw;
    }
    destroy_and_recover(_begin, _end, _cap - _begin);
    _begin = new_begin;
    _end = new_end;
    _cap = new_begin + new_size;
}
template <class T>
void vector<T>::reallocate_insert(iterator pos, const value_type &value) {
    const auto new_size = get_new_cap(1);
    auto new_begin = data_allocator::allocate(new_size);
    auto new_end = new_begin;
    const value_type &value_copy = value;
    try {
        new_end = std::uninitialized_move(_begin, pos, new_begin);
        data_allocator::construct(std::addressof(*new_end), value_copy);
        ++new_end;
        new_end = std::uninitialized_move(pos, _end, new_end);
    } catch(...) {
        data_allocator::deallocate(new_begin, new_size);
        throw;
    }
    destroy_and_recover(_begin, _end, _cap - _begin);
    _begin = new_begin;
    _end = new_end;
    _cap = _begin + new_size;
}
template <class T>
typename vector<T>::iterator
vector<T>::fill_insert(iterator pos, size_type n, const value_type &value) {
    if (n == 0)
        return pos;
    const size_type xpos = pos - _begin;
    const value_type value_copy = value;
    if (static_cast<size_type>(_cap - _end) >= n) {
        const size_type after_elems = _end - pos;
        auto old_end = _end;
        if(after_elems > n) {
            std::uninitialized_copy(_end - n, _end, _end);
            _end += n;
            std::move_backward(pos, old_end - n, old_end);
            std::uninitialized_fill_n(pos, n, value_copy);
        } else {
            _end = std::uninitialized_fill_n(_end, n - after_elems, value_copy);
            _end = std::uninitialized_move(pos, old_end, _end);
            std::uninitialized_fill_n(pos, after_elems, value_copy);
        }
    } else {
        const auto new_size=get_new_cap(n);
        auto new_begin = data_allocator::allocate(new_size);
        auto new_end = new_begin;
        try {
            new_end = std::uninitialized_move(_begin, pos, new_begin);
            new_end = std::uninitialized_fill_n(new_end, value, n);
            new_end = std::uninitialized_move(pos, _end, new_end);
        } catch(...) {
            destroy_and_recover(new_begin, new_end, new_size);
            throw;
        }
        data_allocator::deallocate(_begin, _cap - _begin);
        _begin = new_begin;
        _end = new_end;
        _cap = _begin + new_size;
    }
    return _begin + xpos;
}
template <class T>
template <class IIter>
void vector<T>::copy_insert(iterator pos, IIter first, IIter last) {
    if (first == last)
        return;
    const auto n = std::distance(first, last);
    if ((_cap - _end) >= n) {
        const auto after_elems = _end - pos;
        auto old_end = _end;
        if (after_elems > n) {
            _end = std::uninitialized_copy(_end - n, _end, _end);
            std::move_backward(pos, old_end, old_end);
            std::uninitialized_copy(first, last, pos);
        } else {
            auto mid = first;
            phq::advance(mid, after_elems);
            _end = std::uninitialized_copy(mid, last, _end);
            _end = std::uninitialized_move(pos, old_end, _end);
            std::uninitialized_copy(first, mid, pos);
        }
    } else {
        const auto new_size = get_new_cap(n);
        auto new_begin = data_allocator::allocate(new_size);
        auto new_end = new_begin;
        try {
            new_end = std::uninitialized_move(_begin, pos, new_begin);
            new_end = std::uninitialized_copy(first, last, new_end);
            new_end = std::uninitialized_move(pos, _end, new_end);
        } catch(...) {
            destroy_and_recover(new_begin, new_end, new_size);
            throw;
        }
        data_allocator::deallocate(_begin, _cap - _begin);
        _begin = new_begin;
        _end = new_end;
        _cap = new_begin + new_size;
    }
}
template <class T>
void vector<T>::reinsert(size_type size) {
    auto new_begin = data_allocator::allocate(size);
    try {
        std::uninitialized_move(_begin, _end, new_begin);
    } catch(...) {
        data_allocator::deallocate(new_begin, size);
    }
    data_allocator::deallocate(_begin, _cap - _begin);
    _begin = new_begin;
    _end = _begin + size;
    _cap = _begin + size;
}
template <class T>
bool operator==(const vector<T> &lhs, const vector<T> &rhs) {
    return lhs.size() == rhs.size() && std::equal(lhs.begin(), lhs.end(), rhs.begin());
}
template <class T>
bool operator<(const vector<T> &lhs, const vector<T> &rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}
template <class T>
bool operator!=(const vector<T> &lhs, const vector<T> &rhs) {
    return !(lhs == rhs);
}
template <class T>
bool operator>(const vector<T> &lhs, const vector<T> &rhs) {
    return rhs < lhs;
}
template <class T>
bool operator<=(const vector<T> &lhs, const vector<T> &rhs) {
    return !(rhs < lhs);
}
template <class T>
bool operator>=(const vector<T> &lhs, const vector<T> &rhs) {
    return !(lhs < rhs);
}
template <class T>
void swap(vector<T> &lhs, vector<T> &rhs) {
    lhs.swap(rhs);
}

template <class T>
vector<T>::vector() noexcept {
    try_init();
}

template <class T>
vector<T>::vector(size_type n) {
    fill_init(n, value_type());
}

template <class T>
vector<T>::vector(size_type n, const value_type &value) {
    fill_init(n, value);
}

template <class T>
template <class Iter, typename std::enable_if<phq::is_input_iterator<Iter>::value, int>::type>
vector<T>::vector(Iter first, Iter last) {
    PHQ_DEBUG(!(last < first));
    range_init(first, last);
}

template <class T>
vector<T>::vector(const vector<T> &rhs) {
    range_init(rhs.begin(), rhs.end());
}

template <class T>
vector<T>::vector(vector<T> &&rhs) noexcept : _begin(rhs._begin), _end(rhs._end), _cap(rhs._cap) {
    rhs._begin = nullptr;
    rhs._end = nullptr;
    rhs._cap = nullptr;
}

template <class T>
vector<T>::vector(std::initializer_list<value_type> list) {
    range_init(list.begin(), list.end());
}

template <class T>
vector<T> &vector<T>::operator=(std::initializer_list<value_type> list) {
    vector temp(list.begin(), list.end());
    swap(temp);
    return *this;
}

template <class T>
vector<T>::~vector() {
    destroy_and_recover(_begin, _end, _cap - _begin);
    _begin = _end = _cap = nullptr;
}

template <class T>
typename vector<T>::iterator vector<T>::begin() noexcept {
    return _begin;
}

template <class T>
typename vector<T>::iterator vector<T>::end() noexcept {
    return _end;
}

template <class T>
typename vector<T>::const_iterator vector<T>::begin() const noexcept {
    return _begin;
}

template <class T>
typename vector<T>::const_iterator vector<T>::end() const noexcept {
    return _end;
}

template <class T>
typename vector<T>::reverse_iterator vector<T>::rbegin() noexcept {
    return reverse_iterator(end());
}

template <class T>
typename vector<T>::const_reverse_iterator vector<T>::rbegin() const noexcept {
    return const_reverse_iterator(end());
}

template <class T>
typename vector<T>::reverse_iterator vector<T>::rend() noexcept {
    return reverse_iterator(begin());
}

template <class T>
typename vector<T>::const_reverse_iterator vector<T>::rend() const noexcept {
    return const_reverse_iterator(begin());
}

template <class T>
typename vector<T>::const_iterator vector<T>::cbegin() const noexcept {
    return begin();
}

template <class T>
typename vector<T>::const_iterator vector<T>::cend() const noexcept {
    return end();
}

template <class T>
typename vector<T>::const_reverse_iterator vector<T>::crbegin() const noexcept {
    return rbegin();
}

template <class T>
typename vector<T>::const_reverse_iterator vector<T>::crend() const noexcept {
    return rend();
}

template <class T>
bool vector<T>::empty() const noexcept {
    return _begin == _end;
}

template <class T>
typename vector<T>::size_type vector<T>::size() const noexcept {
    return static_cast<size_type>(_end - _begin);
}

template <class T>
typename vector<T>::size_type vector<T>::max_size() const noexcept {
    return static_cast<size_type>(-1) / sizeof(T);
}

template <class T>
typename vector<T>::size_type vector<T>::capacity() const noexcept {
    return static_cast<size_type>(_cap - _begin);
}

template <class T>
typename vector<T>::reference vector<T>::operator[](size_type n) {
    return *(_begin + n);
} 

template <class T>
typename vector<T>::const_reference vector<T>::operator[](size_type n) const {
    PHQ_DEBUG(n < size());
    return *(_begin + n);
}

template <class T>
typename vector<T>::reference vector<T>::at(size_type n) {
    THORW_OUT_OF_RANGE_IF(!(n < size()), "out of range");
    return (*this)[n];
}

template <class T>
typename vector<T>::const_reference vector<T>::at(size_type n) const {
    THORW_OUT_OF_RANGE_IF(!(n < size()), "out of range");
    return (*this)[n];
}

template <class T>
typename vector<T>::reference vector<T>::front() {
    return *_begin;
}

template <class T>
typename vector<T>::const_reference vector<T>::front() const {
    return *_begin;
}

template <class T>
typename vector<T>::reference vector<T>::back() {
    return *(_end - 1);
}

template <class T>
typename vector<T>::const_reference vector<T>::back() const {
    return *(_end - 1);
}

template <class T>
typename vector<T>::pointer vector<T>::data() noexcept {
    return _begin;
}

template <class T>
typename vector<T>::const_pointer vector<T>::data() const noexcept {
    return _begin;
}

template <class T>
void vector<T>::assign(size_type n, const value_type &value) {
    fill_assign(n, value);
}

template <class T>
template <class Iter, typename std::enable_if<phq::is_input_iterator<Iter>::value, int>::type>
void vector<T>::assign(Iter first, Iter last) {
    PHQ_DEBUG(!(last < first));
    copy_assign(first, last, iterator_category(first));
}

template <class T>
void vector<T>::assign(std::initializer_list<value_type> list) {
    copy_assign(list.begin(), list.end(), phq::forward_iterator_tag{});
}

template <class T>
template <class... Args>
typename vector<T>::iterator vector<T>::emplace(const_iterator pos, Args &&...args) {
    //PHQ_DEBUG(pos >= _begin && pos <= _end);
    iterator xpos = const_cast<iterator>(pos);
    const size_type n = xpos - _begin;
    if (_end != _cap && _end == xpos) {
        data_allocator::construct(std::addressof(*_end), phq::forward<Args>(args)...);
        ++_end;
    }
    else if (_end != _cap) {
        auto new_end = _end;
        data_allocator::construct(std::addressof(*_end), *(_end - 1));
        ++new_end;
        std::copy_backward(xpos, _end - 1, _end);
        *xpos = value_type(phq::forward<Args>(args)...);
        _end = new_end;
    } else { // 空间不够用，需重新分配
        reallocate_emplace(xpos, phq::forward<Args>(args)...);
    }
    return _begin + n;
}

template <class T>
template <class... Args>
void vector<T>::emplace_back(Args &&...args) {
    if (_end < _cap) {
        data_allocator::construct(std::addressof(*_end), phq::forward<Args>(args)...);
        ++_end;
    } else {
        reallocate_emplace(_end, phq::forward<Args>(args)...);
    }
}

template <class T>
void vector<T>::push_back(const value_type &value) {
    if (_end < _cap) {
        data_allocator::construct(std::addressof(*_end), value);
        ++_end;
    } else {
        reallocate_insert(_end, value);
    }
}

template <class T>
void vector<T>::push_back(value_type &&value) {
    emplace_back(phq::move(value));
}

template <class T>
void vector<T>::pop_back() {
    PHQ_DEBUG(!empty());
    data_allocator::destroy(_end - 1);
    --_end;
}

template <class T>
typename vector<T>::iterator vector<T>::insert(const_iterator pos, const value_type &value) {
    PHQ_DEBUG(pos >= _begin && pos <= _end);
    iterator xpos = const_cast<iterator>(pos);
    const size_type n = pos - _begin;
    if (_end != _cap && pos == _end) {
        data_allocator::construct(std::addressof(*_end), value);
        ++_end;
    } else if(_end != _cap) {
        auto new_end = _end;
        data_allocator::construct(std::addressof(*_end), *(_end - 1));
        ++new_end;
        auto value_copy = value;
        std::copy_backward(xpos, _end - 1, _end);
        *xpos = phq::move(value_copy);
        _end = new_end;
    } else {
        reallocate_insert(pos, value);
    }
    return _begin + n;
}


template <class T>
typename vector<T>::iterator vector<T>::insert(const_iterator pos, value_type &&value) {
    return emplace(pos, phq::move(value));
}

template <class T>
typename vector<T>::iterator vector<T>::insert(const_iterator pos, size_type n, const value_type &value) {
    PHQ_DEBUG(pos >= begin() && pos <= end());
    return fill_insert(const_cast<iterator>(pos), n, value);
}

template <class T>
template <class Iter, typename std::enable_if<phq::is_input_iterator<Iter>::value, int>::type>
void vector<T>::insert(const_iterator pos, Iter first, Iter last) {
    PHQ_DEBUG(pos >= begin() && pos <= end());
    copy_insert(const_cast<iterator>(pos), first, last);
}

template <class T>
typename vector<T>::iterator vector<T>::erase(const_iterator pos) {
    PHQ_DEBUG(pos >= begin() && pos <= _end);
    iterator xpos = _begin + (pos - _begin);
    std::move(xpos + 1, _end, xpos);
    data_allocator::destroy(_end - 1);
    --_end;
    return xpos;
}

template <class T>
typename vector<T>::iterator vector<T>::erase(const_iterator first, const_iterator last) {
    PHQ_DEBUG(first >= _begin && last <= _end);
    const auto n = first - _begin;
    iterator xpos = _begin + (first - _begin);
    data_allocator::destroy(std::move(xpos + (last - first), _end, xpos), _end);
    _end = _end - (last - first);
    return _begin + n;
}

template <class T>
void vector<T>::clear() {
    erase(begin(), end());
}

template <class T>
void vector<T>::resize(size_type new_size) {
    return resize(new_size, value_type());
}

template <class T>
void vector<T>::resize(size_type new_size, const value_type &value) {
    if (new_size < size()) {
        erase(_begin + size(), _end);
    } else {
        insert(_end, new_size - size(), value);
    }
}

template <class T>
void vector<T>::reverse() {
    std::reverse(_begin, _end);
}

template <class T>
void vector<T>::swap(vector &rhs) {
    if (this != &rhs) {
        phq::swap(_begin, rhs._begin);
        phq::swap(_end, rhs._end);
        phq::swap(_cap, rhs._cap);
    }
}

template <class T>
vector<T> &vector<T>::operator=(const vector &rhs) {
    if (this != &rhs) {
        const auto len = rhs.size();
        if (len > capacity()) {
            vector temp(rhs.begin(), rhs.end());
            swap(temp);
        }
        else if (size() >= len) {
            auto i = std::copy(rhs.begin(), rhs.end(), begin());
            data_allocator::destroy(i, end());
            _end = _begin + len;
        } else {
            std::copy(rhs.begin(), rhs.begin() + size(), _begin);
            std::uninitialized_copy(rhs.begin() + size(), rhs.end(), _end);
            _cap = _end = _begin + len;
        }
    }
    return *this;
}

template <class T>
vector<T> &vector<T>::operator=(vector &&rhs) noexcept{
    destroy_and_recover(_begin, _end, _cap - _begin);
    _begin = rhs._begin;
    _end = rhs._end;
    _cap = rhs._cap;
    rhs._begin = nullptr;
    rhs._end = nullptr;
    rhs._cap = nullptr;
    return *this;
}

template <class T>
void vector<T>::reserve(size_type n) {
    if (capacity() < n) {
        THORW_LENGTH_ERROR_IF(n > max_size(), "too larger");
        const auto old_size = size();
        auto new_begin = data_allocator::allocate(n);
        std::uninitialized_move(_begin, _end, new_begin);
        data_allocator::deallocate(_begin, _cap - _begin);
        _begin = new_begin;
        _end = new_begin + old_size;
        _cap = new_begin + n;
    }
}

template <class T>
void vector<T>::shrink_to_fit() {
    if (_end < _cap) {
        reinsert(size());
    }
}

} // namespace phq




#endif