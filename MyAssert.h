#ifndef __MY_ASSERT_H
#define __MY_ASSERT_H

#include <cassert>
#include <stdexcept>

#if defined(_DEBUG) && !defined(_TEST)

#define MYASSERT(condition, message) assert((condition) && message);

#else

#define MYASSERT(condition, message) if (!(condition)) throw std::runtime_error(message);

#endif

#endif //__MY_ASSERT_H

