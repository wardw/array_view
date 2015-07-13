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

class ArrayViewTest : public ::testing::Test {
public:
	ArrayViewTest() : vec(4*5*9), av(vec, {4,5,9}) {}
protected:
	virtual void SetUp()
	{
		int n{};
		std::generate(vec.begin(), vec.end(), [&]{ return n++; });
	}

	vector<int> vec;
	array_view<int, 3> av;
};

TEST_F(ArrayViewTest, Constructors)
{
	int start{};
	for (bounds_iterator<3> iter = begin(av.bounds()); iter!=end(av.bounds()); ++iter)
	{
		EXPECT_EQ(start++, av[*iter]);
	}
}

TEST_F(ArrayViewTest, Observers)
{

}

TEST_F(ArrayViewTest, Slicing)
{
	// Slices always act on the most significant dimension
	int x = 2;
	array_view<int, 2> sliced = av[x];
	int start{9*5*x};
	for (bounds_iterator<2> iter = begin(sliced.bounds()); iter!=end(sliced.bounds()); ++iter)
	{
		EXPECT_EQ(start++, sliced[*iter]);
	}

	// Cascade slices
	int y = 3;
	array_view<int, 1> sliced2 = av[x][y];
	int start2{9*5*x + 9*y};
	for (bounds_iterator<1> iter = begin(sliced2.bounds()); iter!=end(sliced2.bounds()); ++iter)
	{
		EXPECT_EQ(start2++, sliced2[*iter]);
	}

	// Cascade to a single index
	int z = 2;
	EXPECT_EQ(9*5*x + 9*y + z, av[x][y][z]);
}

TEST_F(ArrayViewTest, Sectioning)
{
	offset<3> origin{1,2,3};

	// with new bounds
	bounds<3> newBounds{2,3,4};
	strided_array_view<int, 3> section = av.section(origin, newBounds);

	int start = 9*5*origin[0] + 9*origin[1] + origin[2];
	for (bounds_iterator<3> iter = begin(section.bounds()); iter!=end(section.bounds()); ++iter)
	{
		offset<3> idx = *iter;
		int ans = start + 9*5*idx[0] + 9*idx[1] + idx[2];
		EXPECT_EQ(ans, section[*iter]);
	}

	// with bounds extending to extent of source view
	strided_array_view<int, 3> section2 = av.section(origin);
	EXPECT_EQ(av.bounds() - origin, section2.bounds());

	int start2 = 9*5*origin[0] + 9*origin[1] + origin[2];
	for (bounds_iterator<3> iter = begin(section2.bounds()); iter!=end(section2.bounds()); ++iter)
	{
		offset<3> idx = *iter;
		int ans = start2 + 9*5*idx[0] + 9*idx[1] + idx[2];
		EXPECT_EQ(ans, section2[*iter]);
	}
}

class StridedArrayViewTest : public ::testing::Test {
public:
	StridedArrayViewTest() : vec(4*8*12), av(vec, {4,8,12}) {}
protected:
	virtual void SetUp()
	{
		int n{};
		std::generate(vec.begin(), vec.end(), [&]{ return n++; });
	}

	vector<int> vec;
	array_view<int, 3> av;
};

TEST_F(StridedArrayViewTest, Constructors)
{
	// Default
	strided_array_view<int, 3> sav{};
	EXPECT_EQ(0, sav.size());

	// From array_view
	strided_array_view<int, 3> sav2(av);
	int ans{};
	for (bounds_iterator<3> iter = begin(sav2.bounds()); iter!=end(sav2.bounds()); ++iter) {
		EXPECT_EQ(ans++, sav2[*iter]);
	}

	// From strided_array_view
	strided_array_view<int, 3> sav3(sav2);
	int ans2{};
	for (bounds_iterator<3> iter = begin(sav3.bounds()); iter!=end(sav3.bounds()); ++iter) {
		EXPECT_EQ(ans2++, sav3[*iter]);
	}

}

TEST_F(StridedArrayViewTest, Observers)
{
	// strided_array_view class, but from av contigious data
	strided_array_view<int, 3> sav(av);

	bounds<3> bounds = {4,8,12};
	EXPECT_EQ(sav.bounds(), bounds);

	EXPECT_EQ(sav.size(), 4*8*12);

	offset<3> stride = {12*8, 12, 1};
	EXPECT_EQ(sav.stride(), stride);
}

TEST_F(StridedArrayViewTest, Slicing)
{
	strided_array_view<int, 3> sav(av);

	// Slices always act on the most significant dimension
	int x = 2;
	strided_array_view<int, 2> sliced = sav[x];
	int start{12*8*x};
	for (bounds_iterator<2> iter = begin(sliced.bounds()); iter!=end(sliced.bounds()); ++iter)
	{
		EXPECT_EQ(start++, sliced[*iter]);
	}

	// Cascade slices
	int y = 3;
	strided_array_view<int, 1> sliced2 = sav[x][y];

	int start2{12*8*x + 12*y};
	for (bounds_iterator<1> iter = begin(sliced2.bounds()); iter!=end(sliced2.bounds()); ++iter)
	{
		EXPECT_EQ(start2++, sliced2[*iter]);
	}

	// Cascade to a single index
	int z = 7;
	EXPECT_EQ(12*8*x + 12*y + z, sav[x][y][z]);
}

