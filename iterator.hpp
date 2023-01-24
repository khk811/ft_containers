#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include <cstddef>

namespace ft
{
	// standard iterator tag; empty class types used to indicate iterator categories;
	struct input_iterator_tag {};
	struct output_iterator_tag {};
	struct forward_iterator_tag: public input_iterator_tag {};
	struct bidirectional_iterator_tag: public forward_iterator_tag {};
	struct random_access_iterator_tag: public bidirectional_iterator_tag {};

	// primitives;
	template <typename Category, typename T, typename Distance = ptrdiff_t,\
				typename Pointer = T*, typename Reference = T&>
	struct iterator
	{
		typedef T											value_type;
		typedef Distance									difference_type;
		typedef Pointer										pointer;
		typedef Reference									reference;
		typedef Category									iterator_category;
	};

	template<class Iterator>
	struct iterator_traits {
		typedef typename Iterator::difference_type			difference_type;
		typedef typename Iterator::value_type				value_type;
		typedef typename Iterator::pointer					pointer;
		typedef typename Iterator::reference				reference;
		typedef typename Iterator::iterator_category		iterator_category;
	};

	template <typename T>
	struct iterator_traits<T*>
	{
		typedef ptrdiff_t									difference_type;
		typedef T											value_type;
		typedef T*											pointer;
		typedef T&											reference;
		typedef random_access_iterator_tag					iterator_category;
	};

	template<class T>
	struct iterator_traits<const T*>
	{
		typedef ptrdiff_t									difference_type;
		typedef T											value_type;
		typedef const T*									pointer;
		typedef const T&									reference;
		typedef random_access_iterator_tag					iterator_category;
	};

	template <typename Iterator>
	typename iterator_traits<Iterator>::difference_type	do_distance(Iterator first, Iterator last, ft::input_iterator_tag) {
		typename iterator_traits<Iterator>::difference_type	result = 0;
		while (first != last) {
			++first;
			++result;
		}
		return result;
	}

	template <typename Iterator>
	typename iterator_traits<Iterator>::difference_type	do_distance(Iterator first, Iterator last, ft::random_access_iterator_tag)  {
		return last - first;
	}

	template <typename Iterator>
	typename iterator_traits<Iterator>::difference_type	do_distance(Iterator first, Iterator last, std::input_iterator_tag) {
		typename iterator_traits<Iterator>::difference_type	result = 0;
		while (first != last) {
			++first;
			++result;
		}
		return result;
	}

	template <typename Iterator>
	typename iterator_traits<Iterator>::difference_type	do_distance(Iterator first, Iterator last, std::random_access_iterator_tag)  {
		return last - first;
	}

	template <typename Iterator>
	typename iterator_traits<Iterator>::difference_type	distance(Iterator first, Iterator last) {
		return do_distance(first, last, typename iterator_traits<Iterator>::iterator_category());
	}

	template <typename Iterator>
	class reverse_iterator
	: public iterator<typename iterator_traits<Iterator>::iterator_category,
						typename iterator_traits<Iterator>::value_type,
						typename iterator_traits<Iterator>::difference_type,
						typename iterator_traits<Iterator>::pointer,
						typename iterator_traits<Iterator>::reference>
	{
	protected:
		Iterator	current;
	public:
		typedef Iterator											iterator_type;
		typedef typename iterator_traits<Iterator>::difference_type	difference_type;
		typedef typename iterator_traits<Iterator>::reference		reference;
		typedef typename iterator_traits<Iterator>::pointer			pointer;

		reverse_iterator() {}
		explicit reverse_iterator(Iterator x) : current(x) {}
		template <typename U>
		reverse_iterator(const reverse_iterator<U>& u)
		: current(u.base()) {}


		Iterator	base() const {
			return current;
		}

		reference	operator*() const {
			Iterator	tmp = current;
			return *(--tmp);
		}

		pointer		operator->() const {
			return &(operator*());
		}

		reverse_iterator& operator++() {
			--current;
			return *this;
		}

		reverse_iterator operator++(int) {
			reverse_iterator	tmp = *this;
			--current;
			return tmp;
		}

		reverse_iterator& operator--() {
			++current;
			return *this;
		}

		reverse_iterator operator--(int) {
			reverse_iterator	tmp = *this;
			++current;
			return tmp;
		}

		reverse_iterator operator+(difference_type n) const {
			return reverse_iterator(current - n);
		}

		reverse_iterator& operator+=(difference_type n) {
			current -= n;
			return *this;
		}

		reverse_iterator operator-(difference_type n) const {
			return reverse_iterator(current + n);
		}
		reverse_iterator& operator-=(difference_type n) {
			current += n;
			return *this;
		}
		reference operator[](difference_type n) const {
			return *(*this + n);
		}
	};

	template <class IteratorL, class IteratorR>
	bool operator==(const reverse_iterator<IteratorL>& x, const reverse_iterator<IteratorR>& y) {
		return x.base() == y.base();
	}

	template <class IteratorL, class IteratorR>
	bool operator<(const reverse_iterator<IteratorL>& x, const reverse_iterator<IteratorR>& y) {
		return x.base() > y.base();
	}

	template <class IteratorL, class IteratorR>
	bool operator!=(const reverse_iterator<IteratorL>& x, const reverse_iterator<IteratorR>& y) {
		return x.base() != y.base();
	}

	template <class IteratorL, class IteratorR>
	bool operator>(const reverse_iterator<IteratorL>& x, const reverse_iterator<IteratorR>& y) {
		return x.base() < y.base();
	}

	template <class IteratorL, class IteratorR>
	bool operator>=(const reverse_iterator<IteratorL>& x, const reverse_iterator<IteratorR>& y) {
		return x.base() <= y.base();
	}

	template <class IteratorL, class IteratorR>
	bool operator<=(const reverse_iterator<IteratorL>& x, const reverse_iterator<IteratorR>& y) {
		return x.base() >= y.base();
	}

	template <class IteratorL, class IteratorR>
	typename reverse_iterator<IteratorL>::difference_type operator-(const reverse_iterator<IteratorL>& x, const reverse_iterator<IteratorR>& y){
		return y.base() - x.base();
	}

	template <class Iterator>
	reverse_iterator<Iterator> operator+(typename reverse_iterator<Iterator>::difference_type n, const reverse_iterator<Iterator>& x) {
		return reverse_iterator<Iterator>(x.base() - n);
	}

} // namespace ft


#endif

