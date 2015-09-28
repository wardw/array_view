#pragma once

#include <cstddef>
#include <initializer_list>
#include <array>
#include <iterator>
#include <cassert>

// temp
#include <iostream>

/*
template <size_t Rank>
class offset
{
public:
	// constants and types
	static constexpr size_t rank = Rank;
	using reference              = ptrdiff_t&;
	using const_reference        = const ptrdiff_t&;
	using size_type              = size_t;
	using value_type             = ptrdiff_t;

	static_assert(Rank > 0, "Size of Rank must be greater than 0");

	// construction
	constexpr offset() noexcept;
	template <size_t R = Rank, typename = std::enable_if_t<R == 1>>
	constexpr offset(value_type v) noexcept;
	constexpr offset(std::initializer_list<value_type> il);

	// element access
	constexpr reference       operator[](size_type n);
	constexpr const_reference operator[](size_type n) const;

	// arithmetic
	constexpr offset& operator+=(const offset& rhs);
	constexpr offset& operator-=(const offset& rhs);

	template <size_t R = Rank, typename = std::enable_if_t<R == 1>>
	constexpr offset& operator++();
	template <size_t R = Rank, typename = std::enable_if_t<R == 1>>
	constexpr offset  operator++(int);
	template <size_t R = Rank, typename = std::enable_if_t<R == 1>>
	constexpr offset& operator--();
	template <size_t R = Rank, typename = std::enable_if_t<R == 1>>
	constexpr offset  operator--(int);

	constexpr offset  operator+() const noexcept;
	constexpr offset  operator-() const;

	constexpr offset& operator*=(value_type v);
	constexpr offset& operator/=(value_type v);

private:
	std::array<value_type, rank> offset_ = {};
};

// offset equality
template <size_t Rank>
constexpr bool operator==(const offset<Rank>& lhs, const offset<Rank>& rhs) noexcept;
template <size_t Rank>
constexpr bool operator!=(const offset<Rank>& lhs, const offset<Rank>& rhs) noexcept;

// offset arithmetic
template <size_t Rank>
constexpr offset<Rank> operator+(const offset<Rank>& lhs, const offset<Rank>& rhs);
template <size_t Rank>
constexpr offset<Rank> operator-(const offset<Rank>& lhs, const offset<Rank>& rhs);
template <size_t Rank>
constexpr offset<Rank> operator*(const offset<Rank>& lhs, ptrdiff_t v);
template <size_t Rank>
constexpr offset<Rank> operator*(ptrdiff_t v, const offset<Rank>& rhs);
template <size_t Rank>
constexpr offset<Rank> operator/(const offset<Rank>& lhs, ptrdiff_t v);


template <size_t Rank>
class bounds
{
public:
	// constants and types
	static constexpr size_t rank = Rank;
	using reference              = ptrdiff_t&;
	using const_reference        = const ptrdiff_t&;
	using iterator               = bounds_iterator<Rank>;
	using const_iterator         = bounds_iterator<Rank>;
	using size_type              = size_t;
	using value_type             = ptrdiff_t;

	// construction
	constexpr bounds() noexcept;
	template <size_t R = Rank, typename = std::enable_if_t<R == 1>>
	constexpr bounds(value_type v);
	constexpr bounds(std::initializer_list<value_type> il);

	// observers
	constexpr size_type size() const noexcept;
	constexpr bool      contains(const offset<Rank>& idx) const noexcept;

	// iterators
	const_iterator begin() const noexcept;
	const_iterator end() const noexcept;

	// element access
	constexpr reference       operator[](size_type n);
	constexpr const_reference operator[](size_type n) const;

	// arithmetic
	constexpr bounds& operator+=(const offset<Rank>& rhs);
	constexpr bounds& operator-=(const offset<Rank>& rhs);

	constexpr bounds& operator*=(value_type v);
	constexpr bounds& operator/=(value_type v);

private:
	std::array<value_type, rank> bounds_ = {};
};


// bounds equality
template <size_t Rank>
constexpr bool operator==(const bounds<Rank>& lhs, const bounds<Rank>& rhs) noexcept;
template <size_t Rank>
constexpr bool operator!=(const bounds<Rank>& lhs, const bounds<Rank>& rhs) noexcept;

// bounds arithmetic
template <size_t Rank>
constexpr bounds<Rank> operator+(const bounds<Rank>& lhs, const offset<Rank>& rhs);
template <size_t Rank>
constexpr bounds<Rank> operator+(const offset<Rank>& lhs, const bounds<Rank>& rhs);
template <size_t Rank>
constexpr bounds<Rank> operator-(const bounds<Rank>& lhs, const offset<Rank>& rhs);
template <size_t Rank>
constexpr bounds<Rank> operator*(const bounds<Rank>& lhs, ptrdiff_t v);
template <size_t Rank>
constexpr bounds<Rank> operator*(ptrdiff_t v, const bounds<Rank>& rhs);
template <size_t Rank>
constexpr bounds<Rank> operator/(const bounds<Rank>& lhs, ptrdiff_t v);


*/