TEST_F(StridedArrayViewTest, Sectioning)
{
	strided_array_view<int, 3> sav(av);

	offset<3> origin{1,2,3};

	// with new bounds
	bounds<3> newBounds{2,3,4};
	strided_array_view<int, 3> section = sav.section(origin, newBounds);

	int start = 12*8*origin[0] + 12*origin[1] + origin[2];
	for (bounds_iterator<3> iter = begin(section.bounds()); iter!=end(section.bounds()); ++iter)
	{
		offset<3> idx = *iter;
		int ans = start + 12*8*idx[0] + 12*idx[1] + idx[2];
		EXPECT_EQ(ans, section[*iter]);
	}

	// with bounds extending to extent of source view
	strided_array_view<int, 3> section2 = sav.section(origin);
	EXPECT_EQ(sav.bounds() - origin, section2.bounds());

	int start2 = 12*8*origin[0] + 12*origin[1] + origin[2];
	for (bounds_iterator<3> iter = begin(section2.bounds()); iter!=end(section2.bounds()); ++iter)
	{
		offset<3> idx = *iter;
		int ans = start2 + 12*8*idx[0] + 12*idx[1] + idx[2];
		EXPECT_EQ(ans, section2[*iter]);
	}
}

using StridedDataTest = StridedArrayViewTest;

TEST_F(StridedDataTest, Constructors)
{
	// From strided data (even's only)
	strided_array_view<int, 3> strided_sav(vec.data(), {4,8,6}, {12*8, 12, 2});

	int start{};
	for (bounds_iterator<3> iter = begin(strided_sav.bounds()); iter!=end(strided_sav.bounds()); ++iter) {
		EXPECT_EQ(start, strided_sav[*iter]);
		start += 2;
	}
}

TEST_F(StridedDataTest, Observers)
{
	// strided data (even's only)
	strided_array_view<int, 3> sav(vec.data(), {4,8,6}, {12*8, 12, 2});

	bounds<3> bounds = {4,8,6};
	EXPECT_EQ(sav.bounds(), bounds);

	EXPECT_EQ(sav.size(), 4*8*6);

	offset<3> stride = {12*8, 12, 2};
	EXPECT_EQ(sav.stride(), stride);
}

TEST_F(StridedDataTest, Slicing)
{
	strided_array_view<int, 3> sav(vec.data(), {4,8,6}, {12*8, 12, 2});

	// Slices always act on the most significant dimension
	int x = 2;
	strided_array_view<int, 2> sliced = sav[x];
	int start{12*8*x};
	for (bounds_iterator<2> iter = begin(sliced.bounds()); iter!=end(sliced.bounds()); ++iter)
	{
		EXPECT_EQ(start, sliced[*iter]);
		start += 2;
	}

	// Cascade slices
	int y = 3;
	strided_array_view<int, 1> sliced2 = sav[x][y];

	int start2{12*8*x + 12*y};
	for (bounds_iterator<1> iter = begin(sliced2.bounds()); iter!=end(sliced2.bounds()); ++iter)
	{
		EXPECT_EQ(start2, sliced2[*iter]);
		start2 += 2;
	}

	// Cascade to a single index
	int z = 3;
	EXPECT_EQ(12*8*x + 12*y + 2*z, sav[x][y][z]);
}

TEST_F(StridedDataTest, Sectioning)
{
	strided_array_view<int, 3> sav(vec.data(), {4,8,6}, {12*8, 12, 2});

	offset<3> origin{1,2,3};

	// with new bounds
	bounds<3> newBounds{2,3,4};
	strided_array_view<int, 3> section = sav.section(origin, newBounds);

	int start = 12*8*origin[0] + 12*origin[1] + origin[2]*2;
	for (bounds_iterator<3> iter = begin(section.bounds()); iter!=end(section.bounds()); ++iter)
	{
		offset<3> idx = *iter;
		int ans = start + 12*8*idx[0] + 12*idx[1] + idx[2]*2;
		EXPECT_EQ(ans, section[*iter]);
	}

	// with bounds extending to extent of source view
	strided_array_view<int, 3> section2 = sav.section(origin);
	EXPECT_EQ(sav.bounds() - origin, section2.bounds());

	int start2 = 12*8*origin[0] + 12*origin[1] + origin[2]*2;
	for (bounds_iterator<3> iter = begin(section2.bounds()); iter!=end(section2.bounds()); ++iter)
	{
		offset<3> idx = *iter;
		int ans = start2 + 12*8*idx[0] + 12*idx[1] + idx[2]*2;
		EXPECT_EQ(ans, section2[*iter]);
	}
}
