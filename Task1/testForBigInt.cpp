#include "BigInt.cpp"
#include "tests.hpp"


TEST_CASE("Test 1")
{
    MyLongNum a{"155250"};
    MyLongNum b{"-110"};
    MyLongNum c{"155140"};
    MyLongNum d{"-17077500"};

    REQUIRE(a + b == c);
    REQUIRE(a * b == d);
}