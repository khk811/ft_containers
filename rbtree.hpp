#ifndef RBTREE_H
#define RBTREE_H

#include <memory>
#include <algorithm>
#include "algobase.hpp"
#include "iterator.hpp"

namespace ft
{
	enum rb_tree_color {
		red = false,
		black = true
	};

	struct rb_tree_node_base
	{
		typedef rb_tree_node_base*		base_ptr;

		rb_tree_color				color;
		base_ptr					parent;
		base_ptr					left;
		base_ptr					right;

		static base_ptr	minimum(base_ptr x) {
			while (x->left != 0) {
				x = x->left;
			}
			return x;
		}

		static base_ptr	maximum(base_ptr x) {
			while (x->right != 0) {
				x = x->right;
			}
			return x;
		}
	};

	template <typename Val>
	struct rb_tree_node : public rb_tree_node_base
	{
		typedef rb_tree_node<Val>*	link_type;
		Val							value_field;
	};

	template<typename Val, typename Ref, typename Ptr>
	struct rb_tree_iterator
	{
		typedef rb_tree_node_base::base_ptr						base_ptr;
		typedef std::bidirectional_iterator_tag					iterator_category;
		typedef ptrdiff_t										difference_type;
		typedef Val												value_type;
		typedef Ref												reference;
		typedef Ptr												pointer;
		typedef rb_tree_iterator<Val, Val&, Val*>				iterator;
		typedef rb_tree_iterator<Val, const Val&, const Val*>	const_iterator;
		typedef rb_tree_iterator<Val, Ref, Ptr>					self;
		typedef rb_tree_node<Val>*								link_type;

		base_ptr	node;

		void	increment() {
			if (node->right != 0) {
				node = node->right;
				while (node->left != 0) {
					node = node->left;
				}
			} else {
				base_ptr	y = node->parent;
				while (node == y->right) {
					node = y;
					y = y->parent;
				}
				if (node->right != y) {
					node = y;
				}
			}
		}

		void	decrement() {
			if (node->color == red && node->parent->parent == node) {
				node = node->right;
			} else if (node->left != 0) {
				base_ptr	y = node->left;
				while (y->right != 0) {
					y = y->right;
				}
				node = y;
			} else {
				base_ptr	y = node->parent;
				while (node == y->left) {
					node = y;
					y = y->parent;
				}
				node = y;
			}
 		}

		rb_tree_iterator() {}
		rb_tree_iterator(link_type x) { node = x; }
		rb_tree_iterator(const iterator& it) { node = it.node; }

		reference	operator*() const { return link_type(node)->value_field; }
		pointer		operator->() const { return &(operator*()); }
		self&		operator++() {
			increment();
			return *this;
		}

		self		operator++(int) {
			self	tmp = *this;
			increment();
			return tmp;
		}

		self&		operator--() {
			decrement();
			return *this;
		}

		self		operator--(int) {
			self	tmp = *this;
			decrement();
			return tmp;
		}
	};

	template <typename Val, typename Ref, typename Ptr>
	bool	operator==(const rb_tree_iterator<Val, Ref, Ptr>& x, \
	const rb_tree_iterator<Val, Ref, Ptr>& y) {
		return x.node == y.node;
	}

	template <typename Val>
	bool	operator==(const rb_tree_iterator<Val, const Val&, const Val*>& x, \
	const rb_tree_iterator<Val, Val&, Val*>& y) {
		return x.node == y.node;
	}

	template <typename Val>
	bool	operator==(const rb_tree_iterator<Val, Val&, Val*>& x, \
	const rb_tree_iterator<Val, const Val&, const Val*>& y) {
		return x.node == y.node;
	}

	template <typename Val, typename Ref, typename Ptr>
	bool	operator!=(const rb_tree_iterator<Val, Ref, Ptr>& x, \
	const rb_tree_iterator<Val, Ref, Ptr>& y) {
		return x.node != y.node;
	}

	template <typename Val>
	bool	operator!=(const rb_tree_iterator<Val, const Val&, const Val*>& x, \
	const rb_tree_iterator<Val, Val&, Val*>& y) {
		return x.node != y.node;
	}

