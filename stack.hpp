#ifndef STACK_HPP
#define STACK_HPP

#include "vector.hpp"

namespace ft
{
	template <class T, class Container = vector<T> >
	class stack
	{
		template <class _T, class _Container>
		friend bool operator== (const stack<_T, _Container>&, const stack<_T, _Container>&);
		template <class _T, class _Container>
		friend bool operator< (const stack<_T, _Container>&, const stack<_T, _Container>&);
	public:
		typedef	typename Container::value_type		value_type;
		typedef	typename Container::size_type		size_type;
		typedef	Container							container_type;
		typedef typename Container::reference		reference;
		typedef typename Container::const_reference	const_reference;
	protected:
		Container	c;
	public:
		explicit stack(const Container& cont = Container()) : c(cont) {}

		bool empty() const {
			return c.empty();
		}

		size_type size() const {
			return c.size();
		}

		value_type& top() {
			return c.back();
		}

		const value_type& top() const {
			return c.back();
		}

		void push(const value_type& x) {
			c.push_back(x);
		}

		void pop() {
			c.pop_back();
		}
	};
	template <class T, class Container>
	bool operator==(const stack<T, Container>& x, const stack<T, Container>& y) {
		return x.c == y.c;
	}

	template <class T, class Container>
	bool operator< (const stack<T, Container>& x, const stack<T, Container>& y) {
		return x.c < y.c;
	}

	template <class T, class Container>
	bool operator!=(const stack<T, Container>& x, const stack<T, Container>& y) {
		return !(x == y);
	}

	template <class T, class Container>
	bool operator> (const stack<T, Container>& x, const stack<T, Container>& y) {
		return y < x;
	}

	template <class T, class Container>
	bool operator>=(const stack<T, Container>& x, const stack<T, Container>& y) {
		return !(x < y);
	}

	template <class T, class Container>
	bool operator<=(const stack<T, Container>& x, const stack<T, Container>& y) {
		return !(x > y);
	}
} // namespace ft


#endif

