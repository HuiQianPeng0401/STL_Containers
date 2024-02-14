#ifndef _PHQ_ITERATOR_H_
#define _PHQ_ITERATOR_H_

//@ phq
//2024/1/23

#include <cstddef>

#include "type_traits.h"

//迭代器设计，包含了一些模板结构体和全局函数
//参考https://juejin.cn/post/7109086946160279559
namespace phq
{
    
struct input_iterator_tag{};
struct output_iterator_tag{};
struct forward_iterator_tag : public input_iterator_tag{};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag{};

template <typename Category, typename T, typename Distance = ptrdiff_t, typename Pointer = T *, typename Reference = T &>
struct iterator{
    typedef Category iterator_category;
    typedef T value_type;
    typedef Pointer pointer;
    typedef Reference reference;
    typedef Distance difference_type;
};

// 这段语法是一个模板结构体，名为has_iterator_cat。它有一个模板参数T。
// 这个结构体有一个静态成员变量value，它的值是一个布尔值，表示T是否有iterator_category这个嵌套类型。
// 如果T有这个嵌套类型，那么value的值为true，否则为false。
// 这个结构体的实现使用了SFINAE技术，即通过重载函数模板来判断T是否有iterator_category这个嵌套类型。
// 具体来说，这个结构体有两个私有的函数模板test，一个接受任意类型的参数，另一个接受一个指向U::iterator_category的指针类型的参数。
// 如果T有iterator_category这个嵌套类型，那么第二个函数模板会被选择，否则第一个函数模板会被选择。
// 这个结构体的实现中，test<T>(0)会调用第一个函数模板，而test<typename U::iterator_category*>(0)会调用第二个函数模板。
// 如果第二个函数模板被选择，那么sizeof(test<T>(0))的值就会等于sizeof(char)，因为test<typename U::iterator_category*>(0)返回的是一个char类型的值。
// 如果第一个函数模板被选择，那么sizeof(test<T>(0))的值就会大于sizeof(char)，因为test(...)返回的是一个two类型的值。
// 因此，如果T有iterator_category这个嵌套类型，那么value的值就会是true，否则为false。

template <class T>
struct has_iterator_cat
{
private:
    struct two{char a;char b;};
    template <class U>
    static two test(...);
    template <class U>
    static char test(typename U::iterator_category* = 0);
public:
    static const bool value = sizeof(test<T>(0)) == sizeof(char);
};
// 下面一段代码是一个模板结构体iterator_traits_impl，它有两个模板参数：Iterator和bool。这个结构体的作用是提取迭代器的类型信息。
// iterator_category，它表示迭代器的类型，有五种可能的取值：input_iterator_tag、output_iterator_tag、forward_iterator_tag、bidirectional_iterator_tag和random_access_iterator_tag。
// value_type，它表示迭代器所指向的对象的类型。
// pointer，它表示迭代器所指向的对象的指针类型。
// reference，它表示迭代器所指向的对象的引用类型。
// difference_type，它表示两个迭代器之间的距离的类型。
// 这个结构体的实现使用了模板特化，即当第二个模板参数为true时，iterator_category、value_type、pointer、reference和difference_type都是从Iterator中提取的。
// 否则，这些成员变量都没有定义。
// 这个结构体的第二个模板参数bool的作用是控制是否提取迭代器的类型信息。如果这个参数为true，则提取类型信息；否则不提取。

template <class Iterator, bool>
struct iterator_traits_impl{};
template <class Iterator>
struct iterator_traits_impl<Iterator, true>
{
    typedef typename Iterator::iterator_category iterator_category;
    typedef typename Iterator::value_type value_type;
    typedef typename Iterator::pointer pointer;
    typedef typename Iterator::reference reference;
    typedef typename Iterator::difference_type difference_type;
};

// 这段代码定义了一个模板结构体iterator_traits_helper，它有两个模板参数：Iterator和bool。
// 这个结构体的作用是提取迭代器的类型信息。这个结构体的第二个模板参数bool的作用是控制是否提取迭代器的类型信息。如果这个参数为true，则提取类型信息；否则不提取。
// 这个结构体的实现使用了模板继承，即当第二个模板参数为true时，iterator_traits_helper继承自iterator_traits_impl，并从Iterator中提取迭代器的类型信息。
// 否则，iterator_traits_helper不继承自iterator_traits_impl，并不提取迭代器的类型信息。
// std::is_convertible是一个模板类，用于判断一个类型是否可以隐式转换为另一个类型。
// 它有两个模板参数：From和To。如果From类型可以隐式转换为To类型，则std::is_convertible<From, To>::value的值为true，否则为false。
// 若iterator_category是迭代器类型则可提取迭代器类型信息
template <class Iterator, bool>
struct iterator_traits_helper{};
template <class Iterator>
struct iterator_traits_helper<Iterator, true>
  : public iterator_traits_impl<Iterator,
  std::is_convertible<typename Iterator::iterator_category, input_iterator_tag>::value ||
  std::is_convertible<typename Iterator::iterator_category, output_iterator_tag>::value>
{
};

// 萃取迭代器的特性
template <class Iterator>
struct iterator_traits 
  : public iterator_traits_helper<Iterator, has_iterator_cat<Iterator>::value> {};

// 针对原生指针的偏特化版本
template <class T>
struct iterator_traits<T*>
{
    typedef random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef T *pointer;
    typedef T &reference;
    typedef ptrdiff_t difference_type;
};

template <class T>
struct iterator_traits<const T*>
{
    typedef random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef const T *pointer;
    typedef const T &reference;
    typedef ptrdiff_t difference_type;
};
// 这段代码定义了一个模板结构体has_iterator_cat_of，它有三个模板参数：T、U和bool。
// 这个结构体的作用是判断T类型的迭代器是否具有指定的迭代器类型。这个结构体的实现使用了模板继承，
// 即当第三个模板参数为true时，has_iterator_cat_of继承自m_bool_constant，并判断T类型的迭代器是否具有指定的迭代器类型。
// 否则，has_iterator_cat_of不继承自m_bool_constant，并返回false。
// 这个结构体的实现中，has_iterator_cat是一个模板函数，用于判断T类型的迭代器是否具有iterator_category这个嵌套类型。
// 如果T类型的迭代器具有iterator_category这个嵌套类型，则has_iterator_cat<T>::value的值为true，否则为false。
// 在这个结构体的实现中，std::is_convertible<typename iterator_traits<T>::iterator_category, U>::value的值表示T类型的迭代器是否可以隐式转换为U类型的迭代器。
// 如果可以，则has_iterator_cat_of<T, U, true>::value的值为true，否则为false。
template <class T, class U, bool = has_iterator_cat<iterator_traits<T>>::value>
struct has_iterator_cat_of : public m_bool_constant<std::is_convertible<typename iterator_traits<T>::iterator_category, U>::value>
{
};
// 第三个模板参数bool是用于决定是否萃取迭代器类型。如果这个参数为true，则提取迭代器的类型信息；否则不提取。
// 这个参数的默认值是has_iterator_cat<iterator_traits<T>>::value，它表示T类型的迭代器是否具有iterator_category这个嵌套类型。
// 如果T类型的迭代器具有iterator_category这个嵌套类型，则has_iterator_cat<iterator_traits<T>>::value的值为true，否则为false。
// 如果这个参数为false，则has_iterator_cat_of不会从T类型的迭代器中提取迭代器的类型信息，而是返回false。
// 如果这个参数为true，则has_iterator_cat_of会从T类型的迭代器中提取迭代器的类型信息，并判断它是否是指定的迭代器类型。

template <class T, class U>
struct has_iterator_cat_of<T, U, false> : public m_false_type{};

template <class Iterator>
struct is_input_iterator : public has_iterator_cat_of<Iterator, input_iterator_tag>{};

template <class Iterator>
struct is_output_iterator : public has_iterator_cat_of<Iterator, output_iterator_tag>{};

template <class Iterator>
struct is_forward_iterator : public has_iterator_cat_of<Iterator, forward_iterator_tag>{};

template <class Iterator>
struct is_bidirectional_iterator : public has_iterator_cat_of<Iterator, bidirectional_iterator_tag>{};

template <class Iterator>
struct is_random_access_iterator : public has_iterator_cat_of<Iterator, random_access_iterator_tag>{};

template <class Iterator>
struct is_iterator : public m_bool_constant<is_input_iterator<Iterator>::value ||
                                            is_output_iterator<Iterator>::value>
{
};

template <class Iterator>
typename iterator_traits<Iterator>::iterator_category
iterator_category(const Iterator &)
{
    typedef typename iterator_traits<Iterator>::iterator_category Category;
    return Category();
}

template <class Iterator>
typename iterator_traits<Iterator>::difference_type*
distance_type(const Iterator &)
{
    return static_cast<typename iterator_traits<Iterator>::difference_type *>(0);
}

template <class Iterator>
typename iterator_traits<Iterator>::value_type*
value_type(const Iterator &)
{
    return static_cast<typename iterator_traits<Iterator>::value_type *>(0);
}

//以下函数计算迭代器之间的距离
template <class InputIterator>
typename iterator_traits<InputIterator>::difference_type
distance_dispatch(InputIterator first, InputIterator last, input_iterator_tag)
{
    typename iterator_traits<InputIterator>::difference_type n = 0;
    while (first != last) {
        ++first;
        ++n;
    }
    return n;
}

template <class RandomIterator>
typename iterator_traits<RandomIterator>::difference_type
distance_type(RandomIterator first, RandomIterator last, random_access_iterator_tag)
{
    return last - first;
}

template <class InputIterator>
typename iterator_traits<InputIterator>::difference_type
distance(InputIterator first, InputIterator last)
{
    return distance_dispatch(first, last, iterator_category(first));
}

//以下函数让迭代器前进n个距离
template <class InputIterator, class Distance>
void advance_dispatch(InputIterator &i, Distance n, input_iterator_tag)
{
    while (n--)
        ++i;
}

template <class BidirectionalIterator, class Distance>
void advance_dispatch(BidirectionalIterator &i, Distance n, bidirectional_iterator_tag)
{
    if (n >= 0)
        while (n--)
            ++i;
    else
        while (n++)
            --i;
}

template <class RandomIterator, class Distance>
void advance_dispatch(RandomIterator &i, Distance n, random_access_iterator_tag)
{
    i += n;
}

template <class InputIterator, class Distance>
void advance(InputIterator &i, Distance n)
{
    advance_dispatch(i, n, iterator_category(i));
}

//模板类：反向迭代器：reverse_iterator
template <class Iterator>
class reverse_iterator
{
private:
    Iterator current; // 记录对应的正向迭代器
public:
    typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
    typedef typename iterator_traits<Iterator>::value_type value_type;
    typedef typename iterator_traits<Iterator>::difference_type difference_type;
    typedef typename iterator_traits<Iterator>::pointer pointer;
    typedef typename iterator_traits<Iterator>::reference reference;
    typedef Iterator iterator_type;
    typedef reverse_iterator<Iterator> self;
public:
    reverse_iterator() {}
    explicit reverse_iterator(iterator_type i) : current(i) {}
    reverse_iterator(const self &rhs) : current(rhs.current) {}
public:
    iterator_type base() const { return current; } // 取出对应的正向迭代器
    reference operator*() const {  // 实际对应正向的前一个位置
        auto temp = current;
        return *--temp;
    }
    pointer operator->() const { return &(operator*()); }
    self &operator++() {
        --current;
        return *this;
    }
    self operator++(int) {
        self temp = *this;
        --current;
        return temp;
    }
    self &operator--() {
        ++current;
        return *this;
    }
    self operator--(int) {
        self temp = *this;
        ++current;
        return temp;
    }
    self &operator+=(difference_type n) {
        current -= n;
        return *this;
    }
    self operator+(difference_type n) const {
        return self(current - n);
    }
    self &operator-=(difference_type n) {
        current += n;
        return *this;
    }
    self operator-(difference_type n) const {
        return self(current + n);
    }
    reference operator[](difference_type n) const {
        return *(*this + n);
    }
};
template <class Iterator>
typename reverse_iterator<Iterator>::difference_type
operator-(const reverse_iterator<Iterator> &lhs, const reverse_iterator<Iterator> &rhs)
{
    return rhs.base() - lhs.base();
}
template <class Iterator>
bool operator==(const reverse_iterator<Iterator> &lhs, const reverse_iterator<Iterator> &rhs)
{
    return lhs.base() == rhs.base();
}
template <class Iterator>
bool operator<(const reverse_iterator<Iterator> &lhs, const reverse_iterator<Iterator> &rhs)
{
    return rhs.base() < lhs.base();
}
template <class Iterator>
bool operator!=(const reverse_iterator<Iterator> &lhs, const reverse_iterator<Iterator> &rhs)
{
    return !(lhs == rhs);
}
template <class Iterator>
bool operator>(const reverse_iterator<Iterator> &lhs, const reverse_iterator<Iterator> &rhs)
{
    return rhs < lhs;
}
template <class Iterator>
bool operator<=(const reverse_iterator<Iterator> &lhs, const reverse_iterator<Iterator> &rhs)
{
    return !(rhs < lhs);
}
template <class Iterator>
bool operator>=(const reverse_iterator<Iterator> &lhs, const reverse_iterator<Iterator> &rhs)
{
    return !(lhs < rhs);
}

} // namespace phq




#endif