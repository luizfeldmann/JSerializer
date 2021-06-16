#ifndef _CJSONOUTPUTSAX_H_
#define _CJSONOUTPUTSAX_H_

#include "CJsonVariant.h"
#include <stack>

class CJsonOutputSAX {
private:
    CJsonOutputSAX(CJsonOutputSAX&&) = delete;
    CJsonOutputSAX(const CJsonOutputSAX&) = delete;
    CJsonOutputSAX& operator=(const CJsonOutputSAX&) = delete;

    std::string key;
    std::stack<CJsonVariant*> stack;
    template<class, typename... Args> friend bool value(CJsonOutputSAX&, std::string&, Args...);

public:
    CJsonOutputSAX(CJsonVariant& _source);

    bool IsObject() const;
    bool StartObject();
    bool EndObject();

    bool IsArray() const;
    bool StartArray();
    bool EndArray();

    bool IsKey() const;
    bool Key(const std::string&);

    bool IsNull() const;

    bool Bool(bool);
    bool Double(double d);
    bool Integer(int64_t);
    bool Null();
    bool String(const std::string&);
};

#endif

