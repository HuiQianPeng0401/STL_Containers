#ifndef _PHQ_LIST_H_
#define _PHQ_LIST_H_

#include <initializer_list>
#include "iterator.h"
#include "util.h"
#include "exceptdef.h"
#include "allocator.h"

namespace phq
{

template <class T>
struct list_node_base;
template <class T>
struct list_node;

template <class T>
struct node_traits {
    typedef list_node_base<T> *base_ptr;
    typedef list_node<T> *node_ptr;
};

template <class T>
struct list_node_base {
    typedef typename node_traits<T>::base_ptr base_ptr;
    typedef typename node_traits<T>::node_ptr node_ptr;
    base_ptr prev;
    base_ptr next;
    list_node_base() = default;
    base_ptr self() { return static_cast<base_ptr>(&*this); }
    void unlink() { prev = next = self(); }
    node_ptr as_node() { return static_cast<node_ptr>(self()); }
};

template <class T>
struct list_node : public list_node_base<T> {
    typedef typename node_traits<T>::base_ptr base_ptr;
    typedef typename node_traits<T>::node_ptr node_ptr;
    T value;
    list_node() = default;
    list_node(const T &v) : value(v){}
    list_node(T &&v) : value(phq::move(v)){}
    base_ptr as_base() { return static_cast<base_ptr>(&*this); }
    node_ptr self() { return static_cast<node_ptr>(&*this); }
};

template <class T>
struct list_iterator : public phq::iterator<phq::bidirectional_iterator_tag, T> {
    typedef T value_type;
    typedef T *pointer;
    typedef T &reference;
    typedef typename node_traits<T>::base_ptr base_ptr;
    typedef typename node_traits<T>::node_ptr node_ptr;
    typedef list_iterator<T> self;

    base_ptr _node;

    list_iterator() = default;
    list_iterator(base_ptr x) : _node(x){}
    list_iterator(node_ptr x) : _node(x->as_base()){}
    list_iterator(const list_iterator &rhs) : _node(rhs._node){}

    reference operator*() const { return _node->as_node()->value; }
    pointer operator->() const { return &(operator*()); }

    self &operator++() {
        PHQ_DEBUG(_node != nullptr);
        _node = _node->next;
        return *this;
    }
    self operator++(int) {
        self temp = *this;
        ++*this;
        return temp;
    }
    self &operator--() {
        PHQ_DEBUG(_node != nullptr);
        _node = _node->prev;
        return *this;
    }
    self operator--(int) {
        self temp = *this;
        --*this;
        return temp;
    }
    bool operator==(const self &rhs) const { return _node == rhs._node; }
    bool operator!=(const self &rhs) const { return _node != rhs._node; }
};

template <class T>
struct list_const_iterator : public iterator<bidirectional_iterator_tag, T> {
    typedef T value_type;
    typedef const T *pointer;
    typedef const T &reference;
    typedef typename node_traits<T>::base_ptr base_ptr;
    typedef typename node_traits<T>::node_ptr node_ptr;
    typedef list_const_iterator<T> self;

    base_ptr _node;

    list_const_iterator() = default;
    list_const_iterator(base_ptr x) : _node(x){}
    list_const_iterator(node_ptr x) : _node(x->as_base()){}
    list_const_iterator(const list_const_iterator &rhs) : _node(rhs._node){}
    list_const_iterator(const list_iterator<T> &rhs) : _node(rhs._node){}

    reference operator*() const { return _node->as_node()->value; }
    pointer operator->() const { return &(operator*()); }

    self &operator++() {
        PHQ_DEBUG(_node != nullptr);
        _node = _node->next;
        return *this;
    }
    self operator++(int) {
        self temp = *this;
        ++*this;
        return temp;
    }
    self &operator--() {
        PHQ_DEBUG(_node != nullptr);
        _node = _node->prev;
        return *this;
    }
    self operator--(int) {
        self temp = *this;
        --*this;
        return temp;
    }

