#ifndef _PHQ_UTIL_H_
#define _PHQ_UTIL_H_

#include <cstddef>
#include "type_traits.h"
namespace phq
{

// 将参数 arg 转换为右值引用类型，并返回该右值引用。
// 这个函数模板的实现依赖于 std::remove_reference 类型特征，它可以从类型 T 中移除引用。
// 如果 T 是引用类型，则 std::remove_reference<T>::type 是 T 所引用的类型，但不带引用。
// 否则，std::remove_reference<T>::type 就是 T 本身 123。
// 在这个函数模板中，我们使用 std::remove_reference 来获取参数 arg 的类型，并将其转换为右值引用类型。
// 这样做可以避免在函数调用时出现意外的引用类型，从而提高代码的可读性和可维护性。

template <class T>
typename std::remove_reference<T>::type &&move(T &&arg) noexcept
{
    return static_cast<typename std::remove_reference<T>::type &&>(arg);
}

// std::move 的作用是将一个左值强制转换为右值引用类型，以用于移动语义。它的使用表明了对右值的无条件转换，
// 因此可以用于将一个对象的所有权从一个变量转移到另一个变量，以避免不必要的拷贝操作.
// std::forward 的作用是将一个对象转发（或者说传递）给另外一个函数，同时保留此对象的左值性或右值性。它只对被绑定了右值的引用进行转换，而对左值引用不进行转换。
// std::forward 通常用于模板元编程中，例如，当需要将一个函数参数的引用类型去掉时，可以使用 std::forward 来实现。
// 这样做可以避免在函数调用时出现意外的引用类型，从而提高代码的可读性和可维护性


// 这两段代码的区别在于它们的参数类型不同。
// 第一段代码的参数类型是 typename std::remove_reference<T>::type &&，它接受一个右值引用类型的参数。
// 第二段代码的参数类型是 typename std::remove_reference<T>::type &，它接受一个左值引用类型的参数。
template <class T>
T &&forward(typename std::remove_reference<T>::type &arg) noexcept
{
    return static_cast<T &&>(arg);
}
template <class T>
T &&forward(typename std::remove_reference<T>::type &&arg) noexcept
{
    static_assert(!std::is_lvalue_reference<T>::value, "worng forward");
    return static_cast<T &&>(arg);
}
template <class T>
void swap(T &lhs, T &rhs)
{
    auto temp(phq::move(lhs));
    lhs = phq::move(rhs);
    rhs = phq::move(temp);
}
template <class ForwardIter1, class ForwardIter2>
ForwardIter2 swap_range(ForwardIter1 first1, ForwardIter1 last1, ForwardIter2 first2)
{
    for (; first1 != last1; ++first1, (void)++first2)
        phq::swap(*first1, *first2);
    return first2;
}
template <class T, size_t N>
void swap(T (&a)[N], T (&b)[N])
{
    phq::swap_range(a, a + N, b);
}

// 结构体模板pair的实现
template <class T1, class T2>
struct pair
{
    typedef T1 first_type;
    typedef T2 second_type;
    first_type first;
    second_type second;
// 这段代码是C++中的一个构造函数模板，名为 pair。它的作用是创建一个 pair 类型的对象，并将其两个成员变量 first 和 second 初始化为默认值。
// 这个构造函数模板的实现依赖于 std::enable_if 类型特征，它可以根据类型 Other1 和 Other2 是否可默认构造来启用或禁用这个构造函数模板。
// 如果 Other1 和 Other2 都是可默认构造的类型，则这个构造函数模板被启用；否则，它被禁用
// 在这个构造函数模板中，使用了 std::enable_if 来检查类型 Other1 和 Other2 是否可默认构造。
// 如果它们都是可默认构造的类型，那么这个构造函数模板就被启用，并将 first 和 second 初始化为默认值。
// 否则，这个构造函数模板就被禁用，从而避免了在编译时出现错误。

//默认构造
template <class Other1 = T1, class Other2 = T2, typename = typename std::enable_if<std::is_default_constructible<Other1>::value && std::is_default_constructible<Other2>::value, void>::type>
constexpr pair() : first(), second()
{
}
//隐式构造函数
template <class U1 = T1, class U2 = T2,
typename std::enable_if<std::is_copy_constructible<U1>::value &&
                        std::is_copy_constructible<U2>::value &&
                        std::is_convertible<const U1 &, T1>::value &&
                        std::is_convertible<const U2 &, T2>::value,
                        int>::type = 0>
constexpr pair(const T1 &a, const T2 &b) : first(a), second(b)
{
}

//显式构造函数
template <class U1 = T1, class U2 = T2,
typename std::enable_if<std::is_copy_constructible<U1>::value &&
                        std::is_copy_constructible<U2>::value &&
                        (!std::is_convertible<const U1 &, T1>::value ||
                        !std::is_convertible<const U2 &, T2>::value),
                        int>::type = 0> 
explicit constexpr pair(const T1 &a, const T2 &b) : first(a), second(b)
{
};
pair(const pair &rhs) = default;
pair(pair &&rhs) = default;
// 隐式构造
// 在这个构造函数模板中，我们使用了 std::enable_if 来检查类型 Other1 和 Other2 是否可构造为 Ty1 和 Ty2 类型，并且是否可以隐式转换为 Ty1 和 Ty2 类型。
// 如果它们都是可构造为 Ty1 和 Ty2 类型并且可以隐式转换为 Ty1 和 Ty2 类型，那么这个构造函数模板就被启用，并将 first 和 second 初始化为指定的值。
// 否则，这个构造函数模板就被禁用，从而避免了在编译时出现错误。
// 需要注意的是，Other1&& 和 Other2&& 是右值引用类型，它们用于接受右值参数。
// std::forward 函数用于将右值参数转发给 first 和 second 成员变量，以保留其原有的引用类型。
template <class Other1, class Other2,
typename std::enable_if<std::is_constructible<T1, Other1>::value &&
                        std::is_constructible<T2, Other2>::value &&
                        std::is_convertible<Other1 &&, T1>::value &&
                        std::is_convertible<Other2 &&, T2>::value,
                        int>::type = 0>
constexpr pair(Other1 &&a, Other2 &&b) : first(phq::forward<Other1>(a)), second(phq::forward<Other2>(b))
{
}
template <class Other1, class Other2,
typename std::enable_if<std::is_constructible<T1, Other1>::value &&
                        std::is_constructible<T2, Other2>::value &&
                        (!std::is_convertible<Other1, T1>::value || 
                        !std::is_convertible<Other2, T2>::value),
                        int>::type = 0>
explicit constexpr pair(Other1 &&a, Other2 &&b) : first(phq::forward<Other1>(a)), second(phq::forward<Other2>(b))
{
}
template <class Other1, class Other2,
typename std::enable_if<std::is_constructible<T1, const Other1 &>::value &&
                        std::is_constructible<T2, const Other2 &>::value &&
                        std::is_convertible<const Other1 &, T1>::value &&
                        std::is_convertible<const Other2 &, T2>::value,
                        int>::type = 0>
constexpr pair(const pair<Other1, Other2> &other) : first(other.first), second(other.second)
{
}
template <class Other1, class Other2,
typename std::enable_if<std::is_constructible<T1, const Other1 &>::value &&
                        std::is_constructible<T2, const Other2 &>::value &&
                        (!std::is_convertible<const Other1 &, T1>::value ||
                         !std::is_convertible<const Other2 &, T2>::value),
                        int>::type = 0>
explicit constexpr pair(const pair<Other1, Other2> &other) : first(other.first), second(other.second)
{
}
template <class Other1, class Other2,
typename std::enable_if<std::is_constructible<T1, Other1>::value &&
                        std::is_constructible<T2, Other2>::value &&
                        std::is_convertible<Other1, T1>::value && std::is_convertible<Other2, T2>::value,
                        int>::type = 0>
constexpr pair(pair<Other1, Other2> &&other) : first(phq::forward<Other1>(other.first)), second(phq::forward<Other2>(other.second))
{
}
template <class Other1, class Other2,
typename std::enable_if<std::is_constructible<T1, Other1>::value &&
                        std::is_constructible<T2, Other2>::value &&
                        (!std::is_convertible<Other1, T1>::value || 
                         !std::is_convertible<Other2, T2>::value),
                        int>::type = 0>
explicit constexpr pair(pair<Other1, Other2> &&other) : first(phq::forward<Other1>(other.first)), second(phq::forward<Other2>(other.second))
{
}
pair &operator=(const pair &rhs)
{
    if (this != &rhs) {
        first = rhs.first;
        second = rhs.second;
    }
    return *this;
}
pair &operator=(pair &&rhs)
{
    if (this != &rhs) {
        first = phq::move(rhs.first);
        second = phq::move(rhs.second);
    }
    return *this;
}
template <class Other1, class Other2>
pair &operator=(const pair<Other1, Other2> &other)
{
    first = other.first;
    second = other.second;
    return *this;
}
template <class Other1, class Other2>
pair &operator=(pair<Other1, Other2> &&other)
{
    first = phq::forward<Other1>(other.first);
    second = phq::forward<Other2>(other.second);
    return *this;
}
~pair() = default;
void swap(pair &other)
{
    if (this != &other) {
        phq::swap(first, other.first);
        phq::swap(second, other.second);
    }
}
};
template <class T1, class T2>
bool operator==(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs)
{
    return lhs.first == rhs.first && lhs.second == rhs.second;
}
template <class T1, class T2>
bool operator<(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs)
{
    return lhs.first < rhs.first || (lhs.first == rhs.first && lhs.second < rhs.second);
}
template <class T1, class T2>
bool operator!=(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs)
{
    return !(lhs == rhs);
}
template <class T1, class T2>
bool operator>(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs)
{
    return rhs < lhs;
}
template <class T1, class T2>
bool operator<=(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs)
{
    return !(rhs < lhs);
}
template <class T1, class T2>
bool operator>=(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs)
{
    return !(lhs < rhs);
}
template <class T1, class T2>
void swap(pair<T1, T2> &lhs, pair<T1, T2> &rhs)
{
    lhs.swap(rhs);
}
template <class T1, class T2>
pair<T1, T2> make_pair(T1 &&first, T2 &&second)
{
    return pair<T1, T2>(phq::forward<T1>(first), phq::forward<T2>(second));
}
} // namespace phq

#endif