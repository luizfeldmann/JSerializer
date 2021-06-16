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

#include "CJsonNullValue.h"
#include <istream>

// OVERRIDES FROM IJSONVALUE
// =================================================================
EJsonValueType CJsonNullValue::GetType() const {
    return EJsonValueType::JVT_Null;
}

std::ostream& CJsonNullValue::operator<<(std::ostream& os) const {
    os << "null";
    return os;
};

std::istream& CJsonNullValue::operator>>(std::istream& is) {
    static const char expected[4] = {'n', 'u', 'l', 'l'};

    for (uint8_t i = 0; i < sizeof(expected); i++)
        if (std::tolower(is.get()) != expected[i] || is.bad() || is.eof())
            throw std::runtime_error("Could not read 'null' from JSON stream");

    return is;
}
