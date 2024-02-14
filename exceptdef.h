#ifndef _PHQ_EXCEPTDEF_H_
#define _PHQ_EXCEPTDEF_H_

#include <stdexcept>
#include <cassert>

namespace phq
{
// 若expr为真则不会执行任何操作，否则会终止程序执行并在标准错误stderr上显示错误信息
#define PHQ_DEBUG(expr) \
    assert(expr)

#define THORW_LENGTH_ERROR_IF(expr, what) \
    if((expr)) throw std::length_error(what)

#define THORW_OUT_OF_RANGE_IF(expr, what) \
    if((expr)) throw std::out_of_range(what)

#define THROW_RUNTIME_ERROR_IF(expr, what) \
    if((expr)) throw std::runtime_error(what)
    
} // namespace phq




#endif