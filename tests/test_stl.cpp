#include <gtest/gtest.h>
#include <BinarySerializer.h>

TEST(TestBinSer, STATIC_VECTOR_TEST_OK) {
    binser::StaticBinSer ser;

    std::vector<std::string> vec{
            "hello", "world", "one", "two\n\n\t\t"
    };
    std::vector<std::string> outVec;

    ser.write(vec);
    ser.read(outVec);

    EXPECT_TRUE(vec.size() == outVec.size());
    for (int i = 0; i < vec.size(); i++) {
        EXPECT_TRUE(vec[i] == outVec[i]);
    }
}

TEST(TestBinSer, DYNAMIC_VECTOR_TEST_OK) {
    binser::DynamicBinSer ser;

    std::vector<std::string> vec{
            "hello", "world", "one", "two\n\n\t\t"
    };
    std::vector<std::string> outVec;

    ser.write(vec);
    ser.read(outVec);

    EXPECT_TRUE(vec.size() == outVec.size());
    for (int i = 0; i < vec.size(); i++) {
        EXPECT_TRUE(vec[i] == outVec[i]);
    }
}