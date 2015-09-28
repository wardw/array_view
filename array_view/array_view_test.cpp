#include "array_view/array_view.h"

#include <iostream>
#include <numeric>
#include "gtest/gtest.h"

using namespace std;
using namespace av;

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

TEST(bounds, contains)
{
	bounds<3> b = {2,3,4};
	EXPECT_TRUE(b.contains({0,0,0}));
	EXPECT_TRUE(b.contains({1,2,3}));

	EXPECT_FALSE(b.contains({1,2,4}));
	EXPECT_FALSE(b.contains({1,3,3}));
	EXPECT_FALSE(b.contains({2,2,3}));
	EXPECT_FALSE(b.contains({0,0,-1}));
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
	ArrayViewTest() :
		vec(4*8*12),
		testBounds{4,8,12},
		av(vec, testBounds),
		sav(av)
	{
		int n{};
		std::generate(vec.begin(), vec.end(), [&]{ return n++; });
		testStride = av.stride();
	}

protected:
	vector<int> vec;

	bounds<3> testBounds;
	offset<3> testStride;

	// objects under test, one of each (but both under contigious data)
	array_view<int, 3> av;
	strided_array_view<int, 3> sav;
};

using StridedArrayViewTest = ArrayViewTest;

class StridedDataTest : public ArrayViewTest {
public:
	using Avt = ArrayViewTest;

	StridedDataTest() :
		testBounds{Avt::testBounds[0], Avt::testBounds[1], Avt::testBounds[2] / 2},
		testStride{Avt::testStride[0], Avt::testStride[1], 2},
		strided_sav(vec.data(), testBounds, testStride)
	{}

protected:
	bounds<3> testBounds;
	offset<3> testStride;

	// object under test, a strided class with alternate spacing in z (even's only)
	strided_array_view<int, 3> strided_sav;
};

template <typename ArrayView>
void testSectioning(const ArrayView& av, const bounds<3>& testBounds,
	                                     const offset<3>& testOrigin,
	                                     const offset<3>& testStride)
{
	int start{};
	for (int i=0; i<3; i++)	{
		start += testOrigin[i] * testStride[i];
	}

	bounds_iterator<3> iter = begin(av.bounds());
	for (int i=0; i<testBounds[0]; ++i)
	{
		for (int j=0; j<testBounds[1]; ++j)
		{
			for (int k=0; k<testBounds[2]; ++k)
			{
				offset<3> idx = {i,j,k};
				EXPECT_EQ(idx, *iter);

				int off{};
				for (int d=0; d<3; d++) {
					off += idx[d] * testStride[d];
				}

				EXPECT_EQ(start + off, av[*iter++]);
			}
		}
	}
}

template <typename ArrayView>
void testSlicing(const ArrayView& av, const offset<3>& testStride)
{
	// Slices always fix the most significant dimension
	int x = 2;
	strided_array_view<int, 2> sliced = av[x];
	int start = testStride[0] * x;
	for (bounds_iterator<2> iter = begin(sliced.bounds()); iter!=end(sliced.bounds()); ++iter)
	{
		EXPECT_EQ(start, sliced[*iter]);
		start += testStride[2];
	}

	// Cascade slices
	int y = 3;
	strided_array_view<int, 1> sliced2 = av[x][y];
	int start2 = testStride[0] * x + testStride[1] * y;
	for (bounds_iterator<1> iter = begin(sliced2.bounds()); iter!=end(sliced2.bounds()); ++iter)
	{
		EXPECT_EQ(start2, sliced2[*iter]);
		start2 += testStride[2];
	}

	// Cascade to a single index
	int z = 3;
	int start3 = testStride[0] * x + testStride[1] * y + testStride[2] * z;
	EXPECT_EQ(start3, av[x][y][z]);
}

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
	EXPECT_EQ(av.bounds(), testBounds);
	EXPECT_EQ(av.size(), testBounds[0] * testBounds[1] * testBounds[2]);
	EXPECT_EQ(av.stride(), testStride);
}

TEST_F(ArrayViewTest, Slicing)
{
	testSlicing(av, testStride);
}

