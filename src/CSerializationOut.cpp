#include "CSerializationOut.h"

bool CSerializationOut::operator<<(bool b) {
    return Bool(b);
}

bool CSerializationOut::operator<<(double d) {
    return Double(d);
}

bool CSerializationOut::operator<<(float f) {
    return Double(f);
}

bool CSerializationOut::operator<<(int8_t i) {
    return Integer(i);
}

bool CSerializationOut::operator<<(uint8_t i) {
    return Integer(i);
}

bool CSerializationOut::operator<<(int16_t i) {
    return Integer(i);
}

bool CSerializationOut::operator<<(uint16_t i) {
    return Integer(i);
}

bool CSerializationOut::operator<<(int32_t i) {
    return Integer(i);
}

bool CSerializationOut::operator<<(uint32_t i) {
    return Integer(i);
}

bool CSerializationOut::operator<<(int64_t i) {
    return Integer(i);
}

bool CSerializationOut::operator<<(const char* data) {
    return String(data);
}

bool CSerializationOut::operator<<(const std::string& str) {
    return String(str);
}

CSerializationOut::ValueAssignContext::ValueAssignContext(CSerializationOut& so, const std::string& k) :
    out(so),
    key(k)
{
}

CSerializationOut::ValueAssignContext CSerializationOut::operator[](const std::string& key) {
    return ValueAssignContext(*this, key);
}
