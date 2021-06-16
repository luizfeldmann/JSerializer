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

#include "CJsonObjectValue.h"
#include "CJsonFormatter.h"
#include "CJsonVariant.h"
#include "CJsonStringValue.h"

#include <sstream>
#include <iostream>
#include <algorithm>

CJsonObjectValue::CJsonObjectValue(CJsonObjectValue&& rhs) :
    std::list<std::pair<std::string, CJsonVariant>>( std::move( rhs ) )
{

}

CJsonObjectValue& CJsonObjectValue::operator=(CJsonObjectValue&& rhs){
    clear();
    splice(end(), rhs);
    return *this;
}

CJsonVariant* CJsonObjectValue::at(const std::string& key) {
    iterator iter = std::find_if(begin(), end(), [&key](const std::pair<std::string, CJsonVariant>& v)->bool{
        return v.first == key;
    });

    return (iter == end() ? NULL : &iter->second );
}

const CJsonVariant* CJsonObjectValue::at(const std::string& key) const {
    const_iterator iter = std::find_if(cbegin(), cend(), [&key](const std::pair<std::string, CJsonVariant>& v)->bool{
        return v.first == key;
    });

    return (iter == cend() ? NULL : &iter->second );
}

CJsonObjectMemberToken CJsonObjectValue::operator[](const std::string& key) {
    CJsonVariant* const var = at(key);

    if (var == NULL)
        return CJsonObjectMemberToken(key, *this);   // allow the token to insert here
    else
        return CJsonObjectMemberToken(var); // allow the token to read/write here
}

const CJsonObjectMemberToken CJsonObjectValue::operator[](const std::string& key) const {
    const CJsonVariant* const var = at(key);

    if (var == NULL)
        return CJsonObjectMemberToken();                // token can't do anything
    else
        return CJsonObjectMemberToken(var);    // token may read it
}

// OVERRIDES FROM IJSONVALUE
// =================================================================
EJsonValueType CJsonObjectValue::GetType() const {
    return EJsonValueType::JVT_Object;
}

std::ostream& CJsonObjectValue::operator<<(std::ostream& os) const {
    // the object is empty
    if (empty()) {
        os << "{ }";
        return os;
    }

    // open object
    os << "{";

    // only indent objects with more than one field
    const bool indent = (size() > 1);
    if (indent) {
        os << json_indent;
        os << json_endl;
    }
    else
        os << ' ';

    const auto beforeLast = std::prev( cend() );
    for (auto iter = cbegin(); iter != cend(); ++iter) {
        os << "\"" << iter->first << "\" : " << iter->second;

        if (iter != beforeLast) {
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

    // close object
    os << "}";

    return os;
};

std::istream& CJsonObjectValue::operator>>(std::istream& is) {
    // empty from old values
    clear();

    // check it starts with object bracket
    char c;
    if ((c = (char)is.get()) != '{') {
        std::stringstream ss;
        ss << "Attemp parsing JSON Object but found '" << c << " instead of expected '{'";

        std::cerr << ss.str();
        throw std::runtime_error(ss.str());
    }

    for(;;)
    {
        // consume spaces until a '"' string marker is found or the object is closed
        char peek;
        while ((peek = is.peek()) != '"') {
            is.get(); // consume it

            // check for closure
            if (peek == '}')
                return is;

            // check for bad characters
            else if (!std::isspace(peek))
                throw std::runtime_error("Unexpected character inside JSON object while searching for key name");
        }

        // found string, now read it
        CJsonStringValue field_name;
        is >> field_name;

        // consume util past ":"
        while ((c = is.get()) != ':')
            if (!std::isspace(c))
                throw std::runtime_error("Unexpected character inside JSON object while searching for ':' key-value separator");

        // consume spaces until a meaningful value is found
        while (std::isspace(peek = is.peek()))
            is.get();

        // read the value
        emplace_back(field_name, CJsonVariant());
        is >> (CJsonVariant&)back().second;

        // consume whitespaces until a ',' separating the next field is found
        while (std::isspace(peek = is.peek()))
            is.get();

        if (peek == ',' || peek == '}')
        {
            is.get();

            if (peek == '}')
                break;
        }
        else {
            std::stringstream ss;
            ss << "Unexpected character after end of JSON object value '" << peek << "'" << " when '}' or ',' were expected";

            std::cerr << ss.str() << std::endl;
            throw std::runtime_error(ss.str());
        }
    }

    return is;
}
