#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <memory>
#include <cstring>
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

protected:
	allocator_type	data_allocator;
	pointer			start;
	pointer			finish;
	pointer			end_of_storage;

	pointer			vector_allocate(size_type n) {
		return data_allocator.allocate(n);
	}

	void			vector_deallocate(pointer target, size_type n) {
		if (target) {
			data_allocator.deallocate(target, n);
		}
	}

public:

	allocator_type get_allocator() const {
		return allocator_type();
	}

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

	// void					resize(size_type sz, T c = T()) {
	// 	if (sz < size()) {
	// 		// erase(begin() + sz, end());
	// 	} else {
	// 		// insert(end(), sz - size(), c);
	// 	}
	// }

	size_type				capacity() const {
		return size_type(const_iterator(end_of_storage - begin()));
	}

	bool					empty() const {
		return begin() == end();
	}
	void					reserve(size_type n) {
		if (capacity() < n) {
			const size_type	old_size = size();

			// allocate and copy w/memmove?
			pointer	new_start = vector_allocate(n);
			std::memmove(new_start, start, finish - start);

			// destroy and deallocate
			pointer	tmp = start;
			for (; tmp != finish; ++tmp) {
				data_allocator.destroy(tmp);
			}
			vector_deallocate(start, end_of_storage - start);

			// redefine vector var;
			start = new_start;
			finish = new_start + old_size;
			end_of_storage = start + n;
		}
	}

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
	const_reference			back() const;

	// modifiers
	void		push_back(const T& x);
	void		pop_back();
	iterator	insert(iterator position, const T& x);
	void		insert(iterator position, size_type n, const T& x);
	template <class InputIterator>
	void		insert(iterator position, InputIterator first, InputIterator last);
	iterator	erase(iterator position);
	iterator	erase(iterator first, iterator last);

	void		swap(vector<T,Allocator>& x) {
		pointer			x_start = x.start;
		pointer			x_finish = x.finish;
		pointer			x_end_of_storage = x.end_of_storage;

		x.start = this->start;
		x.finish = this->end;
		x.end_of_storage = this->end_of_storage;
		this->start = x.start;
		this->finish = x_finish;
		this->end_of_storage = x_end_of_storage;
	}

	void		clear();

	// Constructor/copy/destroy;
	explicit vector(const Allocator& alloc = Allocator())
	: data_allocator(alloc), start(0), finish(0), end_of_storage(0) {}

	explicit vector(size_type n, const T& value = T(), const Allocator& alloc = Allocator())
	: data_allocator(alloc) {
		start = vector_allocate(n);
		finish = start;
		end_of_storage = start + n;

		for (; n > 0; --n, ++finish)
		{
			data_allocator.construct(finish, value);
		}
	}

	// template <class InputIterator>
	// vector(InputIterator first, InputIterator last, const Allocator& alloc = Allocator())
	// : data_allocator(alloc) {
	// 	// typedef typename
	// 	// 여기서 integral 분기를 type_traits로 해야함.
	// }

	vector(const vector<T,Allocator>& x)
	: data_allocator(x.get_allocator()) {
		start = vector_allocate(x.size());
		finish = start;
		end_of_storage = start + (x.size());

		iterator	x_begin = x.begin();
		iterator	x_end = x.end();

		for (; x_begin != x.end; ++x_begin, ++finish)
		{
			data_allocator.construct(finish, *(x_begin));
		}
	}

	~vector() {
		pointer	tmp = start;
		for (; tmp != finish; ++tmp) {
			data_allocator.destroy(tmp);
		}
		vector_deallocate(start, end_of_storage - start);
	}

	vector<T,Allocator>& operator=(const vector<T,Allocator>& x) {
		if (this != &x) {
			const size_type	x_size = x.size();
			if (capacity() < x_size) {
				// 현재 용량보다 x가 더 클 경우 => 재할당 하고 옮김;
				pointer	new_start = vector_allocate(x_size);
				std::memmove(new_start, x.begin(), x.end() - x.begin());

				pointer	tmp = start;
				for (; tmp != finish; ++tmp) {
					data_allocator.destroy(tmp);
				}
				vector_deallocate(start, end_of_storage - start);
				start = new_start;
				end_of_storage = start + x_size;
			} else if (size() >= x_size) {
				// 현재 채워진 크기보다 x의 크기가 작거나 같을때 => 복사만 하는건가;
			} else {
				// ???
			}
			finish = start + x_size;
		}
		return *this;
	}
	template <class InputIterator>
	void assign(InputIterator first, InputIterator last);

	void assign(size_type n, const T& u);
};
} // namespace ft


#endif