	template <typename Val>
	bool	operator!=(const rb_tree_iterator<Val, Val&, Val*>& x, \
	const rb_tree_iterator<Val, const Val&, const Val*>& y) {
		return x.node != y.node;
	}

	void	rb_tree_rotate_left(rb_tree_node_base* x, rb_tree_node_base*& root) {
		rb_tree_node_base*	y = x->right;

		x->right = y->left;
		if (y->left != 0) {
			y->left->parent = x;
		}
		y->parent = x->parent;
		if (x == root) {
			root = y;
		} else if (x == x->parent->left) {
			x->parent->left = y;
		} else {
			x->parent->right = y;
		}
		y->left = x;
		x->parent = y;
	}

	void	rb_tree_rotate_right(rb_tree_node_base* x, rb_tree_node_base*& root) {
		rb_tree_node_base*	y = x->left;

		x->left = y->right;
		if (y->right != 0) {
			y->right->parent = x;
		}
		y->parent = x->parent;
		if (x == root) {
			root = y;
		} else if (x == x->parent->right) {
			x->parent->right = y;
		} else {
			x->parent->left = y;
		}
		y->right = x;
		x->parent = y;
	}

	void	rb_tree_rebalance(rb_tree_node_base* x, rb_tree_node_base*& root) {
		x->color = red;
		while (x != root && x->parent->color == red)
		{
			if (x->parent == x->parent->parent->left) {	// 부모가 조부모의 왼쪽이면
				rb_tree_node_base*	y = x->parent->parent->right;	// 삼촌노드
				if (y && y->color == red) {	// 삼촌이 있고, 그게 red 면
					x->parent->color = black;
					y->color = black;
					x->parent->parent->color = red;
					x = x->parent->parent;
				} else {
					if (x == x->parent->right) {
						x = x->parent;
						rb_tree_rotate_left(x, root);
					}
					x->parent->color = black;
					x->parent->parent->color = red;
					rb_tree_rotate_right(x->parent->parent, root);
				}
			} else {
				rb_tree_node_base*	y = x->parent->parent->left;
				if (y && y->color == red) {	// 삼촌이 있고 그게 red면;
					x->parent->color = black;
					y->color = black;
					x->parent->parent->color = red;
					x = x->parent->parent;
				} else {
					if (x == x->parent->left) {
						x = x->parent;
						rb_tree_rotate_right(x, root);
					}
					x->parent->color = black;
					x->parent->parent->color = red;
					rb_tree_rotate_left(x->parent->parent, root);
				}
			}
		}
		root->color = black;
	}

