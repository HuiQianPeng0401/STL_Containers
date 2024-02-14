#ifndef _PHQ_DEQUE_H_
#define _PHQ_DEQUE_H_

#include <initializer_list>
#include "iterator.h"
#include "util.h"
#include "exceptdef.h"
#include "allocator.h"
#include <algorithm>
#include <memory>

namespace phq
{
#ifndef DEQUE_MAP_INIT_SIZE
#define DEQUE_MAP_INIT_SIZE 8
#endif

template <class T>
struct deque_buf_size {
    static constexpr size_t value = sizeof(T) < 256 ? 4096 / sizeof(T) : 16;
};

template <class T, class Ref, class Ptr>
struct deque_iterator : public iterator<phq::random_access_iterator_tag, T> {
    typedef deque_iterator<T, T &, T *> iterator;
    typedef deque_iterator<T, const T &, const T *> const_iterator;
    typedef deque_iterator self;
    typedef T value_type;
    typedef Ptr pointer;
    typedef Ref reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef T *value_pointer;
    typedef T **map_pointer;

    static const size_type buffer_size = deque_buf_size<T>::value;

    value_pointer cur; // 指向所在缓冲区的当前元素
    value_pointer first; // 指向所在缓冲区的头部
    value_pointer last; // 指向所在缓冲区的尾部
    map_pointer node; // 缓冲区所在节点

    deque_iterator() noexcept : cur(nullptr), first(nullptr), last(nullptr), node(nullptr){}
    deque_iterator(value_pointer v, map_pointer n) : cur(v), first(*n), last(*n + buffer_size), node(n){}
    deque_iterator(const iterator &rhs) : cur(rhs.cur), first(rhs.first), last(rhs.last), node(rhs.node){}
    deque_iterator(iterator &&rhs) : cur(rhs.cur), first(rhs.first), last(rhs.last), node(rhs.node) {
        rhs.cur = nullptr;
        rhs.first = nullptr;
        rhs.last = nullptr;
        rhs.node = nullptr;
    }
    deque_iterator(const const_iterator &rhs) : cur(rhs.cur), first(rhs.first), last(rhs.last), node(rhs.node){}
    self &operator=(const iterator &rhs) {
        if (this != &rhs) {
            cur = rhs.cur;
            first = rhs.first;
            last = rhs.last;
            node = rhs.node;
        }
        return *this;
    }
    void set_node(map_pointer new_node) {
        node = new_node;
        first = *new_node;
        last = first + buffer_size;
    }

