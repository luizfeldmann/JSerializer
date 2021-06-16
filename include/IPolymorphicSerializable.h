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

#ifndef _IPOLYMORPHICSERIALIZABLE_H_
#define _IPOLYMORPHICSERIALIZABLE_H_

#include "ISerializationScheme.h"

class IPolymorphicSerializable {
public:
    IPolymorphicSerializable() = default;
    virtual ~IPolymorphicSerializable() = default;
    virtual bool operator>>(CSerializationOut&) const = 0;     // serializes itself to the output
    virtual bool operator<<(const CSerializationIn&) = 0; // (de)serializes itself from the input
    virtual const char* class_name() const = 0;               // returns the name of this type
};

DECLARE_SERIALIZATION_SCHEME(IPolymorphicSerializable)

// Declaration of of the required members - must be placed in a 'public' section
#define DECLARE_POLYMORPHIC_SERIALIZATION_MEMBERS() \
    virtual bool operator>>(CSerializationOut&) const; \
    virtual bool operator<<(const CSerializationIn&); \
    virtual const char* class_name() const;

// default implementation of polymorphic serialization - it just calls the default operators
#define IMPLEMENT_POLYMORPHIC_SERIALIZATION_MEMBERS(T) \
bool T::operator>>(CSerializationOut& out) const { return ::operator<<(out, *this); } \
bool T::operator<<(const CSerializationIn& in) { return ::operator>>(in, *this); } \
const char* T::class_name() const { return #T; } \
const static bool is_registered_##T = CPolymorphicFactory::Register<T>(#T);



#endif