	rb_tree_node_base*	rb_tree_rebalance_for_erase(rb_tree_node_base* z, \
	rb_tree_node_base*& root, rb_tree_node_base*& leftmost, rb_tree_node_base*& rightmost) {
		rb_tree_node_base*	y = z;
		rb_tree_node_base*	x = 0;	// nullptr;
		rb_tree_node_base*	x_parent = 0;
		if (y->left == 0) {	// z는 최대 1개의 null 자식을 가짐, y == z;
			x = y->right;	// x는 null 일수 있음;
		} else {
			if (y->right == 0) {	// z가 정확히 한개의 null 자식을 가질떄? y == z
				x = y->left;	// x는 null이 아님;
			} else {	// z가 두개의 null이 아닌 자식들을 가지고 있을때;
				y = y->right;	// z가 successor, x는 null일 수 있음;
				while (y->left != 0) {
					y = y->left;
				}
				x = y->right;
			}
		}
		if (y != z) {	// z에 자리에 y를 재연결함, y가 z의 successor (후계자?);
			z->left->parent = y;
			y->left = z->left;
			if (y != z->right) {
				x_parent = y->parent;
				if (x) {
					x->parent = y->parent;
				}
				y->parent->left = x;
				y->right = z->right;
				z->right->parent = y;
			} else {
				x_parent = y;
			}
			if (root == z) {
				root = y;
			} else if (z->parent->left == z) {
				z->parent->left = y;
			} else {
				z->parent->right = y;
			}
			y->parent = z->parent;
			std::swap(y->color, z->color);
			y = z;	// 이제 y는 지워질 노드를 가리키고 있음;
		} else {	// y == z일 경우,
			x_parent = y->parent;
			if (x) {
				x->parent = y->parent;
			}
			if (root == z) {
				root = x;
			} else {
				if (z->parent->left == z) {
					z->parent->left = x;
				} else {
					z->parent->right = x;
				}
			}
			if (leftmost == z) {
				if (z->right == 0) {	// z->left도 null이어야 함?
					leftmost = z->parent;
				// 만약 z == root면, leftmost == header로 만들어줌;
				} else {
					leftmost = rb_tree_node_base::minimum(x);
				}
			}
			if (rightmost == z) {
				if (z->left == 0) {	// z->right도 null이어야 함;
					rightmost = z->parent;
				// 만약 z == root면, rightmost == header로 만들어줌;
				} else {
					rightmost = rb_tree_node_base::maximum(x);
				}
			}
		}
		if (y->color != red) {
			while (x != root && (x == 0 || x->color == black)) {
				if (x == x_parent->left) {
					rb_tree_node_base*	w = x_parent->right;	// 삼촌;
					if (w->color == red) {
						w->color = black;
						x_parent->color = red;
						rb_tree_rotate_left(x_parent, root);
						w = x_parent->right;
					}
					if ((w->left == 0 || w->left->color == black) && \
					(w->right == 0 || w->right->color == black)) {
						w->color = red;
						x = x_parent;
						x_parent = x_parent->parent;
					} else {
						if (w->right == 0 || w->right->color == black) {
							if (w->left) {
								w->left->color = black;
							}
							w->color = red;
							rb_tree_rotate_right(w, root);
							w = x_parent->right;
						}
						w->color = x_parent->color;
						x_parent->color = black;
						if (w->right) {
							w->right->color = black;
						}
						rb_tree_rotate_left(x_parent, root);
						break;
					}
				} else {	// 위랑 같고, right <-> left만 바뀜;
					rb_tree_node_base*	w = x_parent->left;	// 삼촌;
					if (w->color == red) {
						w->color = black;
						x_parent->color = red;
						rb_tree_rotate_right(x_parent, root);
						w = x_parent->left;
					}
					if ((w->right == 0 || w->right->color == black) && \
					(w->left == 0 || w->left->color == black)) {
						w->color = red;
						x = x_parent;
						x_parent = x_parent->parent;
					} else {
						if (w->left == 0 || w->left->color == black) {
							if (w->right) {
								w->right->color = black;
							}
							w->color = red;
							rb_tree_rotate_left(w, root);
							w = x_parent->left;
						}
						w->color = x_parent->color;
						x_parent->color = black;
						if (w->left) {
							w->left->color = black;
						}
						rb_tree_rotate_right(x_parent, root);
						break;
					}
				}
			}
			if (x) {
				x->color = black;
			}
		}
		return y;
	}

	template<typename Key, typename Val, typename KeyOfVal, typename Compare, \
	typename Alloc >
	class rbtree {
	protected:
		typedef rb_tree_node_base*	base_ptr;
		typedef rb_tree_node<Val>	node_type;

	public:
		typedef Key														key_type;
		typedef Val														value_type;
		typedef value_type*												pointer;
		typedef const value_type*										const_pointer;
		typedef value_type&												reference;
		typedef const value_type&										const_reference;
		typedef node_type*												link_type;
		typedef size_t													size_type;
		typedef ptrdiff_t												difference_type;
		typedef Alloc													allocator_type;

		typedef typename Alloc::template rebind<node_type>::other	node_allocator_type;

		typedef rb_tree_iterator<value_type, reference, pointer>				iterator;
		typedef rb_tree_iterator<value_type, const_reference, const_pointer>	const_iterator;
		typedef ft::reverse_iterator<iterator>									reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>							const_reverse_iterator;

