#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <memory>
#include <algorithm>
#include <cstring>
#include "iterator.hpp"
#include "algobase.hpp"

namespace ft
{
template <class T, class Allocator = std::allocator<T> > class vector {
public:
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

public:
	explicit vector(const Allocator& alloc = Allocator())
	: data_allocator(alloc), start(0), finish(0), end_of_storage(0) {}

	explicit vector(size_type n, const T& value = T(), const Allocator& alloc = Allocator())
	: data_allocator(alloc) {
		start = allocate_n(n);
		finish = start;
		end_of_storage = start + n;

		finish = construct_n(finish, n, const_cast<T&>(value));
	}

	template <class InputIterator>
	vector(InputIterator first, InputIterator last, const Allocator& alloc = Allocator(), \
	typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type* = 0)
	: data_allocator(alloc), start(0), finish(0), end_of_storage(0) {
		if (ft::is_input_iter<typename iterator_traits<InputIterator>::iterator_category>::value) {
			for (; first != last; ++first) {
				push_back(*(first));
			}
		} else {
			size_type	n = ft::distance(first, last);
			start = allocate_n(n);
			end_of_storage = start + n;
			finish = construct_by_range(first, last, start);
		}
	}

	vector(const vector<T,Allocator>& x)
	: data_allocator(x.get_allocator()) {
		start = allocate_n(x.size());
		finish = start;
		end_of_storage = start + (x.size());
		finish = construct_by_range(x.begin(), x.end(), finish);
	}

	~vector() {
		destory_by_range(start, finish);
		deallocate_n_from(start, end_of_storage - start);
	}

	vector<T,Allocator>& operator=(const vector<T,Allocator>& x) {
		if (this != &x) {
			const size_type	x_size = x.size();
			if (capacity() < x_size) {
				pointer	new_start = allocate_n(x_size);
				construct_by_range(x.begin(), x.end(), new_start);
				destory_by_range(start, finish);
				deallocate_n_from(start, end_of_storage - start);
				start = new_start;
				end_of_storage = start + x_size;
			} else if (size() >= x_size) {
				iterator	i(std::copy(x.begin(), x.end(), begin()));
				destory_by_range(i, end());
			} else {
				std::copy(x.begin(), x.begin() + size(), start);
				construct_by_range(x.begin() + size(), x.end(), finish);
			}
			finish = start + x_size;
		}
		return *this;
	}

	template <class InputIterator>
	void assign(InputIterator first, InputIterator last, \
	typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type* = 0) {
		erase(begin(), end());
		insert(begin(), first, last);

	}

	void assign(size_type n, const T& u) {
		erase(begin(), end());
		insert(begin(), n, u);
	}

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
		return std::min<size_type>(data_allocator.max_size(), std::numeric_limits<difference_type>::max());
	}

	void					resize(size_type sz, T c = T()) {
		if (sz > size()) {
			insert(end(), sz - size(), c);
		} else if (sz < size()) {
			erase(begin() + sz, end());
		} else ;
	}

	size_type				capacity() const {
		return size_type(const_iterator(end_of_storage - begin()));
	}

