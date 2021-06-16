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

#include "CJsonOutputSAX.h"

#include "CJsonArrayValue.h"
#include "CJsonBoolValue.h"
#include "CJsonNullValue.h"
#include "CJsonNumberValue.h"
#include "CJsonObjectValue.h"
#include "CJsonStringValue.h"

CJsonOutputSAX::CJsonOutputSAX(CJsonVariant& _source)
{
    stack.push(&_source);
}

bool CJsonOutputSAX::IsObject() const {
    return (!stack.empty())
    ? stack.top()->GetType() == EJsonValueType::JVT_Object
    : false;
}

bool CJsonOutputSAX::IsArray() const {
    return (!stack.empty())
    ? stack.top()->GetType() == EJsonValueType::JVT_Array
    : false;
}

bool CJsonOutputSAX::IsNull() const {
    return (!stack.empty())
    ? stack.top()->GetType() == EJsonValueType::JVT_Null
    : false;
}

bool CJsonOutputSAX::IsKey() const {
    return IsObject() && !key.empty();
}

bool CJsonOutputSAX::StartObject() {
    if (IsNull()) {
        stack.top()->reset( new CJsonObjectValue() );
        return true;
    }

    if (IsKey()) {
        CJsonObjectValue* parent = (CJsonObjectValue*)stack.top()->get();
        parent->emplace_back( key, CJsonVariant() );
        CJsonVariant& child = parent->back().second;
        child.reset( new CJsonObjectValue() );

        stack.push(&child);

        return true;
    }
    else if (IsArray()) {
        CJsonArrayValue* parent = (CJsonArrayValue*)stack.top()->get();
        parent->emplace_back( CJsonVariant() );
        CJsonVariant& child = parent->back();
        child.reset( new CJsonObjectValue() );

        stack.push(&child);

        return true;
    }

    return false;
}

bool CJsonOutputSAX::EndObject() {
    if (!IsObject())
        return false;

    stack.pop();
    return true;
}

bool CJsonOutputSAX::StartArray() {
    if (IsNull()) {
        stack.top()->reset( new CJsonArrayValue() );
        return true;
    }

    if (!IsKey())
        return false;

    CJsonObjectValue* parent = (CJsonObjectValue*)stack.top()->get();
    parent->emplace_back( key, CJsonVariant() );
    CJsonVariant& child = parent->back().second;
    child.reset( new CJsonArrayValue() );

    stack.push(&child);
    return true;
}

bool CJsonOutputSAX::EndArray() {
    if (!IsArray())
        return false;

    stack.pop();
    return true;
}

template<class V, typename... Args> bool value(CJsonOutputSAX& j, std::string& key, Args... args) {
    if (j.stack.empty())
        return false;

    IJsonValue* value = j.stack.top()->get();

    if (j.IsKey()) {
        ((CJsonObjectValue*)value)->emplace_back( key, new V(args...) );
        key.clear();
    }
    else if (j.IsArray())
        ((CJsonArrayValue*)value)->emplace_back( new V(args...) );
    else
        return false;

    return true;
}

bool CJsonOutputSAX::Key(const std::string& _key) {
    if (!IsObject())
        return false;

    key = _key;
    return true;
}

bool CJsonOutputSAX::Bool(bool b) {
    return value<CJsonBoolValue, bool>(*this, key, b);
}

bool CJsonOutputSAX::Double(double d) {
    return value<CJsonNumberValue, double>(*this, key, d);
}

bool CJsonOutputSAX::Integer(int64_t i) {
    return value<CJsonNumberValue, int64_t>(*this, key, i);
}

bool CJsonOutputSAX::Null() {
    return value<CJsonNullValue>(*this, key);
}

bool CJsonOutputSAX::String(const std::string& str) {
    return value<CJsonStringValue, const std::string&>(*this, key, str);
}
