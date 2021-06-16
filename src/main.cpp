#include <iostream>

#include <sstream>
#include <string>

#include "CJsonFormatter.h"
#include "JSerializer.h"

#include "ISerializationScheme.h"

// BIRTHDAY CLASS
// =====================================

// IN .h FILE
struct birthday {
    int day;
    int month;
    int year;
};

DECLARE_SERIALIZATION_SCHEME(birthday)

// IN .cpp FILE
BEGIN_SERIALIZATION_SCHEME(birthday)
    SERIALIZATION_FIELD(day)
    SERIALIZATION_FIELD(month)
    SERIALIZATION_FIELD(year)
END_SERIALIZATION_SCHEME()

// ANIMAL CLASS
// =====================================

// IN .h FILE
struct animal : public IPolymorphicSerializable {
    std::string name;
    birthday bday;

    DECLARE_POLYMORPHIC_SERIALIZATION_MEMBERS()
};

DECLARE_SERIALIZATION_SCHEME(animal)

// IN .cpp FILE
BEGIN_SERIALIZATION_SCHEME(animal)
    SERIALIZATION_INHERIT(IPolymorphicSerializable)
    SERIALIZATION_FIELD(name)
    SERIALIZATION_FIELD(bday)
END_SERIALIZATION_SCHEME()

IMPLEMENT_POLYMORPHIC_SERIALIZATION_MEMBERS(animal)

// DOG CLASS
// =====================================

// IN .h FILE
struct dog : public animal {
    bool bGoodBoy;
    float cuteness;

    DECLARE_POLYMORPHIC_SERIALIZATION_MEMBERS()
};

DECLARE_SERIALIZATION_SCHEME(dog)

// IN .cpp FILE
BEGIN_SERIALIZATION_SCHEME(dog)
    SERIALIZATION_INHERIT(animal)
    SERIALIZATION_FIELD(bGoodBoy)
    SERIALIZATION_FIELD(cuteness)
END_SERIALIZATION_SCHEME()

IMPLEMENT_POLYMORPHIC_SERIALIZATION_MEMBERS(dog)

// CAT CLASS
// =====================================

// IN .h FILE
struct cat : public animal {
    double weight;
    std::string color;

    DECLARE_POLYMORPHIC_SERIALIZATION_MEMBERS()
};

DECLARE_SERIALIZATION_SCHEME(cat)

// IN .cpp FILE
BEGIN_SERIALIZATION_SCHEME(cat)
    SERIALIZATION_INHERIT(animal)
    SERIALIZATION_FIELD(weight)
    SERIALIZATION_FIELD(color)
END_SERIALIZATION_SCHEME()

IMPLEMENT_POLYMORPHIC_SERIALIZATION_MEMBERS(cat)

// FARM CLASS
// =====================================
struct farm {
    std::string name;
    std::vector<std::unique_ptr<animal>> animals;
};

// IN .h FILE
DECLARE_SERIALIZATION_SCHEME(farm)

// IN .cpp FILE
BEGIN_SERIALIZATION_SCHEME(farm)
    SERIALIZATION_FIELD(name)
    SERIALIZATION_FIELD(animals)
END_SERIALIZATION_SCHEME()

