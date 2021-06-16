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

#ifndef _IJSONVALUE_H_
#define _IJSONVALUE_H_

#include <ostream>

enum EJsonValueType {
    JVT_Object,
    JVT_Array,
    JVT_String,
    JVT_Bool,
    JVT_Number,
    JVT_Null
};

class IJsonValue {
public:
    IJsonValue() = default;
    virtual ~IJsonValue() = default;

    virtual EJsonValueType GetType() const = 0;
    virtual std::ostream& operator<<(std::ostream&) const = 0;
    virtual std::istream& operator>>(std::istream&) = 0;
};

inline std::ostream& operator<<(std::ostream& os, const IJsonValue& v) {
    return v.operator<<(os);
}

inline std::istream& operator>>(std::istream& is, IJsonValue& v) {
    return v.operator>>(is);
}

#endif // _IJSONVALUE_H_
