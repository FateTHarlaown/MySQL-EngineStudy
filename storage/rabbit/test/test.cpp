//
// Created by NorSnow_ZJ on 2018/12/27.
//

#include <gmock/gmock.h>

GTEST_API_ int main(int argc, char **argv) {
    testing::GTEST_FLAG(color) = "yes";
    testing::InitGoogleMock(&argc, argv);
    
    return RUN_ALL_TESTS();
}
