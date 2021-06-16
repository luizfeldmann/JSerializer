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

#include "CJsonStringValue.h"
#include <sstream>

CJsonStringValue::CJsonStringValue(const std::string& str) :
    std::string( str )
{
}

EJsonValueType CJsonStringValue::GetType() const {
    return EJsonValueType::JVT_String;
}

std::ostream& CJsonStringValue::operator<<(std::ostream& os) const {
    os << "\"" << escape(*this) << "\"";
    return os;
};

static char* hex(char c) {
    static char text[6];
    sprintf(text, "%04x", c);
    return text;
}

std::string CJsonStringValue::escape(const std::string& src) {
    std::stringstream sb;

    const char* first = src.c_str();
    const char* last  = first + src.size();
    for (const char* c = src.c_str(); c != last; c++) {
        switch (*c) {
            case '"':  sb << "\\\""; break;
            case '\\': sb << "\\\\"; break;
            case '/': sb << "\\/"; break;
            case '\b': sb << "\\b"; break;
            case '\t': sb << "\\t"; break;
            case '\n': sb << "\\n"; break;
            case '\f': sb << "\\f"; break;
            case '\r': sb << "\\r"; break;

            default:
                if (*c < ' ')
                    sb << "\\u" << hex(*c);
                else
                    sb << (char)*c;
            break;
        }
    }

    return sb.str();
}

std::istream& CJsonStringValue::operator>>(std::istream& is) {
    char c, prev = '"';

    // check the string starts with a quote
    if (is.get() != prev)
        throw std::logic_error("JSON string is not quoted");


    // read characters until the next quote is found
    std::stringstream ss;
    for (c = is.get(); !((c == '"') && (prev != '\\')); prev = c, c = is.get() )
    {
        if (c == EOF || is.eof() || is.bad())
            throw std::underflow_error("Unexpected end of stream while parsing JSON string");

        ss << c;
    }

    // unescape the sequence
    this->std::string::operator=( unescape(ss.str()) );

    return is;
}

std::string CJsonStringValue::unescape(const std::string& src) {
    std::stringstream sb;

    for (const char* c = src.c_str(); *c != '\0'; c++) {
        // regular unescaped characters
        if (*c != '\\') {
            sb << *c;
            continue;
        }

        const char next = *(++c);
        switch(next)
        {
            case '\0':
                throw std::underflow_error("Unexpected end of string escaping JSON");
            break;

            case '"':
            case '\\':
            case '/':
                sb << next;
            break;

            case 'b': sb << '\b'; break;
            case 't': sb << '\t'; break;
            case 'n': sb << '\n'; break;
            case 'f': sb << '\f'; break;
            case 'r': sb << '\r'; break;

            case 'u': {
                char code[5] = {0};
                for (uint8_t i = 0; i < 4; i++)
                    if ((code[i] = *(++c)) == '\0')
                        throw std::underflow_error("Unexpected end of string while escaping \\uXXXX sequence");

                unsigned int hex;
                if (sscanf(code, "%04x", &hex) != 1 || hex > ' ')
                    throw std::runtime_error("Could not parse \\u escape sequence");

                sb << char(hex);
            }
            break;
        }
    }

    return sb.str();
}
