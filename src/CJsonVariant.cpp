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

#include "CJsonVariant.h"

#include "CJsonArrayValue.h"
#include "CJsonBoolValue.h"
#include "CJsonObjectValue.h"
#include "CJsonStringValue.h"

#include <sstream>
#include <iostream>
#include <iomanip>

CJsonVariant::CJsonVariant() :
    std::unique_ptr<IJsonValue>( new CJsonNullValue() )
{
}

CJsonVariant::CJsonVariant(bool b) :
    std::unique_ptr<IJsonValue>( new CJsonBoolValue(b) )
{

}

CJsonVariant::CJsonVariant(double d) :
    std::unique_ptr<IJsonValue>( new CJsonNumberValue(d) )
{

}

CJsonVariant::CJsonVariant(int64_t i) :
    std::unique_ptr<IJsonValue>( new CJsonNumberValue(i) )
{

}

CJsonVariant::CJsonVariant(const std::string& s) :
    std::unique_ptr<IJsonValue>( new CJsonStringValue(s) )
{

}

CJsonVariant::CJsonVariant(const CJsonNullValue&&) :
    std::unique_ptr<IJsonValue>( new CJsonNullValue() )
{

}

CJsonVariant& CJsonVariant::operator=(bool b) {
    reset( new CJsonBoolValue(b) );
    return *this;
}

CJsonVariant& CJsonVariant::operator=(double d) {
    reset ( new CJsonNumberValue(d) );
    return *this;
}

CJsonVariant& CJsonVariant::operator=(int64_t i) {
    reset ( new CJsonNumberValue(i) );
    return *this;
}

CJsonVariant& CJsonVariant::operator=(const std::string& str) {
    reset ( new CJsonStringValue(str) );
    return *this;
}

CJsonVariant& CJsonVariant::operator=(const CJsonNullValue&&) {
    reset ( );
    return *this;
}

// OVERRIDES FROM IJSONVALUE
// =================================================================
EJsonValueType CJsonVariant::GetType() const {
    if (this->operator bool())
        return this->get()->GetType();

    return EJsonValueType::JVT_Null;
}

std::ostream& CJsonVariant::operator<<(std::ostream& os) const {
    if (this->operator bool())
        return this->get()->operator<<(os);

    os << "null";
    return os;
}

std::istream& CJsonVariant::operator>>(std::istream& is)
{
    const char peek = is.peek();

    // decide which type of value is coming
    switch (peek) {
        case '"':
            reset( new CJsonStringValue() );
        break;

        case '[':
            reset( new CJsonArrayValue() );
        break;

        case '{':
            reset( new CJsonObjectValue() );
        break;

        case 'N':
        case 'n':
            reset( new CJsonNullValue() );
        break;

        case 'T':
        case 't':
        case 'F':
        case 'f':
            reset( new CJsonBoolValue() );
        break;

        case '+':
        case '-':
        case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
            reset( new CJsonNumberValue() );
        break;

        default: {
            std::stringstream err;
            err << "Invalid JSON value: unexpected character '" << (char)peek << "' or 0x" << std::hex << std::setfill('0') << std::setw(2) << (int)peek << " cannot be parsed";

            std::cerr << err.str() << std::endl;
            throw std::runtime_error(err.str());
        }
    }

    // read to the IJsonValue* stored in this unique_ptr
    is >> *get();

    return is;
}
