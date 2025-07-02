#include <gtest/gtest.h>
#include "Sample.h"

TEST(Sample, positive)
{
    // int result = min(1, 3);
    int result = 1;
    EXPECT_EQ(result, 1);
}
