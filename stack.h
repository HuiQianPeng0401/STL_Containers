#ifndef _PHQ_STACK_H_
#define _PHQ_STACK_H_

#include "deque.h"

namespace phq
{
template <class T, class Container = phq::deque<T>>
class stack {
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
    stack() = default;
    explicit stack(size_type n) : c_(n){}
    stack(size_type n, const value_type &value) : c_(n, value){}
    template <class IIter>
    stack(IIter first, IIter last) : c_(first, last){}
    stack(std::initializer_list<T> ilist) : c_(ilist.begin(), ilist.end()){}
    stack(const Container &c) : c_(c){}
    stack(Container &&c) noexcept : c_(phq::move(c)){}
    stack(const stack &rhs) : c_(rhs.c_){}
    stack(stack &&rhs) noexcept : c_(phq::move(rhs.c_)){}
    stack &operator=(const stack &rhs) {
        c_ = rhs.c_;
        return *this;
    }
    stack &operator=(stack &&rhs) noexcept {
        c_ = phq::move(rhs.c_);
        return *this;
    }
    stack &operator=(std::initializer_list<T> ilist) {
        c_ = ilist;
        return *this;
    }
    ~stack() = default;
    reference top() { return c_.back(); }
    const_reference top() const { return c_.back(); }
    bool empty() const noexcept { return c_.empty(); }
    size_type size() const noexcept { return c_.size(); }
    template <class... Args>
    void emplace(Args &&...args) { c_.emplace_back(phq::forward<Args>(args)...); }
    void push(const value_type &value) { c_.push_back(value); }
    void push(value_type &&value) { c_.push_back(value); }
    void pop() { c_.pop_back(); }
    void swap(stack &rhs) noexcept {
        phq::swap(c_, rhs.c_);
    }

public:
    friend bool operator==(const stack &lhs, const stack &rhs) { return lhs.c_ == rhs.c_; }
    friend bool operator<(const stack &lhs, const stack &rhs) { return lhs.c_ < rhs.c_; }
};

template <class T, class Container>
bool operator==(const stack<T, Container> &lhs, const stack<T, Container> &rhs) {
    return lhs == rhs;
}

template <class T, class Container>
bool operator<(const stack<T, Container> &lhs, const stack<T, Container> &rhs) {
    return lhs < rhs;
}

template <class T, class Container>
bool operator!=(const stack<T, Container> &lhs, const stack<T, Container> &rhs) {
    return !(lhs == rhs);
}

template <class T, class Container>
bool operator>(const stack<T, Container> &lhs, const stack<T, Container> &rhs) {
    return rhs < lhs;
}

template <class T, class Container>
bool operator<=(const stack<T, Container> &lhs, const stack<T, Container> &rhs) {
    return !(rhs < lhs);
}

template <class T, class Container>
bool operator>=(const stack<T, Container> &lhs, const stack<T, Container> &rhs) {
    return !(lhs < rhs);
}

template <class T, class Container>
void swap(stack<T, Container> &lhs, stack<T, Container> &rhs) noexcept {
    lhs.swap(rhs);
}

} // namespace phq



#endif