	protected:
		allocator_type		data_allocator;
		node_allocator_type	node_allocator;
		link_type			header;
		size_type			node_count;
		Compare				key_compare;

		allocator_type	get_allocator() const {
			return node_allocator;
		}

		link_type	get_node() {
			return node_allocator.allocate(1);
		}

		void		put_node(link_type target) {
			return node_allocator.deallocate(target, 1);
		}

		link_type	create_node(const value_type& x) {
			link_type	tmp = get_node();

			try
			{
				data_allocator.construct(&(tmp->value_field ), x);
			}
			catch(...)
			{
				put_node(tmp);
				throw;
			}
			return tmp;
		}

		link_type	clone_node(link_type x) {
			link_type	tmp = create_node(x->value_field);
			tmp->color = x->color;
			tmp->left = 0;
			tmp->right = 0;
			return tmp;
		}

		void	destroy_node(link_type p) {
			data_allocator.destroy(&(p->value_field));
			put_node(p);
		}

		link_type&	root() const {
			return (link_type&)header->parent;
		}

		link_type&	leftmost() const {
			return (link_type&)header->left;
		}

		link_type&	rightmost() const {
			return (link_type&)header->right;
		}

		static link_type&	s_left(link_type x) {
			return (link_type&)x->left;
		}

		static link_type&	s_left(base_ptr x) {
			return (link_type&)x->left;
		}

		static link_type&	s_right(link_type x) {
			return (link_type&)x->right;
		}

		static link_type&	s_right(base_ptr x) {
			return (link_type&)x->right;
		}

		static link_type&	s_parent(link_type x) {
			return (link_type&)x->parent;
		}

		static link_type&	s_parent(base_ptr x) {
			return (link_type&)x->parent;
		}

		static reference	s_value(link_type x) {
			return x->value_field;
		}

		static reference	s_value(base_ptr x) {
			return ((link_type)x)->value_field;
		}

		static const Key&	s_key(link_type x) {
			return KeyOfVal()(s_value(x));
		}

		static const Key&	s_key(base_ptr x) {
			return KeyOfVal()(s_value(link_type(x)));
		}

		static rb_tree_color&	s_color(link_type x) {
			return x->color;
		}

		static rb_tree_color&	s_color(base_ptr x) {
			return link_type(x)->color;
		}

		static link_type	s_minimum(link_type x) {
			return (link_type)rb_tree_node_base::minimum(x);
		}

		static link_type	s_maximum(link_type x) {
			return (link_type)rb_tree_node_base::maximum(x);
		}

	public:
		// allocator, deallocator;
		rbtree()
		: data_allocator(allocator_type()), node_allocator(node_allocator_type()), \
		header(get_node()), node_count(0), key_compare(){
			s_color(header) = red;
			root() = 0;
			leftmost() = header;
			rightmost() = header;
		}

		rbtree(const Compare& comp)
		: data_allocator(allocator_type()), node_allocator(node_allocator_type()), \
		header(get_node()), node_count(0), key_compare(comp) {
			s_color(header) = red;
			root() = 0;
			leftmost() = header;
			rightmost() = header;
		}

		rbtree(const Compare& comp, const allocator_type& a)
		: data_allocator(a), node_allocator(node_allocator_type()), \
		header(get_node()), node_count(0), key_compare(comp) {
			s_color(header) = red;
			root() = 0;
			leftmost() = header;
			rightmost() = header;
		}

		rbtree(const rbtree<Key, Val, KeyOfVal, Compare, Alloc>& x)
		: data_allocator(x.get_allocator()), node_allocator(node_allocator_type()), \
		header(get_node()), node_count(0), key_compare(x.key_compare) {
			if (x.root() == 0) {
				s_color(header) = red;
				root() = 0;
				leftmost() = header;
				rightmost() = header;
			} else {
				s_color(header) = red;
				root() = rb_copy(x.root(), header);
				leftmost() = s_minimum(root());
				rightmost() = s_maximum(root());
			}
			node_count = x.node_count;
		}

		~rbtree() {
			clear();
			put_node(header);
		}

