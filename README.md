An implementation of the C++17 proposal for multidimensional arrays and indexing

A working implementation of the ISO C++ proposal ["Multidimensional bounds, offset and array_view"][1] by Reis, Laksberg and Gor Nishanov.

The key idea behind the proposal is the representation of a multidimensional array as a view over serially contigious or strided data and the straightforward expression for multidimensional indexing into these arrays. Among other things this allows for iterating over such arrays in a way that works naturally with existing algorithms.

More details can be found in the [original proposal][1] and later revisions (this work as of [revision 7][3]). Or see the examples in the tests.  If you want to build the tests there is a CMakeLists.txt for building with CMake and Google Test.

### A very quick overview

	int X = 12;
	int Y = 8;
	int Z = 6;

	vector<int> vec(X*Y*Z);

An `offset` represents a multidimensional index, effectively representing an integral vector into our multidimensional array space. A `bounds` defines the extents of this array space, and has a similar interface to `offset`. An `array_view` is our view, typically on a container but in general for data with the one requiement that the data is layed out contigiously in memory. A view has reference semantics, so our `vec` must stay in scope.

	bounds<3> extents = {X,Y,Z};
	array_view<int,3> av(vec, extents);

Access an element

	offset<3> idx = {5,3,2};
	av[idx] = 30;
	..
	av[{5,3,2}] = 30;  // neat

Iterate through each index of the view

	for (auto& idx : av.bounds())
	{
		auto i = idx[0];
		auto j = idx[1];
		auto k = idx[2];
		av[idx] = i * j * k;
	}

or use `bounds_iterator` explicitly. A `bounds_iterator` is a (almost) random access iterator whose `value_type` is an `offset` into the array space. Iterating a `bounds_iterator` is typically done over the range defined by `bounds`.

	bounds_iterator<3> first = begin(av.bounds());
	bounds_iterator<3> last = end(av.bounds());

	for_each(first, last, [&av](offset<3>& idx) {
        // dereferencing an iterator returns a const offset<Rank>
		auto i = idx[0];
		auto j = idx[1];
		auto k = idx[2];
		assert(av[idx] == i * j * k);
	});

#### Slicing

Slicing takes a lower dimensional 'slice' as a new view. Slices `[]` always fix the most significant dimension (here `x`)

	int x0 = 5;
	int y0 = 3;
	array_view<int, 2> slice2d = av[x0];      // a 2d slice in the yz plane
	array_view<int, 1> slice1d = av[x0][y0];  // a row in z, also the contigious dimension

	assert(( slice2d[{3,2}] == 30 ));

For dimensions of rank 1, you can omit the `initializer_list`

	assert( slice1d[2] == 30 );

#### Sectioning

Sectioning creates a new view of the data given a new set of bounds that fully subsume the original. Sections must be of the same rank as the original.

	offset<3> origin = {6, 3, 2};
	bounds<3> window = {3, 3, 2};
	auto section = av.section(origin, window);

    // work just with this section of the data
	int sum = std::accumulate(begin(section.bounds()), end(section.bounds()), 0,
	                          [&](int a, offset<3> idx) {
		return a + section[idx];
	});

### Strided data

A final class `strided_array_view` relaxes the requirement that the least significant dimension of the referenced data must be contigious. All views created as sections of data return a `strided_array_view`. You can construct a strided view from an `array_view`, or directly with a pointer to data in memory for which you provide the stride.

For example, to define a view of our `vec` but of only every third element

    array_view<int,3> av = .. // as before

    // The strides in X and Y are as calculated for `av`, but the stride in Z is no longer 1
	offset<3> newStride = {av.stride()[0], av.stride()[1], 3};
	bounds<3> newExtents = {X, Y, Z/3};
	strided_array_view<int,3> sav(vec.data(), newExtents, newStride);

	for (auto& idx : sav.bounds())
	{
		EXPECT_EQ(0, sav[idx] % 3);
	}

Taking sliced views of contigious data, sections of sliced view and other views of views all compose and work naturally as you would expect.

### Ackwnoldegments

This work is based almost exclusively on the original proposal by Reis, Laksberg and Gor Nishanov [[1]] and subsequent revisions (latest [revision 7][3]). As i have read, the proposal itself builds on previous work by Callahan, Levanoni and Sutter for their work desiging the original interfaces for C++ AMP.

### Excuses

My experience implementing the standard is zero, so I am sure this falls way short of the requirements. I needed something working for my purposes; boost didn't fit, i'm not using Windows and i'm not waiting until 2017. Nevertheless, I thought to try and reflect the proposal faithfully as an exercise. All comments & suggestions are very welcome.


[1]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n3851.pdf
[2]: https://msdn.microsoft.com/en-us/library/hh265137.aspx
[3]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4512.html