int main(int argc, char** argv)
{
    std::stringstream temp;

    {
        cat* c = new cat();
        c->name = "Frajola";
        c->color = "black";
        c->weight = 3.14;
        c->bday = {1, 1, 2000};

        dog* d = new dog();
        d->name = "Rex";
        d->bGoodBoy = true;
        d->cuteness = 1000;
        d->bday = {23, 05, 1997};

        farm f;
        f.name = "Napoles";
        f.animals.emplace_back( c );
        f.animals.emplace_back( d );

        Serialize(temp, f);
    }

    std::cout << "Serialized : " << std::endl;
    std::cout << temp.str() << std::endl;

    farm f2;
    Deserialize(temp, f2);

    std::cout << "Deserialized : " << std::endl;
    Serialize(std::cout, f2);


    /*{
        CJsonBoolValue b1 = true;
        std::cout << b1 << std::endl;

        b1 = false;
        std::cout << b1 << std::endl;

        CJsonBoolValue b2 = b1;
        b1 = true;
        std::cout << b1 << std::endl;
        std::cout << b2 << std::endl;
    }

    std::cout << std::endl;

    {
        CJsonNullValue n1;
        CJsonNullValue n2;
        n2 = n1;
        CJsonNullValue n3(n1);

        std::cout << n1 << std::endl;
        std::cout << n2 << std::endl;
        std::cout << n3 << std::endl;
    }

    std::cout << std::endl;

    {
        CJsonStringValue str1("hello world");
        CJsonStringValue str2 = "hello\tworld";
        CJsonStringValue str3( str1 );
        CJsonStringValue str4 = str2;

        std::cout << str1 << std::endl;
        std::cout << str2 << std::endl;
        std::cout << str3 << std::endl;
        std::cout << str4 << std::endl;

        str2 = "hello\nworld";
        str4 = "hello\"world";

        std::cout << str2 << std::endl;
        std::cout << str4 << std::endl;
    }

    std::cout << std::endl;

    {
        CJsonNumberValue n1(10);
        CJsonNumberValue n2(3.14);

        CJsonNumberValue n3 = 3;
        CJsonNumberValue n4 = 4.1;

        std::cout << n1 << std::endl;
        std::cout << n2 << std::endl;
        std::cout << n3 << std::endl;
        std::cout << n4 << std::endl;

        n3 = n1.get<int>() + n2.get<float>();
        std::cout << n3 << std::endl;
    }

    std::cout << std::endl;

    {
        CJsonArrayValue arr1;
        arr1.emplace_back( "first" );
        arr1.emplace_back( true );
        arr1.emplace_back( false );
        arr1.emplace_back( CJsonNullValue() );
        arr1.emplace_back( 3.14 );
        arr1.emplace_back( (int64_t) 255 );

        CJsonArrayValue arr2 = std::move(arr1);

        std::cout << arr1 << std::endl;
        std::cout << arr2 << std::endl;
    }

    std::cout << std::endl;

    {
        CJsonVariant v1( true );
        CJsonVariant v2( (int64_t)128 );
        CJsonVariant v3( 6.28 );
        CJsonVariant v4( "hello" );

        CJsonVariant v5 = std::move(v1);
        CJsonVariant v6 = std::move(v2);
        CJsonVariant v7 = std::move(v3);
        CJsonVariant v8 = std::move(v4);

        std::cout << v1 << std::endl;
        std::cout << v2 << std::endl;
        std::cout << v3 << std::endl;
        std::cout << v4 << std::endl;

        std::cout << v5 << std::endl;
        std::cout << v6 << std::endl;
        std::cout << v7 << std::endl;
        std::cout << v8 << std::endl;
    }

    {
        CJsonObjectValue o1;
        o1["string"] = "string";
        o1["real"] = 3.14;
        o1["bool"] = true;

        CJsonObjectValue o2;
        o2["null"] = CJsonNullValue();
        o2["hello"] = CJsonStringValue("world");
        o2["array"] = new CJsonArrayValue();
        o2["obj"] = CJsonVariant( new CJsonObjectValue( std::move(o1) ));

        std::cout << o1 << std::endl;
        std::cout << o2 << std::endl;
    }

    {
        std::stringstream mem;
        {
            CJsonVariant var;
            CJsonOutputSAX sax(var);

            sax.StartObject();

            sax.Key("hello");
            sax.String("world");

            sax.Key("integer");
            sax.Integer(135);

            sax.Key("real");
            sax.Double(9.81);

            sax.Key("array");
            sax.StartArray();
            sax.Bool(true);
            sax.Bool(false);
            sax.EndArray();

            sax.Key("child");
            sax.StartObject();
            sax.Key("null");
            sax.Null();
            sax.Key("String");
            sax.String("value");
            sax.EndObject();

            mem << var;
        }

        CJsonObjectValue read_obj;
        mem >> read_obj;

        CJsonFormatter(std::cout) << read_obj;
    }*/
}
