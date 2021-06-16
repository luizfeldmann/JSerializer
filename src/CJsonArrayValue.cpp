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

#include "CJsonArrayValue.h"
#include "CJsonFormatter.h"
#include "CJsonVariant.h"
#include <sstream>
#include <iostream>

CJsonArrayValue::CJsonArrayValue(CJsonArrayValue&& rhs) :
    std::list<CJsonVariant>( std::move( rhs ) )
{

}

CJsonArrayValue& CJsonArrayValue::operator=(CJsonArrayValue&& rhs) {
    clear();
    splice(end(), rhs);
    return *this;
}

EJsonValueType CJsonArrayValue::GetElementType() const {
    if (empty())
        return EJsonValueType::JVT_Null;

    return front()->GetType();
}

// OVERRIDES FROM IJSONVALUE
// =================================================================
EJsonValueType CJsonArrayValue::GetType() const {
    return EJsonValueType::JVT_Array;
}

std::ostream& CJsonArrayValue::operator<<(std::ostream& os) const {
    // if empty just print the trivial brackets
    if (empty()) {
        os << "[ ]";
        return os;
    }

    // open array
    os << "[";

    // only indent arrays with more than one object
    const bool indent = (size() > 1);
    if (indent) {
        os << json_indent;
        os << json_endl;
    }
    else
        os << ' ';

    // print all the fields
    auto before_last = std::prev(cend());
    for (auto iter = cbegin(); iter != cend(); ++iter) {
        iter->operator<<(os);

        // only add separator or new line if there is another item next
        if (iter != before_last) {
            os << ',';

            if (indent) os << json_endl;
        }
    }

    // return to previous indentation level
    if (indent) {
        os << json_unindent;
        os << json_endl;
    }
    else
        os << " ";

    // close array
    os << "]";

    return os;
};

std::istream& CJsonArrayValue::operator>>(std::istream& is)
{
    // empty from old values
    clear();

    // check it starts with object bracket
    char c;
    if ((c = (char)is.get()) != '[') {
        std::stringstream err;
        err << "Attempt parsing JSON array but found unexpected '" << c << "' instead of '['";

        std::cerr << err.str() << std::endl;
        throw std::runtime_error(err.str());
    }

    // read all the entries
    for(;;)
    {
        // consume white spaces until the value starts
        char peek;
        while ( std::isspace(peek = is.peek()) )
            is.get();

        // check if the array closed
        if (peek == ']') {
            is.get(); // consume the ']'
            return is;
        }

        // create the value and read to it
        emplace_back( CJsonVariant() );
        is >> (CJsonVariant&)back();

        // consume white spaces until the ',' separator or the ']'
        while (std::isspace(peek = is.peek()))
            is.get();

        if (peek == ']' || peek == ',') {
            is.get(); // consume the ']' or ','

            if (peek == ']')
                return is; // end of array
            // else continue (next element)
        }
        else {
            std::stringstream err;
            err << "Attempt parsing JSON array but found unexpected '" << c << "' instead of ']' or ','";

            std::cerr << err.str() << std::endl;
            throw std::runtime_error(err.str());
        }
    }

    return is;
}