namespace av
{

template <size_t Rank> class offset;
template <size_t Rank> class bounds;
template <size_t Rank> class bounds_iterator;
template <typename T, size_t Rank> class array_view;
template <typename T, size_t Rank> class strided_array_view;

template <size_t Rank>
class offset
{
public:
	// constants and types
	static constexpr size_t rank = Rank;
	using reference              = ptrdiff_t&;
	using const_reference        = const ptrdiff_t&;
	using size_type              = size_t;
	using value_type             = ptrdiff_t;

	static_assert(Rank > 0, "Size of Rank must be greater than 0");

	// construction
	constexpr offset() noexcept {}
	template <size_t R = Rank, typename = std::enable_if_t<R == 1>>
	constexpr offset(value_type v) noexcept { (*this)[0] = v; }
	constexpr offset(std::initializer_list<value_type> il);

	// element access
	constexpr reference       operator[](size_type n) { return offset_[n]; }
	constexpr const_reference operator[](size_type n) const { return offset_[n]; }

	// arithmetic
	template <size_t R = Rank, typename = std::enable_if_t<R == 1>>
	constexpr offset& operator++()    { return ++(*this)[0]; }
	template <size_t R = Rank, typename = std::enable_if_t<R == 1>>
	constexpr offset  operator++(int) { return offset<Rank>{(*this)[0]++}; }
	template <size_t R = Rank, typename = std::enable_if_t<R == 1>>
	constexpr offset& operator--()    { return --(*this)[0]; }
	template <size_t R = Rank, typename = std::enable_if_t<R == 1>>
	constexpr offset  operator--(int) { return offset<Rank>{(*this)[0]--}; }

	constexpr offset& operator+=(const offset& rhs);
	constexpr offset& operator-=(const offset& rhs);

	constexpr offset  operator+() const noexcept { return *this; }
	constexpr offset  operator-() const
	{
		offset<Rank> copy{*this};
		for (value_type& elem : offset_) {
			elem *= -1;
		}
		return copy;
	}

