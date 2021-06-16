#ifndef _CJSONNULLVALUE_H_
#define _CJSONNULLVALUE_H_

#include "IJsonValue.h"

/**
 * @brief CJsonNullValue
          Read/writes the 'null' word
 */
class CJsonNullValue : public IJsonValue {
public:
    ~CJsonNullValue() = default;
    CJsonNullValue() = default;
    CJsonNullValue(CJsonNullValue&&) = default;
    CJsonNullValue(const CJsonNullValue&) = default;
    CJsonNullValue& operator=(CJsonNullValue&&) = default;
    CJsonNullValue& operator=(const CJsonNullValue&) = default;

    EJsonValueType GetType() const final override;
    std::ostream& operator<<(std::ostream&) const final override;
    std::istream& operator>>(std::istream&) final override;
};

#endif // _CJSONNULLVALUE_H_
