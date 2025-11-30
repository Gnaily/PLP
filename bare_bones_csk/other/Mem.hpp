#if !defined(_POINTER_H_)
#define _POINTER_H_
#include <memory>

#define USE_PLAIN_POINTER 1
#if USE_PLAIN_POINTER

#else

#define NEW(T) std::make_shared<T>
#define PTR(T) std:shared_ptr<T>
#define CAST(T) std:dynamic_pointer_cast<T>
#define CLASS(T) Class T : public std::enable_shared_from_this<T>
#define THIS shared_from_this()
#define Ref(T) T&

#endif 