	bool					empty() const {
		return begin() == end();
	}
	void					reserve(size_type n) {
		if (capacity() < n) {
			const size_type	old_size = size();
			pointer	new_start = allocate_n(n);

			construct_by_range(start, finish, new_start);
			destory_by_range(start, finish);
			deallocate_n_from(start, end_of_storage - start);

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

	reference				at(size_type n) {
		if (n < size()) {
			return (*this)[n];
		} else {
			std::__throw_out_of_range("vector");
		}
	}
	const_reference			at(size_type n) const {
		if (n < size()) {
			return (*this)[n];
		} else {
			std::__throw_out_of_range("vector");
		}
	}
	reference				front() {
		return *begin();
	}

	const_reference			front() const {
		return *begin();
	}

	reference				back() {
		return *(end() - 1);
	}

	const_reference			back() const {
		return *(end() - 1);
	}

	// modifiers
	void		push_back(const T& x) {
		if (finish != end_of_storage) {
			data_allocator.construct(finish, x);
			++finish;
		} else {
			insert(end(), x);
		}
	}

	void		pop_back() {
		--finish;
		data_allocator.destroy(finish);
	}

	iterator	insert(iterator position, const T& x) {
		size_type	n = position - begin();
		if (finish != end_of_storage && position == end()) {
			data_allocator.construct(finish, x);
			++finish;
		} else {
			if (finish != end_of_storage) {
				data_allocator.construct(finish, *(finish - 1));
				++finish;
				T	x_copy = x;
				std::copy_backward(position, iterator(finish - 2), iterator(finish - 1));
				*position = x_copy;
			} else {
				const size_type	old_size = size();
				size_type	new_capacity = 0;
				if (old_size != 0) {
					new_capacity = old_size * 2;
				} else {
					new_capacity = 1;
				}
				pointer	new_start = allocate_n(new_capacity);
				pointer new_finish = new_start;
				try
				{
					new_finish = construct_by_range(iterator(start), position, new_finish);
					data_allocator.construct(new_finish, x);
					++new_finish;
					new_finish = construct_by_range(position, iterator(finish), new_finish);
				}
				catch(...)
				{
					destory_by_range(new_start, new_finish);
					deallocate_n_from(new_start, new_capacity);
					throw;
				}
				destory_by_range(start, finish);
				deallocate_n_from(start, end_of_storage - start);
				start = new_start;
				finish = new_finish;
				end_of_storage = new_start + new_capacity;
			}
		}
		return begin() + n;
	}


	void		insert(iterator position, size_type n, const T& x) {
		if (n != 0) {
			if (size_type(end_of_storage - finish) >= n) {
				T	x_copy = x;
				const size_type elem_after = end() - position;
				iterator	old_finish(finish);
				if (elem_after > n) {
					construct_by_range(finish - n, finish, finish);
					finish += n;
					std::copy_backward(position, old_finish - n, old_finish);
					std::fill(position, position + n, x_copy);

				} else {
					construct_n(finish, n - elem_after, x_copy);
					finish += n - elem_after;
					construct_by_range(position, old_finish, finish);
					finish += elem_after;
					std::fill(position, old_finish, x_copy);
				}
			} else {
				const size_type	old_size = size();
				const size_type	len = old_size + std::max(old_size, n);
				iterator	new_start(allocate_n(len));
				iterator	new_finish(new_start);
				try
				{
					new_finish = construct_by_range(begin(), position, new_start);
					new_finish = construct_n(new_finish, n, x);
					new_finish = construct_by_range(position, end(), new_finish);
				}
				catch(...)
				{
					destory_by_range(new_start, new_finish);
					deallocate_n_from(new_start, len);
					throw;
				}
				destory_by_range(start, finish);
				deallocate_n_from(start, end_of_storage - start);
				start = new_start;
				finish = new_finish;
				end_of_storage = new_start + len;
			}
		}
	}

	template <class InputIterator>
	void		insert(iterator position, InputIterator first, InputIterator last, \
	typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type* = 0) {
		typedef typename iterator_traits<InputIterator>::iterator_category	IterCategory;
		range_insert(position, first, last, IterCategory());
	}

	iterator	erase(iterator position) {
		if (position + 1 != end()) {
			std::copy(position + 1, end(), position);
		}
		--finish;
		data_allocator.destroy(finish);
		return position;
	}

	iterator	erase(iterator first, iterator last) {
		iterator	tmp = std::copy(last, end(), first);

		destory_by_range(tmp, end());
		finish = finish - (last - first);
		return first;
	}

	void		swap(vector<T,Allocator>& x) {
		std::swap(data_allocator, x.data_allocator);
		std::swap(start, x.start);
		std::swap(finish, x.finish);
		std::swap(end_of_storage, x.end_of_storage);
	}

	void		clear() {
		erase(begin(), end());
	}

private:
	allocator_type	data_allocator;
	pointer			start;
	pointer			finish;
	pointer			end_of_storage;

	pointer			allocate_n(size_type n) {
		return data_allocator.allocate(n);
	}

	void			deallocate_n_from(pointer first, size_type n) {
		if (first) {
			data_allocator.deallocate(first, n);
		}
	}

	template<typename Iterator>
	pointer			construct_by_range(Iterator r_first, Iterator r_last, pointer d_first, \
	typename ft::enable_if<!ft::is_integral<Iterator>::value, Iterator>::type* = 0) {
		pointer	curr = d_first;

		for (; r_first != r_last; ++r_first, ++curr) {
			try
			{
				data_allocator.construct(curr, *(r_first));
			}
			catch(...)
			{
				destory_by_range(d_first, curr);
				throw;
			}
		}
		return curr;
	}

	pointer			construct_n(pointer first, size_type n, const T& value) {
		for (; n > 0; --n, ++first) {
			data_allocator.construct(first, value);
		}
		return first;
	}

	template <class Iterator>
	void			destory_by_range(Iterator r_first, Iterator r_last, \
	typename ft::enable_if<!ft::is_integral<Iterator>::value, Iterator>::type* = 0) {
		for (; r_first != r_last; r_first++) {
			data_allocator.destroy(r_first);
		}
	}

	void			destroy_n(pointer first, size_type n) {
		for (; n > 0; --n, ++first) {
			data_allocator.destroy(first);
		}
	}

	template<class InputIterator>
	void	range_insert(iterator position, InputIterator first, InputIterator last, input_iterator_tag) {
		for (; first != last; ++first) {
			position = insert(position, *first);
			++position;
		}
	}

	template<class ForwardIterator>
	void	range_insert(iterator position, ForwardIterator first, ForwardIterator last, forward_iterator_tag) {
		if (first != last) {
			size_type	n = ft::distance(first, last);
			if (size_type(end_of_storage - finish) >= n) {
				const size_type	elem_after = end() - position;
				iterator	old_finish(finish);
				if (elem_after > n) {
					construct_by_range(finish - n, finish, finish);
					finish += n;
					std::copy_backward(position, old_finish - n, old_finish);
					std::copy(first, last, position);
				} else {
					ForwardIterator	mid = first;
					for (size_type n = elem_after; n > 0; --n) {
						mid++;
					}
					construct_by_range(mid, last, finish);
					finish += n - elem_after;
					construct_by_range(position, old_finish, finish);
					finish += elem_after;
					std::copy(first, mid, position);
				}
			} else {
				const size_type	old_size = size();
				const size_type	len = old_size + std::max(old_size, n);
				iterator			new_start(allocate_n(len));
				iterator			new_finish(new_start);
				try
				{
					new_finish = construct_by_range(iterator(start), position, new_start);
					new_finish = construct_by_range(first, last, new_finish);
					new_finish = construct_by_range(position, iterator(finish), new_finish);
				}
				catch(...)
				{
					destory_by_range(new_start, new_finish);
					deallocate_n_from(new_start, len);
					throw;
				}
				destory_by_range(start, finish);
				deallocate_n_from(start, end_of_storage - start);
				start = new_start;
				finish = new_finish;
				end_of_storage = new_start + len;
			}
		 }
	}


};

// Operator Overloading;
template <class T, class Allocator>
bool	operator==(const vector<T,Allocator>& x, const vector<T,Allocator>& y) {
	return (x.size() == y.size() && ft::equal(x.begin(), x.end(), y.begin()));
}

template <class T, class Allocator>
bool	operator< (const vector<T,Allocator>& x, const vector<T,Allocator>& y) {
	return ft::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
}

template <class T, class Allocator>
bool	operator!=(const vector<T,Allocator>& x, const vector<T,Allocator>& y) {
	return !(x == y);
}

template <class T, class Allocator>
bool	operator> (const vector<T,Allocator>& x, const vector<T,Allocator>& y) {
	return y < x;
}

template <class T, class Allocator>
bool	operator>=(const vector<T,Allocator>& x, const vector<T,Allocator>& y) {
	return !(x < y);
}

template <class T, class Allocator>
bool	operator<=(const vector<T,Allocator>& x, const vector<T,Allocator>& y) {
	return !(y < x);
}

template <class T, class Allocator>
void	swap(vector<T, Allocator>& x, vector<T, Allocator>& y) {
	x.swap(y);
}

}

#endif