	constexpr offset& operator*=(value_type v);
	constexpr offset& operator/=(value_type v);

private:
	std::array<value_type, rank> offset_ = {};
};

template <size_t Rank>
constexpr offset<Rank>::offset(std::initializer_list<value_type> il)
{
	std::copy(il.begin(), il.end(), offset_.data());
}

// arithmetic
template <size_t Rank>
constexpr offset<Rank>& offset<Rank>::operator+=(const offset& rhs)
{
	for (size_type i=0; i<Rank; ++i) {
		(*this)[i] += rhs[i];
	}
	return *this;
}

template <size_t Rank>
constexpr offset<Rank>& offset<Rank>::operator-=(const offset& rhs)
{
	for (size_type i=0; i<Rank; ++i) {
		(*this)[i] -= rhs[i];
	}
	return *this;
}

template <size_t Rank>
constexpr offset<Rank>& offset<Rank>::operator*=(value_type v)
{
	for (value_type& elem : offset_) {
		elem *= v;
	}
	return *this;
}

template <size_t Rank>
constexpr offset<Rank>& offset<Rank>::operator/=(value_type v)
{
	for (value_type& elem : offset_) {
		elem /= v;
	}
	return *this;
}


// Free functions

// offset equality
template <size_t Rank>
constexpr bool operator==(const offset<Rank>& lhs, const offset<Rank>& rhs) noexcept
{
	for (size_t i=0; i<Rank; ++i) {
		if (lhs[i] != rhs[i]) return false;
	}
	return true;
}

template <size_t Rank>
constexpr bool operator!=(const offset<Rank>& lhs, const offset<Rank>& rhs) noexcept
{ return !(lhs == rhs); }

// offset arithmetic
template <size_t Rank>
constexpr offset<Rank> operator+(const offset<Rank>& lhs, const offset<Rank>& rhs)
{ return offset<Rank>{lhs} += rhs; }

template <size_t Rank>
constexpr offset<Rank> operator-(const offset<Rank>& lhs, const offset<Rank>& rhs)
{ return offset<Rank>{lhs} -= rhs; }

template <size_t Rank>
constexpr offset<Rank> operator*(const offset<Rank>& lhs, ptrdiff_t v)
{ return offset<Rank>{lhs} *= v; }

template <size_t Rank>
constexpr offset<Rank> operator*(ptrdiff_t v, const offset<Rank>& rhs)
{ return offset<Rank>{rhs} *= v; }

template <size_t Rank>
constexpr offset<Rank> operator/(const offset<Rank>& lhs, ptrdiff_t v)
{ return offset<Rank>{lhs} /= v; }


template <size_t Rank>
class bounds
{
public:
	// constants and types
	static constexpr size_t rank = Rank;
	using reference              = ptrdiff_t&;
	using const_reference        = const ptrdiff_t&;
	using iterator               = bounds_iterator<Rank>;
	using const_iterator         = bounds_iterator<Rank>;
	using size_type              = size_t;
	using value_type             = ptrdiff_t;

	static_assert(Rank > 0, "Size of Rank must be greater than 0");

	// construction
	constexpr bounds() noexcept {};
	// Question: why is this constructor not `noexcept` ?
	template <size_t R = Rank, typename = std::enable_if_t<R == 1>>
	constexpr bounds(value_type v) { (*this)[0] = v; postcondition(); }
	constexpr bounds(std::initializer_list<value_type> il);

	// observers
	constexpr size_type size() const noexcept;
	constexpr bool      contains(const offset<Rank>& idx) const noexcept;

	// iterators
	const_iterator begin() const noexcept { return const_iterator{*this}; };
	const_iterator end() const noexcept {
		iterator iter{*this};
		return iter._setOffTheEnd();
	}

	// element access
	constexpr reference       operator[](size_type n) { return bounds_[n]; }
	constexpr const_reference operator[](size_type n) const { return bounds_[n]; }

	// arithmetic
	constexpr bounds& operator+=(const offset<Rank>& rhs);
	constexpr bounds& operator-=(const offset<Rank>& rhs);

	constexpr bounds& operator*=(value_type v);
	constexpr bounds& operator/=(value_type v);

private:
	std::array<value_type, rank> bounds_ = {};

