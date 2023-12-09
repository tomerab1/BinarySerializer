#include <gtest/gtest.h>
#include <BinarySerializer.h>

TEST(TestBinSer, STATIC_STORAGE_INT_OK) {
    binser::StaticBinSer ser{};
    int n = 100;
    int outN;

    ser.write(n);
    ser.read(outN);

    EXPECT_EQ(n, outN);
}

TEST(TestBinSer, DYNAMIC_STORAGE_INT_OK) {
    binser::DynamicBinSer ser{};
    int n = 100;
    int outN;

    ser.write(n);
    ser.read(outN);

    EXPECT_EQ(n, outN);
}

TEST(TestBinSer, STATIC_STORAGE_NEG_INT_OK) {
    binser::StaticBinSer ser{};
    int n = -100;
    int outN;

    ser.write(n);
    ser.read(outN);

    EXPECT_EQ(n, outN);
}

TEST(TestBinSer, DYNAMIC_STORAGE_NEG_INT_OK) {
    binser::DynamicBinSer ser{};
    int n = -100;
    int outN;

    ser.write(n);
    ser.read(outN);

    EXPECT_EQ(n, outN);
}

TEST(TestBinSer, STATIC_STORAGE_UINT_OK) {
    binser::StaticBinSer ser{};
    unsigned n = 100;
    unsigned outN;

    ser.write(n);
    ser.read(outN);

    EXPECT_EQ(n, outN);
}

TEST(TestBinSer, DYNAMIC_STORAGE_UINT_OK) {
    binser::DynamicBinSer ser{};
    unsigned n = 100;
    unsigned outN;

    ser.write(n);
    ser.read(outN);

    EXPECT_EQ(n, outN);
}

TEST(TestBinSer, STATIC_STORAGE_UINT_OVERFLOW_OK) {
    binser::StaticBinSer ser{};
    unsigned n = -1;
    unsigned outN;

    ser.write(n);
    ser.read(outN);

    EXPECT_EQ(n, outN);
}

TEST(TestBinSer, DYNAMIC_STORAGE_UINT_OVERFLOW_OK) {
    binser::DynamicBinSer ser{};
    unsigned n = -1;
    unsigned outN;

    ser.write(n);
    ser.read(outN);

    EXPECT_EQ(n, outN);
}

TEST(TestBinSer, STATIC_STORAGE_CHAR_OK) {
    binser::StaticBinSer ser{};
    char c = 'a';
    char outC;

    ser.write(c);
    ser.read(outC);

    EXPECT_EQ(c, outC);
}

TEST(TestBinSer, DYNAMIC_STORAGE_CHAR_OK) {
    binser::DynamicBinSer ser{};
    char c = 'a';
    char outC;

    ser.write(c);
    ser.read(outC);

    EXPECT_EQ(c, outC);
}

TEST(TestBinSer, STATIC_STORAGE_FLOAT_OK) {
    binser::StaticBinSer ser{};
    float f = 10.24f;
    float outF;

    ser.write(f);
    ser.read(outF);

    EXPECT_TRUE(std::abs(f - outF) < std::numeric_limits<float>::epsilon());
}

TEST(TestBinSer, STATIC_STORAGE_FLOAT_MAX_OK) {
    binser::StaticBinSer ser{};
    float f = std::numeric_limits<float>::max();
    float outF;

    ser.write(f);
    ser.read(outF);

    EXPECT_TRUE(std::abs(f - outF) < std::numeric_limits<float>::epsilon());
}

TEST(TestBinSer, DYNAIC_STORAGE_FLOAT_MAX_OK) {
    binser::DynamicBinSer ser{};
    float f = std::numeric_limits<float>::max();
    float outF;

    ser.write(f);
    ser.read(outF);

    EXPECT_TRUE(std::abs(f - outF) < std::numeric_limits<float>::epsilon());
}

TEST(TestBinSer, STATIC_STORAGE_FLOAT_MIN_OK) {
    binser::StaticBinSer ser{};
    float f = std::numeric_limits<float>::min();
    float outF;

    ser.write(f);
    ser.read(outF);

    EXPECT_TRUE(std::abs(f - outF) < std::numeric_limits<float>::epsilon());
}

