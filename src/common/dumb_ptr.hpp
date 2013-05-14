#ifndef TMWA_COMMON_DUMB_PTR_HPP
#define TMWA_COMMON_DUMB_PTR_HPP
//    ptr.hpp - temporary new/delete wrappers
//
//    Copyright © 2013 Ben Longbons <b.r.longbons@gmail.com>
//
//    This file is part of The Mana World (Athena server)
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "sanity.hpp"

// unmanaged new/delete-able pointer
// should be replaced by std::unique_ptr<T>
template<class T>
class dumb_ptr
{
    T *impl;
public:
    explicit
    dumb_ptr(T *p=nullptr)
    : impl(p)
    {}

    void delete_()
    {
        delete impl;
        forget();
    }
    template<class... A>
    void new_(A&&... a)
    {
        impl = new T(std::forward<A>(a)...);
    }
    template<class... A>
    static
    dumb_ptr<T> make(A&&... a)
    {
        return dumb_ptr<T>(new T(std::forward<A>(a)...));
    }
    void forget()
    {
        impl = nullptr;
    }

    T& operator *() const
    {
        return *impl;
    }
    T *operator->() const
    {
        return impl;
    }

    explicit
    operator bool()
    {
        return impl;
    }
    bool operator !()
    {
        return !impl;
    }
};

// unmanaged new/delete-able pointer
// should be replaced by std::unique_ptr<T[]> or std::vector<T>
template<class T>
class dumb_ptr<T[]>
{
    T *impl;
    size_t sz;
public:
    dumb_ptr() : impl(), sz() {}
    dumb_ptr(T *p, size_t z)
    : impl(p)
    , sz(z)
    {}

    void delete_()
    {
        delete[] impl;
        forget();
    }
    void new_(size_t z)
    {
        impl = new T[z];
    }
    static
    dumb_ptr<T[]> make(size_t z)
    {
        return dumb_ptr<T[]>(new T[z], z);
    }
    void forget()
    {
        impl = nullptr;
        sz = 0;
    }

    T& operator[](size_t i) const
    {
        return impl[i];
    }

    explicit
    operator bool()
    {
        return impl;
    }
    bool operator !()
    {
        return !impl;
    }
};

#endif // TMWA_COMMON_DUMB_PTR_HPP
