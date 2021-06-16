#ifndef _CJSONBOOLVALUE_H_
#define _CJSONBOOLVALUE_H_

#include "IJsonValue.h"

/**
 * @brief CJsonBoolValue
          IJsonValue implementation for boolean
          Read/writes 'true' or 'false'
 */
class CJsonBoolValue : public IJsonValue {
protected:
    bool value;

public:
    ~CJsonBoolValue() = default;
    CJsonBoolValue() = default;
    CJsonBoolValue(CJsonBoolValue&&) = default;
    CJsonBoolValue(const CJsonBoolValue&) = default;
    CJsonBoolValue& operator=(CJsonBoolValue&&) = default;
    CJsonBoolValue& operator=(const CJsonBoolValue&) = default;

    CJsonBoolValue(bool _value);
    CJsonBoolValue& operator=(bool);
    bool operator==(const CJsonBoolValue&) const;
    bool operator!=(const CJsonBoolValue&) const;
    bool operator==(bool) const;
    bool operator!=(bool) const;
    CJsonBoolValue& operator!();

    operator bool() const;

    EJsonValueType GetType() const final override;
    std::ostream& operator<<(std::ostream&) const final override;
    std::istream& operator>>(std::istream&) final override;
};

#endif // _CJSONBOOLVALUE_H_