TEST(TestBinSer, DYNAMIC_STORAGE_FLOAT_MIN_OK) {
    binser::DynamicBinSer ser{};
    float f = std::numeric_limits<float>::min();
    float outF;

    ser.write(f);
    ser.read(outF);

    EXPECT_TRUE(std::abs(f - outF) < std::numeric_limits<float>::epsilon());
}

TEST(TestBinSer, STATIC_STORAGE_DOUBLE_OK) {
    binser::StaticBinSer ser{};
    double d = 1124124124.124124;
    double outD;

    ser.write(d);
    ser.read(outD);

    EXPECT_TRUE(std::abs(d - outD) < std::numeric_limits<double>::epsilon());
}

TEST(TestBinSer, DYNAMIC_STORAGE_DOUBLE_OK) {
    binser::DynamicBinSer ser{};
    double d = 1124124124.124124;
    double outD;

    ser.write(d);
    ser.read(outD);

    EXPECT_TRUE(std::abs(d - outD) < std::numeric_limits<double>::epsilon());
}

TEST(TestBinSer, STATIC_STORAGE_DOUBLE_MAX_OK) {
    binser::StaticBinSer ser{};
    double d = std::numeric_limits<double>::max();
    double outD;

    ser.write(d);
    ser.read(outD);

    EXPECT_TRUE(std::abs(d - outD) < std::numeric_limits<double>::epsilon());
}

TEST(TestBinSer, DYNAMIC_STORAGE_DOUBLE_MAX_OK) {
    binser::DynamicBinSer ser{};
    double d = std::numeric_limits<double>::max();
    double outD;

    ser.write(d);
    ser.read(outD);

    EXPECT_TRUE(std::abs(d - outD) < std::numeric_limits<double>::epsilon());
}

TEST(TestBinSer, STATIC_STORAGE_DOUBLE_MIN_OK) {
    binser::StaticBinSer ser{};
    double d = std::numeric_limits<double>::min();
    double outD;

    ser.write(d);
    ser.read(outD);

    EXPECT_TRUE(std::abs(d - outD) < std::numeric_limits<double>::epsilon());
}

TEST(TestBinSer, DYNAMIC_STORAGE_DOUBLE_MIN_OK) {
    binser::DynamicBinSer ser{};
    double d = std::numeric_limits<double>::min();
    double outD;

    ser.write(d);
    ser.read(outD);

    EXPECT_TRUE(std::abs(d - outD) < std::numeric_limits<double>::epsilon());
}

TEST(TestBinSer, STATIC_STORAGE_ARRAY_OK) {
    binser::StaticBinSer ser{};
    int arr[] = { 1, 2, 3, 4, 5, 6, -10, -9, -8, -7 };
    int outArr[sizeof(arr)/sizeof(int)];

    ser.write(arr);
    ser.read(outArr);

    for (int i = 0; i < (sizeof(arr)/sizeof(int)); i++) {
        EXPECT_EQ(arr[i], outArr[i]);
    }
}

TEST(TestBinSer, DYNAMIC_STORAGE_ARRAY_OK) {
    binser::DynamicBinSer ser{};
    int arr[] = { 1, 2, 3, 4, 5, 6, -10, -9, -8, -7 };
    int outArr[sizeof(arr)/sizeof(int)];

    ser.write(arr);
    ser.read(outArr);

    for (int i = 0; i < (sizeof(arr)/sizeof(int)); i++) {
        EXPECT_EQ(arr[i], outArr[i]);
    }
}

TEST(TestBinSer, STATIC_STORAGE_CHAR_PTR_OK) {
    binser::StaticBinSer ser{};
    const char* cp = "hello world !!!";
    char* dcp = (char*)std::malloc(std::strlen(cp) + 1);

    ser.write(cp);
    ser.read(dcp);

    size_t len = std::strlen(cp);
    for (int i = 0; i < len; i++) {
        EXPECT_EQ(cp[i], dcp[i]);
    }
}

TEST(TestBinSer, DYNAMIC_STORAGE_CHAR_PTR_OK) {
    binser::DynamicBinSer ser{};
    const char* cp = "hello world !!!";
    char* dcp = (char*)std::malloc(std::strlen(cp) + 1);

    ser.write(cp);
    ser.read(dcp);

    size_t len = std::strlen(cp);
    for (int i = 0; i < len; i++) {
        EXPECT_EQ(cp[i], dcp[i]);
    }
}

