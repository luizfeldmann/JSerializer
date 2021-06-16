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

#ifndef _CSERIALIZATIONOUT_
#define _CSERIALIZATIONOUT_

#include "CJsonOutputSAX.h"

class CSerializationOut : protected CJsonOutputSAX
{
public:
    using CJsonOutputSAX::CJsonOutputSAX;

    using CJsonOutputSAX::StartObject;
    using CJsonOutputSAX::EndObject;

    using CJsonOutputSAX::StartArray;
    using CJsonOutputSAX::EndArray;

    bool operator<<(bool);

    bool operator<<(double);
    bool operator<<(float);

    bool operator<<(int8_t);
    bool operator<<(uint8_t);

    bool operator<<(int16_t);
    bool operator<<(uint16_t);

    bool operator<<(int32_t);
    bool operator<<(uint32_t);

    bool operator<<(int64_t);

    bool operator<<(const char*);
    bool operator<<(const std::string&);

    template<typename T, typename = typename std::enable_if<std::is_enum<T>::value, T>::type>
    bool operator<<(T data) {
        return Integer((int64_t)data);
    }

    class ValueAssignContext {
    protected:
        CSerializationOut& out;
        const std::string key;

    private:
        ValueAssignContext(CSerializationOut& so, const std::string& k);
        ValueAssignContext(const ValueAssignContext&) = delete;         // non copyable
        ValueAssignContext(ValueAssignContext&&) = delete;              // non movable
        ValueAssignContext& operator=(const ValueAssignContext&);       // non copy-assignable
        ValueAssignContext& operator=(ValueAssignContext&&) = delete;   // non move-assignable
        friend class CSerializationOut;

    public:
        template <class T>
        bool operator=(const T& data) {
            if (!out.Key(key.c_str()))
                return false;

            return (out << data);
        }
    };

    ValueAssignContext operator[](const std::string&);
};

#endif
