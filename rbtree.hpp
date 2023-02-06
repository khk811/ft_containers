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
		typedef bidirectional_iterator_tag						iterator_tag;
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
					node = y;
				}
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
		rb_tree_iterator(Link_type x) { node = x; }
		rb_tree_iterator(const iterator& it) { node = it.node; }

		reference	operator*() const { return Link_type(node)->value_field; }
		pointer		operator->() const { return &(operator*()); }
		Self&		operator++() {
			increment();
			return *this;
		}

		Self		operator++(int) {
			Self	tmp = *this;
			increment();
			return tmp;
		}

		Self&		operator--() {
			decrement();
			return *this;
		}

		Self		operator--(int) {
			Self	tmp = *this;
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
	const rb_tree_iterator<Val, const Val&, const Val*>& y) {
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
	const rb_tree_iterator<Val, const Val&, const Val*>& y) {
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
					x = y->right;
				}
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
				root = z;
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
						rb_tree_rotate_left(x_parent, root);
						break;
					}
				}
			}
		}
	}

	template<typename Key, typename Val, typename KeyOfVal, typename Compare, \
	typename Alloc = std::allocator<Val> >
	class rbtree {
	protected:
		typedef rb_tree_node_base*	base_ptr;
		typedef rb_tree_node<Val>	rb_tree_node;

	public:
		typedef Key														key_type;
		typedef Val														value_type;
		typedef value_type*												pointer;
		typedef const value_type*										const_pointer;
		typedef value_type&												reference;
		typedef const value_type&										const_reference;
		typedef rb_tree_node*											link_type;
		typedef size_t													size_type;
		typedef ptrdiff_t												difference_type;
		typedef Alloc													allocator_type;

		typedef typename Alloc::template rebind<rb_tree_node>::other	node_allocator_type;

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

		allocator_type	get_allocator() {
			return allocator_type();
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
				data_allocator.construct(&(tmp->value_field), x);
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
			tmp->left = x->left;
			tmp->right = x->right;
			return tmp;
		}

		void	destroy_node(link_type p) {
			data_allocator.destory(&(p->value_field));
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
			return (link_type)x->color;
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

		rbtree(const rbtree<key, val, key_of_val, compare, alloc>& x)
		: data_allocator(x.get_allocator()), node_allocator(node_allocator_type()), \
		header(get_node()), node_count(0), key_compare(x.key_compare) {
			if (x.root() == 0) {
				s_color(header) = red;
				root() = 0;
				leftmost() = header;
				rightmost() = header;
			} else {
				s_color(header) = red;
				// root() = rb_copy(x.root(), header);
				leftmost() = s_minimum(root());
				rightmost() = s_maximum(root());
			}
		}

		~rbtree() {
			clear();
		}
	};

} // namespace ft


#endif

