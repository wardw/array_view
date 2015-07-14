#pragma once

#include <cstddef>
#include <initializer_list>
#include <array>

namespace hummingbird
{

template <size_t Rank> class offset;

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

// class template bounds
template <size_t Rank> class bounds;

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

// class template bounds_iterator
template <size_t Rank> class bounds_iterator;

template <size_t Rank>
bool operator==(const bounds_iterator<Rank>& lhs,
                const bounds_iterator<Rank>& rhs);
template <size_t Rank>
bool operator!=(const bounds_iterator<Rank>& lhs,
                const bounds_iterator<Rank>& rhs);
template <size_t Rank>
bool operator<(const bounds_iterator<Rank>& lhs,
               const bounds_iterator<Rank>& rhs);
template <size_t Rank>
bool operator<=(const bounds_iterator<Rank>& lhs,
                const bounds_iterator<Rank>& rhs);
template <size_t Rank>
bool operator>(const bounds_iterator<Rank>& lhs,
               const bounds_iterator<Rank>& rhs);
template <size_t Rank>
bool operator>=(const bounds_iterator<Rank>& lhs,
                const bounds_iterator<Rank>& rhs);
template <size_t Rank>
bounds_iterator<Rank> operator+(typename bounds_iterator<Rank>::difference_type n,
                                const bounds_iterator<Rank>& rhs);

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

// construction
template <size_t Rank>
constexpr offset<Rank>::offset() noexcept {
	//static_assert(Rank > 0, "Size of Rank must be greater than 0");
}

template <size_t Rank> template <size_t R, typename EnableIf>
constexpr offset<Rank>::offset(value_type v) noexcept
{
	offset_[0] = v;
}

template <size_t Rank>
constexpr offset<Rank>::offset(std::initializer_list<value_type> il)
{
	//static_assert(Rank > 0, "Size of Rank must be greater than 0");

	// Todo: fails, try gcc?
	//static_assert(il.size() == Rank, "Size of Rank must equal the size of the initialiser list");
	for (size_type i=0; i<il.size(); ++i) {
		offset_[i] = *(il.begin() + i);
	}
}

// element access
template <size_t Rank>
constexpr ptrdiff_t& offset<Rank>::operator[](size_type n)
{
	//static_assert(n < offset_.size(), "Requires n < Rank");
	return offset_[n];
}

template <size_t Rank>
constexpr const ptrdiff_t& offset<Rank>::operator[](size_type n) const
{
	//static_assert(n < offset_.size(), "Requires n < Rank");
	return offset_[n];
}

// arithmetic
template <size_t Rank>
constexpr offset<Rank>& offset<Rank>::operator+=(const offset& rhs)
{
	for (size_type i=0; i<Rank; ++i) {
		offset_[i] += rhs[i];
	}
	return *this;
}

template <size_t Rank>
constexpr offset<Rank>& offset<Rank>::operator-=(const offset& rhs)
{
	for (size_type i=0; i<Rank; ++i) {
		offset_[i] -= rhs[i];
	}
	return *this;
}

template <size_t Rank> template <size_t R, typename EnableIf>
constexpr offset<Rank>& offset<Rank>::operator++()
{
	return ++offset_[0];
}

template <size_t Rank> template <size_t R, typename EnableIf>
constexpr offset<Rank> offset<Rank>::operator++(int)
{
	return offset<Rank>{offset_[0]++};
}

template <size_t Rank> template <size_t R, typename EnableIf>
constexpr offset<Rank>& offset<Rank>::operator--()
{
	return --offset_[0];
}

template <size_t Rank> template <size_t R, typename EnableIf>
constexpr offset<Rank> offset<Rank>::operator--(int)
{
	return offset<Rank>{offset_[0]--};
}

template <size_t Rank>
constexpr offset<Rank> offset<Rank>::operator+() const noexcept
{
	return *this;
}

template <size_t Rank>
constexpr offset<Rank> offset<Rank>::operator-() const
{
	offset<Rank> copy{*this};
	for (auto& elem : offset_) {
		elem *= -1;
	}
	return copy;
}

template <size_t Rank>
constexpr offset<Rank>& offset<Rank>::operator*=(value_type v)
{
	for (auto& elem : offset_) {
		elem *= v;
	}
	return *this;
}

template <size_t Rank>
constexpr offset<Rank>& offset<Rank>::operator/=(value_type v)
{
	for (auto& elem : offset_) {
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
{
	return !(lhs == rhs);
}

// offset arithmetic
template <size_t Rank>
constexpr offset<Rank> operator+(const offset<Rank>& lhs, const offset<Rank>& rhs)
{
	return offset<Rank>{lhs} += rhs;
}

template <size_t Rank>
constexpr offset<Rank> operator-(const offset<Rank>& lhs, const offset<Rank>& rhs)
{
	return offset<Rank>{lhs} -= rhs;
}

template <size_t Rank>
constexpr offset<Rank> operator*(const offset<Rank>& lhs, ptrdiff_t v)
{
	return offset<Rank>{lhs} *= v;
}

template <size_t Rank>
constexpr offset<Rank> operator*(ptrdiff_t v, const offset<Rank>& rhs)
{
	return offset<Rank>{rhs} *= v;
}

template <size_t Rank>
constexpr offset<Rank> operator/(const offset<Rank>& lhs, ptrdiff_t v)
{
	return offset<Rank>{lhs} /= v;
}


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

template <size_t Rank>
constexpr bounds<Rank>::bounds() noexcept
{
	//static_assert(Rank > 0, "Size of Rank must be greater than 0");

}

// Question: why is this constructor not `noexcept` ?
template <size_t Rank> template <size_t R, typename EnableIf>
constexpr bounds<Rank>::bounds(value_type v)
{
	bounds_[0] = v;
}

template <size_t Rank>
constexpr bounds<Rank>::bounds(std::initializer_list<value_type> il)
{
	//static_assert(Rank > 0, "Size of Rank must be greater than 0");

	// Todo: fails, try gcc?
	//static_assert(il.size() == Rank, "Size of Rank must equal the size of the initialiser list");
	for (size_type i=0; i<il.size(); ++i) {
		bounds_[i] = *(il.begin() + i);
	}
}

// template <size_t Rank>
// constexpr size_type size() const noexcept;
// **got to here**


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
{
	return !(lhs == rhs);
}



}
