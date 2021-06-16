// Copyright (c) 2021 Luiz Gustavo Pfitscher e Feldmann
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef _CONTAINERSERIALIZATION_H_
#define _CONTAINERSERIALIZATION_H_

#include <type_traits>

#include "CSerializationIn.h"
#include "CSerializationOut.h"


// =====================================================================================
// IMPLEMENTATION OF INPUT/OUTPUT OPERATORS FOR (DE)SERIALIZING GENERIC CONTAINERS
// =====================================================================================
template<typename, typename = void> struct has_iterator : std::false_type {};
template <typename Container> struct has_iterator<Container, std::enable_if_t<std::is_same<typename Container::value_type, typename Container::iterator::value_type>::value>> : std::true_type {};

template<typename, typename = void> struct has_const_iterator : std::false_type {};
template <typename Container> struct has_const_iterator<Container, std::enable_if_t<std::is_same<typename Container::value_type, typename std::remove_const<typename Container::const_iterator::value_type>::type>::value>> : std::true_type {};

// ======================================================================================
// OUTPUT
// ======================================================================================
template <typename T> typename std::enable_if<has_const_iterator<T>::value, bool>::type
operator<<(CSerializationOut& out, const T& container) {
    if (!out.StartArray())
        return false;

    bool b = true;
    for (auto it = std::cbegin(container); it != std::cend(container); ++it) {
        const typename T::value_type& element = *it;
        b &= (out << element);
    }

    if (!out.EndArray())
        return false;

    return b;
}

// ======================================================================================
// INPUT
// ======================================================================================
template <template <typename, typename...> class Container, typename Content, typename Params> typename std::enable_if<std::is_same<Content, typename Container<Content, Params>::iterator::value_type>::value, bool>::type
operator>>(const CSerializationIn& in, Container<Content, Params>& container)
{
    static_assert(std::is_default_constructible<Content>::value, "Serialized container's elements must be default-constructible");

    if (!in.IsArray())
        return false;

    bool status = true;
    for (auto iter = in.cbegin(); iter != in.cend(); ++iter)
    {
        CSerializationIn element(*iter);
        container.emplace_back(); // construct with default arguments

        if ( !(status &= (element >> container.back())) ) // write to value in the back
            break;
    }

    return status;
}

#endif
