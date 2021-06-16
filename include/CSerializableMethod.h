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

#ifndef _CSERIALIZABLEMETHOD_H_
#define _CSERIALIZABLEMETHOD_H_

#include "ISerializableMember.h"

template <typename ObjectType, typename ReturnType>
struct CSerializableMethod : public ISerializableMember<ObjectType>
{
protected:
    ReturnType(ObjectType::*PointerToMethod)() const;

public:
    CSerializableMethod(const std::string& key, ReturnType(ObjectType::*method)() const) :
        ISerializableMember<ObjectType>(key), PointerToMethod(method)
    {
    };

    virtual bool Serialize(const ObjectType& instance, CSerializationOut& out) const {
        return out[ISerializableMember<ObjectType>::name] = (instance.*PointerToMethod)();
    }

    virtual bool Deserialize(ObjectType& instance, const CSerializationIn& in) const {
        return true; // there is no place to save it, so just ignore it
    }
};

#define SERIALIZATION_METHOD(name) << new CSerializableMethod<ObjectType, decltype(std::declval<ObjectType>().name())>(#name, &ObjectType::name)

#endif // _CSERIALIZABLEMETHOD_H_
