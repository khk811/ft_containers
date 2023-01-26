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

	/**
	 * @brief 매개변수 n 만큼 data_allocator로 allocate를 함
	 *
	 * @param n data_allocator로 allocate할 길이 n
	 * @return pointer
	 */
	pointer			allocate_n(size_type n) {
		return data_allocator.allocate(n);
	}

	/**
	 * @brief target 부터 n만큼 data_allocator로 deallocate함
	 *
	 * @param first deallocate를 시작할 지점
	 * @param n deallocate 할 길이
	 */
	void			deallocate_n_from(pointer first, size_type n) {
		if (first) {
			data_allocator.deallocate(first, n);
		}
	}

	/**
	 * @brief pointer r_first부터 r_last까지 value의 값으로 construct를 함
	 *
	 * @param r_first range_first: construct 할 시작점을 가리키는 pointer
	 * @param r_last range_last: construct가 끝나는 점의 pointer
	 * @param d_first dest_first: construct값이 저장될 시작점을 가리키는 pointer
	 * @return dest_first가 range 만큼 이동한 후의 pointer 값
	 *
	 * @note 함수 내부에서 iterator를 사용하고 나서 나왔을때 위치 변경이 될까봐
	 * 함수 스택 내에 복사를 하고 사용하게 작성해놓음
	 * 만약 문제가 없다면 해당 라인 삭제를 해도 좋음
	 */
	pointer			construct_by_range(const_iterator r_first, const_iterator r_last, pointer d_first) {
		pointer	r_first_copy(const_cast<pointer>(r_first));
		pointer	r_last_copy(const_cast<pointer>(r_last));

		for (; r_first_copy != r_last_copy; ++r_first_copy, ++d_first) {
			data_allocator.construct(d_first, *(r_first_copy));
		}
		return d_first;
	}

	// 진짜 하다하다 안돼서 하나 더 만듬; 환장해
	template<typename Iterator>
	pointer			construct_by_range(Iterator r_first, Iterator r_last, pointer d_first, \
	typename ft::enable_if<!ft::is_integral<Iterator>::value, Iterator>::type* = 0) {
		Iterator	r_first_copy(r_first);
		Iterator	r_last_copy(r_last);

		for (; r_first_copy != r_last_copy; ++r_first_copy, ++d_first) {
			data_allocator.construct(d_first, *(r_first_copy));
		}
		return d_first;
	}

	/**
	 * @brief pointer first가 가리키는 점부터 크기 n 만큼 value 값으로 construct를 함
	 *
	 * @param first construct 할 시작점을 가리키는 iterator
	 * @param n construct할 길이(크기)
	 * @param value construct할 값
	 * @return pointer first: n길이 후의 first값을 반환함
	 */
	pointer			construct_n(pointer first, size_type n, const T& value) {
		for (; n > 0; --n, ++first) {
			data_allocator.construct(first, value);
		}
		return first;
	}

	/**
	 * @brief pointer r_first부터 r_last까지 destroy를 함
	 *
	 * @param r_first range_first: destroy 할 시작점을 가리키는 pointer
	 * @param r_last range_last: destroy가 끝나는 점의 pointer
	 *
	 * @note 함수 내부에서 iterator를 사용하고 나서 나왔을때 위치 변경이 될까봐
	 * 함수 스택 내에 복사를 하고 사용하게 작성해놓음
	 * 만약 문제가 없다면 해당 라인 삭제를 해도 좋음
	 * 매개변수 const_iterator 이부분 바뀌여야 하는지, 오버로딩 할지 결정하길
	 */
	void			destory_by_range(const_iterator r_first, const_iterator r_last) {
		pointer	r_first_copy(const_cast<pointer>(r_first));
		pointer	r_last_copy(const_cast<pointer>(r_last));

		for (; r_first_copy != r_last_copy; ++r_first_copy)
		{
			data_allocator.destroy(r_first_copy);
		}
	}

	/**
	 * @brief pointer first가 가리키는 점부터 크기 n 만큼 value 값으로 destroy를 함
	 *
	 * @param first destroy 할 시작점을 가리키는 iterator
	 * @param n destroy할 길이(크기)
	 */
	void			destroy_n(pointer first, size_type n) {
		for (; n > 0; --n, ++first) {
			data_allocator.destroy(first);
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
		// return size_type(-1) / sizeof(T);
		return data_allocator.max_size();
	}

	void					resize(size_type sz, T c = T()) {
		if (sz < size()) {
			erase(begin() + sz, end());
		} else {
			insert(end(), sz - size(), c);
		}
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

			// allocate and copy w/memmove?
			pointer	new_start = allocate_n(n);
			construct_by_range(start, finish, new_start);

			// destroy and deallocate
			// pointer	tmp = start;
			// for (; tmp != finish; ++tmp) {
			// 	data_allocator.destroy(tmp);
			// }
			destory_by_range(start, finish);
			deallocate_n_from(start, end_of_storage - start);

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
			} else { // finish == end_of_storage;
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
				{	// move elements to new_start;
					new_finish = construct_by_range(iterator(start), position, new_finish);
					data_allocator.construct(new_finish, x);
					++new_finish;
					new_finish = construct_by_range(position, iterator(finish), new_finish);
				}
				catch(...)	// if it failed, destroy all of it;
				{
					//error handlling;
					destory_by_range(new_start, new_finish);
					deallocate_n_from(new_start, new_capacity);
					// throw exception;
					throw;
				}
				destory_by_range(start, finish);
				deallocate_n_from(start, end_of_storage - start);
				// define new_start, new_finish by this->new_start, new_finish;
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
		for (; first != last; ++first) {
			position = insert(position, *first);
			++position;
		}
	}

	iterator	erase(iterator position) {
		if (position + 1 != end()) {
			std::copy(position + 1, end(), position);
		}
		// popback?
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
		pointer			x_start = x.start;
		pointer			x_finish = x.finish;
		pointer			x_end_of_storage = x.end_of_storage;

		x.start = this->start;
		x.finish = this->finish;
		x.end_of_storage = this->end_of_storage;
		this->start = x_start;
		this->finish = x_finish;
		this->end_of_storage = x_end_of_storage;
	}

	void		clear() {
		erase(begin(), end());
	}


	// Constructor/copy/destroy;
	explicit vector(const Allocator& alloc = Allocator())
	: data_allocator(alloc), start(0), finish(0), end_of_storage(0) {}

	explicit vector(size_type n, const T& value = T(), const Allocator& alloc = Allocator())
	: data_allocator(alloc) {
		start = allocate_n(n);
		finish = start;
		end_of_storage = start + n;

		finish = construct_n(finish, n, const_cast<T&>(value));
		// for (; n > 0; --n, ++finish)
		// {
		// 	data_allocator.construct(finish, value);
		// }
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

		// 여기서 const vector& 때문에 x.begin(),  x.end()가 const를 뱉으니까 문제가 됨
		finish = construct_by_range(x.begin(), x.end(), finish);
		// iterator	x_begin = const_cast<iterator>(x.begin());
		// iterator	x_end = const_cast<iterator>(x.end());

		// for (; x_begin != x_end; ++x_begin, ++finish)
		// {
		// 	data_allocator.construct(finish, *(x_begin));
		// }
	}

	~vector() {
		// pointer	tmp = start;
		// for (; tmp != finish; ++tmp) {
		// 	data_allocator.destroy(tmp);
		// }
		destory_by_range(start, finish);
		deallocate_n_from(start, end_of_storage - start);
	}

	vector<T,Allocator>& operator=(const vector<T,Allocator>& x) {
		if (this != &x) {
			const size_type	x_size = x.size();
			if (capacity() < x_size) {
				// 현재 용량보다 x가 더 클 경우 => 재할당 하고 옮김;
				pointer	new_start = allocate_n(x_size);
				// std::memmove(new_start, x.begin(), x.end() - x.begin());
				construct_by_range(x.begin(), x.end(), new_start);
				// pointer	tmp = start;
				// for (; tmp != finish; ++tmp) {
				// 	data_allocator.destroy(tmp);
				// }
				destory_by_range(start, finish);
				deallocate_n_from(start, end_of_storage - start);
				start = new_start;
				end_of_storage = start + x_size;
			} else if (size() >= x_size) {
				// 현재 채워진 크기보다 x의 크기가 작거나 같을때 => 복사만 하는건가;
				iterator	i(std::copy(x.begin(), x.end(), begin()));
				destory_by_range(i, end());
			} else {
				// ???
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
		iterator	curr(begin());
		for (; first != last && curr != end(); ++curr, ++first) {
			*(curr) = *(first);
 		}
		if (first == last) {
			erase(curr, end());
		} else {
			insert(end(), first, last);
		}
	}

	void assign(size_type n, const T& u) {
		if (n > capacity()) {
			vector<T, Allocator>	tmp(n, u, get_allocator());
			tmp.swap(*this);
		} else if (n > size()) {
			std::fill(begin(), end(), u);
			finish = construct_n(finish, n - size(), u);
		} else {
			erase(std::fill_n(begin(), n, u), end());
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

} // namespace ft


#endif
