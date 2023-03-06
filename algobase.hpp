#ifndef ALGOBASE_HPP
#define ALGOBASE_HPP

namespace ft
{
	template <class InputIterator1, class InputIterator2>
	bool	equal ( InputIterator1 first1, InputIterator1 last1, InputIterator2 first2 )
	{
		while (first1 != last1) {
			if (!(*first1 == *first2)) {
				return false;
			}
			++first1; ++first2;
		}
		return true;
	}

	template <class InputIterator1, class InputIterator2>
	bool	lexicographical_compare (InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2)
	{
		while (first1 != last1)
		{
			if (first2 == last2 || *first2 < *first1) {
				return false;
			} else if (*first1 < *first2) {
				return true;
			}
			++first1; ++first2;
		}
		return (first2 != last2);
	}

	template <class T1, class T2>
	struct	pair
	{
		typedef T1	first_type;
		typedef T2	second_type;

		T1	first;
		T2	second;

		pair() : first(), second() {}

		pair( const T1& x, const T2& y ) : first(x), second(y) {}

		template<class U, class V>
		pair (const pair<U,V>& pr) : first(pr.first), second(pr.second) {}

		pair& operator= (const pair& pr) {
			first = pr.first;
			second = pr.second;
			return *this;
		}
	};

	template <class T1, class T2>
	bool operator==( const ft::pair<T1, T2>& lhs, const ft::pair<T1, T2>& rhs ) {
		return (lhs.first == rhs.first && lhs.second == rhs.second);
	}

	template<class T1, class T2 >
	bool operator!=( const ft::pair<T1, T2>& lhs, const ft::pair<T1, T2>& rhs ) {
		return !(lhs == rhs);
	}

	template<class T1, class T2 >
	bool operator<( const ft::pair<T1, T2>& lhs, const ft::pair<T1, T2>& rhs ) {
		return lhs.first < rhs.first || (!(rhs.first < lhs.first) && lhs.second < rhs.second);
	}

	template<class T1, class T2 >
	bool operator<=( const ft::pair<T1, T2>& lhs, const ft::pair<T1, T2>& rhs ) {
		return !(rhs < lhs);
	}

	template<class T1, class T2 >
	bool operator>(const ft::pair<T1, T2>& lhs, const ft::pair<T1, T2>& rhs ) {
		return rhs < lhs;
	}

	template<class T1, class T2 >
	bool operator>=( const ft::pair<T1, T2>& lhs, const ft::pair<T1, T2>& rhs ) {
		return !(lhs < rhs);
	}

	template <class T1,class T2>
	pair<T1,T2> make_pair (T1 x, T2 y)
	{
	return ( pair<T1,T2>(x,y) );
	}

}

#endif