	void postcondition() { /* todo */ };
};

// construction
template <size_t Rank>
constexpr bounds<Rank>::bounds(std::initializer_list<value_type> il)
{
	// Todo: fails, try gcc?
	//static_assert(il.size() == Rank, "Size of Rank must equal the size of the initialiser list");

	std::copy(il.begin(), il.end(), bounds_.data());
	postcondition();
}

// observers
template <size_t Rank>
constexpr size_t bounds<Rank>::size() const noexcept
{
	size_type product{1};
	for (const value_type& elem : bounds_) {
		product *= elem;
	}
	return product;
}

template <size_t Rank>
constexpr bool bounds<Rank>::contains(const offset<Rank>& idx) const noexcept
{
	for (size_type i=0; i<Rank; ++i) {
		if ( !(0 <= idx[i] && idx[i] < (*this)[i]) ) return false;
	}
	return true;
}

// iterators
// todo

// arithmetic
template <size_t Rank>
constexpr bounds<Rank>& bounds<Rank>::operator+=(const offset<Rank>& rhs)
{
	for (size_type i=0; i<Rank; ++i) {
		bounds_[i] += rhs[i];
	}
	postcondition();
	return *this;
}

template <size_t Rank>
constexpr bounds<Rank>& bounds<Rank>::operator-=(const offset<Rank>& rhs)
{
	for (size_type i=0; i<Rank; ++i) {
		bounds_[i] -= rhs[i];
	}
	postcondition();
	return *this;
}

template <size_t Rank>
constexpr bounds<Rank>& bounds<Rank>::operator*=(value_type v)
{
	for (value_type& elem : bounds_) {
		elem *= v;
	}
	postcondition();
	return *this;
}

template <size_t Rank>
constexpr bounds<Rank>& bounds<Rank>::operator/=(value_type v)
{
	for (value_type& elem : bounds_) {
		elem /= v;
	}
	postcondition();
	return *this;
}


// Free functions

// bounds equality
template <size_t Rank>
constexpr bool operator==(const bounds<Rank>& lhs, const bounds<Rank>& rhs) noexcept
{
	for (size_t i=0; i<Rank; ++i) {
		if (lhs[i] != rhs[i]) return false;
	}
	return true;
}

template <size_t Rank>
constexpr bool operator!=(const bounds<Rank>& lhs, const bounds<Rank>& rhs) noexcept
{ return !(lhs == rhs); }

// bounds arithmetic
template <size_t Rank>
constexpr bounds<Rank> operator+(const bounds<Rank>& lhs, const offset<Rank>& rhs)
{ return bounds<Rank>{lhs} += rhs; }

template <size_t Rank>
constexpr bounds<Rank> operator+(const offset<Rank>& lhs, const bounds<Rank>& rhs)
{ return bounds<Rank>{rhs} += lhs; }

template <size_t Rank>
constexpr bounds<Rank> operator-(const bounds<Rank>& lhs, const offset<Rank>& rhs)
{ return bounds<Rank>{lhs} -= rhs; }

template <size_t Rank>
constexpr bounds<Rank> operator*(const bounds<Rank>& lhs, ptrdiff_t v)
{ return bounds<Rank>{lhs} *= v; }

template <size_t Rank>
constexpr bounds<Rank> operator*(ptrdiff_t v, const bounds<Rank>& rhs)
{ return bounds<Rank>{rhs} *= v; }

template <size_t Rank>
constexpr bounds<Rank> operator/(const bounds<Rank>& lhs, ptrdiff_t v)
{ return bounds<Rank>{lhs} /= v; }

template <size_t Rank>
bounds_iterator<Rank> begin(const bounds<Rank>& b) noexcept 
{ return b.begin(); }

template <size_t Rank>
bounds_iterator<Rank> end(const bounds<Rank>& b) noexcept 
{ return b.end(); }


template <size_t Rank>
class bounds_iterator
	// : public iterator<random_access_iterator_tag,
    //                   offset<Rank>, ptrdiff_t, offset<Rank>*, offset<Rank>&>
{
public:
	using iterator_category = std::random_access_iterator_tag; // unspecified but satisfactory
	using value_type        = offset<Rank>;
	using difference_type   = ptrdiff_t;
	using pointer           = offset<Rank>*;  // unspecified but satisfactory (?)
	using reference         = const offset<Rank>;

	static_assert(Rank > 0, "Size of Rank must be greater than 0");

	bounds_iterator(const bounds<Rank> bounds, offset<Rank> off = offset<Rank>()) noexcept
	 : bounds_(bounds), offset_(off) {}

	bool operator==(const bounds_iterator& rhs) const { 
		// Requires *this and rhs are iterators over the same bounds object.
		return offset_ == rhs.offset_;
	}

	bounds_iterator& operator++();
	bounds_iterator  operator++(int);
	bounds_iterator& operator--();
	bounds_iterator  operator--(int);

	bounds_iterator  operator+(difference_type n) const;
	bounds_iterator& operator+=(difference_type n);
	bounds_iterator  operator-(difference_type n) const;
	bounds_iterator& operator-=(difference_type n);

	difference_type  operator-(const bounds_iterator& rhs) const;

	// Note this iterator is not a true random access iterator, nor meets N4512
	// + operator* returns a value type (and not a reference)
	// + operator-> returns a pointer to the current value type, which breaks N4512 as this
	//   must be considered invalidated after any subsequent operation on this iterator
	reference operator*() const { return offset_; }
	pointer   operator->() const { return &offset_; }

	reference operator[](difference_type n) const { 
		bounds_iterator<Rank> iter(*this);
		return (iter += n).offset_;
	}

	bounds_iterator& _setOffTheEnd();

private:
	bounds<Rank> bounds_;
	offset<Rank> offset_;
};

// template <size_t Rank>
// bounds_iterator<Rank>& bounds_iterator<Rank>::operator++()
// {
// 	// todo: replace with iterative equivalent
// 	std::function<void(size_t)> increment = [&](size_t dim)
// 	{ 
// 		if (dim == -1) {
// 			offset_[0] = -1;
// 			return;
// 		}
			
// 		if (++offset_[dim] < bounds_[dim]) {
// 			return;
// 		}
// 		else {
// 			offset_[dim] = 0;
// 			increment(dim-1);
// 		}
// 	};

// 	increment(Rank-1);
//     return (*this);
// }

template <size_t Rank>
bounds_iterator<Rank> bounds_iterator<Rank>::operator++(int)
{
	bounds_iterator tmp(*this);
	++(*this);
	return tmp;
}

template <size_t Rank>
bounds_iterator<Rank>& bounds_iterator<Rank>::operator++()
{
	// watchit: dim must be signed in order to fail the condition dim>=0
	for (int dim=(Rank-1); dim>=0; --dim)
	{
		if (++offset_[dim] < bounds_[dim])
			return (*this);
		else
			offset_[dim] = 0;
	}

	// off-the-end value
	_setOffTheEnd();

	return *this;
}

template <size_t Rank>
bounds_iterator<Rank>& bounds_iterator<Rank>::operator--()
{
	// watchit: dim must be signed in order to fail the condition dim>=0
	for (int dim=(Rank-1); dim>=0; --dim)
	{
		if (--offset_[dim] >= 0)
			return (*this);
		else
			offset_[dim] = bounds_[dim]-1;
	}
	
	// before-the-start value
	for (int dim=0; dim<Rank-1; ++dim) {
		offset_[dim] = 0;
	}
	offset_[Rank-1] = -1;
	return *this;
}

template <size_t Rank>
bounds_iterator<Rank> bounds_iterator<Rank>::operator--(int)
{
	bounds_iterator tmp(*this);
	--(*this);
	return tmp;
}

template <size_t Rank>
bounds_iterator<Rank>& bounds_iterator<Rank>::_setOffTheEnd()
{
	for (int dim=0; dim<Rank-1; ++dim) {
		offset_[dim] = bounds_[dim]-1;
	}
	offset_[Rank-1] = bounds_[Rank-1];

	return *this;
}

template <size_t Rank>
bounds_iterator<Rank>& bounds_iterator<Rank>::operator+=(difference_type n)
{
	for (int dim=(Rank-1); dim>=0; --dim)
	{
		difference_type remainder = (n + offset_[dim]) % bounds_[dim];
		n = (n + offset_[dim]) / bounds_[dim];
		offset_[dim] = remainder;
	}
	assert(n == 0);  // no overflow
	return *this;
}

template <size_t Rank>
bounds_iterator<Rank> bounds_iterator<Rank>::operator+(difference_type n) const
{
	bounds_iterator<Rank> iter(*this);
	return iter += n;
}

template <size_t Rank>
bounds_iterator<Rank>& bounds_iterator<Rank>::operator-=(difference_type n)
{
	// take (diminished) radix compliment 
	auto diminishedRadixComplement = [&]() {
		for (int dim=(Rank-1); dim>=0; --dim)
		{
			offset_[dim] = bounds_[dim] - offset_[dim];
		}
	};

	diminishedRadixComplement();
	*this += n;
	diminishedRadixComplement();

	return *this;
}

template <size_t Rank>
bounds_iterator<Rank> bounds_iterator<Rank>::operator-(difference_type n) const
{
	bounds_iterator<Rank> iter(*this);
	return iter -= n;
}

// Free functions

template <size_t Rank>
bool operator==(const bounds_iterator<Rank>& lhs, const bounds_iterator<Rank>& rhs)
{ return lhs.operator==(rhs); }

template <size_t Rank>
bool operator!=(const bounds_iterator<Rank>& lhs, const bounds_iterator<Rank>& rhs)
{ return !lhs.operator==(rhs); }

template <size_t Rank>
bool operator<(const bounds_iterator<Rank>& lhs, const bounds_iterator<Rank>& rhs)
{ return rhs - lhs > 0; }

template <size_t Rank>
bool operator<=(const bounds_iterator<Rank>& lhs, const bounds_iterator<Rank>& rhs)
{ return !(lhs > rhs); }

template <size_t Rank>
bool operator>(const bounds_iterator<Rank>& lhs, const bounds_iterator<Rank>& rhs)
{ return rhs < lhs; }

template <size_t Rank>
bool operator>=(const bounds_iterator<Rank>& lhs, const bounds_iterator<Rank>& rhs)
{ return !(lhs < rhs); }

template <size_t Rank>
bounds_iterator<Rank> operator+(typename bounds_iterator<Rank>::difference_type n,
                                const bounds_iterator<Rank>& rhs);

namespace {

