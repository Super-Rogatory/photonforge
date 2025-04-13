#ifndef __vec__
#define __vec__

#include <cmath>
#include <iostream>
#include <cassert>

static const double pi = 4 * atan(1.0);

template<class T, int n> struct vec;
template<class T, int n> T dot(const vec<T,n>& u,const vec<T,n>& v);

template<class T, int n>
struct vec {
    T x[n];

    inline vec() { make_zero(); }

    inline explicit vec(const T& a) {
        assert(n == 1); x[0] = a;
    }

    inline vec(const T& a, const T& b) {
        assert(n == 2); x[0] = a; x[1] = b;
    }

    inline vec(const T& a, const T& b, const T& c) {
        assert(n == 3); x[0] = a; x[1] = b; x[2] = c;
    }

    template<class U>
    inline explicit vec(const vec<U,n>& v) {
        for(int i = 0; i < n; i++) x[i] = (T)v.x[i];
    }

    inline void make_zero() { fill(0); }
    inline void fill(T value) { for(int i = 0; i < n; i++) x[i] = value; }

    inline vec& operator += (const vec& v) { for(int i = 0; i < n; i++) x[i] += v.x[i]; return *this; }
    inline vec& operator -= (const vec& v) { for(int i = 0; i < n; i++) x[i] -= v.x[i]; return *this; }
    inline vec& operator *= (const vec& v) { for(int i = 0; i < n; i++) x[i] *= v.x[i]; return *this; }
    inline vec& operator /= (const vec& v) { for(int i = 0; i < n; i++) x[i] /= v.x[i]; return *this; }
    inline vec& operator *= (const T& c) { for(int i = 0; i < n; i++) x[i] *= c; return *this; }
    inline vec& operator /= (const T& c) { for(int i = 0; i < n; i++) x[i] /= c; return *this; }

    inline vec operator + () const { return *this; }
    inline vec operator - () const { vec r; for(int i = 0; i < n; i++) r[i] = -x[i]; return r; }

    inline vec operator + (const vec& v) const { vec r; for(int i = 0; i < n; i++) r[i] = x[i] + v.x[i]; return r; }
    inline vec operator - (const vec& v) const { vec r; for(int i = 0; i < n; i++) r[i] = x[i] - v.x[i]; return r; }
    inline vec operator * (const vec& v) const { vec r; for(int i = 0; i < n; i++) r[i] = x[i] * v.x[i]; return r; }
    inline vec operator / (const vec& v) const { vec r; for(int i = 0; i < n; i++) r[i] = x[i] / v.x[i]; return r; }

    inline vec operator * (const T& c) const { vec r; for(int i = 0; i < n; i++) r[i] = x[i] * c; return r; }
    inline vec operator / (const T& c) const { vec r; for(int i = 0; i < n; i++) r[i] = x[i] / c; return r; }

    inline const T& operator[] (int i) const { return x[i]; }
    inline T& operator[] (int i) { return x[i]; }

    inline T magnitude_squared() const { return dot(*this, *this); }
    inline T magnitude() const { return sqrt(magnitude_squared()); }

    inline vec normalized() const {
        T mag = magnitude();
        return mag ? *this / mag : vec();
    }
};

template <class T, int n>
inline vec<T,n> operator * (const T& c, const vec<T,n>& v) { return v * c; }

template <class T, int n>
inline T dot(const vec<T,n> & u, const vec<T,n> & v)
{
    T r  =  0;
    for(int i = 0; i < n; i++) r += u.x[i] * v.x[i];
    return r;
}

template <class T >
inline vec<T,3> cross(const vec<T,3> & u, const vec<T,3> & v)
{
    return vec<T,3> (
        u[1] * v[2] - u[2] * v[1],
        u[2] * v[0] - u[0] * v[2],
        u[0] * v[1] - u[1] * v[0]);
}

template<class T, int d>
inline vec<T,d> componentwise_max(const vec<T,d>& a, const vec<T,d>& b)
{
    vec<T,d> r;
    for(int i=0; i<d; i++) r[i] = std::max(a[i], b[i]);
    return r;
}

template<class T, int d>
inline vec<T,d> componentwise_min(const vec<T,d>& a, const vec<T,d>& b)
{
    vec<T,d> r;
    for(int i=0; i<d; i++) r[i] = std::min(a[i], b[i]);
    return r;
}

template <class T, int n>
inline std::ostream& operator << (std::ostream& out, const vec<T,n> & u)
{
    for(int i = 0; i < n; i++)
    {
        if(i) out << ' ';
        out << u[i];
    }
    return out;
}

template <class T, int n>
inline std::istream& operator >> (std::istream& in, vec<T,n> & u)
{
    for(int i = 0; i < n; i++)
    {
        in >> u[i];
    }
    return in;
}

typedef vec<double,2> vec2;
typedef vec<double,3> vec3;
typedef vec<int,2> ivec2;
typedef vec<int,3> ivec3;
typedef vec<float,2> vec2f;
typedef vec<float,3> vec3f;

#endif