    reference operator*() const { return *cur; }
    pointer operator->() const { return cur; }
    difference_type operator-(const self &x) const {
        return static_cast<difference_type>(buffer_size) * (node - x.node) + (cur - first) - (x.cur - x.first);
    }
    self &operator++() {
        ++cur;
        if (cur == last) {
            set_node(node + 1);
            cur = first;
        }
        return *this;
    }
    self operator++(int) {
        self temp = *this;
        ++*this;
        return temp;
    }
    self &operator--() {
        if (cur == first) {
            set_node(node - 1);
            cur = last;
        }
        --cur;
        return *this;
    }
    self operator--(int) {
        self temp = *this;
        --*this;
        return temp;
    }
                    // 1.看能否在不改变node的情况下进行指针位置的移动
                    // 2.调整node
                    //     offset==(n+x) -->> 最后的位置==first + (n + x)
                    //     x代表cur 距离first的距离,也就是cur当前的下标
                    //     (offset>0): offset/diffenrence_type(buffer_size())
                    //     -->>    y==(n + x)/(sz) -->>移动的node节点个数为y
                    //     (offset<0)   -difference_type((-offset -1) / buffer_size()) - 1
                    //     -->>    y==-(-n - x -1)/(sz) -1 -->> y==(n + x +1)/(sz) -1
                    //     不同于(offset<0)的情况,因为当前处于first位置，移动的距离在
                    //         -1到－(sz)是都应该移动一个node节点所以才取上式子;            
                    // 3.调整cur
                    //     (offset>0) :offset==(n + x)
                    //     node_offset*difference_type(buffer_size())==（n + x -r)
                    //     (r代表余数)
                    //     -->>z(位移量)==r
                    //     (offset<0)  :offset==(n + x)
                    //     node_offset*difference_type(buffer_size())==(n + x +r-sz)
                    //     -->>z(位移量)==-sz + r
                    //     cur =z;
    self &operator+=(difference_type n) { // 此函数是deque中最重要的一个函数
        const auto offset = n + (cur - first);
        if (offset >= 0 && offset < static_cast<difference_type>(buffer_size)) {
            cur += n;
        } else {
            const auto node_offset = offset > 0 ? offset / static_cast<difference_type>(buffer_size) : -static_cast<difference_type>((-offset - 1) / buffer_size) - 1;
            set_node(node + node_offset);
            cur = first + (offset - node_offset * static_cast<difference_type>(buffer_size));
        }
        return *this;
    }
    self operator+(difference_type n) const {
        self temp = *this;
        return temp += n;
    }
    self &operator-=(difference_type n) {
        return *this += -n;
    }
    self operator-(difference_type n) const {
        self temp = *this;
        return temp -= n;
    }
    reference operator[](difference_type n) const { return *(*this + n); }
    bool operator==(const self &rhs) const { return cur == rhs.cur; }
    bool operator<(const self &rhs) const { return node == rhs.node ? (cur < rhs.cur) : (node < rhs.node); }
    bool operator!=(const self &rhs) const { return !(*this == rhs); }
    bool operator>(const self &rhs) const { return rhs < *this; }
    bool operator<=(const self &rhs) const { return !(rhs < *this); }
    bool operator>=(const self &rhs) const { return !(*this < rhs); }
};

template <class T>
class deque {
public:
    typedef allocator<T> allocator_type;
    typedef allocator<T> data_allocator;
    typedef allocator<T *> map_allocator;
    typedef typename allocator_type::value_type value_type;
    typedef typename allocator_type::pointer pointer;
    typedef typename allocator_type::const_pointer const_pointer;
    typedef typename allocator_type::reference reference;
    typedef typename allocator_type::const_reference const_reference;
    typedef typename allocator_type::size_type size_type;
    typedef typename allocator_type::difference_type difference_type;
    typedef pointer *map_pointer;
    typedef deque_iterator<T, T &, T *> iterator;
    typedef deque_iterator<T, const T &, const T *> const_iterator;
    typedef phq::reverse_iterator<iterator> reverse_iterator;
    typedef phq::reverse_iterator<const_iterator> const_reverse_iterator;

    allocator_type get_allocator() { return allocator_type(); }

    static const size_type buffer_size = deque_buf_size<T>::value;

private:
    iterator _begin;
    iterator _end;
    map_pointer _map;
    size_type _map_size;

public:
    deque() { fill_init(0, value_type()); }
    explicit deque(size_type n) { fill_init(n, value_type()); }
    deque(size_type n, const value_type &value) { fill_init(n, value); }
    template <class IIter, typename std::enable_if<phq::is_input_iterator<IIter>::value, int>::type = 0>
    deque(IIter first, IIter last) { copy_init(first, last, iterator_category(first)); }
    deque(std::initializer_list<value_type> ilist) { copy_init(ilist.begin(), ilist.end(), phq::forward_iterator_tag()); }
    deque(const deque &rhs) { copy_init(rhs.begin(), rhs.end(), phq::forward_iterator_tag()); }
    deque(deque &&rhs) noexcept : _begin(phq::move(rhs._begin)), _end(phq::move(rhs._end)), _map(rhs._map), _map_size(rhs._map_size) { rhs._map = nullptr, rhs._map_size = 0; }
    deque &operator=(const deque &rhs);
    deque &operator=(deque &&rhs);
    deque &operator=(std::initializer_list<value_type> ilist) {
        deque temp(ilist);
        swap(temp);
        return *this;
    }
    ~deque() {
        if (_map != nullptr) {
            clear();
            data_allocator::deallocate(*_begin.node, buffer_size);
            *_begin.node = nullptr;
            map_allocator::deallocate(_map, _map_size);
            _map = nullptr;
        }
    }

public:
    iterator begin() noexcept { return _begin; }
    const_iterator begin() const noexcept { return _begin; }
    iterator end() noexcept { return _end; }
    const_iterator end() const noexcept { return _end; }
    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const noexcept { return reverse_iterator(end()); }
    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const noexcept { return reverse_iterator(begin()); }
    const_iterator cbegin() const noexcept { return begin(); }
    const_iterator cend() const noexcept { return end(); }
    const_reverse_iterator crbegin() const noexcept { return rbegin(); }
    const_reverse_iterator crend() const noexcept { return rend(); }

