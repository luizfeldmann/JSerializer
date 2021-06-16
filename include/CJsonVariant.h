#ifndef _CJSONVARIANT_H_
#define _CJSONVARIANT_H_

#include <memory>
#include "IJsonValue.h"
#include "CJsonNullValue.h"
#include "CJsonNumberValue.h"

class CJsonVariant : public IJsonValue, public std::unique_ptr<IJsonValue> {
public:
    using std::unique_ptr<IJsonValue>::unique_ptr;

    CJsonVariant();
    CJsonVariant(bool);
    CJsonVariant(double);
    CJsonVariant(int64_t);
    CJsonVariant(const std::string&);
    CJsonVariant(const CJsonNullValue&&);

    CJsonVariant& operator=(bool);
    CJsonVariant& operator=(double);
    CJsonVariant& operator=(int64_t);
    CJsonVariant& operator=(const std::string&);
    CJsonVariant& operator=(const CJsonNullValue&&);

    template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value && !std::is_pointer<T>::value>::type>
    CJsonVariant& operator=(T value) {
        reset( new CJsonNumberValue( value ) );
        return *this;
    }

    EJsonValueType GetType() const final override;
    std::ostream& operator<<(std::ostream&) const final override;
    std::istream& operator>>(std::istream&) final override;
};

#endif // _CJSONVARIANT_H_
