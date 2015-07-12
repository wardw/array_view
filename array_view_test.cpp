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

	return os << ")";
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

TEST(bounds_iterator_test, increment)
{
	bounds<3> b = {4,5,6};
	bounds_iterator<3> iter{b};

	for (int i=0; i<(4*5*6); i++)
	{
		cout << *iter++ << endl;
	}
	cout << "off-the-end: " << *iter << endl;
	cout << "--off-the-end: " << *(--iter) << endl;
}

TEST(bounds_iterator_test, decrement)
{
	bounds<3> b = {4,5,6};
	bounds_iterator<3> iter{b, {3,4,6}};  // off-the-end value

	for (int i=0; i<(4*5*6); i++)
	{
		cout << *(--iter) << endl;
	}
	cout << "before-the-start: " << *(--iter) << endl;
	cout << "++before-the-start" << *(++iter) << endl;
}

TEST(bounds_iterator_test, beginYend)
{
	bounds<3> b = {4,5,6};

	bounds_iterator<3> iter = b.begin();
	while (iter != b.end())
	{
		cout << *iter++ << endl;
	}
	cout << "off-the-end: " << *iter << endl;
	cout << "--off-the-end: " << *(--iter) << endl;

	EXPECT_EQ(b.begin(), begin(b));
	EXPECT_EQ(b.end(), end(b));
}

TEST(bounds_iterator_test, difference)
{
	bounds<3> b = {4,5,9};
	bounds_iterator<3> iter(b, {2,4,7});
	iter += 25;

	EXPECT_EQ(3, (*iter)[0]);
	EXPECT_EQ(2, (*iter)[1]);
	EXPECT_EQ(5, (*iter)[2]);

	iter -= 25;
	EXPECT_EQ(2, (*iter)[0]);
	EXPECT_EQ(4, (*iter)[1]);
	EXPECT_EQ(7, (*iter)[2]);

	bounds_iterator<3> iter2 = iter + 25;
	EXPECT_EQ(3, (*iter2)[0]);
	EXPECT_EQ(2, (*iter2)[1]);
	EXPECT_EQ(5, (*iter2)[2]);

	bounds_iterator<3> iter3 = iter2 - 25;
	EXPECT_EQ(2, (*iter3)[0]);
	EXPECT_EQ(4, (*iter3)[1]);
	EXPECT_EQ(7, (*iter3)[2]);

	offset<3> off = iter3[25];
	EXPECT_EQ(3, off[0]);
	EXPECT_EQ(2, off[1]);
	EXPECT_EQ(5, off[2]);
}

TEST(array_view_test, constructors)
{
	vector<int> vec(4*5*9);
	std::generate_n(vec.begin(), 4*5*9, [&](){ static int curr{}; return curr++; });

	array_view<int, 3> av(vec, {4,5,9});

	int ans{};
	for (bounds_iterator<3> iter = begin(av.bounds()); iter!=end(av.bounds()); ++iter)
	{
		EXPECT_EQ(ans++, av[*iter]);
	}
}

TEST(array_view_test, slice)
{
	vector<int> vec(4*5*9);
	std::generate_n(vec.begin(), 4*5*9, [&](){ static int curr{}; return curr++; });

	array_view<int, 3> av(vec, {4,5,9});

	int slice = 2;
	array_view<int, 2> av2 = av[slice];

	int ans{slice*5*9};
	for (bounds_iterator<2> iter = begin(av2.bounds()); iter!=end(av2.bounds()); ++iter)
	{
		EXPECT_EQ(ans++, av2[*iter]);
		//cout << av2[*iter] << endl;
	}

	// Cascade slices
	int slice2 = 3;
	array_view<int, 1> av3 = av[slice][slice2];

	int ans2{slice*5*9 + slice2*9};
	for (bounds_iterator<1> iter = begin(av3.bounds()); iter!=end(av3.bounds()); ++iter)
	{
		EXPECT_EQ(ans2++, av3[*iter]);
		//cout << av3[*iter] << endl;
	}

	// Cascade to a single index
	EXPECT_EQ(119, av[slice][slice2][2]);
}
