#ifndef SET_HPP
#define SET_HPP

#include "rbtree.hpp"

namespace ft
{

template<class T>
struct Identity
{
	typedef T	argument_type;
	typedef T	result_type;
	argument_type&	operator()(argument_type& x) const {
		return x;
	}
	const argument_type&	operator() (const argument_type& x) const {
		return x;
	}
};


template<class Key, class Compare = std::less<Key>, class Alloc = std::allocator<Key> >
class set;

template<class Key, class Compare, class Alloc>
bool	operator==(const set<Key, Compare, Alloc>& x, \
const set<Key, Compare, Alloc>& y);

template<class Key, class Compare, class Alloc>
bool	operator<(const set<Key, Compare, Alloc>& x, \
const set<Key, Compare, Alloc>& y);

template<class Key, class Compare, class Alloc>
class set {
public:
	typedef Key				key_type;
	typedef Key				value_type;
	typedef Compare			key_compare;
	typedef Compare			value_compare;

private:
	typedef ft::Identity<value_type>	key_of_value;
	typedef rbtree<key_type, value_type, key_of_value, key_compare, Alloc>	rbtree_type;
	rbtree_type	tree;

public:
	typedef typename rbtree_type::const_pointer				pointer;
	typedef typename rbtree_type::const_pointer				const_pointer;
	typedef typename rbtree_type::const_reference			reference;
	typedef typename rbtree_type::const_reference			const_reference;
	typedef typename rbtree_type::const_iterator			iterator;
	typedef typename rbtree_type::const_iterator			const_iterator;
	typedef typename rbtree_type::const_reverse_iterator	reverse_iterator;
	typedef typename rbtree_type::const_reverse_iterator	const_reverse_iterator;
	typedef typename rbtree_type::size_type					size_type;
	typedef typename rbtree_type::difference_type			difference_type;
	typedef typename rbtree_type::allocator_type			allocator_type;

	explicit set(const Compare& comp = key_compare(), const allocator_type& a = allocator_type())
	: tree(comp, a) {}

	template<class InputIterator>
	set(InputIterator first, InputIterator last, \
	const Compare& comp = key_compare(), const allocator_type& a = allocator_type())
	: tree(comp, a) {
		tree.insert(first, last);
	}

	set(const set<Key, Compare, Alloc>& x)
	: tree(x.tree) {}

	~set() {
		clear();
	}

	set<Key, Compare, Alloc>& operator=(const set<Key, Compare, Alloc>& x) {
		tree = x.tree;
		return *this;
	}

	key_compare	key_comp() const {
		return tree.key_comp();
	}

	value_compare	value_comp() const {
		return tree.key_comp();
	}

	allocator_type	get_allocator() const {
		return tree.get_allocator();
	}

	// accessor;
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

	bool				empty() const {
		return tree.empty();
	}

	size_type			size() const {
		return tree.size();
	}

	size_type			max_size() const {
		return tree.max_size();
	}

	void				swap(set<Key, Compare, Alloc>& x) {
		tree.swap(x.tree);
	}

	// insert / erase;
	ft::pair<iterator, bool>	insert(const value_type& x) {
		ft::pair<typename rbtree_type::iterator, bool>	p = tree.insert(x);
		return ft::pair<iterator, bool>(p.first, p.second);
	}

	iterator	insert(iterator position, const value_type& x) {
		typedef typename rbtree_type::iterator	rbtree_iterator;
		return tree.insert((rbtree_iterator&)position, x);
	}

	template<class InputIterator>
	void		insert(InputIterator first, InputIterator last) {
		tree.insert(first, last);
	}

	void		erase(iterator position) {
		typedef typename rbtree_type::iterator	rbtree_iterator;
		tree.erase((rbtree_iterator&)position);
	}

	size_type	erase(const key_type& x) {
		return tree.erase(x);
	}

	void		erase(iterator first, iterator last) {
		typedef typename rbtree_type::iterator	rbtree_iterator;
		tree.erase((rbtree_iterator&)first, (rbtree_iterator&)last);
	}

	void		clear() {
		tree.clear();
	}

	iterator	find(const key_type& x) const {
		return tree.find(x);
	}

	size_type	count(const key_type& x) const {
		if (tree.find(x) == tree.end()) {
			return 0;
		} else {
			return 1;
		}
	}

	iterator	lower_bound(const key_type& x) const {
		return tree.lower_bound(x);
	}
	iterator	upper_bound(const key_type& x) const {
		return tree.upper_bound(x);
	}

	pair<iterator, iterator>	equal_range(const key_type& x) const {
		return tree.equal_range(x);
	}

	template<class K1, class C1, class A1>
	friend bool operator==(const set<K1, C1, A1>& x, const set<K1, C1, A1>& y);
	template<class K1, class C1, class A1>
	friend bool operator<(const set<K1, C1, A1>& x, const set<K1, C1, A1>& y);
};

template<class Key, class Compare, class Alloc>
bool	operator==(const set<Key, Compare, Alloc>& x, const set<Key, Compare, Alloc>& y) {
	return x.tree == y.tree;
}

template<class Key, class Compare, class Alloc>
bool	operator<(const set<Key, Compare, Alloc>& x, const set<Key, Compare, Alloc>& y) {
	return x.tree < y.tree;
}

template<class Key, class Compare, class Alloc>
bool	operator!=(const set<Key, Compare, Alloc>& x, const set<Key, Compare, Alloc>& y) {
	return !(x == y);
}

template<class Key, class Compare, class Alloc>
bool	operator>(const set<Key, Compare, Alloc>& x, const set<Key, Compare, Alloc>& y) {
	return y < x;
}

template<class Key, class Compare, class Alloc>
bool	operator<=(const set<Key, Compare, Alloc>& x, const set<Key, Compare, Alloc>& y) {
	return !(y < x);
}

template<class Key, class Compare, class Alloc>
bool	operator>=(const set<Key, Compare, Alloc>& x, const set<Key, Compare, Alloc>& y) {
	return !(x < y);
}

template<class Key, class Compare, class Alloc>
void	swap(const set<Key, Compare, Alloc>& x, const set<Key, Compare, Alloc>& y) {
	x.swap(y);
}

}


#endif

