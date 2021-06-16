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

#ifndef _CPOLYMORHPICSERIALIZABLE_H_
#define _CPOLYMORHPICSERIALIZABLE_H_

#include "IPolymorphicSerializable.h"
#include "CPolymorphicFactory.h"

template<typename ObjectType> typename std::enable_if<std::is_base_of<IPolymorphicSerializable, ObjectType>::value, bool>::type
operator<<(CSerializationOut& out, const std::unique_ptr<ObjectType>& uptr)
{
    if (uptr)
        return uptr->operator>>(out); // calls the virtual serialization method

    // object is NULL
    if (!out.StartObject())
        return false;

    out["class_name"] = ""; // EMPTY CLASS NAME (could be "NULL" but we save space this way)

    if (!out.EndObject())
        return false;

    return true;
}

template<typename ObjectType> typename std::enable_if<std::is_base_of<IPolymorphicSerializable, ObjectType>::value, bool>::type
operator>>(const CSerializationIn& in, std::unique_ptr<ObjectType>& uptr)
{
    // read the name of the class
    if (!in.IsObject())
        return false;

    std::string class_name;
    if (!(in["class_name"] >> class_name))
        return false;

    // pointer is NULL ?
    if (class_name.empty())
    {
        uptr.reset();
        return true;
    }

    // create instance from class name
    ObjectType* newobj = dynamic_cast<ObjectType*>( CPolymorphicFactory::Make(class_name) );
    if (!newobj)
        return false; // failed to construct

    uptr.reset( newobj ); // store instance in unique_ptr
    return uptr->operator<<(in); // read additional data using virtual reader;
}

#endif
