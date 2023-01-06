#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <iostream>
#include <memory>
#include "ft_iterator.hpp"

namespace ft {

template <typename _Tp, typename _Allocator>
class _Vector_alloc_base {
public:
	typedef _Allocator			allocator_type;
	allocator_type	get_allocator() const { return _M_data_allocator; }
	_Vector_alloc_base(const _Allocator& __a) : \
	_M_data_allocator(__a), _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
protected:
	allocator_type		_M_data_allocator;
	_Tp*				_M_start;
	_Tp*				_M_finish;
	_Tp*				_M_end_of_storage;

	_Tp*	_M_allocate(size_t __n) { return _M_data_allocator.allocator(__n); }
	void	_M_deallocate(_Tp* __p, size_t __n) { if (__p) _M_data_allocator.deallocate(__p, __n); }
};

template <typename _Tp, typename _Alloc>
struct _Vector_base : public _Vector_alloc_base<_Tp, _Alloc> {
	typedef _Vector_alloc_base<_Tp, _Alloc>		_Base;
	typedef	typename _Base::allocator_type	allocator_type;

	_Vector_base(const allocator_type& __a) : _Base(__a) {}
	_Vector_base(size_t __n, const allocator_type& __a) : _Base(__a) {
		this->_M_start = this->_M_allocate(__n);
		this->_M_finish = this->_M_start;
		this->_M_end_of_storage = this->_M_start + __n;
	}
	~_Vector_base() { this->_M_deallocate(this->_M_start, this->_M_end_of_storage - this->_M_start); }
};

template <typename _Tp, typename _Alloc = std::allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc>
{
private:
	typedef _Vector_base<_Tp, _Alloc>							_Base;
	typedef vector<_Tp, _Alloc>									vector_type;
public:
	typedef _Tp													value_type;
	typedef value_type*											pointer;
	typedef const value_type*									const_pointer;
	typedef ft::__normal_iterator<pointer, vector_type>			iterator;
	typedef ft::__normal_iterator<const_pointer, vector_type>	const_iterator;
	typedef value_type&											reference;
	typedef const value_type&									const_reference;
	typedef size_t												size_type;
	typedef ptrdiff_t											difference_type;

	typedef typename _Base::allocator_type						allocator_type;
	allocator_type	get_allocator() const { return _Base::get_allocator(); }

	typedef reverse_iterator<const_iterator>					const_reverse_iterator;
	typedef reverse_iterator<iterator>							reverse_iterator;

protected:
	using _Base::_M_allocate;
	using _Base::_M_deallocate;
	using _Base::_M_start;
	using _Base::_M_finish;
	using _Base::_M_end_of_storage;

public:
	iterator				begin() { return iterator(_M_start); }
	const_iterator			begin() const { return const_iterator(_M_start); }
	iterator				end() { return iterator(_M_finish); }
	const_iterator			end() const { return const_iterator(_M_finish); }
	reverse_iterator		rbegin() { return reverse_iterator(end()); }
	const_reverse_iterator	rbegin() const { return const_reverse_iterator(end()); }
	reverse_iterator		rend() { return reverse_iterator(begin()); }
	const_reverse_iterator	rend() const { return const_reverse_iterator(begin()); }

	size_type				size() const { return size_type(end() - begin()); }
	size_type				max_size() const { return size_type(-1) / sizeof(_Tp); }
	size_type				capacity() const { return size_type(const_iterator(_M_end_of_storage - begin())); }
	bool					empty() const { return begin() == end(); }

	reference				operator[](size_type __n) { return *(begin() + __n); }
	const_reference			operator[](size_type __n) const { return *(begin() + __n); }

	explicit vector(const allocator_type& __a = allocator_type())
	: _Base(__a) {}

	// vector(size_type __n, const _Tp& value, const allocator_type& __a = allocator_type())
	// : _Base(__n, __a) {
	// 	// fill the _Tp;
	// }

	~vector() {}


};

}

#endif
