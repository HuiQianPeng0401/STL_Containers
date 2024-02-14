#ifndef _PHQ_TYPE_TRAITS_H_
#define _PHQ_TYPE_TRAITS_H_

// 提取类型信息

#include <type_traits>

namespace phq
{
// 这段代码定义了一个命名空间phq，其中包含了一些模板结构体和模板别名，以及一个模板类的前向声明和一个模板类的特化。
// 首先，这个命名空间中定义了一个模板结构体m_integral_constant，它有两个模板参数：T和v。这个结构体有一个静态成员变量value，它的类型是T，值是v。这个结构体的作用是将一个值封装为一个类型。
// 接下来，这个命名空间中定义了一个模板别名m_bool_constant，它有一个模板参数b，类型为bool。这个模板别名可以用于定义一个类型为bool的常量。例如，可以这样定义一个名为is_true的常量：
// m_bool_constant<true> is_true; 这个常量的值为true。同样地，可以定义一个名为is_false的常量：

// m_bool_constant<false> is_false; 这个常量的值为false。
// 接下来，这个命名空间中定义了一个模板结构体is_pair，它有一个模板参数T。这个结构体有一个静态成员变量value，它的类型是bool，值为true或false。如果T是一个pair类型，则value的值为true，否则为false。这个结构体的实现使用了模板特化，即当T是pair类型时，value的值为true，否则为false。
// 最后，这个命名空间中还有一个模板类的前向声明pair，以及一个模板类pair的特化。这个模板类有两个模板参数：T1和T2。这个模板类的作用是将两个类型组合成一个类型。这个模板类的特化使用了命名空间mystl中定义的模板结构体m_integral_constant和模板别名m_bool_constant。



// 这段代码是一个模板结构体，名为m_integral_constant。它有两个模板参数：T和v。
// 这个结构体有一个静态成员变量value，它的类型是T，值是v。这个结构体的作用是将一个值封装为一个类型。这个结构体可以用于编写泛型代码，
// 例如，可以使用这个结构体来定义一个类型为int的常量。例如，可以这样定义一个名为pi的常量：m_integral_constant<int, 3> pi;。
// 这个结构体的实现非常简单，它只是将一个值封装为一个类型。
template <class T, T v>
struct m_itergral_constant
{
    static constexpr T value = v;
};

// 这段代码定义了一个模板别名m_bool_constant，它有一个模板参数b，类型为bool。这个模板别名可以用于定义一个类型为bool的常量。
// 这个常量的值为false。这个模板别名的实现使用了另一个模板结构体m_integral_constant，它有两个模板参数：T和v。
// 这个结构体有一个静态成员变量value，它的类型是T，值是v。这个结构体的作用是将一个值封装为一个类型。这个模板别名的实现中，m_bool_constant<b>相当于m_integral_constant<bool, b>。
// 因此，m_bool_constant<true>相当于m_integral_constant<bool, true>，m_bool_constant<false>相当于m_integral_constant<bool, false>。
template <bool b>
using m_bool_constant = m_itergral_constant<bool, b>;
typedef m_bool_constant<true> m_true_type;
typedef m_bool_constant<false> m_false_type;

template <class T1, class T2>
struct pair;

template <class T>
struct is_pair : phq::m_false_type{};

template <class T1, class T2>
struct is_pair<phq::pair<T1, T2>> : phq::m_true_type{};

}

#endif