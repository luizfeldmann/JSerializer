#ifndef _CJSONOBJEECTMEMBERTOKEN_h_
#define _CJSONOBJEECTMEMBERTOKEN_h_

#include <string>

class CJsonVariant;
class CJsonObjectValue;

class CJsonObjectMemberToken {
private:
    CJsonObjectMemberToken(CJsonObjectMemberToken&&) = delete;
    CJsonObjectMemberToken(const CJsonObjectMemberToken&) = delete;
    CJsonObjectMemberToken& operator=(const CJsonObjectMemberToken&) = delete;
    CJsonObjectMemberToken& operator=(CJsonObjectMemberToken&&) = delete;

protected:
    const std::string key;
    CJsonObjectValue* object = NULL;
    CJsonVariant* variant = NULL;
    const CJsonVariant* constvariant = NULL;

public:
    CJsonObjectMemberToken() = default;
    CJsonObjectMemberToken(CJsonVariant&);       // used to read or modify a value that is already present
    CJsonObjectMemberToken(const CJsonVariant&); // used to read a value that is already present
    CJsonObjectMemberToken(const std::string&, CJsonObjectValue&);   // used to insert a value that is not already present

    operator const CJsonVariant&() const;        // decays to const variant for reading
    CJsonVariant& operator=(CJsonVariant&&);     // assigns value
};

#endif
