#include <gtest/gtest.h>

#include "DistinctObjectPairTracker.h"

using namespace ::testing;

class DistinctObjectPairTrackerTests : public Test
{
public:
    DistinctObjectPairTrackerTests()
    {

    }
};

class DummyObject
{
public:
    int dummyInt;
};

TEST_F(DistinctObjectPairTrackerTests, Size_GivenAddCalled3Times_Returns3)
{
    DistinctObjectPairTracker<DummyObject> tracker;

    DummyObject obj[3];

    tracker.Add(obj[0], obj[1]);
    tracker.Add(obj[1], obj[2]);
    tracker.Add(obj[2], obj[0]);

    ASSERT_EQ(tracker.Size(), 3u);
}

TEST_F(DistinctObjectPairTrackerTests, Contains_GivenAddedInOppositeOrder_ReturnsTrue)
{
    DistinctObjectPairTracker<DummyObject> tracker;

    DummyObject obj[2];

    tracker.Add(obj[1], obj[0]);

    ASSERT_TRUE(tracker.Contains(obj[0], obj[1]));
}
