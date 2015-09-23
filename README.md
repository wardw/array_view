A working implementation of the ISO C++ proposal ["Multidimensional bounds, offset and array_view"][1] by Reis, Laksberg and Gor Nishanov.

Two key ideas behind the proposal are the representation of a multidimensional array as a view over contiguous (or strided) data, and the straightforward expression for multidimensional indexing into these arrays. This allows for iterating over such arrays in a way that works naturally with existing algorithms, among other things.

More details can be found in the [original proposal][1] and later revisions (this work as of [revision 7][3]).  Alternatively, see the examples in the tests. To build the tests there is a CMakeLists.txt for building with CMake and Google Test.

### A very quick overview

	int X = 12;
	int Y = 8;
	int Z = 6;

	vector<int> vec(X*Y*Z);

	bounds<3> extents = {X,Y,Z};
	array_view<int,3> av(vec, extents);

A `bounds` defines the extents of an array space from which we wish to create a view. An `array_view` is our multidimensional view onto our vector, or indeed over any data laid out contiguously in memory (with the single alternative `strided_array_view` admitting data of an regular stride). A view has reference semantics, so our `vec` must stay valid for the lifetime of `av`.

An `offset` represents a multidimensional index, effectively representing an integral vector into this multidimensional array space. `offset` and `bounds` share similar interfaces. To access an element

	offset<3> idx = {5,3,2};

	av[idx] = 28;
	av[{5,3,2}] = 28;  // also fine

The `bounds` class provides `begin` and `end` members to iterators that iterate over each index of the bounds, allowing iteration with a simple range-based for loop

	for (auto& idx : av.bounds())
	{
		auto i = idx[0];
		auto j = idx[1];
		auto k = idx[2];
		av[idx] = i**2 + j - k;
	}

Alternatively, you can construct a `bounds_iterator` directly. A `bounds_iterator` is a (nearly) random access iterator whose `value_type` is an `offset` into the array space. Iterating a `bounds_iterator` is typically done over the range defined by `bounds`.

	bounds_iterator<3> first = begin(av.bounds());
	bounds_iterator<3> last = end(av.bounds());

    // Dereferencing an iterator returns a `const offset<Rank>`. Indices are always immutable.
	for_each(first, last, [&av](const offset<3>& idx) {
		auto i = idx[0];
		auto j = idx[1];
		auto k = idx[2];
		assert(av[idx] == i**2 + j - k);
	});

#### Slicing

Slicing returns a lower dimensional 'slice' as a new view. Slices always fix the most significant dimension (here, `x`)

	int x0 = 5, y0 = 3;
	array_view<int, 2> slice2d = av[x0];      // a 2d slice in the yz plane
	array_view<int, 1> slice1d = av[x0][y0];  // a row in z, also the contiguous dimension

	assert( slice2d[{3,2}] == 28 );

For dimensions of rank 1, you can omit the `initializer_list`

	assert( slice1d[2] == 28 );

#### Sectioning

Sectioning creates a new view of the data given a new set of bounds that fully subsume the original. Sections must be of the same rank as the original. All views created from sections return a `strided_array_view`.

	offset<3> origin = {6, 3, 2};
	bounds<3> window = {3, 3, 2};
	auto section = av.section(origin, window);

    // Work just with this section of the data
	int sum = std::accumulate(begin(section.bounds()), end(section.bounds()), 0,
	                          [&](int a, offset<3> idx) {
		return a + section[idx];
	});

### Strided data

An additional class `strided_array_view` relaxes the requirement that the least significant dimension of the referenced data must be contiguous. Strided views may commonly arise as constructed by an array_view returning a section, but it is also possible to construct a strided view directly with a pointer to data in memory and for which you must provide the stride.

For example, to explicitly create a new view of our `vec` that includes every third element and is 1/3 the extent in the Z dimension

    array_view<int,3> av = .. // as before

    // The strides in X and Y are as calculated as for `av`, but the stride in Z is no longer 1
	bounds<3> newExtents = {X, Y, Z/3};
	offset<3> newStride = {av.stride()[0], av.stride()[1], 3};
	strided_array_view<int,3> sav(vec.data(), newExtents, newStride);

	for (auto& idx : sav.bounds())
	{
		// do something with sav[idx]
	}

Taking sliced views of contiguous data, sections of sliced view and other views of views all compose and work naturally as you would expect.

### Acknowledgements

This implementation follows the original proposal by Reis, Laksberg and Gor Nishanov [[1]] and subsequent revisions (latest [revision 7][3]). As understood, the proposal itself builds on previous work by Callahan, Levanoni and Sutter for their work designing the original interfaces for C++ AMP.

### Limitations

My experience implementing the standard is zero, I expect this to fall short of the requirements for a conforming implementation. I was looking for an immediate solution for my purposes; boost didn't fit, I'm currently not based on Windows and didn't want to wait until 2017. Also I thought this might make a good exercise. All comments & suggestions are very welcome.


[1]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n3851.pdf
[2]: https://msdn.microsoft.com/en-us/library/hh265137.aspx
[3]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4512.html