TEST_F(ArrayViewTest, Sectioning)
{
	offset<3> origin{1,2,3};

	// section with new bounds
	bounds<3> newBounds{2,3,4};
	testSectioning(av.section(origin, newBounds), newBounds, origin, testStride);

	// section with bounds extending to extent of source view
	strided_array_view<int,3> sectioned = av.section(origin);

	bounds<3> remainingBounds = testBounds - origin;
	EXPECT_EQ(remainingBounds, sectioned.bounds());
	testSectioning(sectioned, remainingBounds, origin, testStride);
}

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
	EXPECT_EQ(sav.bounds(), testBounds);
	EXPECT_EQ(sav.size(), testBounds[0] * testBounds[1] * testBounds[2]);
	EXPECT_EQ(sav.stride(), testStride);
}

TEST_F(StridedArrayViewTest, Slicing)
{
	testSlicing(sav, testStride);
}

TEST_F(StridedArrayViewTest, Sectioning)
{
	offset<3> origin{1,2,3};

	// section with new bounds
	bounds<3> newBounds{2,3,4};
	testSectioning(sav.section(origin, newBounds), newBounds, origin, testStride);

	// section with bounds extending to extent of source view
	strided_array_view<int,3> sectioned = sav.section(origin);

	bounds<3> remainingBounds = testBounds - origin;
	EXPECT_EQ(remainingBounds, sectioned.bounds());
	testSectioning(sectioned, remainingBounds, origin, testStride);
}

TEST_F(StridedDataTest, Constructors)
{
	int start{};
	for (bounds_iterator<3> iter = begin(strided_sav.bounds()); iter!=end(strided_sav.bounds()); ++iter) {
		EXPECT_EQ(start, strided_sav[*iter]);
		start += 2;
	}
}

TEST_F(StridedDataTest, Observers)
{
	EXPECT_EQ(strided_sav.bounds(), testBounds);
	EXPECT_EQ(strided_sav.size(), testBounds[0] * testBounds[1] * testBounds[2]);
	EXPECT_EQ(strided_sav.stride(), testStride);
}

TEST_F(StridedDataTest, Slicing)
{
	testSlicing(strided_sav, testStride);
}

TEST_F(StridedDataTest, Sectioning)
{
	offset<3> origin{1,2,3};

	// section with new bounds
	bounds<3> newBounds{2,3,4};
	testSectioning(strided_sav.section(origin, newBounds), newBounds, origin, testStride);

	// section with bounds extending to extent of source view
	strided_array_view<int,3> sectioned = strided_sav.section(origin);

	bounds<3> remainingBounds = testBounds - origin;
	EXPECT_EQ(remainingBounds, sectioned.bounds());
	testSectioning(sectioned, remainingBounds, origin, testStride);
}

TEST(ArrayView, Example)
{
	int X = 12;
	int Y = 8;
	int Z = 6;

	vector<int> vec(X*Y*Z);

	bounds<3> extents = {X,Y,Z};
	array_view<int,3> av(vec, extents);

	// Access an element.
	offset<3> idx = {5,3,2};
	av[idx] = 30;

	// Iterate through each index of the view
	for (auto& idx : av.bounds())
	{
		auto i = idx[0];
		auto j = idx[1];
		auto k = idx[2];
		av[idx] = i * j * k;
	}

	// or use a bounds_iterator explicitly
	bounds_iterator<3> first = begin(av.bounds());
	bounds_iterator<3> last = end(av.bounds());

	for_each(first, last, [&av](const offset<3>& idx) {
		auto i = idx[0];
		auto j = idx[1];
		auto k = idx[2];
		EXPECT_EQ(i * j * k, av[idx]);
	});

	// Slicing
	int x0 = 5;
	int y0 = 3;
	array_view<int, 2> slice2d = av[x0];      // a 2d slice in the yz plane
	array_view<int, 1> slice1d = av[x0][y0];  // a row in z (also the contigious dimension)

	EXPECT_EQ(30, ( slice2d[{3,2}] ));
	EXPECT_EQ(30, slice1d[2]);

	// Sectioning
	offset<3> origin = {6, 3, 2};
	bounds<3> window = {3, 3, 2};
	auto section = av.section(origin, window);

	int sum = std::accumulate(begin(section.bounds()), end(section.bounds()), 0,
	                          [&](int a, offset<3> idx) {
		return a + section[idx];
	});

    // Strided data
	offset<3> newStride = {av.stride()[0], av.stride()[1], 3};
	bounds<3> newExtents = {X, Y, Z/3};
	strided_array_view<int,3> sav(vec.data(), newExtents, newStride);

	for (auto& idx : sav.bounds())
	{
		EXPECT_EQ(0, sav[idx] % 3);
	}
}
