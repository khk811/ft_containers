#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <memory>
#include "iterator.hpp"

namespace ft
{
template <class T, class Allocator = std::allocator<T> > class vector {
public:
	// types:
	typedef typename Allocator::reference				reference;
	typedef typename Allocator::const_reference 		const_reference;
	typedef std::size_t									size_type;
	typedef std::ptrdiff_t								difference_type;
	typedef T											value_type;
	typedef Allocator									allocator_type;
	typedef typename Allocator::pointer					pointer;
	typedef typename Allocator::const_pointer			const_pointer;
	typedef pointer										iterator;
	typedef const_pointer								const_iterator;
	typedef ft::reverse_iterator<iterator>				reverse_iterator;
	typedef ft::reverse_iterator<const_iterator>		const_reverse_iterator;

private:
	allocator_type	data_allocator;
	pointer			start;
	pointer			finish;
	pointer			end_of_storage;

public:

	vector<T,Allocator>& operator=(const vector<T,Allocator>& x);
	template <class InputIterator>
	void assign(InputIterator first, InputIterator last);
	void assign(size_type n, const T& u);
	allocator_type get_allocator() const;

	// iterators;
	iterator				begin() {
		return iterator(start);
	}

	const_iterator			begin() const {
		return const_iterator(start);
	}

	iterator				end() {
		return iterator(finish);
	}

	const_iterator			end() const {
		return const_iterator(finish);
	}

	reverse_iterator		rbegin() {
		return reverse_iterator(end());
	}

	const_reverse_iterator	rbegin() const {
		return const_reverse_iterator(end());
	}

	reverse_iterator		rend() {
		return reverse_iterator(begin());
	}

	const_reverse_iterator	rend() const {
		return const_reverse_iterator(begin());
	}

	// capacity
	size_type				size() const {
		return size_type(end() - begin());
	}

	size_type				max_size() const {
		return size_type(-1) / sizeof(T);
	}

	void					resize(size_type sz, T c = T());

	size_type				capacity() const {
		return size_type(const_iterator(end_of_storage - begin()));
	}

	bool					empty() const {
		return begin() == end();
	}
	void					reserve(size_type n);

	// element access;
	reference				operator[](size_type n) {
		return *(begin() + n);
	}

	const_reference			operator[](size_type n) const {
		return *(begin() + n);
	}

	const_reference			at(size_type n) const;
	reference				at(size_type n);
	reference				front();
	const_reference			front() const;
	reference				back();
	const_reference				back() const;

	// modifiers

	// Constructor/copy/destroy;
	explicit vector(const Allocator& alloc = Allocator())
	: data_allocator(alloc), start(0), finish(0), end_of_storage(0) {}

	explicit vector(size_type n, const T& value = T(), const Allocator& alloc = Allocator()) {
		data_allocator = alloc;
		start = alloc.allocate(n);
		finish = start;
		end_of_storage = start + n;
		for (; n > 0; --n, ++finish)
		{
			data_allocator.constrct(finish, value);
		}
	}

	template <class InputIterator>
	vector(InputIterator first, InputIterator last, const Allocator& = Allocator());

	vector(const vector<T,Allocator>& x) {
		data_allocator = x.get_allocator()
	}

	~vector() {
		pointer	tmp = start;
		for (; tmp != finish; ++tmp) {
			data_allocator.destroy(tmp);
		}
		data_allocator.deallocate(start, end_of_storage - start);
	}
};
} // namespace ft


#endif