TEST(TestBinSer, STATIC_STORAGE_BOOL_OK) {
    binser::StaticBinSer ser{};
    bool b = true;
    bool outB;

    ser.write(b);
    ser.read(outB);

    EXPECT_EQ(b, outB);
}

TEST(TestBinSer, DYNAMIC_STORAGE_BOOL_OK) {
    binser::DynamicBinSer ser{};
    bool b = true;
    bool outB;

    ser.write(b);
    ser.read(outB);

    EXPECT_EQ(b, outB);
}

TEST(TestBinSer, STATIC_STORAGE_ENUM_OK) {
    enum class Color { RED, GREEN, BLUE };
    binser::StaticBinSer ser{};

    Color color = Color::GREEN;
    Color outColor;

    ser.write(color);
    ser.read(outColor);

    EXPECT_EQ(static_cast<int>(color), static_cast<int>(outColor));
}

TEST(TestBinSer, DYNAMIC_STORAGE_ENUM_OK) {
    enum class Color { RED, GREEN, BLUE };
    binser::DynamicBinSer ser{};

    Color color = Color::GREEN;
    Color outColor;

    ser.write(color);
    ser.read(outColor);

    EXPECT_EQ(static_cast<int>(color), static_cast<int>(outColor));
}

TEST(TestBinSer, STATIC_STORAGE_POINTER_OK) {
    binser::StaticBinSer ser{};
    int* ptr = new int(42);
    int* outPtr = new int;

    ser.write(*ptr);
    ser.read(*outPtr);

    EXPECT_EQ(*ptr, *outPtr);

    delete ptr;
    delete outPtr;
}

TEST(TestBinSer, DYNAMIC_STORAGE_POINTER_OK) {
    binser::DynamicBinSer ser{};
    int* ptr = new int(42);
    int* outPtr = new int;

    ser.write(*ptr);
    ser.read(*outPtr);

    EXPECT_EQ(*ptr, *outPtr);

    delete ptr;
    delete outPtr;
}

TEST(TestBinSer, STATIC_STORAGE_STRING_OK) {
    binser::StaticBinSer ser{};
    std::string str = "Hello, Binary Serialization!";
    std::string outStr;

    ser.write(str);
    ser.read(outStr);

    EXPECT_EQ(str, outStr);
}

TEST(TestBinSer, DYNAMIC_STORAGE_STRING_OK) {
    binser::DynamicBinSer ser{};
    std::string str = "Hello, Binary Serialization!";
    std::string outStr;

    ser.write(str);
    ser.read(outStr);

    EXPECT_EQ(str, outStr);
}


struct Person {
    std::string name;
    int age{};

    static void serialize(binser::StaticBinSer& ser, const Person& person) {
        ser.write(person.name);
        ser.write(person.age);
    }

    static void serialize(binser::DynamicBinSer & ser, const Person& person) {
        ser.write(person.name);
        ser.write(person.age);
    }

    static Person deserialize(binser::StaticBinSer& ser) {
        Person res;

        ser.read(res.name);
        ser.read(res.age);

        return res;
    }

    static Person deserialize(binser::DynamicBinSer & ser) {
        Person res;

        ser.read(res.name);
        ser.read(res.age);

        return res;
    }
};

TEST(TestBinSer, STATIC_STORAGE_STRUCT_OK) {
    binser::StaticBinSer ser{};
    Person person = { "John Doe", 25 };

    Person::serialize(ser, person);
    Person outPerson = Person::deserialize(ser);

    EXPECT_EQ(person.name, outPerson.name);
    EXPECT_EQ(person.age, outPerson.age);
}

TEST(TestBinSer, DYNAMIC_STORAGE_STRUCT_OK) {
    binser::DynamicBinSer ser{};
    Person person = { "John Doe", 25 };

    Person::serialize(ser, person);
    Person outPerson = Person::deserialize(ser);

    EXPECT_EQ(person.name, outPerson.name);
    EXPECT_EQ(person.age, outPerson.age);
}