	template <typename Viewable, typename U, typename View = std::remove_reference_t<Viewable>>
	using is_viewable_on_u = std::integral_constant<bool,
			std::is_convertible<typename View::size_type, ptrdiff_t>::value &&
			std::is_convertible<typename View::value_type*, std::add_pointer_t<U>>::value &&
			std::is_same<std::remove_cv_t<typename View::value_type>, std::remove_cv_t<U>>::value

		>;

	// template <typename Viewable, ValueType, Pointer>
	// using is_viewable_on_u = std::integral_constant<bool,
	// 		std::is_convertible<typename Viewable::size_type, ptrdiff_t>::value &&
	// 		std::is_convertible<typename Viewable::pointer, Pointer>::value &&
	// 		std::is_same<std::remove_cv_t<typename Viewable::value_type>, std::remove_cv_t<ValueType>>::value
	// 	>;

	template <typename T, typename U>
	using is_viewable_value = std::integral_constant<bool,
			std::is_convertible<std::add_pointer_t<T>, std::add_pointer_t<U>>::value &&
			std::is_same<std::remove_cv_t<T>, std::remove_cv_t<U>>::value
		>;

	template <typename T, size_t Rank>
	constexpr T& view_access(T* data, const offset<Rank>& idx, const offset<Rank>& stride)
	{
		ptrdiff_t off{};
		for (size_t i=0; i<Rank; ++i)
		{
			off += idx[i] * stride[i];
		}
		return data[off];
	}

} // namespace

template <typename T, size_t Rank = 1>
class array_view
{
public:
	static constexpr size_t rank = Rank;
	using offset_type            = offset<Rank>;
	using bounds_type            = bounds<Rank>;
	using size_type              = size_t;
	using value_type             = T;
	using pointer                = T*;
	using reference              = T&;

