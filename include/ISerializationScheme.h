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

#ifndef _ISERIALIZATIONSCHEME_H_
#define _ISERIALIZATIONSCHEME_H_

#include "CSerializableMember.h"
#include "CSerializableMethod.h"

#include <vector>
#include <functional>

class ISerializationScheme {
private:
    ISerializationScheme(const ISerializationScheme&) = delete;
    ISerializationScheme& operator=(const ISerializationScheme&) = delete;

public:
    ISerializationScheme() = default;
    ISerializationScheme(ISerializationScheme&&) = default;
    virtual ~ISerializationScheme() = default;
};

#include <iostream>

template <typename ObjectType>
class CSerializationScheme : public ISerializationScheme, public std::vector< std::unique_ptr<ISerializableMember<ObjectType>> >
{
protected:
    struct SchemeDelegate {
        std::function<bool(const ObjectType&, CSerializationOut&)> const Serialize;
        std::function<bool(ObjectType&, const CSerializationIn&)> const Deserialize;
    };
    std::vector<SchemeDelegate> inheritedSchemes;

public:
    CSerializationScheme() = default;

    // writes from memory to the stream (SerializationOut)
    bool Serialize(const ObjectType& instance, CSerializationOut& out, bool IsDelegateCall = false) const
    {
        // create object only if necessary (this function may be a delegate, so the caller already created the object)
        if (!IsDelegateCall)
            if (!out.StartObject())
                return false;

        for (const SchemeDelegate& delegate : inheritedSchemes)
            if (delegate.Serialize)
                if (!delegate.Serialize(instance, out))
                    return false;

        bool status = true;
        for (const std::unique_ptr<ISerializableMember<ObjectType>>& member : *this) {
            status &= member->Serialize(instance, out);
        }

        if (!IsDelegateCall)
            if (!out.EndObject())
                return false;

        return status;
    }

    // reads from the stream (SerializationIn) to memory
    bool Deserialize(ObjectType& instance, const CSerializationIn& in) const
    {
        for (const SchemeDelegate& delegate : inheritedSchemes)
            if (delegate.Deserialize)
                if (!delegate.Deserialize(instance, in))
                    return false;

        if (!in.IsObject())
            return false;

        bool status = true;
        for (const std::unique_ptr<ISerializableMember<ObjectType>>& member : *this) {
            if ( !(status &= member->Deserialize(instance, in))  )
                break;
        }

        return status;
    }

    // list a property in this scheme
    CSerializationScheme& operator<<(ISerializableMember<ObjectType>*&& insert)
    {
        this->emplace_back( std::unique_ptr<ISerializableMember<ObjectType>>(insert) );
        return *this;
    }

    // list properties from base class here
    template<typename ParentClass, typename = typename std::enable_if<std::is_base_of<ParentClass, ObjectType>::value, void>::type>
    CSerializationScheme& operator<<(const CSerializationScheme<ParentClass>& insert)
    {
        inheritedSchemes.emplace_back(SchemeDelegate{
            .Serialize = [&insert](const ObjectType& instance, CSerializationOut& out)->bool{
                return insert.Serialize(dynamic_cast<const ParentClass&>(instance), out, true);
            },
            .Deserialize = [&insert](ObjectType& instance, const CSerializationIn& in)->bool{
                return insert.Deserialize(dynamic_cast<ParentClass&>(instance), in);
            }
        });

        return *this;
    }
};

template<class ObjectType> const CSerializationScheme<ObjectType>& GetSerializationScheme();
template<class SchemeType> struct is_serialization_scheme : std::conditional< std::is_base_of<ISerializationScheme, SchemeType>::value , std::true_type, std::false_type>::type {};

#define DECLARE_SERIALIZATION_SCHEME(T) \
template<> const CSerializationScheme<T>& GetSerializationScheme<T>(); \
bool operator>>(const CSerializationIn& in, T& data); \
bool operator<<(CSerializationOut& out, const T& data);

#define BEGIN_SERIALIZATION_SCHEME(T) \
bool operator>>(const CSerializationIn& in, T& data) { return GetSerializationScheme<T>().Deserialize(data, in);   } \
bool operator<<(CSerializationOut& out, const T& data)    { return GetSerializationScheme<T>().Serialize(data, out); } \
template<> const CSerializationScheme<T>& GetSerializationScheme<T>() { \
using ObjectType = T; \
    static const CSerializationScheme<ObjectType> scheme( std::move( \
        CSerializationScheme<ObjectType>()

#define END_SERIALIZATION_SCHEME() \
    )); \
    return scheme; \
}

#define SERIALIZATION_FIELD(name) << new CSerializableMember<ObjectType, decltype(ObjectType::name)>(#name, &ObjectType::name)
#define SERIALIZATION_INHERIT(name) << GetSerializationScheme<name>()

#endif
