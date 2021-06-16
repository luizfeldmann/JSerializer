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

#include "CJsonBoolValue.h"
#include <istream>

// CONSTRUCT FROM VALUE
CJsonBoolValue::CJsonBoolValue(bool _value) :
    value(_value)
{
}

CJsonBoolValue::operator bool() const {
    return value;
}

CJsonBoolValue& CJsonBoolValue::operator=(bool b) {
    value = b;
    return *this;
}

bool CJsonBoolValue::operator==(const CJsonBoolValue& other) const {
    return value == other.value;
}
bool CJsonBoolValue::operator!=(const CJsonBoolValue& other) const {
    return value != other.value;
}

bool CJsonBoolValue::operator==(bool b) const {
    return value == b;
}
bool CJsonBoolValue::operator!=(bool b) const {
    return value != b;
}

CJsonBoolValue& CJsonBoolValue::operator!() {
    value = !value;
    return *this;
}


// OVERRIDES FROM IJSONVALUE
// =================================================================
EJsonValueType CJsonBoolValue::GetType() const {
    return EJsonValueType::JVT_Bool;
}

std::ostream& CJsonBoolValue::operator<<(std::ostream& os) const {
    os << (value ? "true" : "false");
    return os;
};

std::istream& CJsonBoolValue::operator>>(std::istream& is) {
    switch (is.get()) {
        case '0': value = false; break;
        case '1': value = true; break;

        case 't':
        case 'T':
            {
                static const char expected[3] = {'r', 'u', 'e'};
                for (uint8_t i = 0; i < sizeof(expected); i++)
                    if (std::tolower(is.get()) != expected[i] || is.bad() || is.eof())
                        goto error;

                value = true;
            }
        break;

        case 'f':
        case 'F':
            {
                static const char expected[4] = {'a', 'l', 's', 'e'};
                for (uint8_t i = 0; i < sizeof(expected); i++)
                    if (std::tolower(is.get()) != expected[i] || is.bad() || is.eof())
                        goto error;

                value = false;
            }
        break;

        default:
            error:
            throw std::runtime_error("Cannot parse boolean from stream");
    }

    return is;
}