	static_assert(Rank > 0, "Size of Rank must be greater than 0");

	constexpr array_view() noexcept : data_(nullptr) {} 

	template <typename Viewable, size_t R = Rank, 
	          typename = std::enable_if_t<R == 1 &&
	                                      is_viewable_on_u<Viewable, value_type>::value
	                                      // todo: && decay_t<Viewable> is not a specialization of array_view
	                                     >
	         >
	constexpr array_view(Viewable&& vw) : data_(vw.data()), bounds_(vw.size()) {
		// todo assert static_cast<U*>(vw.data()) points to contigious data
		// of at least vw.size()
	}

	template <typename U, size_t R = Rank,
	          typename = std::enable_if_t<R == 1 &&
	                                      //std::is_convertible<std::add_pointer_t<U>, pointer>::value && 
		                                  //std::is_same<std::remove_cv_t<U>, std::remove_cv_t<value_type>>::value
		                                  is_viewable_value<U, value_type>::value
	                                     >
	>
  	constexpr array_view(const array_view<U, R>& rhs) noexcept
  		: data_(rhs.data()), bounds_(rhs.bounds()) {}

	template <size_t Extent,
	          typename = std::enable_if_t<Extent == 1> >
	constexpr array_view(value_type (&arr)[Extent]) noexcept
		: data_(arr), bounds_(Extent) {}

