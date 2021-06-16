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

#ifndef _CSERIALIZABLEMEMBER_H_
#define _CSERIALIZABLEMEMBER_H_

#include "ISerializableMember.h"

template <typename ObjectType, typename MemberType>
struct CSerializableMember : public ISerializableMember<ObjectType>
{
protected:
    MemberType ObjectType::* const PointerToMember;

public:
    CSerializableMember(const std::string& key, MemberType ObjectType::*ptr) :
        ISerializableMember<ObjectType>(key),
        PointerToMember(ptr)
    {
    }

    virtual bool Serialize(const ObjectType& source_instance, CSerializationOut& output) const override {
        return output[ISerializableMember<ObjectType>::name] = source_instance.*PointerToMember;
    }

    virtual bool Deserialize(ObjectType& dest_instance, const CSerializationIn& input) const override {
        if (!input.IsObject())
            return false;

        try {
            const CSerializationIn value = input[ISerializableMember<ObjectType>::name];
            return (value >> dest_instance.*PointerToMember);
        } catch(std::exception& ex) {
            return false;
        }
    }
};

#endif
