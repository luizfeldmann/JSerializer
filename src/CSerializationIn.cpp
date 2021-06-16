#include "CSerializationIn.h"

#include <cstring>
#include <sstream>
#include "CJsonBoolValue.h"
#include "CJsonObjectValue.h"
#include "CJsonStringValue.h"

CSerializationIn::CSerializationIn(const CJsonVariant& parent) :
    _internal(parent)
{
}

CSerializationIn CSerializationIn::operator[](const std::string& key) const {
    if (!IsObject())
        throw std::bad_variant_access();

    CJsonObjectValue* obj = dynamic_cast<CJsonObjectValue*>(_internal.get());
    if (!obj)
        throw std::bad_cast();

    CJsonVariant* child = obj->at(key);
    if (!child)
        throw std::invalid_argument(key);

    return CSerializationIn(*child);
}

CJsonArrayValue::const_iterator CSerializationIn::cbegin() const {
    if (!IsArray())
        throw std::bad_variant_access();

    CJsonArrayValue* arr = dynamic_cast<CJsonArrayValue*>(_internal.get());
    if (!arr)
        throw std::bad_cast();

    return arr->cbegin();
}

CJsonArrayValue::const_iterator CSerializationIn::cend() const {
    if (!IsArray())
        throw std::bad_variant_access();

    CJsonArrayValue* arr = dynamic_cast<CJsonArrayValue*>(_internal.get());
    if (!arr)
        throw std::bad_cast();

    return arr->cend();
}

bool CSerializationIn::IsObject() const {
    return _internal.GetType() == EJsonValueType::JVT_Object;
}

bool CSerializationIn::IsArray() const{
    return _internal.GetType() == EJsonValueType::JVT_Array;
}

template<typename Type, EJsonValueType e>
static const Type* cast_variant(const IJsonValue* ptr) {
    if (!ptr) return NULL;

    if (ptr->GetType() != e)
        return NULL;

    return dynamic_cast<const Type*>(ptr);
}

bool CSerializationIn::operator>>(bool& b) const {
    const CJsonBoolValue* value = cast_variant<CJsonBoolValue, EJsonValueType::JVT_Bool>(_internal.get());

    if (!value)
        return false;

    b = *value;
    return true;
}

bool CSerializationIn::operator>>(std::string& data) const {
    const CJsonStringValue* value = cast_variant<CJsonStringValue, EJsonValueType::JVT_String>(_internal.get());

    if (!value)
        return false;

    data = *value;

    return true;
}

bool CSerializationIn::operator>>(char* data) const {
    std::string temp;

    if (!operator>>(temp))
        return false;

    strcpy(data, temp.c_str());
    return true;
}

template<typename Type>
static bool get_numeric(const IJsonValue* src, Type& data) {
    const CJsonNumberValue* value = cast_variant<CJsonNumberValue, EJsonValueType::JVT_Number>(src);
    if (!value)
        return false;

    if (!value->can_get<Type>())
        return false;

    data = value->get<Type>();
    return true;
}

bool CSerializationIn::operator>>(float& f) const {
    return get_numeric<float>(_internal.get(), f);
}

bool CSerializationIn::operator>>(double& d) const {
    return get_numeric<double>(_internal.get(), d);
}

bool CSerializationIn::operator>>(int8_t& i) const {
    return get_numeric<int8_t>(_internal.get(), i);
}

bool CSerializationIn::operator>>(uint8_t& i) const {
    return get_numeric<uint8_t>(_internal.get(), i);
}

bool CSerializationIn::operator>>(int16_t& i) const {
    return get_numeric<int16_t>(_internal.get(), i);
}
bool CSerializationIn::operator>>(uint16_t& i) const {
    return get_numeric<uint16_t>(_internal.get(), i);
}

bool CSerializationIn::operator>>(int32_t& i) const {
    return get_numeric<int32_t>(_internal.get(), i);
}
bool CSerializationIn::operator>>(uint32_t& i) const {
    return get_numeric<uint32_t>(_internal.get(), i);
}

bool CSerializationIn::operator>>(int64_t& i) const {
    return get_numeric<int64_t>(_internal.get(), i);
}