	template <typename U,
	          typename = std::enable_if_t<is_viewable_value<U, value_type>::value
	                                      //std::is_convertible<std::add_pointer_t<U>, pointer>::value && 
		                                  //std::is_same<std::remove_cv_t<U>, std::remove_cv_t<value_type>>::value
	                                     >
	>
 	constexpr array_view(const array_view<U, Rank>& rhs) noexcept
 		: data_(rhs.data()), bounds_(rhs.bounds()) {}

 	template <typename Viewable,
 	          typename = std::enable_if_t<is_viewable_on_u<Viewable, value_type>::value>
 	         >
 	constexpr array_view(Viewable&& vw, bounds_type bounds)
 		: data_(vw.data()), bounds_(bounds)
	{
		assert(bounds.size() <= vw.size());
	}

 	constexpr array_view(pointer ptr, bounds_type bounds)
 		: data_(ptr), bounds_(bounds) {}

 	// observers
 	constexpr bounds_type bounds() const noexcept { return bounds_; }
 	constexpr size_type   size()   const noexcept { return bounds().size(); }
 	constexpr offset_type stride() const noexcept;// { return stride_; }
 	constexpr pointer     data()   const noexcept { return data_; }

 	constexpr reference operator[](const offset_type& idx) const
 	{
		assert(bounds().contains(idx) == true); 
		return view_access(data_, idx, stride());
 	}

	// slicing and sectioning
 	template <size_t R = Rank, typename = std::enable_if_t< R>=2 >>
 	constexpr array_view<T, Rank-1> operator[](ptrdiff_t slice) const
  	{
  		assert(0 <= slice && slice < bounds()[0]);

  		av::bounds<Rank-1> new_bounds{};
  		for (size_t i=0; i<rank-1; ++i) {
  			new_bounds[i] = bounds()[i+1];
  		}

  		ptrdiff_t off = slice * stride()[0];

  		return array_view<T, Rank-1>(data_ + off, new_bounds);
  	}

  	constexpr strided_array_view<T, Rank>
  	section(const offset_type& origin, const bounds_type& section_bounds) const
  	{
		// todo: requirement is for any idx in section_bounds (boundary fail)
  		//assert(bounds().contains(origin + section_bounds) == true);
  		return strided_array_view<T, Rank>(&(*this)[origin], section_bounds, stride());
  	}

