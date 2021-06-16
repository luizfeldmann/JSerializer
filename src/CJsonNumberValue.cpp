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

#include "CJsonNumberValue.h"

#include <locale>
#include <sstream>

// DEFAULT CONSTRUCTOR
CJsonNumberValue::CJsonNumberValue() :
    CJsonNumberValueBase((int64_t)0)
{
}

// CONSTRUCT FROM DOUBLE
CJsonNumberValue::CJsonNumberValue(double d) :
    CJsonNumberValueBase(d)
{

}

// CONSTRUCT FROM INTEGER
CJsonNumberValue::CJsonNumberValue(int64_t i) :
    CJsonNumberValueBase(i)
{
}

// ASSIGN FROM INTEGER
CJsonNumberValue& CJsonNumberValue::operator=(int64_t i) {
    this->CJsonNumberValueBase::operator=( i );
    return *this;
}

// ASSIGN FROM DOUBLE
CJsonNumberValue& CJsonNumberValue::operator=(double d) {
    this->CJsonNumberValueBase::operator=( d );
    return *this;
}

// OVERRIDES FROM IJSONVALUE
// =================================================================
EJsonValueType CJsonNumberValue::GetType() const {
    return EJsonValueType::JVT_Number;
}

template <class T>
class punct_facet: public std::numpunct<T> {
protected:
    const T sep;
    T do_decimal_point() const { return sep; }

public:
    punct_facet(T _sep) : sep(_sep) {}
};

std::ostream& CJsonNumberValue::operator<<(std::ostream& os) const {
    // configure stream to use '.' separator and print decimal
    os.imbue(std::locale(os.getloc(), new punct_facet<char>('.') ));
    os << std::dec;

    std::visit([&os](const auto& v)->void{
        os << v;
    }, (CJsonNumberValueBase&)*this);

    return os;
}

#include <iostream>
std::istream& CJsonNumberValue::operator>>(std::istream& is) {
    std::stringstream ss;

    bool point = false;
    bool exp = false;

    for (;;) {
        char peek = is.peek();

        if (peek == EOF)
            break;

        if (std::isdigit(peek) || (!point && (point |= (peek == '.'))) || (!exp && (exp |= (peek == 'E' || peek == 'e'))) || (peek == '+') || (peek == '-') ) {
            ss << (char)is.get();
            continue;
        }

        break;
    }

    if (point || exp)
        this->CJsonNumberValueBase::operator=( std::stod(ss.str(), NULL) );
    else
        this->CJsonNumberValueBase::operator=( std::stoll( ss.str(), NULL ) );

    return is;
}
