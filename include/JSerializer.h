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

#ifndef _JSERIALIZER_H_
#define _JSERIALIZER_H_

#include "CJsonArrayValue.h"
#include "CJsonBoolValue.h"
#include "CJsonNullValue.h"
#include "CJsonNumberValue.h"
#include "CJsonObjectValue.h"
#include "CJsonStringValue.h"
#include "CJsonVariant.h"

#include "CJsonFormatter.h"
#include "ISerializationScheme.h"
#include "ContainerSerialization.h"
#include "CPolymorphicSerializable.h"

template <class ObjectType>
bool Serialize(std::ostream& os, const ObjectType& data){
    CJsonVariant json;
    CSerializationOut serializer(json);
    const bool status = (serializer << data);

    if (status)
        CJsonFormatter(os) << json;

    return status;
}

template <class ObjectType>
bool Deserialize(std::istream& is, ObjectType& data){
    CJsonVariant json;

    is >> json;

    CSerializationIn deserializer(json);
    return (deserializer >> data);
}

#endif // _JSERIALIZER_H_
