#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <iostream>
#include <memory>
#include "ft_iterator.hpp"

namespace ft {

template <class _Tp, class _Allocator>
class _Vector_alloc_base {
public:
	typedef typename _Allocator			allocator_type;
	allocator_type	get_allocator() const { return _M_data_allocator; }
	_Vector_alloc_base(const _Allocator& __a) : \
	_M_data_allocator(__a), _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
protected:
	allocator_type	_M_data_allocator;
	_Tp*			_M_start;
	_Tp*			_M_finish;
	_Tp*			_M_end_of_storage;

	_Tp*	_M_allocate(size_t __n) { return _M_data_allocator.allocator(__n); }
	void	_M_deallocate(_Tp* __p, size_t __n) { if (__p) _M_data_allocator.deallocate(__p, __n); }
};

template <class _Tp, class _Alloc>
struct _Vector_base : public _Vector_alloc_base<_Tp, _Alloc> {
	typedef _Vector_alloc_base<_Tp, _Alloc>		_Base;
	typedef	typename _Base::allocator_type				allocator_type;

	_Vector_base(const allocator_type& __a) : _Base(__a) {}
	_Vector_base(size_t __n, const allocator_type& __a) : _Base(__a) {
		_M_start = _M_allocate(__n);
		_M_finish = _M_start;
		_M_end_of_storage - _M_start + __n;
	}
	~_Vector_base() { _M_deallocate(_M_start, _M_end_of_storage - _M_start); }
};

template <class _Tp, class _Alloc = std::allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc>
{
private:
	typedef _Vector_base<_Tp, _Alloc>							_Base;
	typedef vector<_Tp, _Alloc>									vector_type;
public:
	typedef _Tp													value_type;
	typedef value_type*											pointer;
	typedef const value_type*									const_pointer;
	typedef ft::iterator<pointer, vector<_Tp, _Alloc> >			iterator;
	typedef ft::iterator<const_pointer, vector<_Tp, _Alloc> >	const_iterator;
	typedef value_type&											reference;
	typedef const value_type&									const_reference;
	typedef size_t												size_type;
	typedef ptrdiff_t											difference_type;

	typedef typename _Base::allocator_type						allocator_type;
	allocator_type	get_allocator() const { return _Base::get_allocator(); }

protected:
	using _Base::_M_allocate;
	using _Base::_M_deallocate;
	using _Base::_M_start;
	using _Base::_M_finish;
	using _Base::_M_end_of_storage;

public:
	iterator		begin() { return iterator(_M_start); }
	const_iterator	begin() const { return const_iterator(_M_start); }
	iterator		end() { return iterator(_M_finish); }
	const_iterator	end() const { return const_iterator(_M_finish); }
	size_type		size() const { return size_type(end() - begin()); }
	reference		operator[](size_type __n) { return *(begin() + __n); }
};

}

#endif
