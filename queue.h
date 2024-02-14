#ifndef _PHQ_QUEUE_H_
#define _PHQ_QUEUE_H_

#include "deque.h"
#include "vector.h"

namespace phq
{
template <class T, class Container = phq::deque<T>>
class queue {
public:
    typedef Container container_type;
    typedef typename Container::value_type value_type;
    typedef typename Container::size_type size_type;
    typedef typename Container::reference reference;
    typedef typename Container::const_reference const_reference;

    static_assert(std::is_same<T, value_type>::value, "value_type <> T");

private:
    container_type c_;

public:
    queue() = default;
    explicit queue(size_type n) : c_(n){}
    queue(size_type n, const value_type &value) : c_(n, value){}
    template <class IIter>
    queue(IIter first, IIter last) : c_(first, last){}
    queue(std::initializer_list<T> ilist) : c_(ilist.begin(), ilist.end()){}
    queue(const Container &c) : c_(c){}
    queue(Container &&c) noexcept : c_(phq::move(c)){}
    queue(const queue &rhs) : c_(rhs.c_){}
    queue(queue &&rhs) noexcept : c_(phq::move(rhs.c_)){}
    queue &operator=(const queue &rhs) {
        c_ = rhs.c_;
        return *this;
    }
    queue &operator=(queue &&rhs) noexcept {
        c_ = phq::move(rhs.c_);
        return *this;
    }
    queue &operator=(std::initializer_list<T> ilist) {
        c_ = ilist;
        return *this;
    }
    ~queue() = default;
    reference front() { return c_.front(); }
    const_reference front() const { return c_.front(); }
    reference back() { return c_.back(); }
    const_reference back() const { return c_.back(); }
    bool empty() const noexcept { return c_.empty(); }
    size_type size() const noexcept { return c_.size(); }
    template <class... Args>
    void emplace(Args &&...args) { c_.emplace_back(phq::forward<Args>(args)...); }
    void push(const value_type &value) { c_.push_back(value); }
    void push(value_type &&value) { c_.emplace_back(phq::move(value)); }
    void pop() { c_.pop_front(); }
    void swap(queue &rhs) noexcept { phq::swap(c_, rhs.c_); }

public:
    friend bool operator==(const queue &lhs, const queue &rhs) { return lhs.c_ == rhs.c_; }
    friend bool operator<(const queue &lhs, const queue &rhs) { return lhs.c_ < rhs.c_; }
};

template <class T, class Container>
bool operator==(const queue<T, Container> &lhs, const queue<T, Container> &rhs) {
    return lhs == rhs;
}

template <class T, class Container>
bool operator<(const queue<T, Container> &lhs, const queue<T, Container> &rhs) {
    return lhs < rhs;
}

template <class T, class Container>
bool operator>(const queue<T, Container> &lhs, const queue<T, Container> &rhs) {
    return rhs < lhs;
}

template <class T, class Container>
bool operator!=(const queue<T, Container> &lhs, const queue<T, Container> &rhs) {
    return !(lhs == rhs);
}

template <class T, class Container>
bool operator<=(const queue<T, Container> &lhs, const queue<T, Container> &rhs) {
    return !(rhs < lhs);
}

template <class T, class Container>
bool operator>=(const queue<T, Container> &lhs, const queue<T, Container> &rhs) {
    return !(lhs < rhs);
}

template <class T, class Container>
void swap(queue<T, Container> &lhs, queue<T, Container> &rhs) noexcept {
    lhs.swap(rhs);
}

} // namespace phq



#endif