		rbtree<Key, Val, KeyOfVal, Compare, Alloc>&	operator=(const rbtree<Key, Val, KeyOfVal, Compare, Alloc>& x) {
			// copy assignment operator;
			if (this != &x) {
				clear();
				node_count = 0;
				key_compare = x.key_compare;
				if (x.root() == 0) {
					root() = 0;
					leftmost() = header;
					rightmost() = header;
				} else {
					root() = rb_copy(x.root(), header);
					leftmost() = s_minimum(root());
					rightmost() = s_maximum(root());
					node_count = x.node_count;
				}
			}
			return *this;
		}

		// Accessors;
		Compare	key_comp() const {
			return key_compare;
		}

		iterator	begin() {
			return leftmost();
		}

		const_iterator	begin() const {
			return leftmost();
		}

		iterator	end() {
			return header;
		}

		const_iterator	end() const {
			return header;
		}

		reverse_iterator	rbegin() {
			return reverse_iterator(end());
		}

		const_reverse_iterator	rbegin() const {
			return const_reverse_iterator(end());
		}

		reverse_iterator	rend() {
			return reverse_iterator(begin());
		}

		const_reverse_iterator	rend() const {
			return const_reverse_iterator(begin());
		}

		bool	empty() const {
			return (node_count == 0);
		}

		size_type	size() const {
			return node_count;
		}

		size_type	max_size() const {
			// return node_allocator.max_size();
			std::cout << "\n===" << std::endl;
			std::cout << "max_size of node_allocator: " << (size_type)node_allocator.max_size() << std::endl;
			std::cout << "max_size of allocator: " << (size_type)data_allocator.max_size() << std::endl;
			std::cout << "\n===\n" << std::endl;
			return std::min<size_type>(node_allocator.max_size(), std::numeric_limits<difference_type>::max());
			// return std::numeric_limits<difference_type>::max();
			// return std::min<size_type>(std::numeric_limits<size_type>::max() / sizeof(rb_tree_node), \
			// std::numeric_limits<difference_type>::max());
			// return size_type(-1);
		}

		void	swap(rbtree<Key, Val, KeyOfVal, Compare, Alloc>& t) {
			std::swap(header, t.header);
			std::swap(node_count, t.node_count);
			std::swap(key_compare, t.key_compare);
		}

		// Insert/Erase;
		// 중복 허용을 하지 않으니 insert_, erase_unique만 작성함;
		ft::pair<iterator,bool>	insert(const value_type& v) {
			link_type	y = header;
			link_type	x = root();
			bool		comp = true;
			while (x != 0) {
				y = x;
				comp = key_compare(KeyOfVal()(v), s_key(x));
				if (comp) {
					x = s_left(x);
				} else {
					x = s_right(x);
				}
			}
			iterator	j = iterator(y);
			if (comp) {
				if (j == begin()) {
					return ft::pair<iterator, bool>(rb_insert(x, y, v), true);
				} else {
					--j;
				}
			}
			if (key_compare(s_key(j.node), KeyOfVal()(v))) {
				return ft::pair<iterator, bool>(rb_insert(x, y, v), true);
			}
			return ft::pair<iterator, bool>(j, false);
		}

		iterator	insert(iterator position, const value_type& v) {
			if (position.node == header->left) {
				if (size() > 0 && key_compare(KeyOfVal()(v), s_key(position.node))) {
					return rb_insert(position.node, position.node, v);
				} else {
					return insert(v).first;
				}
			} else if (position.node == header) {
				if (key_compare(s_key(rightmost()), KeyOfVal()(v))) {
					return rb_insert(0, rightmost(), v);
				} else {
					return insert(v).first;
				}
			} else {
				iterator	before = position;
				--before;
				if (key_compare(s_key(before.node), KeyOfVal()(v)) && \
				key_compare(KeyOfVal()(v), s_key(position.node))) {
					if (s_right(before.node) == 0) {
						return rb_insert(0, before.node, v);
					} else {
						return rb_insert(position.node, position.node, v);
					}
				} else {
					return insert(v).first;
				}
			}
		}

