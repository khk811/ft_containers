#ifndef ITERATOR_TRAITS_HPP
#define ITERATOR_TRAITS_HPP

//https://github.com/gcc-mirror/gcc/blob/releases/gcc-3.1/libstdc%2B%2B-v3/include/bits/stl_iterator_base_types.h

namespace ft
{

struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

template <typename _Category, typename _Tp, typename _Distance = ptrdiff_t, \
typename _Pointer = _Tp*, typename _Reference = _Tp&>
struct iterator
{
	typedef _Category	iterator_category;
	typedef _Tp			value_type;
	typedef _Distance	difference_type;
	typedef _Pointer	pointer;
	typedef	_Reference	reference;
};

template <typename _Iterator>
struct iterator_traits
{
	typedef typename _Iterator::iterator_category	iterator_category;
	typedef typename _Iterator::value_type			value_type;
	typedef typename _Iterator::differance_type		differece_type;
	typedef typename _Iterator::pointer				pointer;
	typedef typename _Iterator::reference			reference;
};

template <typename _Tp>
struct iterator_traits<_Tp*>
{
	typedef random_access_iterator_tag	iterator_category;
	typedef _Tp							value_type;
	typedef ptrdiff_t					difference_type;
	typedef _Tp*						pointer;
	typedef _Tp&						reference;
};

template <typename _Tp>
struct iterator_traits<const _Tp*>
{
	typedef random_access_iterator_tag	iterator_category;
	typedef _Tp							value_type;
	typedef ptrdiff_t					difference_type;
	typedef const _Tp*					pointer;
	typedef const _Tp&					reference;
};

// normal iterator?
template <typename _Iterator, typename _Container>
class __normal_iterator
: public iterator<typename iterator_traits<_Iterator>::iterator_category,\
				typename iterator_traits<_Iterator>::value_type,\
				typename iterator_traits<_Iterator>::difference_type,\
				typename iterator_traits<_Iterator>::pointer,\
				typename iterator_traits<_Iterator>::reference>
{
protected:
	_Iterator _M_current;

public:
	typedef typename iterator_traits<_Iterator>::difference_type	difference_type;
	typedef typename iterator_traits<_Iterator>::reference			reference;
	typedef typename iterator_traits<_Iterator>::pointer			pointer;

	__normal_iterator() : _M_current(_Iterator()) { }
	__normal_iterator(const _Iterator& __i) : _M_current(__i) { }

	template <typename _Iter>
	__normal_iterator(const __normal_iterator<_Iter, _Container>& __i)
	: _M_current(__i.base()) { }

	reference	operator*() const { return *_M_current; }
	pointer		operator->() const { return _M_current; }
	__normal_iterator&	operator++() {
		++_M_current;
		return *this;
	}
	__normal_iterator	operator++(int) { return __normal_iterator(_M_current++); }
	__normal_iterator&	operator--() {
		--_M_current;
		return *this;
	}
	__normal_iterator	operator--(int) { return __normal_iterator(_M_current--); }

	reference	operator[](const difference_type& __n) const {
		return _M_current[__n];
	}
	__normal_iterator&	operator+=(const difference_type& __n) {
		_M_current += __n;
		return *this;
	}
	__normal_iterator	operator+(const difference_type& __n) const {
		return __normal_iterator(_M_current + __n);
	}
	__normal_iterator&	operator-=(const difference_type& __n) {
		_M_current -= __n;
		return *this;
	}
	__normal_iterator	operator-(const difference_type& __n) const {
		return __normal_iterator(_M_current - __n);
	}
	difference_type	operator-(const __normal_iterator& __i) const {
		return _M_current - __i._M_current;
	}
	const _Iterator&	base() const { return _M_current; }
};

template <typename _IteratorL, typename _IteratorR, typename _Container>
bool	operator==(const __normal_iterator<_IteratorL, _Container>& __lhs, \
					const __normal_iterator<_IteratorR, _Container>& __rhs) {
	return __lhs.base() == __rhs.base();
}

template <typename _Iterator, typename _Container>
bool	operator==(const __normal_iterator<_Iterator, _Container>& __lhs, \
					const __normal_iterator<_Iterator, _Container>& __rhs) {
	return __lhs.base() == __rhs.base();
}

template <typename _IteratorL, typename _IteratorR, typename _Container>
bool	operator!=(const __normal_iterator<_IteratorL, _Container>& __lhs, \
					const __normal_iterator<_IteratorR, _Container>& __rhs) {
	return __lhs.base() != __rhs.base();
}

template <typename _Iterator, typename _Container>
bool	operator!=(const __normal_iterator<_Iterator, _Container>& __lhs, \
					const __normal_iterator<_Iterator, _Container>& __rhs) {
	return __lhs.base() != __rhs.base();
}

template <typename _IteratorL, typename _IteratorR, typename _Container>
bool	operator<(const __normal_iterator<_IteratorL, _Container>& __lhs, \
					const __normal_iterator<_IteratorR, _Container>& __rhs) {
	return __lhs.base() < __rhs.base();
}

template <typename _Iterator, typename _Container>
bool	operator<(const __normal_iterator<_Iterator, _Container>& __lhs, \
					const __normal_iterator<_Iterator, _Container>& __rhs) {
	return __lhs.base() < __rhs.base();
}

template <typename _IteratorL, typename _IteratorR, typename _Container>
bool	operator>(const __normal_iterator<_IteratorL, _Container>& __lhs, \
					const __normal_iterator<_IteratorR, _Container>& __rhs) {
	return __lhs.base() > __rhs.base();
}

template <typename _Iterator, typename _Container>
bool	operator>(const __normal_iterator<_Iterator, _Container>& __lhs, \
					const __normal_iterator<_Iterator, _Container>& __rhs) {
	return __lhs.base() > __rhs.base();
}

template <typename _IteratorL, typename _IteratorR, typename _Container>
bool	operator<=(const __normal_iterator<_IteratorL, _Container>& __lhs, \
					const __normal_iterator<_IteratorR, _Container>& __rhs) {
	return __lhs.base() <= __rhs.base();
}

template <typename _Iterator, typename _Container>
bool	operator<=(const __normal_iterator<_Iterator, _Container>& __lhs, \
					const __normal_iterator<_Iterator, _Container>& __rhs) {
	return __lhs.base() <= __rhs.base();
}

template <typename _IteratorL, typename _IteratorR, typename _Container>
bool	operator>=(const __normal_iterator<_IteratorL, _Container>& __lhs, \
					const __normal_iterator<_IteratorR, _Container>& __rhs) {
	return __lhs.base() >= __rhs.base();
}

template <typename _Iterator, typename _Container>
bool	operator>=(const __normal_iterator<_Iterator, _Container>& __lhs, \
					const __normal_iterator<_Iterator, _Container>& __rhs) {
	return __lhs.base() >= __rhs.base();
}

template <typename _Iterator, typename _Container>
__normal_iterator<_Iterator, _Container>	operator+(typename __normal_iterator<_Iterator, _Container>::difference_type __n,\
const __normal_iterator<_Iterator, _Container>& __i) {
	return __normal_iterator<_Iterator, _Container>(__i.base() + __n);
}

// reverse_iterator;
template <typename _Iterator>
class reverse_iterator
: public iterator<typename iterator_traits<_Iterator>::iterator_category,\
				typename iterator_traits<_Iterator>::value_type,\
				typename iterator_traits<_Iterator>::difference_type,\
				typename iterator_traits<_Iterator>::pointer,\
				typename iterator_traits<_Iterator>::reference>
{
protected:
	_Iterator	current;

public:
	typedef _Iterator												iterator_type;
	typedef typename iterator_traits<_Iterator>::difference_type	difference_type;
	typedef typename iterator_traits<_Iterator>::reference			reference;
	typedef typename iterator_traits<_Iterator>::pointer			pointer;

	reverse_iterator() {}
	reverse_iterator(const reverse_iterator& __x)
	: current(__x.current) { }
	template <typename _Iter>
	reverse_iterator(const reverse_iterator<_Iter>& __x)
	: current(__x.base()) {}
	iterator_type	base() const { return current; }

	reference	operator*() const {
		_Iterator	__tmp = current;
		return *(--__tmp);
	}

	pointer	operator->() const {
		return &(operator*());
	}

	reverse_iterator&	operator++() {
		--current;
		return *this;
	}

	reverse_iterator&	operator++(int) {
		reverse_iterator	__tmp = *this;
		--current;
		return __tmp;
	}

	reverse_iterator&	operator--() {
		++current;
		return *this;
	}

	reverse_iterator&	operator--(int) {
		reverse_iterator	__tmp = *this;
		++current;
		return __tmp;
	}

	reverse_iterator	operator+(difference_type __n) const {
		return reverse_iterator(current - __n);
	}

	reverse_iterator&	operator+=(difference_type __n) {
		current -= __n;
		return *this;
	}

	reverse_iterator	operator-(difference_type __n) const {
		return reverse_iterator(current + __n);
	}

	reverse_iterator&	operator-=(difference_type __n) {
		current += __n;
		return *this;
	}

	reference	operator[](difference_type __n) const {
		return *(*this + __n);
	}
 };

template <typename _Iterator>
bool	operator==(const reverse_iterator<_Iterator>& __x, \
					const reverse_iterator<_Iterator>& __y) {
	return __x.base() == __y.base();
}

template <typename _Iterator>
bool	operator<(const reverse_iterator<_Iterator>& __x, \
					const reverse_iterator<_Iterator>& __y) {
	return __y.base() < __x.base();
}

template <typename _Iterator>
bool	operator!=(const reverse_iterator<_Iterator>& __x,
					const reverse_iterator<_Iterator>& __y) {
	return !(__x.base() == __y.base());
}

template <typename _Iterator>
bool	operator>(const reverse_iterator<_Iterator>& __x, \
					const reverse_iterator<_Iterator>& __y) {
	return __y < __x;
}

template <typename _Iterator>
bool	operator<=(const reverse_iterator<_Iterator>& __x, \
					const reverse_iterator<_Iterator>& __y) {
	return !(__y < __x);
}

template <typename _Iterator>
bool	operator>=(const reverse_iterator<_Iterator>& __x, \
					const reverse_iterator<_Iterator>& __y) {
	return !(__x < __y);
}

template <typename _Iterator>
typename reverse_iterator<_Iterator>::difference_type operator-\
(const reverse_iterator<_Iterator>& __x, const reverse_iterator<_Iterator>& __y) {
	return __y.base() - __x.base();
}

template <typename _Iterator>
reverse_iterator<_Iterator> operator+\
(typename reverse_iterator<_Iterator>::difference_type __n, const reverse_iterator<_Iterator>& __x) {
	return reverse_iterator<_Iterator>(__x.base() - __n);
}

} // namespace ft;


#endif