    bool operator==(const self &rhs) const { return _node == rhs._node; }
    bool operator!=(const self &rhs) const { return _node != rhs._node; }
};

template <class T>
class list { // 首先要清楚list是一个环状双向链表
    typedef phq::allocator<T> allocator_type;
    typedef phq::allocator<T> data_allocator;
    typedef phq::allocator<list_node<T>> node_allocator;
    typedef phq::allocator<list_node_base<T>> base_allocator;
    typedef typename allocator_type::value_type value_type;
    typedef typename allocator_type::pointer pointer;
    typedef typename allocator_type::const_pointer const_pointer;
    typedef typename allocator_type::reference reference;
    typedef typename allocator_type::const_reference const_reference;
    typedef typename allocator_type::size_type size_type;
    typedef typename allocator_type::difference_type difference_type;
    typedef list_iterator<T> iterator;
    typedef list_const_iterator<T> const_iterator;
    typedef phq::reverse_iterator<iterator> reverse_iterator;
    typedef phq::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef typename node_traits<T>::base_ptr base_ptr;
    typedef typename node_traits<T>::node_ptr node_ptr;

    allocator_type get_allocator() { return node_allocator(); }

private:
    base_ptr _node;//list是一个环状双向链表，_node指向刻意置于尾端的一个空白节点
    size_type _size;

public:
    list();
    explicit list(size_type n);
    list(size_type n, const T &value) { fill_init(n, value); }
    template <class Iter, typename std::enable_if<phq::is_input_iterator<Iter>::value, int>::type = 0>
    list(Iter first, Iter last);
    list(std::initializer_list<T> ilist);
    list(const list &rhs);
    list(list &&rhs) noexcept;
    list &operator=(const list &rhs);
    list &operator=(list &&rhs) noexcept;
    list &operator=(std::initializer_list<T> ilist);
    ~list();

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

public:
    bool empty() const noexcept;
    size_type size() const noexcept;
    size_type max_size() const noexcept;
    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;
    void assign(size_type n, const value_type &value);
    template <class Iter, typename std::enable_if<phq::is_input_iterator<Iter>::value, int>::type = 0>
    void assign(Iter first, Iter last);
    void assign(std::initializer_list<T> ilist);
    template <class... Args>
    void emplace_front(Args &&...args);
    template <class... Args>
    void emplace_back(Args &&...args);
    template <class... Args>
    iterator emplace(const_iterator pos, Args &&...args);
    iterator insert(const_iterator pos, value_type &&value);
    iterator insert(const_iterator pos, const value_type &value);
    iterator insert(const_iterator pos, size_type n, const value_type &value);
    template <class Iter, typename std::enable_if<phq::is_input_iterator<Iter>::value, int>::type = 0>
    iterator insert(const_iterator pos, Iter first, Iter last);
    void push_back(const value_type &value);
    void push_back(value_type &&value);
    void push_front(const value_type &value);
    void push_front(value_type &&value);
    void pop_front();
    void pop_back();
    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);
    void clear();
    void resize(size_type new_size);
    void resize(size_type new_size, const value_type &value);
    void swap(list &rhs) noexcept;
    void splice(const_iterator pos, list &other);//将other接在pos之前
    void splice(const_iterator pos, list &other, const_iterator it);//将it所指节点接在pos之前
    void splice(const_iterator pos, list &other, const_iterator first, const_iterator last);//将other的[first,last)接在pos之前
    void remove(const value_type &value);
    template <class UnaryPredicate>
    void remove_if(UnaryPredicate pred);
    void unique();
    template <class BinaryPredicate>
    void unique(BinaryPredicate pred);
    void merge(list &x);
    template <class Compare>
    void merge(list &x, Compare cmp);
    void sort();
    template <class Compared>
    void sort(Compared cmp);
    void reverse();

private: // helper functions
    template <class... Args>
    node_ptr create_node(Args &&...args);
    void destroy_node(node_ptr p);
    void fill_init(size_type n, const value_type &value);
    template <class Iter>
    void copy_init(Iter first, Iter last);
    iterator link_iter_node(const_iterator pos, base_ptr node);
    void link_nodes(base_ptr p, base_ptr first, base_ptr last);
    void link_nodes_at_front(base_ptr first, base_ptr last);
    void link_nodes_at_back(base_ptr first, base_ptr last);
    void unlink_nodes(base_ptr first, base_ptr last);
    void fill_assign(size_type n, const value_type &value);
    template <class Iter>
    void copy_assign(Iter first, Iter last);
    iterator fill_insert(const_iterator pos, size_type n, const value_type &value);
    template <class Iter>
    iterator copy_insert(const_iterator pos, size_type n, Iter first);
    template <class Compared>
    iterator list_sort(iterator first, iterator last, size_type n, Compared comp);
    template <class Compared>
    iterator merge_sort(iterator first, iterator last, size_type n, Compared comp);
    template <class Compared>
    void merges(iterator lo, iterator mi, iterator hi, Compared comp);
};