  	constexpr strided_array_view<T, Rank>
  	section(const offset_type& origin) const
  	{
  		// todo: requires checking for any idx in bounds() - origin
		// assert(bounds().contains(bounds()) == true);
  		return strided_array_view<T, Rank>(&(*this)[origin], bounds() - origin, stride());
   	}

private:
	pointer data_;
	bounds_type bounds_;
};

template <typename T, size_t Rank>
constexpr typename array_view<T,Rank>::offset_type array_view<T,Rank>::stride() const noexcept
{
	offset_type stride{};
	stride[rank-1] = 1;
	for (int dim=static_cast<int>(rank)-2; dim>=0; --dim)
	{
		stride[dim] = stride[dim+1] * bounds()[dim + 1];
	}
	return stride;
}

template <class T, size_t Rank = 1>
class strided_array_view
{
public:
	// constants and types
	static constexpr size_t rank = Rank;
	using offset_type            = offset<Rank>;
	using bounds_type            = bounds<Rank>;
	using size_type              = size_t;
	using value_type             = T;
	using pointer                = T*;
	using reference              = T&;

	// constructors, copy, and assignment
	constexpr strided_array_view() noexcept
		: data_{nullptr}, bounds_{}, stride_{} {}

	template <typename U, typename = std::enable_if_t<is_viewable_value<U, value_type>::value>>
	constexpr strided_array_view(const array_view<U, Rank>& rhs) noexcept
		: data_{rhs.data()}, bounds_{rhs.bounds()}, stride_{rhs.stride()} {}
	template <typename U, typename = std::enable_if_t<is_viewable_value<U, value_type>::value>>
	constexpr strided_array_view(const strided_array_view<U, Rank>& rhs) noexcept
		: data_{rhs.data_}, bounds_{rhs.bounds()}, stride_{rhs.stride()} {}

	constexpr strided_array_view(pointer ptr, bounds_type bounds, offset_type stride)
		: data_(ptr), bounds_(bounds), stride_(stride)
	{
		// assert that sum(idx[i] * stride[i]) fits in ptrdiff_t
	}

	// observers
	constexpr bounds_type bounds() const noexcept { return bounds_; }
	constexpr size_type   size()   const noexcept { return bounds_.size(); }
	constexpr offset_type stride() const noexcept { return stride_; }

	// element access
	constexpr reference operator[](const offset_type& idx) const
	{
		assert(bounds().contains(idx) == true);
		return view_access(data_, idx, stride_);
	}

	// slicing and sectioning
 	template <size_t R = Rank, typename = std::enable_if_t< R>=2 >>
	constexpr strided_array_view<T, Rank-1> operator[](ptrdiff_t slice) const
	{
		assert(0 <= slice && slice < bounds()[0]);

  		av::bounds<Rank-1> new_bounds{};
  		for (size_t i=0; i<rank-1; ++i) {
  			new_bounds[i] = bounds()[i+1];
  		}

  		av::offset<Rank-1> new_stride{};
  		for (size_t i=0; i<rank-1; ++i) {
  			new_stride[i] = stride()[i+1];
  		}

  		ptrdiff_t off = slice * stride()[0];

  		return strided_array_view<T, Rank-1>(data_ + off, new_bounds, new_stride);
	}

	constexpr strided_array_view<T, Rank>
	section(const offset_type& origin, const bounds_type& section_bounds) const
	{
		// todo: requirement is for any idx in section_bounds (boundary fail)
		//assert(bounds().contains(origin + section_bounds) == true);
  		return strided_array_view<T, Rank>(&(*this)[origin], section_bounds, stride());
	}

	constexpr strided_array_view<T, Rank>
	section(const offset_type& origin) const
	{
		// todo: requires checking for any idx in bounds() - origin
		// assert(bounds().contains(bounds()) == true);
  		return strided_array_view<T, Rank>(&(*this)[origin], bounds() - origin, stride());
	}

private:
	pointer     data_;
	bounds_type bounds_;
	offset_type stride_;
};



}
