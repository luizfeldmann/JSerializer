#include "CPolymorphicFactory.h"

std::map<std::string, std::function<IPolymorphicSerializable*(void)>>& CPolymorphicFactory::get_constructors() {
    static std::map<std::string, std::function<IPolymorphicSerializable*(void)>> the_map;
    return the_map;
}

IPolymorphicSerializable* CPolymorphicFactory::Make(const std::string& class_name)
{
    const auto& constructors = get_constructors();

    auto found = constructors.find(class_name);
    if (found == constructors.cend())
        return NULL;

    if (!(found->second))
        return NULL;

    return found->second();
}