template <class T>
template <class... Args>
typename list<T>::node_ptr list<T>::create_node(Args &&...args) {
    node_ptr p = node_allocator::allocate(1);
    try {
        data_allocator::construct(std::addressof(p->value), phq::forward<Args>(args)...);
        p->prev = nullptr;
        p->next = nullptr;
    } catch(...) {
        node_allocator::deallocate(p);
        throw;
    }
    return p;
}

template <class T>
void list<T>::destroy_node(node_ptr p) {
    data_allocator::destroy(std::addressof(p->value));
    node_allocator::deallocate(p);
}

template <class T>
void list<T>::fill_init(size_type n, const value_type &value) {
    _node = base_allocator::allocate(1);
    _node->unlink();
    _size = n;
    try {
        while (n > 0) {
            auto node = create_node(value);
            link_nodes_at_back(node->as_base(), node->as_base());
            --n;
        }
    } catch(...) {
        clear();
        base_allocator::deallocate(_node);
        _node = nullptr;
        throw;
    }
}

template <class T>
template <class Iter>
void list<T>::copy_init(Iter first, Iter last) {
    _node = base_allocator::allocate(1);
    _node->unlink();
    size_type n = phq::distance(first, last);
    _size = n;
    try {
        while (n > 0) {
            auto node = create_node(*first);
            link_nodes_at_back(node->as_base(), node->as_base());
            --n;
            ++first;
        }
    } catch(...) {
        clear();
        base_allocator::deallocate(_node);
        _node = nullptr;
        throw;
    }
}

template <class T>
typename list<T>::iterator list<T>::link_iter_node(const_iterator pos, base_ptr link_node) {
    if (pos == _node->next) {
        link_nodes_at_front(link_node, link_node);
    }
    else if (pos == _node) {
        link_nodes_at_back(link_node, link_node);
    }
    else {
        link_nodes(pos._node, link_node, link_node);
    }
    return iterator(link_node); 
}

template <class T>
void list<T>::link_nodes(base_ptr pos, base_ptr first, base_ptr last) {
    pos->prev->next = first;
    first->prev = pos->prev;
    pos->prev = last;
    last->next = pos;
}

template <class T>
void list<T>::link_nodes_at_front(base_ptr first, base_ptr last) {
    first->prev = _node;
    last->next = _node->next;
    last->next->prev = last;
    _node->next = first;
}

template <class T>
void list<T>::link_nodes_at_back(base_ptr first, base_ptr last) {
    last->next = _node;
    first->prev = _node->prev;
    first->prev->next = first;
    _node->prev = last;
}

template <class T>
void list<T>::unlink_nodes(base_ptr first, base_ptr last) {
    first->prev->next = last->next;
    last->next->prev = first->prev;
}

template <class T>
void list<T>::fill_assign(size_type n, const value_type &value) {
    auto b = begin();
    auto e = end();
    for (; n > 0 && b != e; --n, ++b) {
        *b = value;
    }
    if (n > 0) {
        insert(e, n, value);
    } else {
        erase(b, e);
    }
}

template <class T>
template <class Iter>
void list<T>::copy_assign(Iter first, Iter last) {
    auto b = begin();
    auto e = end();
    for (; b != e && first != last; ++b, ++first) {
        *b = *first;
    }
    if (first == last) {
        erase(b, e);
    }
    else {
        insert(b, first, last);
    }
}

