#ifndef _CJSONNUMBERVALUE_H_
#define _CJSONNUMBERVALUE_H_

#include <variant>
#include <cstdint>
#include <limits>
#include <type_traits>

#include "IJsonValue.h"

#include <iostream>

using CJsonNumberValueBase = std::variant<double, int64_t>;

/**
 * @brief CJsonNumberValue
 *        Represents a JSON number that can be either an integer or a real (double)
 */
class CJsonNumberValue : public IJsonValue, protected CJsonNumberValueBase {
public:
    virtual ~CJsonNumberValue() = default;


    CJsonNumberValue(CJsonNumberValue&&) = default;
    CJsonNumberValue(const CJsonNumberValue&) = default;
    CJsonNumberValue& operator=(CJsonNumberValue&&) = default;
    CJsonNumberValue& operator=(const CJsonNumberValue&) = default;

    CJsonNumberValue();
    CJsonNumberValue(double);
    CJsonNumberValue(int64_t);

    CJsonNumberValue& operator=(int64_t);
    CJsonNumberValue& operator=(double);

    template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value && !std::is_pointer<T>::value>::type>
    CJsonNumberValue& operator=(T value)
    {
        static_assert(!std::is_same<uint64_t, T>::value, "UINT64 is not supported!");
        CJsonNumberValueBase::operator=( (typename std::conditional<std::is_floating_point<T>::value, double, int64_t>::type)value );
        return *this;
    }

    /**
     * @brief Constructs this JSON number from any arithmetic type under 64 bits
     *        Floating point types are converted to double
     *        Other integer types are converted to int64_t
     */
    template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value>::type>
    CJsonNumberValue(T value) :
        CJsonNumberValueBase((typename std::conditional<std::is_floating_point<T>::value, double, int64_t>::type)value)
    {
        static_assert(!std::is_same<uint64_t, T>::value, "UINT64 is not supported!");
    }

    /**
     * @brief Casts this JSON number to the desired type
     *        This conversion may cause overflow and produce bad results depending on the actual stored type
     */
    template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value>::type>
    T get() const {
        return std::visit([](const auto& v)->T { return (T)v; }, (CJsonNumberValueBase&)*this );
    }

    /**
     * @brief Checks if the stored value can be cast to the desired type without overflow
     */
    template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value>::type>
    bool can_get() const
    {
        return std::visit([](const auto& v)-> bool
        {
            const auto bot = std::numeric_limits<T>::lowest();
            const auto top = std::numeric_limits<T>::max();

            return (v >= bot) && (v <= top );
        }, (CJsonNumberValueBase&)*this );
    }

    EJsonValueType GetType() const final override;
    std::ostream& operator<<(std::ostream&) const final override;
    std::istream& operator>>(std::istream&) final override;
};

#endif // _CJSONNUMBERVALUE_H_
