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

#ifndef _CPOLYMORPHICFACTORY_H_
#define _CPOLYMORPHICFACTORY_H_

#include "IPolymorphicSerializable.h"
#include <map>

class CPolymorphicFactory
{
private:
    static std::map<std::string, std::function<IPolymorphicSerializable*(void)>>& get_constructors();

public:
    template<class ObjectType>
    static typename std::enable_if<std::is_base_of<IPolymorphicSerializable, ObjectType>::value, bool>::type Register(const char* name)
    {
        get_constructors().operator[](name) = []()->IPolymorphicSerializable*{ return new ObjectType();};
        return true;
    }

    static IPolymorphicSerializable* Make(const std::string& class_name);
};

#endif // _CPOLYMORPHICFACTORY_H_