template <class T>
typename list<T>::iterator list<T>::fill_insert(const_iterator pos, size_type n, const value_type &value) {
    iterator r(pos._node);
    if (n != 0) {
        const auto add_size = n;
        auto node = create_node(value);
        node->prev = nullptr;
        r = iterator(node);
        iterator end = r;
        try {
            for (--n; n > 0; --n, ++end) {
                auto next = create_node(value);
                end._node->next = next->as_base();
                next->prev = end._node;
            }
            _size += add_size;
        } catch(...) {
            auto dnode = end._node;
            while(true) {
                auto prev = dnode->prev;
                destroy_node(dnode->as_node());
                if (prev == nullptr)
                    break;
                dnode = prev;
            }
            throw;
        }
        link_nodes(pos._node, r._node, end._node);
    }
    return r;
}

template <class T>
template <class Iter>
typename list<T>::iterator list<T>::copy_insert(const_iterator pos, size_type n, Iter first) {
    iterator r(pos._node);
    if (n != 0) {
        const auto add_size = n;
        auto node = create_node(*first);
        node->prev = nullptr;
        r = iterator(node);
        iterator end = r;
        try {
            for (--n, ++first; n > 0; --n, ++first, ++end) {
                auto next = create_node(*first);
                end._node->next = next->as_base();
                next->prev = end._node;
            }
            _size += add_size;
        } catch(...) {
            auto dnode = end._node;
            while(true) {
                auto prev = dnode->prev;
                destroy_node(dnode->as_node());
                if (prev == nullptr)
                    break;
                dnode = prev;
            }
            throw;
        }
        link_nodes(pos._node, r._node, end._node);
    }
    return r;
}

template <class T>
bool operator==(const list<T> &lhs, const list<T> &rhs) {
    auto first_l = lhs.begin();
    auto last_l = lhs.end();
    auto first_r = rhs.begin();
    auto last_r = rhs.end();
    for (; first_l != last_l && first_r != last_r && *first_l == *first_r; ++first_l, ++first_r)
        ;
    return first_l == last_l && first_r == last_r;
}

