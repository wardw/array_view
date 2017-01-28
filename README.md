## array_view

An implementation of the ISO C++ proposal [_Multidimensional bounds, offset and array_view_][1] by Mendakiewicz & Sutter.  This implements the original proposal [N3851][1] up to the last [revision 7][3].  As I understand it, the proposal was with an aim for inclusion in C++17 although is no longer under consideration in it's current form.  For later contributions, see the successive proposal [p0122r0][4] (and related [GSL][5] implementation), and alternative proposals such as [P0546][6].

Principles behind the proposal are the representation of a multidimensional array as a view over contiguous (or strided) data and the straightforward expression for multidimensional indexing into these arrays. The result is a safe, bounded view that works naturally with existing algorithms while maintaining a nice, expressive syntax.

The library is header-only with no external dependencies.  If you want to build the tests there is a CMakeLists.txt for building with CMake and Google Test.


### Example usage

The following is a _very brief_ overview of some typical usage. For more details, see the [original proposal][1].

```cpp
int X = 12;
int Y = 8;
int Z = 6;

vector<int> vec(X*Y*Z);

bounds<3> bnds = {X,Y,Z};
array_view<int,3> av(vec, bnds);
```

A `bounds<Rank>` defines a multidimensional bounds into an array space of rank `Rank` over which we wish to construct a view.  Above, we construct an `array_view<T,Rank>` over our `vec` using the bounds `bnds`.  Note that a view has reference semantics so our `vec` must stay valid for the lifetime of `av`.

An `offset<Rank>` represents a multidimensional index, an integral vector into this array space. Both `offset<Rank>` and `bounds<Rank>` share similar interfaces. To access an element:

```cpp
offset<3> idx = {5,3,2};

av[idx] = 28;
av[{5,3,2}] = 28;  // also fine
```

The `bounds<Rank>` class provides `begin()` and `end()` member functions that return iterators that iterate over each index of the bounds in turn.  Iteration increments first along the least significant dimension (contiguous for non-strided data). This facility allows iteration with a simple range-based for loop:

```cpp
for (auto& idx : av.bounds())
{
	auto i = idx[0];
	auto j = idx[1];
	auto k = idx[2];  // least significant dimension, incremented first

	av[idx] = i**2 + j - k;
}
```

Above, `bounds()` returns the current bounds to the array_view.  An alternative would be to construct a `bounds_iterator` directly.  A `bounds_iterator` is a (nearly) random access iterator whose `value_type` is an `offset` into the array space.  Iterating a `bounds_iterator` is typically done over the range defined by `bounds`:

```cpp
bounds_iterator<3> first = begin(av.bounds());
bounds_iterator<3> last = end(av.bounds());

// Dereferencing an iterator returns a `const offset<Rank>`. Indices are always immutable.
for_each(first, last, [&av](const offset<3>& idx) {
	auto i = idx[0];
	auto j = idx[1];
	auto k = idx[2];
	assert(av[idx] == i**2 + j - k);
});
```

#### Slicing

Slicing returns a lower dimensional 'slice' as a new view of the same data. Slices always slice from the most significant dimensions (here, `x`):

```cpp
int x0 = 5, y0 = 3;
array_view<int, 2> slice2d = av[x0];      // a 2d slice in the yz plane
array_view<int, 1> slice1d = av[x0][y0];  // a row in z, also the contiguous dimension

assert( slice2d[{3,2}] == 28 );
```

For dimensions of rank 1, you can omit the `initializer_list`:

```cpp
assert( slice1d[2] == 28 );
```

#### Sectioning

Sectioning creates a new view given a new bounds that fully subsumes the original. Sections must be of the same rank as the original. All views created from sections return a `strided_array_view`:

```cpp
offset<3> origin = {6,3,2};
bounds<3> window = {3,3,2};
auto section = av.section(origin, window);

// Work with just this section of the data
int sum = std::accumulate(begin(section.bounds()), end(section.bounds()), 0,
                          [&](int a, offset<3> idx) {
	return a + section[idx];
});
```

#### Strided data

An additional class `strided_array_view` relaxes the requirement that the least significant dimension of the referenced data must be contiguous.  Strided views commonly arise by sectioning an `array_view`, but it is possible to construct a strided view directly with a pointer to data in memory for which you must provide the stride.

For example, the following creates a new view of `vec` that includes every third element and is one-third the extent in the Z dimension:

```cpp
array_view<int,3> av = .. // as before

// The strides in X and Y are as calculated as for `av`, but the stride in Z is no longer 1
bounds<3> newExtents = {X, Y, Z/3};
offset<3> newStride = {av.stride()[0], av.stride()[1], 3};
strided_array_view<int,3> sav(vec.data(), newExtents, newStride);

for (auto& idx : sav.bounds())
{
	// do something with sav[idx]
}
```

Taking sliced views of contiguous data, sections of sliced views and views of views all compose and work naturally as you would expect.


### Acknowledgements

This implementation follows the original proposal by Mendakiewicz & Sutter and subsequent revisions (latest [revision 7][3]). As noted in the proposal, the proposal itself builds on previous work by Callahan, Levanoni and Sutter for their work designing the original interfaces for C++ AMP.


### Limitations

My experience implementing the standard is zero, I expect this to fall way short of a conforming implementation.  However, as a practical solution it has served sufficient for my purposes, and at least supports a working set of tests. Also I thought this might make an interesting exercise. All comments & suggestions welcome.

[1]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n3851.pdf
[2]: https://msdn.microsoft.com/en-us/library/hh265137.aspx
[3]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4512.html
[4]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0122r0.pdf
[5]: https://github.com/Microsoft/GSL
[6]: https://github.com/kokkos/array_ref/blob/master/proposals/P0546.rst

