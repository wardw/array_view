#include "hummingbird/solve/array_view.h"

#include <iostream>
#include "gtest/gtest.h"

using namespace std;
using namespace hummingbird;

template <size_t Rank>
ostream& operator<<(ostream& os, const offset<Rank>& off)
{
	os << "("
	   << off[0];

	for (size_t i=1; i<off.rank; ++i) {
		os << "," << off[i];
	}

	os << ")";

}

TEST(offset_test, Initialize)
{
	offset<4> off0;
	EXPECT_EQ(0, off0[0]);
	EXPECT_EQ(0, off0[1]);
	EXPECT_EQ(0, off0[2]);
	EXPECT_EQ(0, off0[3]);

	offset<1> off1(4);
	EXPECT_EQ(4, off1[0]);

	offset<3> off2 = {1,2,3};
	EXPECT_EQ(1, off2[0]);
	EXPECT_EQ(2, off2[1]);
	EXPECT_EQ(3, off2[2]);

	// Constraints
	//offset<0> off3;  // Should not compile: Size of Rank must be greater than 0
	//offset<0> off3({});  // Should not compile: Size of Rank must be greater than 0

	// todo
	offset<2> off4 = {1,2,3};  // Should not compile: "Size of Rank must equal the size of the initialiser list"

}


TEST(bounds_test, size)
{
	bounds<3> b = {2,3,4};
	EXPECT_EQ(24, b.size());
}
