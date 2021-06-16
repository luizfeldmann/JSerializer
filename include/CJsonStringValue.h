#ifndef _CJSONSTRINGVALUE_H_
#define _CJSONSTRINGVALUE_H_

#include <string>
#include "IJsonValue.h"

class CJsonStringValue: public IJsonValue, public std::string {
public:
    using std::string::string;
    CJsonStringValue() = default;
    ~CJsonStringValue() = default;
    CJsonStringValue(CJsonStringValue&&) = default;
    CJsonStringValue(const CJsonStringValue&) = default;
    CJsonStringValue& operator=(CJsonStringValue&&) = default;
    CJsonStringValue& operator=(const CJsonStringValue&) = default;


    CJsonStringValue(const std::string&);

    static std::string escape(const std::string& src);
    static std::string unescape(const std::string& src);

    EJsonValueType GetType() const final override;
    std::ostream& operator<<(std::ostream&) const final override;
    std::istream& operator>>(std::istream&) final override;
};

#endif // _CJSONSTRINGVALUE_H_