		template<typename InputIterator>
		void		insert(InputIterator first, InputIterator last) {
			for (; first != last; ++first) {
				insert(*first);
			}
		}

		void		erase(iterator position) {
			link_type	y = \
			(link_type)rb_tree_rebalance_for_erase(position.node, \
			header->parent, header->left, header->right);
			destroy_node(y);
			--node_count;
		}

		size_type	erase(const key_type& x) {
			ft::pair<iterator, iterator>	p = equal_range(x);
			size_type	n = ft::distance(p.first, p.second);
			erase(p.first, p.second);
			return n;
		}

		void		erase(iterator first, iterator last) {
			if (first == begin() && last == end()) {
				clear();
			} else {
				while (first != last) {
					erase(first++);
				}
			}
		}

		void		erase(const key_type* first, const key_type* last) {
			while (first != last) {
				erase(*(first)++);
			}
		}

		void		clear() {
			if (node_count != 0) {
				rb_erase(root());
				leftmost() = header;
				root() = 0;
				rightmost() = header;
				node_count = 0;
			}
		}

		// Set operation;
		iterator		find(const key_type& k) {
			link_type	y = header;
			link_type	x = root();

			while (x != 0) {
				if (!key_compare(s_key(x), k)) {
					y = x;
					x = s_left(x);
				} else {
					x = s_right(x);
				}
			}
			iterator	j = iterator(y);
			if (j == end() || key_compare(k, s_key(j.node))) {
				return end();
			} else {
				return j;
			}
		}

		const_iterator	find(const key_type& k) const {
			link_type	y = header;
			link_type	x = root();

			while (x != 0) {
				if (!key_compare(s_key(x), k)) {
					y = x;
					x = s_left(x);
				} else {
					x = s_right(x);
				}
			}
			const_iterator	j = const_iterator(y);
			if (j == end() || key_compare(k, s_key(j.node))) {
				return end();
			} else {
				return j;
			}
		}

		size_type		count(const key_type& k) const {
			ft::pair<const_iterator, const_iterator>	p = equal_range(k);
			size_type	n = ft::distance(p.first, p.second);
			return n;
		}

		iterator		lower_bound(const key_type& k) {
			link_type	y = header;
			link_type 	x = root();

			while (x != 0) {
				if (!(key_compare(s_key(x), k))) {
					y = x;
					x = s_left(x);
				} else {
					x = s_right(x);
				}
			}
			return iterator(y);
		}

		const_iterator	lower_bound(const key_type& k) const {
			link_type	y = header;
			link_type 	x = root();

			while (x != 0) {
				if (!(key_compare(s_key(x), k))) {
					y = x;
					x = s_left(x);
				} else {
					x = s_right(x);
				}
			}
			return const_iterator(y);
		}

		iterator		upper_bound(const key_type& k) {
			link_type	y = header;
			link_type 	x = root();

			while (x != 0) {
				if (key_compare(k, s_key(x))) {
					y = x;
					x = s_left(x);
				} else {
					x = s_right(x);
				}
			}
			return iterator(y);

		}
		const_iterator	upper_bound(const key_type& k) const {
			link_type	y = header;
			link_type 	x = root();

			while (x != 0) {
				if (key_compare(k, s_key(x))) {
					y = x;
					x = s_left(x);
				} else {
					x = s_right(x);
				}
			}
			return const_iterator(y);
		}

		ft::pair<iterator, iterator>	equal_range(const key_type& k) {
			return ft::pair<iterator, iterator>(lower_bound(k), upper_bound(k));
		}

		ft::pair<const_iterator, const_iterator> equal_range(const key_type& k) const {
			return ft::pair<const_iterator, const_iterator>(lower_bound(k), upper_bound(k));
		}


		private:
		// 멤버 함수 내부적으로 이용하는 insert, copy, erase 함수
		// _M_ prefix가 있었지만 rb_로 작성함;

