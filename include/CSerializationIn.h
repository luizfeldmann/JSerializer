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

#ifndef _CSERIALIZATIONIN_H_
#define _CSERIALIZATIONIN_H_

#include "CJsonVariant.h"
#include "CJsonArrayValue.h"

class CSerializationIn
{
protected:
    const CJsonVariant& _internal;

public:
    CSerializationIn(const CJsonVariant&);

    bool IsObject() const;
    bool IsArray() const;

    CJsonArrayValue::const_iterator cbegin() const;
    CJsonArrayValue::const_iterator cend() const;
    CSerializationIn operator[](const std::string&) const;

    bool operator>>(bool&) const;

    bool operator>>(float&) const;
    bool operator>>(double&) const;

    bool operator>>(int8_t&) const;
    bool operator>>(uint8_t&) const;

    bool operator>>(int16_t&) const;
    bool operator>>(uint16_t&) const;

    bool operator>>(int32_t&) const;
    bool operator>>(uint32_t&) const;

    bool operator>>(int64_t&) const;

    bool operator>>(std::string& data) const;
    bool operator>>(char* data) const;

    template<typename T, typename = typename std::enable_if<std::is_enum<T>::value, T>::type>
    bool operator>>(T& data) const {
        int64_t temp;

        if (!operator>>(temp))
            return false;

        data = (T)temp;

        return true;
    }
};

#endif
