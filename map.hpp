#ifndef MAP_HPP
#define MAP_HPP

#include "rbtree.hpp"

namespace ft
{

template<class Pair>
struct SelectFirst
{
	typedef Pair						argument_type;
	typedef typename Pair::first_type	result_type;

	typename Pair::first_type&	operator()(Pair& x) const {
		return x.first;
	}

	const typename Pair::first_type&	operator()(const Pair& x) const {
		return x.first;
	}
};


template <class Key, class T, class Compare = std::less<Key>, \
typename Alloc = std::allocator<ft::pair<const Key, T> > >
class map
{
public:
	typedef Key						key_type;
	typedef T						data_type;
	typedef T						mapped_type;
	typedef ft::pair<const Key, T>	value_type;
	typedef Compare					key_compare;

private:
	typedef ft::rbtree<key_type, value_type, \
	SelectFirst<value_type>, key_compare, Alloc>	rbtree_type;
	rbtree_type	tree;
public:
	typedef typename rbtree_type::pointer					pointer;
	typedef typename rbtree_type::const_pointer				const_pointer;
	typedef typename rbtree_type::reference					reference;
	typedef typename rbtree_type::const_reference			const_reference;
	typedef typename rbtree_type::iterator					iterator;
	typedef typename rbtree_type::const_iterator			const_iterator;
	typedef typename rbtree_type::reverse_iterator			reverse_iterator;
	typedef typename rbtree_type::const_reverse_iterator	const_reverse_iterator;
	typedef typename rbtree_type::size_type					size_type;
	typedef typename rbtree_type::difference_type			difference_type;
	typedef typename rbtree_type::allocator_type			allocator_type;

	class value_compare {
		friend class map<Key, T, Compare, Alloc>;
		protected:
			Compare	comp;
			value_compare(Compare c)
			: comp(c) {}
		public:
			typedef value_type	first_argument_type;
			typedef value_type	second_argument_type;
			typedef bool		result_type;

			bool	operator()(const value_type& x, const value_type& y) const {
				return comp(x.first, y.first);
			}
	};

	// Construct, Destruct, Copy Assignment Operator Overload
	explicit map(const Compare& comp = Compare(), const allocator_type& a = allocator_type())
	: tree(comp, a) {}

	template<class InputIterator>
	map(InputIterator first, InputIterator last, const Compare& comp = Compare(), \
	const allocator_type& a = allocator_type())
	: tree(comp, a) {
		tree.insert(first, last);
	}

	map(const map& x)
	: tree(x.tree) {}

	map<Key, T, Compare, Alloc>&	operator=(const map<Key, T, Compare, Alloc>& x) {
		tree = x.tree;
		return *this;
	}

	~map() {}

	// Accessors

	key_compare	key_comp() const {
		return tree.key_comp();
	}

	value_compare	value_comp() const {
		return value_compare(tree.key_comp());
	}

	allocator_type	get_allocator() const {
		return tree.get_allocator();
	}

	iterator	begin() {
		return tree.begin();
	}

	const_iterator	begin() const {
		return tree.begin();
	}

	iterator	end() {
		return tree.end();
	}

	const_iterator	end() const {
		return tree.end();
	}

	reverse_iterator	rbegin() {
		return tree.rbegin();
	}

	const_reverse_iterator	rbegin() const {
		return tree.rbegin();
	}

	reverse_iterator	rend() {
		return tree.rend();
	}

	const_reverse_iterator	rend() const {
		return tree.rend();
	}

	bool	empty() const {
		return tree.empty();
	}

	size_type	size() const {
		return tree.size();
	}

	size_type	max_size() const {
		return tree.max_size();
	}

	T&	operator[](const key_type& k) {
		iterator	i = lower_bound(k);
		if (i == end() || key_comp()(k, (*i).first)) {
			i = insert(i, value_type(k, T()));
		}
		return (*i).second;
	}

	void	swap(map<Key, T, Compare, Alloc>& x) {
		tree.swap(x.tree);
	}

	// insert / erase
	ft::pair<iterator, bool>	insert(const value_type& x) {
		return tree.insert(x);
	}

	iterator	insert(iterator position, const value_type& x) {
		return tree.insert(position, x);
	}

	template<class InputIterator>
	void	insert(InputIterator first, InputIterator last) {
		tree.insert(first, last);
	}

	void	erase(iterator position) {
		tree.erase(position);
	}

	size_type	erase(const key_type& x) {
		return tree.erase(x);
	}

	void	erase(iterator first, iterator last) {
		tree.erase(first, last);
	}

	void	clear() {
		tree.clear();
	}

	iterator	find(const key_type& x) {
		return tree.find(x);
	}

	const_iterator	find(const key_type& x) const {
		return tree.find(x);
	}

	size_type	count(const key_type& x) const {
		if (tree.find(x) == tree.end()) {
			return 0;
		} else {
			return 1;
		}
	}

	iterator	lower_bound(const key_type& x) {
		return tree.lower_bound(x);
	}

	const_iterator	lower_bound(const key_type& x) const {
		return tree.lower_bound(x);
	}

	iterator	upper_bound(const key_type& x) {
		return tree.upper_bound(x);
	}

	const_iterator	upper_bound(const key_type& x) const {
		return tree.upper_bound(x);
	}

	ft::pair<iterator, iterator>	equal_range(const key_type& x) {
		return tree.equal_range(x);
	}

	ft::pair<const_iterator, const_iterator>	equal_range(const key_type& x) const {
		return tree.equal_range(x);
	}

	template<class K1, class T1, class C1, class A1>
	friend bool	operator==(const map<K1, T1, C1, A1>&, const map<K1, T1, C1, A1>&);
	template<class K1, class T1, class C1, class A1>
	friend bool	operator<(const map<K1, T1, C1, A1>&, const map<K1, T1, C1, A1>&);
};

template<class Key, class T, class Compare, class Alloc>
bool	operator==(const map<Key, T, Compare, Alloc>& x,
const map<Key, T, Compare, Alloc>& y) {
	return x.tree == y.tree;
}

template<class Key, class T, class Compare, class Alloc>
bool	operator<(const map<Key, T, Compare, Alloc>& x, \
const map<Key, T, Compare, Alloc>& y) {
	return x.tree < y.tree;
}

template<class Key, class T, class Compare, class Alloc>
bool	operator!=(const map<Key, T, Compare, Alloc>& x, \
const map<Key, T, Compare, Alloc>& y) {
	return !(x == y);
}

template<class Key, class T, class Compare, class Alloc>
bool	operator>(const map<Key, T, Compare, Alloc>& x, \
const map<Key, T, Compare, Alloc>& y) {
	return y < x;
}

template<class Key, class T, class Compare, class Alloc>
bool	operator<=(const map<Key, T, Compare, Alloc>&x, \
const map<Key, T, Compare, Alloc>& y) {
	return !(y < x);
}

template<class Key, class T, class Compare, class Alloc>
bool	operator>=(const map<Key, T, Compare, Alloc>& x, \
const map<Key, T, Compare, Alloc>& y) {
	return !(x < y);
}

template<class Key, class T, class Compare, class Alloc>
void	swap(map<Key, T, Compare, Alloc>& x, map<Key, T, Compare, Alloc>& y) {
	x.swap(y);
}

}

#endif
