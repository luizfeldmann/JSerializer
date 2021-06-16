#ifndef _CJSONARRAYVALUE_H_
#define _CJSONARRAYVALUE_H_

#include <list>
#include "CJsonVariant.h"

class CJsonArrayValue: public IJsonValue, public std::list<CJsonVariant> {
private:
    /**
     * @brief This type of value cannot be copied because the underlying types may not be copyable either
     */
    explicit CJsonArrayValue(const CJsonArrayValue&) = delete;
    CJsonArrayValue& operator=(const CJsonArrayValue&) = delete;

public:
    ~CJsonArrayValue() = default;

    CJsonArrayValue() = default;
    CJsonArrayValue(CJsonArrayValue&&);
    CJsonArrayValue& operator=(CJsonArrayValue&&);

    using iterator = std::list<CJsonVariant>::iterator;
    using const_iterator = std::list<CJsonVariant>::const_iterator;

    /**
     * @brief Returns the type of the elements stored in this array
     * @details An empty array will return the NULL type
     *          The check is performed on the first element only, assuming all types are the same
     */
    EJsonValueType GetElementType() const;

    EJsonValueType GetType() const final override;
    std::ostream& operator<<(std::ostream&) const final override;
    std::istream& operator>>(std::istream&) final override;
};

#endif // _CJSONARRAYVALUE_H_
