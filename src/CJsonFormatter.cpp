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

#include "CJsonFormatter.h"

// =================================================================================
// CJsonFormatter
// =================================================================================
CJsonFormatter::CJsonFormatter(  std::ostream& _out ) :
    std::ostream(this),
    out(_out)
{
}

CJsonFormatter::int_type CJsonFormatter::overflow(int_type c) {
    if (c == EOF)
        return c;

    if (out.put(c).eof())
        return EOF;

    if (c == '\n') {
        for (unsigned int i = 0; i < spaces; i++)
            if (out.put(' ').eof())
                return EOF;
    }

    return c;
}

void CJsonFormatter::Indent() {
    spaces += indent;
}

void CJsonFormatter::Unindent() {
    if (spaces < indent)
        spaces = 0;
    else
        spaces -= indent;
}

// =================================================================================
// CJsonIndenterControl
// =================================================================================
class CJsonFormatterControl {
private:
    CJsonFormatterControl(CJsonFormatterControl&&) = delete;
    CJsonFormatterControl(const CJsonFormatterControl&) = delete;
    CJsonFormatterControl& operator=(const CJsonFormatterControl&) = delete;

public:
    CJsonFormatterControl() = default;
    virtual CJsonFormatter& operator<<(CJsonFormatter& os) const = 0;
    virtual std::ostream& operator<<(std::ostream& os) const {
        return os;
    }
};

CJsonFormatter& operator<<(CJsonFormatter& os, const CJsonFormatterControl& c) {
    return c.operator<<(os);
}

std::ostream& operator<<(std::ostream& os, const CJsonFormatterControl& c) {
    try {
        CJsonFormatter& ji = dynamic_cast<CJsonFormatter&>(os);
        return c.operator<<(ji);
    }
    catch (...) {
        return c.operator<<(os);
    }
}

// =================================================================================
// Predefined controls
// =================================================================================
class CJsonFormatterControl_Indent : public CJsonFormatterControl {
public:
    CJsonFormatter& operator<<(CJsonFormatter& os) const final override {
        os.Indent();
        return os;
    }
};

class CJsonFormatterControl_Unindent : public CJsonFormatterControl {
public:
    CJsonFormatter& operator<<(CJsonFormatter& os) const final override {
        os.Unindent();
        return os;
    }
};

class CJsonFormatterControl_LineFeed : public CJsonFormatterControl {
public:
    CJsonFormatter& operator<<(CJsonFormatter& os) const final override {
        os << std::endl;
        return os;
    }

    virtual std::ostream& operator<<(std::ostream& os) const final override {
        os << ' ';
        return os;
    }
};

const CJsonFormatterControl& json_indent = CJsonFormatterControl_Indent();
const CJsonFormatterControl& json_unindent = CJsonFormatterControl_Unindent();
const CJsonFormatterControl& json_endl = CJsonFormatterControl_LineFeed();
