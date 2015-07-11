#pragma once

#include <cstddef>
#include <initializer_list>
#include <array>

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

namespace hummingbird
{

template <size_t Rank> class offset;

template <size_t Rank> class bounds;


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
	constexpr reference       operator[](size_type n)
	{
		//static_assert(n < offset_.size(), "Requires n < Rank");
	  	return offset_[n];
	}
	constexpr const_reference operator[](size_type n) const
	{
		//static_assert(n < offset_.size(), "Requires n < Rank");
		return offset_[n];
	}

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
	//static_assert(Rank > 0, "Size of Rank must be greater than 0");

	// Todo: fails, try gcc?
	//static_assert(il.size() == Rank, "Size of Rank must equal the size of the initialiser list");
	for (size_type i=0; i<il.size(); ++i) {
		(*this)[i] = *(il.begin() + i);
	}
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
	//using iterator               = bounds_iterator<Rank>;  // next
	//using const_iterator         = bounds_iterator<Rank>;
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
	//const_iterator begin() const noexcept;
	//const_iterator end() const noexcept;

	// element access
	constexpr reference       operator[](size_type n)
	{
		//static_assert(n < bounds_.size(), "Requires n < Rank");
	  	return bounds_[n];
	}
	constexpr const_reference operator[](size_type n) const
	{
		//static_assert(n < bounds_.size(), "Requires n < Rank");
		return bounds_[n];
	}

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
	//static_assert(Rank > 0, "Size of Rank must be greater than 0");

	// Todo: fails, try gcc?
	//static_assert(il.size() == Rank, "Size of Rank must equal the size of the initialiser list");
	for (size_type i=0; i<il.size(); ++i) {
		(*this)[i] = *(il.begin() + i);
	}
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
	for (size_t i=0; i<Rank; ++i) {
		if ( !(0 <= idx[i] && idx[i] <= (*this)[i]) )  return false;
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


}
