#include <gtest/gtest.h>
#include <BinarySerializer.h>

using StaticBinarySerializer = binser::Serializer<binser::polices::StaticStoragePolicy<1024>>;

TEST(TestBinSer, INT_OK) {
    StaticBinarySerializer ser{};
    int n = 100;
    int outN;

    ser.write(n);
    ser.read(outN);

    EXPECT_EQ(n, outN);
}

TEST(TestBinSer, NEG_INT_OK) {
    StaticBinarySerializer ser{};
    int n = -100;
    int outN;

    ser.write(n);
    ser.read(outN);

    EXPECT_EQ(n, outN);
}

TEST(TestBinSer, UINT_OK) {
    StaticBinarySerializer ser{};
    unsigned n = 100;
    unsigned outN;

    ser.write(n);
    ser.read(outN);

    EXPECT_EQ(n, outN);
}

TEST(TestBinSer, UINT_OVERFLOW_OK) {
    StaticBinarySerializer ser{};
    unsigned n = -1;
    unsigned outN;

    ser.write(n);
    ser.read(outN);

    EXPECT_EQ(n, outN);
}

TEST(TestBinSer, CHAR_OK) {
    StaticBinarySerializer ser{};
    char c = 'a';
    char outC;

    ser.write(c);
    ser.read(outC);

    EXPECT_EQ(c, outC);
}

TEST(TestBinSer, FLOAT_OK) {
    StaticBinarySerializer ser{};
    float f = 10.24f;
    float outF;

    ser.write(f);
    ser.read(outF);

    EXPECT_TRUE(std::abs(f - outF) < std::numeric_limits<float>::epsilon());
}

TEST(TestBinSer, FLOAT_MAX_OK) {
    StaticBinarySerializer ser{};
    float f = std::numeric_limits<float>::max();
    float outF;

    ser.write(f);
    ser.read(outF);

    EXPECT_TRUE(std::abs(f - outF) < std::numeric_limits<float>::epsilon());
}

TEST(TestBinSer, FLOAT_MIN_OK) {
    StaticBinarySerializer ser{};
    float f = std::numeric_limits<float>::min();
    float outF;

    ser.write(f);
    ser.read(outF);

    EXPECT_TRUE(std::abs(f - outF) < std::numeric_limits<float>::epsilon());
}

TEST(TestBinSer, DOUBLE_OK) {
    StaticBinarySerializer ser{};
    double d = 1124124124.124124;
    double outD;

    ser.write(d);
    ser.read(outD);

    EXPECT_TRUE(std::abs(d - outD) < std::numeric_limits<double>::epsilon());
}

TEST(TestBinSer, DOUBLE_MAX_OK) {
    StaticBinarySerializer ser{};
    double d = std::numeric_limits<double>::max();
    double outD;

    ser.write(d);
    ser.read(outD);

    EXPECT_TRUE(std::abs(d - outD) < std::numeric_limits<double>::epsilon());
}

TEST(TestBinSer, DOUBLE_MIN_OK) {
    StaticBinarySerializer ser{};
    double d = std::numeric_limits<double>::min();
    double outD;

    ser.write(d);
    ser.read(outD);

    EXPECT_TRUE(std::abs(d - outD) < std::numeric_limits<double>::epsilon());
}

TEST(TestBinSer, ARRAY_OK) {
    StaticBinarySerializer ser{};
    int arr[] = { 1, 2, 3, 4, 5, 6, -10, -9, -8, -7 };
    int outArr[sizeof(arr)/sizeof(int)];

    ser.write(arr);
    ser.read(outArr);

    for (int i = 0; i < (sizeof(arr)/sizeof(int)); i++) {
        EXPECT_EQ(arr[i], outArr[i]);
    }
}

TEST(TestBinSer, CHAR_PTR_OK) {
    StaticBinarySerializer ser{};
    const char* cp = "hello world !!!";
    char* dcp = (char*)std::malloc(std::strlen(cp) + 1);

    ser.write(cp);
    ser.read(dcp);

    size_t len = std::strlen(cp);
    for (int i = 0; i < len; i++) {
        EXPECT_EQ(cp[i], dcp[i]);
    }
}

TEST(TestBinSer, BOOL_OK) {
    StaticBinarySerializer ser{};
    bool b = true;
    bool outB;

    ser.write(b);
    ser.read(outB);

    EXPECT_EQ(b, outB);
}

TEST(TestBinSer, ENUM_OK) {
    enum class Color { RED, GREEN, BLUE };
    binser::Serializer<binser::polices::StaticStoragePolicy<1024>> ser{};

    Color color = Color::GREEN;
    Color outColor;

    ser.write(color);
    ser.read(outColor);

    EXPECT_EQ(static_cast<int>(color), static_cast<int>(outColor));
}

TEST(TestBinSer, POINTER_OK) {
    binser::Serializer<binser::polices::StaticStoragePolicy<1024>> ser{};
    int* ptr = new int(42);
    int* outPtr = new int;

    ser.write(*ptr);
    ser.read(*outPtr);

    EXPECT_EQ(*ptr, *outPtr);

    delete ptr;
    delete outPtr;
}

TEST(TestBinSer, STRING_OK) {
    binser::Serializer<binser::polices::StaticStoragePolicy<1024>> ser{};
    std::string str = "Hello, Binary Serialization!";
    std::string outStr;

    ser.write(str);
    ser.read(outStr);

    EXPECT_EQ(str, outStr);
}

struct Person {
    std::string name;
    int age{};

    static void serialize(binser::Serializer<binser::polices::StaticStoragePolicy<1024>>& ser, const Person& person) {
        ser.write(person.name);
        ser.write(person.age);
    }

    static Person deserialize(binser::Serializer<binser::polices::StaticStoragePolicy<1024>>& ser) {
        Person res;

        ser.read(res.name);
        ser.read(res.age);

        return res;
    }

};

TEST(TestBinSer, STRUCT_OK) {
    binser::Serializer<binser::polices::StaticStoragePolicy<1024>> ser{};
    Person person = { "John Doe", 25 };

    Person::serialize(ser, person);
    Person outPerson = Person::deserialize(ser);

    EXPECT_EQ(person.name, outPerson.name);
    EXPECT_EQ(person.age, outPerson.age);
}
