#ifndef _CJSONOBJECTVALUE_H_
#define _CJSONOBJECTVALUE_H_

#include <list>
#include "CJsonVariant.h"
#include "CJsonObjectMemberToken.h"

class CJsonObjectValue : public IJsonValue, public std::list<std::pair<std::string, CJsonVariant>>  {
private:
    /**
     * @brief This JSON Object may contain other types which are not copyable, therefore it cannot itself be copyable
     */
    CJsonObjectValue(const CJsonObjectValue&) = delete;
    CJsonObjectValue& operator=(const CJsonObjectValue&) = delete;

public:
    ~CJsonObjectValue() = default;

    CJsonObjectValue() = default;
    CJsonObjectValue(CJsonObjectValue&&);
    CJsonObjectValue& operator=(CJsonObjectValue&&);

    /**
     * @brief Allows read/write access to a field of this object
     *        If the field does not exist, it reads to a NULL type
     *        If the field does not exist, assigning it will create it
     * @return A temporary handler that facilitates read/write/creation access to this field
     */
    CJsonObjectMemberToken operator[](const std::string&);
    const CJsonObjectMemberToken operator[](const std::string&) const;

    CJsonVariant* at(const std::string&);
    const CJsonVariant* at(const std::string&) const;

    EJsonValueType GetType() const final override;
    std::ostream& operator<<(std::ostream&) const final override;
    std::istream& operator>>(std::istream&) final override;
};

#endif // _CJSONOBJECTVALUE_H_
