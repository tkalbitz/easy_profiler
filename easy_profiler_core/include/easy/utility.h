

#ifndef EASY_PROFILER_UTILITY_H
#define EASY_PROFILER_UTILITY_H

#include <easy/details/easy_compiler_support.h>
#include <algorithm>
#include <utility>
#include <math.h>

namespace estd {

//////////////////////////////////////////////////////////////////////////

    namespace detail {
        template <class T, bool isMovable, bool isBig> struct swapper EASY_FINAL {
            inline void swap(T& a, T& b) const {
                T c = a;
                a = b;
                b = c;
            } };

        template <class T> struct swapper<T, true, true> EASY_FINAL {
            inline void swap(T& a, T& b) const {
                T c = ::std::move(a);
                a = ::std::move(b);
                b = ::std::move(c);
            } };
    }

    template <class T>
    inline void swap(T& a, T& b)
    {
        using Swp = ::estd::detail::swapper<T, ::std::is_move_constructible<T>::value && ::std::is_move_assignable<T>::value, (sizeof(T) > 8)>;
        const Swp S;
        S.swap(a, b);
    }

//////////////////////////////////////////////////////////////////////////

    namespace detail {
        template <class T, bool BiggerThanPtr> struct hasher {
            using type = const T&;
            EASY_FORCE_INLINE size_t operator () (type value) const { return ::std::hash<T> {}(value); } };

        template <class T> struct hasher<T, false> {
            using type = T;
            EASY_FORCE_INLINE size_t operator () (type value) const { return static_cast<size_t>(value); } };
    }

    template <class T> struct hash EASY_FINAL : public ::estd::detail::hasher<T, (sizeof(T) > sizeof(void*))> {
        using ::estd::detail::hasher<T, (sizeof(T) > sizeof(void*))>::operator();
    };

    template <class T> struct hash<T*> EASY_FINAL {
        EASY_FORCE_INLINE size_t operator () (const T* value) const { return reinterpret_cast<size_t>(value); } };

    template <class T> struct hash<const T*> EASY_FINAL {
        EASY_FORCE_INLINE size_t operator () (const T* value) const { return reinterpret_cast<size_t>(value); } };

//////////////////////////////////////////////////////////////////////////

    template <class T, class Q, class W>
    inline EASY_CONSTEXPR_FCN Q clamp(T min_value, Q value, W max_value) {
        return static_cast<Q>(min_value < value ? (value < max_value ? value : max_value) : min_value);
    }

    template <class T>
    EASY_FORCE_INLINE EASY_CONSTEXPR_FCN T sqr(T value) {
        return value * value;
    }

    template <class T>
    EASY_FORCE_INLINE EASY_CONSTEXPR_FCN int sign(T value) { return value < 0 ? -1 : 1; }

    template <class T>
    inline EASY_CONSTEXPR_FCN T absmin(T a, T b) { return abs(a) < abs(b) ? a : b; }

    template <int N, class T>
    inline T logn(T value) {
        EASY_STATIC_CONSTEXPR double div = 1.0 / log2((double)N);
        return log2(value) * div;
    }

//////////////////////////////////////////////////////////////////////////

} // end of namespace estd.

#endif // EASY_PROFILER_UTILITY_H