		iterator	rb_insert(base_ptr x_ptr, base_ptr y_ptr, const value_type& to_insert) {
			link_type	x = (link_type)x_ptr;
			link_type	y = (link_type)y_ptr;
			link_type	z;	// new node (to_insert)?

			if (y == header || x != 0 || key_compare(KeyOfVal()(to_insert), s_key(y))) {
				z = create_node(to_insert);
				s_left(y) = z;
				if (y == header) {
					root() = z;
					rightmost() = z;
				} else if (y == leftmost()) {
					leftmost() = z;
				}
			} else {
				z = create_node(to_insert);
				s_right(y) = z;
				if (y == rightmost()) {
					rightmost() = z;
				}
			}
			s_parent(z) = y;
			s_left(z) = 0;
			s_right(z) = 0;
			rb_tree_rebalance(z, header->parent);
			++node_count;
			return iterator(z);
		}

		link_type	rb_copy(link_type x, link_type p) {
			link_type	top = clone_node(x);
			top->parent = p;

			try
			{
				if (x->right) {
					top->right = rb_copy(s_right(x), top);
				}
				p = top;
				x = s_left(x);
				while (x != 0)
				{
					link_type	y = clone_node(x);
					p->left = y;
					y->parent = p;
					if (x->right) {
						y->right = rb_copy(s_right(x), y);
					}
					p = y;
					x = s_left(x);
				}
			}
			catch(...)
			{
				rb_erase(top);
				throw;
			}
			return top;
		}

		void		rb_erase(link_type x) {
			while (x != 0) {
				rb_erase(s_right(x));
				link_type	y = s_left(x);
				destroy_node(x);
				x = y;
			}
		}
	};

	// Operator Overloading;
	template <typename _Key, typename _Val, typename _KeyOfVal, typename _Compare, typename _Alloc>
	bool	operator==(const rbtree<_Key, _Val, _KeyOfVal, _Compare, _Alloc>& x, \
	const rbtree<_Key, _Val, _KeyOfVal, _Compare, _Alloc>& y) {
		return x.size() == y.size() && ft::equal(x.begin(), x.end(), y.begin());
	}

	template <typename _Key, typename _Val, typename _KeyOfVal, typename _Compare, typename _Alloc>
	bool	operator<(const rbtree<_Key, _Val, _KeyOfVal, _Compare, _Alloc>& x, \
	const rbtree<_Key, _Val, _KeyOfVal, _Compare, _Alloc>& y) {
		return ft::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
	}

	template <typename _Key, typename _Val, typename _KeyOfVal, typename _Compare, typename _Alloc>
	bool	operator!=(const rbtree<_Key, _Val, _KeyOfVal, _Compare, _Alloc>& x, \
	const rbtree<_Key, _Val, _KeyOfVal, _Compare, _Alloc>& y) {
		return !(x == y);
	}

	template <typename _Key, typename _Val, typename _KeyOfVal, typename _Compare, typename _Alloc>
	bool	operator>(const rbtree<_Key, _Val, _KeyOfVal, _Compare, _Alloc>& x, \
	const rbtree<_Key, _Val, _KeyOfVal, _Compare, _Alloc>& y) {
		return y < x;
	}

	template <typename _Key, typename _Val, typename _KeyOfVal, typename _Compare, typename _Alloc>
	bool	operator<=(const rbtree<_Key, _Val, _KeyOfVal, _Compare, _Alloc>& x, \
	const rbtree<_Key, _Val, _KeyOfVal, _Compare, _Alloc>& y) {
		return !(y < x);
	}
	template <typename _Key, typename _Val, typename _KeyOfVal, typename _Compare, typename _Alloc>
	bool	operator>=(const rbtree<_Key, _Val, _KeyOfVal, _Compare, _Alloc>& x, \
	const rbtree<_Key, _Val, _KeyOfVal, _Compare, _Alloc>& y) {
		return!(x < y);
	}

	template <typename _Key, typename _Val, typename _KeyOfVal, typename _Compare, typename _Alloc>
	void	swap(const rbtree<_Key, _Val, _KeyOfVal, _Compare, _Alloc>& x, \
	const rbtree<_Key, _Val, _KeyOfVal, _Compare, _Alloc>& y) {
		x.swap(y);
	}

} // namespace ft


#endif