    bool empty() const noexcept { return begin() == end(); }
    size_type size() const noexcept { return _end - _begin; }
    size_type max_size() const noexcept { return static_cast<size_type>(-1); }
    void resize(size_type new_size) { resize(new_size, value_type()); }
    void resize(size_type new_size, const value_type &value);
    void shrink_to_fit() noexcept;
    reference operator[](size_type n) {
        PHQ_DEBUG(n < size());
        return _begin[n];
    }
    const_reference operator[](size_type n) const {
        PHQ_DEBUG(n < size());
        return _begin[n];
    }
    reference at(size_type n) {
        THORW_OUT_OF_RANGE_IF(!(n < size()), "deque<T>::at() subscript out of range");
        return (*this)[n];
    }
    const_reference at(size_type n) const {
        THORW_OUT_OF_RANGE_IF(!(n < size()), "deque<T>::at() subscript out of range");
        return (*this)[n];
    }
    reference front() {
        PHQ_DEBUG(!empty());
        return *begin();
    }
    const_reference front() const {
        PHQ_DEBUG(!empty());
        return *begin();
    }
    reference back() {
        PHQ_DEBUG(!empty());
        return *(end() - 1);
    }
    const_reference back() const {
        PHQ_DEBUG(!empty());
        return *(end() - 1);
    }
    void assign(size_type n, const value_type &value) { fill_assign(n, value); }
    template <class IIter, typename std::enable_if<phq::is_input_iterator<IIter>::value, int>::type = 0>
    void assign(IIter first, IIter last) { copy_assign(first, last, iterator_category(first)); }
    void assign(std::initializer_list<value_type> ilist) { copy_assign(ilist.begin(), ilist.end(), phq::forward_iterator_tag{}); }
    template <class... Args>
    void emplace_front(Args &&...args);
    template <class... Args>
    void emplace_back(Args &&...args);
    template <class... Args>
    iterator emplace(iterator pos, Args &&...args);
    void push_front(const value_type &value);
    void push_front(value_type &&value) { emplace_front(phq::move(value)); }
    void push_back(const value_type &value);
    void push_back(value_type &&value) { emplace_back(phq::move(value)); }
    void pop_front();
    void pop_back();
    iterator insert(iterator pos, const value_type &value);
    iterator insert(iterator pos, value_type &&value);
    void insert(iterator pos, size_type n, const value_type &value);
    template <class IIter, typename std::enable_if<phq::is_input_iterator<IIter>::value, int>::type = 0>
    void insert(iterator pos, IIter first, IIter last) { insert_dispatch(pos, first, last, iterator_category(first)); }
    iterator erase(iterator pos);
    iterator erase(iterator first, iterator last);
    void clear();
    void swap(deque &rhs) noexcept;

private:
    map_pointer create_map(size_type size);
    void create_buffer(map_pointer nstart, map_pointer nfinish);
    void destroy_buffer(map_pointer nstart, map_pointer nfinish);
    void map_init(size_type n);
    void fill_init(size_type n, const value_type &value);
    template <class IIter>
    void copy_init(IIter, IIter, input_iterator_tag);
    template <class FIter>
    void copy_init(FIter, FIter, forward_iterator_tag);
    void fill_assign(size_type n, const value_type &value);
    template <class IIter>
    void copy_assign(IIter first, IIter last, input_iterator_tag);
    template <class FIter>
    void copy_assign(FIter first, FIter last, forward_iterator_tag);
    template <class... Args>
    iterator insert_aux(iterator pos, Args &&...args);
    void fill_insert(iterator pos, size_type n, const value_type &value);
    template <class FIter>
    void copy_insert(iterator, FIter, FIter, size_type);
    template <class IIter>
    void insert_dispatch(iterator, IIter, IIter, input_iterator_tag);
    template <class FIter>
    void insert_dispatch(iterator, FIter, FIter, forward_iterator_tag);
    void require_capacity(size_type n, bool front);
    void reallocate_map_at_front(size_type need);
    void reallocate_map_at_back(size_type need);
};

template <class T>
void deque<T>::clear() {
    for (map_pointer cur = _begin.node + 1; cur < _end.node; ++cur) {
        data_allocator::destroy(*cur, *cur + buffer_size);
    }
    if (_begin.node != _end.node) {
        phq::destroy(_begin.cur, _begin.last);
        phq::destroy(_end.first, _end.cur);
    } else {
        phq::destroy(_begin.cur, _end.cur);
    }
    shrink_to_fit();
    _end = _begin;
}

template <class T>
void deque<T>::shrink_to_fit() noexcept {
    for (auto cur = _map; cur < _begin.node; ++cur) {
        data_allocator::deallocate(*cur, buffer_size);
        *cur = nullptr;
    }
    for (auto cur = _end.node + 1; cur < _map + _map_size; ++cur) {
        data_allocator::deallocate(*cur, buffer_size);
        *cur = nullptr;
    }
}

template <class T>
typename deque<T>::map_pointer deque<T>::create_map(size_type size) {
    map_pointer p = nullptr;
    p = map_allocator::allocate(size);
    for (size_type i = 0; i < size; ++i)
        *(p + i) = nullptr;
    return p;
}

template <class T>
void deque<T>::create_buffer(map_pointer nstart, map_pointer nfinish) {
    map_pointer cur;
    try {
        for (cur = nstart; cur <= nfinish; ++cur) {
            *cur = data_allocator::allocate(buffer_size);
        }
    } catch(...) {
        while (cur != nstart) {
            --cur;
            data_allocator::deallocate(*cur, buffer_size);
            *cur = nullptr;
        }
        throw;
    }
}

template <class T>
void deque<T>::destroy_buffer(map_pointer nstart, map_pointer nfinish) {
    for (map_pointer n = nstart; n <= nfinish; ++n) {
        data_allocator::deallocate(*n, buffer_size);
        *n = nullptr;
    }
}

template <class T>
void deque<T>::map_init(size_type n) {
    const size_type nNode = n / buffer_size + 1;
    _map_size = std::max(static_cast<size_type>(DEQUE_MAP_INIT_SIZE), nNode + 2);
    try {
        _map = create_map(_map_size);
    } catch(...) {
        _map = nullptr;
        _map_size = 0;
        throw;
    }
    // 移至中间方便向头尾扩充
    map_pointer nstart = _map + (_map_size - nNode) / 2;
    map_pointer nfinish = nstart + nNode - 1;
    try {
        create_buffer(nstart, nfinish);
    } catch(...) {
        map_allocator::deallocate(_map, _map_size);
        _map = nullptr;
        _map_size = 0;
        throw;
    }
    _begin.set_node(nstart);
    _end.set_node(nfinish);
    _begin.cur = _begin.first;
    _end.cur = _end.first + (n % buffer_size);
}

template <class T>
void deque<T>::fill_init(size_type n, const value_type &value) {
    map_init(n);
    if (n != 0) {
        for (auto cur = _begin.node; cur < _end.node; ++cur) {
            std::uninitialized_fill(*cur, *cur + buffer_size, value);
        }
        std::uninitialized_fill(_end.first, _end.cur, value);
    }
}

template <class T>
template <class IIter>
void deque<T>::copy_init(IIter first, IIter last, input_iterator_tag) {
    const size_type n = phq::distance(first, last);
    map_init(n);
    for (; first != last; ++first) {
        emplace_back(*first);
    }
}

template <class T>
template <class FIter>
void deque<T>::copy_init(FIter first, FIter last, forward_iterator_tag) {
    const size_type n = phq::distance(first, last);
    map_init(n);
    for (auto cur = _begin.node; cur < _end.node; ++cur) {
        auto next = first;
        phq::advance(next, buffer_size);
        std::uninitialized_copy(first, next, *cur);
        first = next;
    }
    std::uninitialized_copy(first, last, _end.first);
}

template <class T>
void deque<T>::fill_assign(size_type n, const value_type &value) {
    if (n > size()) {
        std::fill(begin(), end(), value);
        insert(end(), n - size(), value);
    } else {
        erase(begin() + n, end());
        std::fill(begin(), end(), value);
    }
}

template <class T>
template <class IIter>
void deque<T>::copy_assign(IIter first, IIter last, input_iterator_tag) {
    auto first1 = begin();
    auto last1 = end();
    for (; first != last && first1 != last1; ++first, ++first1) {
        *first1 = *first;
    }
    if (first1 != last1) {
        erase(first1, last1);
    } else {
        insert_dispatch(_end, first, last, input_iterator_tag{});
    }
}

template <class T>
template <class FIter>
void deque<T>::copy_assign(FIter first, FIter last, forward_iterator_tag) {
    const size_type len1 = size();
    const size_type len2 = phq::distance(first, last);
    if (len1 < len2) {
        auto next = first;
        phq::advance(next, len1);
        std::copy(first, next, _begin);
        insert_dispatch(_end, next, last, forward_iterator_tag{});
    } else {
        erase(std::copy(first, last, _begin), _end);
    }
}

template <class T>
template <class... Args>
typename deque<T>::iterator deque<T>::insert_aux(iterator pos, Args &&...args) {
    const size_type elems_before = pos - _begin;
    value_type value_copy = value_type(phq::forward<Args>(args)...);
    if (elems_before < (size() / 2)) {
        emplace_front(front());
        auto front1 = _begin;
        ++front1;
        auto front2 = front1;
        ++front2;
        pos = _begin + elems_before;
        auto idx = pos;
        ++idx;
        std::copy(front2, idx, front1);
    } else {
        emplace_back(back());
        auto back1 = _end;
        --back1;
        auto back2 = back1;
        --back2;
        pos = _begin + elems_before;
        std::copy_backward(pos, back2, back1);
    }
    *pos = phq::move(value_copy);
    return pos;
}

template <class T>
void deque<T>::fill_insert(iterator pos, size_type n, const value_type &value) {
    const size_type elems_before = pos - _begin;
    const size_type len = size();
    auto value_copy = value;
    if (elems_before < (len / 2)) {
        require_capacity(n, true);
        auto old_begin = _begin;
        auto new_begin = _begin - n;
        pos = _begin + elems_before;
        try {
            if (elems_before >= n) {
                auto begin_n = _begin + n;
                std::uninitialized_copy(_begin, begin_n, new_begin);
                _begin = new_begin;
                std::copy(begin_n, pos, old_begin);
                std::fill(pos - n, pos, value_copy);
            } else {
                std::uninitialized_fill(std::uninitialized_copy(_begin, pos, new_begin), _begin, value_copy);
                _begin = new_begin;
                std::fill(old_begin, pos, value_copy);
            }
        } catch(...) {
            if (new_begin.node != _begin.node) {
                destroy_buffer(new_begin.node, _begin.node - 1);
            }
            throw;
        }
    } else {
        require_capacity(n, false);
        auto old_end = _end;
        auto new_end = _end + n;
        const size_type elems_after = len - elems_before;
        pos = _end - elems_after;
        try {
            if (elems_after > n) {
                auto end_n = _end - n;
                std::uninitialized_copy(end_n, _end, _end);
                _end = new_end;
                std::copy_backward(pos, end_n, old_end);
                std::fill(pos, pos + n, value_copy);
            } else {
                std::uninitialized_fill(_end, pos + n, value_copy);
                std::uninitialized_copy(pos, _end, pos + n);
                _end = new_end;
                std::fill(pos, old_end, value_copy);
            }
        } catch(...) {
            if (new_end.node != _end.node) {
                destroy_buffer(_end.node + 1, new_end.node);
            }
            throw;
        }
    }
}

template <class T>
void deque<T>::require_capacity(size_type n, bool front) {
    if (front && (static_cast<size_type>(_begin.cur - _begin.first) < n)) {
        const size_type need_buffer = (n - (_begin.cur - _begin.first)) / buffer_size + 1;
        if (need_buffer > static_cast<size_type>(_begin.node - _map)) {
            reallocate_map_at_front(need_buffer);
            return;
        }
        create_buffer(_begin.node - need_buffer, _begin.node - 1);
    } else if (!front && (static_cast<size_type>(_end.last - _end.cur - 1) < n)) {
        const size_type need_buffer = (n - (_end.last - _end.cur - 1)) / buffer_size + 1;
        if (need_buffer > static_cast<size_type>((_map + _map_size) - _end.node - 1)) {
            reallocate_map_at_back(need_buffer);
            return;
        }
        create_buffer(_end.node + 1, _end.node + need_buffer);
    }
}

template <class T>
void deque<T>::reallocate_map_at_front(size_type need_buffer) {
    const size_type new_map_size = std::max(_map_size << 1, _map_size + need_buffer + DEQUE_MAP_INIT_SIZE);
    map_pointer new_map = create_map(new_map_size);
    const size_type old_buffer = _end.node - _begin.node + 1;
    const size_type new_buffer = old_buffer + need_buffer;
    auto start = new_map + (new_map_size - new_buffer) / 2;
    auto mid = start + need_buffer;
    auto finish = mid + old_buffer;
    create_buffer(start, mid - 1);
    for (auto s1 = mid, s2 = _begin.node; s1 != finish; ++s1, ++s2) {
        *s1 = *s2;
    }
    map_allocator::deallocate(_map, _map_size);
    _map = new_map;
    _map_size = new_map_size;
    _begin = iterator(*mid + (_begin.cur - _begin.first), mid);
    _end = iterator(*(finish - 1) + (_end.cur - _end.first), finish - 1);
}

template <class T>
void deque<T>::reallocate_map_at_back(size_type need_buffer) {
    const size_type new_map_size = std::max(_map_size << 1, _map_size + need_buffer + DEQUE_MAP_INIT_SIZE);
    map_pointer new_map = create_map(new_map_size);
    const size_type old_buffer = _end.node - _begin.node + 1;
    const size_type new_buffer = old_buffer + need_buffer;
    auto start = new_map + ((new_map_size - new_buffer) / 2);
    auto mid = start + old_buffer;
    auto finish = mid + need_buffer;
    for (auto s1 = start, s2 = _begin.node; s1 != mid; ++s1, ++s2) {
        *s1 = *s2;
    }
    create_buffer(mid, finish - 1);
    map_allocator::deallocate(_map, _map_size);
    _map = new_map;
    _map_size = new_map_size;
    _begin = iterator(*start + (_begin.cur - _begin.first), start);
    _end = iterator(*(mid - 1) + (_end.cur - _end.first), mid - 1);
}

template <class T>
template <class FIter>
void deque<T>::copy_insert(iterator pos, FIter first, FIter last, size_type n) {
    const size_type elems_before = pos - _begin;
    auto len = size();
    if (elems_before < (len / 2)) {
        require_capacity(n, true);
        auto old_begin = _begin;
        auto new_begin = _begin - n;
        pos = _begin + elems_before;
        try {
            if (elems_before >= n) {
                auto begin_n = _begin + n;
                std::uninitialized_copy(_begin, begin_n, new_begin);
                _begin = new_begin;
                std::copy(begin_n, pos, old_begin);
                std::copy(first, last, pos - n);
            } else {
                auto mid = first;
                phq::advance(mid, n - elems_before);
                std::uninitialized_copy(first, mid, std::uninitialized_copy(_begin, pos, new_begin));
                _begin = new_begin;
                std::copy(mid, last, old_begin);
            }
        } catch(...) {
            if (new_begin.node != _begin.node) {
                destroy_buffer(new_begin.node, _begin.node - 1);
            }
            throw;
        }
    } else {
        require_capacity(n, false);
        auto old_end = _end;
        auto new_end = _end + n;
        const size_type elems_after = len - elems_before;
        pos = _end - elems_after;
        try {
            if (elems_after > n) {
                auto end_n = _end - n;
                std::uninitialized_copy(end_n, _end, _end);
                _end = new_end;
                std::copy_backward(pos, end_n, old_end);
                std::copy(first, last, pos);
            } else {
                auto mid = first;
                phq::advance(mid, elems_after);
                std::uninitialized_copy(pos, _end, std::uninitialized_copy(mid, last, _end));
                _end = new_end;
                std::copy(first, mid, pos);
            }
        } catch(...) {
            if (new_end.node != _end.node) {
                destroy_buffer(_end.node + 1, new_end.node);
            }
            throw;
        }
    }
}

template <class T>
template <class IIter>
void deque<T>::insert_dispatch(iterator pos, IIter first, IIter last, input_iterator_tag) {
    if (last <= first)
        return;
    const size_type n = phq::distance(first, last);
    const size_type elems_before = pos - _begin;
    if (elems_before < (size() / 2)) {
        require_capacity(n, true);
    } else {
        require_capacity(n, false);
    }
    pos = _begin + elems_before;
    auto cur = --last;
    for (size_type i = 0; i < n; ++i, --cur) {
        insert(pos, *cur);
    }
}

template <class T>
template <class FIter>
void deque<T>::insert_dispatch(iterator pos, FIter first, FIter last, forward_iterator_tag) {
    if (last <= first)
        return;
    const size_type n = phq::distance(first, last);
    if (pos.cur == _begin.cur) {
        require_capacity(n, true);
        auto new_begin = _begin - n;
        try {
            std::uninitialized_copy(first, last, new_begin);
            _begin = new_begin;
        } catch(...) {
            if (new_begin.node != _begin.node) {
                destroy_buffer(new_begin.node, _begin.node - 1);
            }
            throw;
        }
    } else if (pos.cur == _end.cur) {
        require_capacity(n, false);
        auto new_end = _end + n;
        try {
            std::uninitialized_copy(first, last, _end);
            _end = new_end;
        } catch(...) {
            if (new_end.node != _end.node) {
                destroy_buffer(_end.node + 1, new_end.node);
            }
            throw;
        }
    } else {
        copy_insert(pos, first, last, n);
    }
}

template <class T>
deque<T> &deque<T>::operator=(const deque<T> &rhs) {
    if (this != &rhs) {
        const auto len = size();
        if (len >= rhs.size()) {
            erase(std::copy(rhs._begin, rhs._end, _begin), _end);
        } else {
            iterator mid = rhs._begin + static_cast<difference_type>(len);
            std::copy(rhs._begin, rhs._end, _begin);
            insert(_end, mid, rhs._end);
        }
    }
    return *this;
}

template <class T>
deque<T> &deque<T>::operator=(deque &&rhs) {
    clear();
    _begin = phq::move(rhs._begin);
    _end = phq::move(rhs._end);
    _map = rhs._map;
    _map_size = rhs._map_size;
    rhs._map = nullptr;
    rhs._map_size = 0;
    return *this;
}

template <class T>
void deque<T>::resize(size_type new_size, const value_type &value) {
    const auto len = size();
    if (new_size < len) {
        erase(_begin + new_size, _end);
    } else {
        insert(_end, new_size - len, value);
    }
}

template <class T>
template <class... Args>
void deque<T>::emplace_front(Args &&...args) {
    if (_begin.cur != _begin.first) {
        data_allocator::construct(_begin.cur - 1, phq::forward<Args>(args)...);
        --_begin.cur;
    } else {
        require_capacity(1, true);
        try {
            --_begin;
            data_allocator::construct(_begin.cur, phq::forward<Args>(args)...);
        } catch(...) {
            ++_begin;
            throw;
        }
    }
}

template <class T>
template <class... Args>
void deque<T>::emplace_back(Args &&...args) {
    if (_end.cur != _end.last - 1) {
        data_allocator::construct(_end.cur, phq::forward<Args>(args)...);
        ++_end.cur;
    } else {
        require_capacity(1, false);
        data_allocator::construct(_end.cur, phq::forward<Args>(args)...);
        ++_end;
    }
}

template <class T>
template <class... Args>
typename deque<T>::iterator deque<T>::emplace(iterator pos, Args &&...args) {
    if (pos.cur == _begin.cur) {
        emplace_front(phq::forward<Args>(args)...);
        return _begin;
    } else if(pos.cur == _end.cur) {
        emplace_back(phq::forward<Args>(args)...);
        return _end - 1;
    }
    return insert_aux(pos, phq::forward<Args>(args)...);
}

template <class T>
void deque<T>::push_front(const value_type &value) {
    if (_begin.cur != _begin.first) {
        data_allocator::construct(_begin.cur - 1, value);
        --_begin.cur;
    } else {
        require_capacity(1, true);
        try {
            --_begin;
            data_allocator::construct(_begin.cur, value);
        } catch(...) {
            ++_begin;
            throw;
        }
    }
}

template <class T>
void deque<T>::push_back(const value_type &value) {
    if (_end.cur != _end.last - 1) {
        data_allocator::construct(_end.cur, value);
        ++_end.cur;
    } else {
        require_capacity(1, false);
        data_allocator::construct(_end.cur, value);
        ++_end;
    }
}

template <class T>
void deque<T>::pop_front() {
    PHQ_DEBUG(!empty());
    if (_begin.cur != _begin.last - 1) {
        data_allocator::destroy(_begin.cur);
        ++_begin.cur;
    } else {
        data_allocator::destroy(_begin.cur);
        ++_begin;
        destroy_buffer(_begin.node - 1, _begin.node - 1);
    }
}

template <class T>
void deque<T>::pop_back() {
    PHQ_DEBUG(!empty());
    if (_end.cur != _end.first) {
        --_end.cur;
        data_allocator::destroy(_end.cur);
    } else {
        --_end;
        data_allocator::destroy(_end.cur);
        destroy_buffer(_end.node + 1, _end.node + 1);
    }
}

template <class T>
typename deque<T>::iterator deque<T>::insert(iterator pos, const value_type &value) {
    if (pos.cur == _begin.cur) {
        push_front(value);
        return _begin;
    } else if(pos.cur == _end.cur) {
        push_back(value);
        auto temp = _end;
        --_end;
        return temp;
    } else {
        insert_aux(pos, value);
    }
}

template <class T>
typename deque<T>::iterator deque<T>::insert(iterator pos, value_type &&value) {
    if (pos.cur == _begin.cur) {
        emplace_front(phq::move(value));
        return _begin;
    } else if(pos.cur == _end.cur) {
        emplace_back(phq::move(value));
        return _end - 1;
    } else {
        return insert_aux(pos, phq::move(value));
    }
}

template <class T>
void deque<T>::insert(iterator pos, size_type n, const value_type &value) {
    if (pos.cur == _begin.cur) {
        require_capacity(n, true);
        auto new_begin = _begin - n;
        std::uninitialized_fill_n(new_begin, n, value);
        _begin = new_begin;
    } else if(pos.cur == _end.cur) {
        require_capacity(n, false);
        auto new_end = _end + n;
        std::uninitialized_fill_n(_end, n, value);
        _end = new_end;
    } else {
        fill_insert(pos, n, value);
    }
}

template <class T>
typename deque<T>::iterator deque<T>::erase(iterator pos) {
    auto next = pos;
    ++next;
    const size_type elems_before = pos - _begin;
    if (elems_before < (size() / 2)) {
        std::copy_backward(_begin, pos, next);
        pop_front();
    } else {
        std::copy(next, _end, pos);
        pop_back();
    }
    return _begin + elems_before;
}

template <class T>
typename deque<T>::iterator deque<T>::erase(iterator first, iterator last) {
    if (first == _begin && last == _end) {
        clear();
        return end();
    } else {
        const size_type len = last - first;
        const size_type elems_before = first - _begin;
        if (elems_before < (size() - len) / 2) {
            std::copy_backward(_begin, first, last);
            // 猜测copy_backward两个都是从后往前赋值，即last--->,first--->_begin
            auto new_begin = _begin + len;
            data_allocator::destroy(_begin.cur, new_begin.cur);
            _begin = new_begin;
        } else {
            std::copy(last, _end, first);
            auto new_end = _end - len;
            data_allocator::destroy(new_end.cur, _end.cur);
            _end = new_end;
        }
        return _begin + elems_before;
    }
}

template <class T>
void deque<T>::swap(deque &rhs) noexcept {
    if (this != &rhs) {
        phq::swap(_begin, rhs._begin);
        phq::swap(_end, rhs._end);
        phq::swap(_map, rhs._map);
        phq::swap(_map_size, rhs._map_size);
    }
}

template <class T>
bool operator==(const deque<T> &lhs, const deque<T> &rhs) {
    return lhs.size() == rhs.size() && std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <class T>
bool operator<(const deque<T> &lhs, const deque<T> &rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <class T>
bool operator!=(const deque<T> &lhs, const deque<T> &rhs) {
    return !(lhs == rhs);
}

template <class T>
bool operator>(const deque<T> &lhs, const deque<T> &rhs) {
    return rhs < lhs;
}

template <class T>
bool operator<=(const deque<T> &lhs, const deque<T> &rhs) {
    return !(rhs < lhs);
}

template <class T>
bool operator>=(const deque<T> &lhs, const deque<T> &rhs) {
    return !(lhs < rhs);
}

template <class T>
void swap(deque<T> &lhs, deque<T> &rhs) {
    lhs.swap(rhs);
}

} // namespace phq



#endif