template <class T>
bool operator<(const list<T> &lhs, const list<T> &rhs) {
    return std::lexicographical_compare(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
}

template <class T>
bool operator!=(const list<T> &lhs, const list<T> &rhs) {
    return !(lhs == rhs);
}

template <class T>
bool operator>(const list<T> &lhs, const list<T> &rhs) {
    return rhs < lhs;
}

template <class T>
bool operator<=(const list<T> &lhs, const list<T> &rhs) {
    return !(rhs < lhs);
}

template <class T>
bool operator>=(const list<T> &lhs, const list<T> &rhs) {
    return !(lhs < rhs);
}

template <class T>
void swap(list<T> &lhs, list<T> &rhs) noexcept {
    lhs.swap(rhs);
}

template <class T>
list<T>::list() {
    fill_init(0, value_type());
}

template <class T>
list<T>::list(size_type n) {
    fill_init(n, value_type());
}

template <class T>
template <class Iter, typename std::enable_if<phq::is_input_iterator<Iter>::value, int>::type>
list<T>::list(Iter first, Iter last) {
    copy_init(first, last);
}

template <class T>
list<T>::list(std::initializer_list<T> ilist) {
    copy_init(ilist.begin(), ilist.end());
}

template <class T>
list<T>::list(const list &rhs) {
    copy_init(rhs.cbegin(), rhs.cend());
}

template <class T>
list<T>::list(list &&rhs) noexcept :_node(rhs._node), _size(rhs._size) {
    rhs._node = nullptr;
    rhs._size = 0;
}

template <class T>
list<T>& list<T>::operator=(const list &rhs) {
    if (this != &rhs) {
        assign(rhs.begin(), rhs.end());
    }
    return *this;
}

template <class T>
list<T> &list<T>::operator=(list &&rhs) noexcept {
    clear();
    splice(end(), rhs);
    return *this;
}

template <class T>
list<T> &list<T>::operator=(std::initializer_list<T> ilist) {
    list temp(ilist.begin(), ilist.end());
    swap(temp);
    return *this;
}

template <class T>
list<T>::~list() {
    if (_node != nullptr) {
        clear();
        base_allocator::deallocate(_node);
        _node = nullptr;
        _size = 0;
    }
}

template <class T>
typename list<T>::iterator list<T>::begin() noexcept {
    return _node->next;
}

template <class T>
typename list<T>::const_iterator list<T>::begin() const noexcept {
    return _node->next;
}

template <class T>
typename list<T>::iterator list<T>::end() noexcept {
    return _node;
}

template <class T>
typename list<T>::const_iterator list<T>::end() const noexcept {
    return _node;
}

template <class T>
typename list<T>::reverse_iterator list<T>::rbegin() noexcept {
    return reverse_iterator(end());
}

template <class T>
typename list<T>::const_reverse_iterator list<T>::rbegin() const noexcept {
    return reverse_iterator(end());
}

template <class T>
typename list<T>::reverse_iterator list<T>::rend() noexcept {
    return reverse_iterator(begin());
}

template <class T>
typename list<T>::const_reverse_iterator list<T>::rend() const noexcept {
    return reverse_iterator(begin());
}

template <class T>
typename list<T>::const_iterator list<T>::cbegin() const noexcept {
    return begin();
}

template <class T>
typename list<T>::const_iterator list<T>::cend() const noexcept {
    return end();
}

template <class T>
typename list<T>::const_reverse_iterator list<T>::crbegin() const noexcept {
    return rbegin();
}

template <class T>
typename list<T>::const_reverse_iterator list<T>::crend() const noexcept {
    return rend();
}

template <class T>
bool list<T>::empty() const noexcept {
    return _node->next == _node;
}

template <class T>
typename list<T>::size_type list<T>::size() const noexcept {
    return _size;
}

template <class T>
typename list<T>::size_type list<T>::max_size() const noexcept {
    return static_cast<size_type>(-1);
}

template <class T>
typename list<T>::reference list<T>::front() {
    PHQ_DEBUG(!empty());
    return *begin();
}

template <class T>
typename list<T>::const_reference list<T>::front() const {
    PHQ_DEBUG(!empty());
    return *begin();
}

template <class T>
typename list<T>::reference list<T>::back() {
    PHQ_DEBUG(!empty());
    return *(--end());
}

template <class T>
typename list<T>::const_reference list<T>::back() const {
    PHQ_DEBUG(!empty());
    return *(--end());
}

template <class T>
void list<T>::assign(size_type n, const value_type &value) {
    fill_assign(n, value);
}

template <class T>
template <class Iter, typename std::enable_if<phq::is_input_iterator<Iter>::value, int>::type>
void list<T>::assign(Iter first, Iter last) {
    copy_assign(first, last);
}

template <class T>
void list<T>::assign(std::initializer_list<T> ilist) {
    copy_assign(ilist.begin(), ilist.end());
}

template <class T>
template <class... Args>
void list<T>::emplace_front(Args &&...args) {
    auto link_node = create_node(phq::forward<Args>(args)...);
    link_nodes_at_front(link_node->as_base(), link_node->as_base());
    ++_size;
}

template <class T>
template <class... Args>
void list<T>::emplace_back(Args &&...args) {
    auto link_node = create_node(phq::forward<Args>(args)...);
    link_nodes_at_back(link_node->as_base(), link_node->as_base());
    ++_size;
}

template <class T>
template <class... Args>
typename list<T>::iterator list<T>::emplace(const_iterator pos, Args &&...args) {
    auto link_node = create_node(phq::forward<Args>(args)...);
    link_nodes(pos._node, link_node->as_base(), link_node->as_base());
    ++_size;
    return iterator(link_node);
}

template <class T>
typename list<T>::iterator list<T>::insert(const_iterator pos, const value_type &value) {
    auto link_node = create_node(value);
    ++_size;
    return link_iter_node(pos, link_node->as_base());
}

template <class T>
typename list<T>::iterator list<T>::insert(const_iterator pos, value_type &&value) {
    auto link_node = create_node(phq::move(value));
    ++_size;
    return link_iter_node(pos, link_node->as_base());
}

template <class T>
typename list<T>::iterator list<T>::insert(const_iterator pos, size_type n, const value_type &value) {
    return fill_insert(pos, n, value);
}

template <class T>
template <class Iter, typename std::enable_if<phq::is_input_iterator<Iter>::value, int>::type>
typename list<T>::iterator list<T>::insert(const_iterator pos, Iter first, Iter last) {
    size_type n = phq::distance(first, last);
    return copy_insert(pos, n, first);
}

template <class T>
void list<T>::push_front(const value_type &value) {
    auto link_node = create_node(value);
    link_nodes_at_front(link_node->as_base(), link_node->as_base());
    ++_size;
}

template <class T>
void list<T>::push_front(value_type &&value) {
    emplace_front(phq::move(value));
}

template <class T>
void list<T>::push_back(const value_type &value) {
    auto link_node = create_node(value);
    link_nodes_at_back(link_node->as_base(), link_node->as_base());
    ++_size;
}

template <class T>
void list<T>::push_back(value_type &&value) {
    emplace_back(phq::move(value));
}

template <class T>
void list<T>::pop_front() {
    PHQ_DEBUG(!empty());
    auto next = _node->next;
    unlink_nodes(next, next);
    destroy_node(next->as_node());
    --_size;
}

template <class T>
void list<T>::pop_back() {
    PHQ_DEBUG(!empty());
    auto prev = _node->prev;
    unlink_nodes(prev, prev);
    destroy_node(prev->as_node());
    --_size;
}

template <class T>
void list<T>::resize(size_type new_size) {
    resize(new_size, value_type());
}

template <class T>
void list<T>::swap(list &rhs) noexcept {
    phq::swap(_node, rhs._node);
    phq::swap(_size, rhs._size);
}

template <class T>
void list<T>::remove(const value_type &value) {
    remove_if([&](const value_type &v){ return v == value; });
}

template <class T>
void list<T>::unique() {
    unique(std::equal_to<T>());
}

template <class T>
void list<T>::merge(list &x) {
    merge(x, std::less<T>());
}

template <class T>
void list<T>::sort() {
    list_sort(begin(), end(), size(), std::less<T>());
}

template <class T>
template <class Compare>
void list<T>::sort(Compare comp) {
    list_sort(begin(), end(), size(), comp);
}

template <class T>
typename list<T>::iterator list<T>::erase(const_iterator pos) {
    PHQ_DEBUG(pos != cend());
    auto node = pos._node;
    auto next = node->next;
    unlink_nodes(node, node);
    destroy_node(node->as_node());
    --_size;
    return iterator(next);
}

template <class T>
typename list<T>::iterator list<T>::erase(const_iterator first, const_iterator last) {
    if (first != last) {
        unlink_nodes(first._node, last._node->prev);
        while (first != last) {
            auto cur = first._node;
            ++first;
            destroy_node(cur->as_node());
            --_size;
        }
    }
    return iterator(last._node);
}

template <class T>
void list<T>::clear() {
    if (_size != 0) {
        auto cur = _node->next;
        for (base_ptr next = cur->next; cur != _node; cur = next, next = cur->next) {
            destroy_node(cur->as_node());
        }
        _node->unlink();
        _size = 0;
    }
}

template <class T>
void list<T>::resize(size_type new_size, const value_type &value) {
    auto b = begin();
    size_type len = 0;
    while (b != end() && len < new_size) {
        ++b;
        ++len;
    }
    if (len == new_size) {
        erase(b, _node);
    } else {
        insert(_node, new_size - len, value);
    }
}

template <class T>
void list<T>::splice(const_iterator pos, list &other) {
    PHQ_DEBUG(this != &other);
    if (!other.empty()) {
        auto first = other._node->next;
        auto last = other._node->prev;
        other.unlink_nodes(first, last);
        link_nodes(pos._node, first, last);
        _size += other._size;
        other._size = 0;
    }
}

template <class T>
void list<T>::splice(const_iterator pos, list &other, const_iterator it) {
    if (pos._node != it._node && pos._node != it._node->next) {
        auto node = it._node;
        other.unlink_nodes(node, node);
        link_nodes(pos._node, node, node);
        ++_size;
        --other._size;
    }
}

template <class T>
void list<T>::splice(const_iterator pos, list &other, const_iterator first, const_iterator last) {
    if (first != last && this != &other) {
        size_type n = phq::distance(first, last);
        auto first_node = first._node;
        auto last_node = last._node->prev;
        other.unlink_nodes(first_node, last_node);
        link_nodes(pos._node, first_node, last_node);
        _size += n;
        other._size -= n;
    }
}

template <class T>
template <class UnaryPredicate>
void list<T>::remove_if(UnaryPredicate pred) {
    auto first = begin();
    auto last = end();
    for (auto next = first; first != last; first = next) {
        ++next;
        if(pred(*first)) {
            erase(first);
        }
    }
}

template <class T>
template <class BinaryPredicate>
void list<T>::unique(BinaryPredicate pred) {
    auto first = begin();
    auto last = end();
    auto next = first;
    ++next;
    while (next != last) {
        if (pred(*first, *next)) {
            erase(next);
        } else {
            first = next;
        }
        next = first;
        ++next;
    }
}

template <class T>
template <class Compare>
void list<T>::merge(list &other, Compare comp) {
    if (this != &other) {
        auto first1 = begin();
        auto last1 = end();
        auto first2 = other.begin();
        auto last2 = other.end();
        while (first1 != last1 && first2 != last2) {
            if (comp(*first2, *first1)) {
                auto next = first2;
                ++next;
                for (; next != last2 && comp(*next, *first1); ++next)
                    ;
                auto first = first2._node;
                auto last = next._node->prev;
                first2 = next;
                other.unlink_nodes(first, last);
                link_nodes(first1._node, first, last);
                ++first1;
            } else {
                ++first2;
            }
        }
        if (first2 != last2) {
            auto first = first2._node;
            auto last = last2._node->prev;
            other.unlink_nodes(first, last);
            link_nodes(last1._node, first, last);
        }
        _size += other._size;
        other._size = 0;
    }
}

template <class T>
void list<T>::reverse() {
    if (_size <= 1)
        return;
    auto first = begin();
    auto last = end();
    while (first != last) {
        phq::swap(first._node->prev, first._node->next);
        first._node = first._node->prev;
    }
    phq::swap(last._node->prev, last._node->next);
}

template <class T>
template <class Compared>
typename list<T>::iterator list<T>::list_sort(iterator f1, iterator l2, size_type n, Compared comp) {
    if (n <= 1)
        return f1;
    if (n == 2) {
        if (comp(*--l2, *f1)) {
            auto ln = l2._node;
            unlink_nodes(ln, ln);
            link_nodes(f1._node, ln, ln);
            return l2;
        }
        return f1;
    }
    auto n2 = n / 2;
    auto l1 = f1;
    phq::advance(l1, n2);
    auto result = f1 = list_sort(f1, l1, n2, comp);
    auto f2 = l1 = list_sort(l1, l2, n - n2, comp);
    if (comp(*f2, *f1)) {
        auto m = f2;
        ++m;
        for (; m != l2 && comp(*m, *f1); ++m)
            ;
        auto f = f2._node;
        auto l = m._node->prev;
        result = f2;
        l1 = f2 = m;
        unlink_nodes(f, l);
        m = f1;
        ++m;
        link_nodes(f1._node, f, l);
        f1 = m;
    } else {
        ++f1;
    }
    while (f1 != l1 && f2 != l2) {
        if (comp(*f2, *f1)) {
            auto m = f2;
            ++m;
            for (; m != l2 && comp(*m, *f1); ++m)
                ;
            auto f = f2._node;
            auto l = m._node->prev;
            if (l1 == f2)
                l1 = m;
            f2 = m;
            unlink_nodes(f, l);
            m = f1;
            ++m;
            link_nodes(f1._node, f, l);
            f1 = m;
        } else {
            ++f1;
        }
    }
    return result;
}

template <class T>
template <class Compared>
void list<T>::merges(iterator lo, iterator mi, iterator hi, Compared comp) {
    
}

} // namespace phq



#endif