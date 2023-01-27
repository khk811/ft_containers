#ifndef RBTREE_H
#define RBTREE_H


#include "algobase.hpp"
#include "iterator.hpp"

namespace ft
{
	enum rb_tree_color {
		red = false,
		black = true
	};

	template <typename Val>
	struct rb_tree_node
	{
		typedef rb_tree_node*		base_ptr;
		typedef rb_tree_node<Val>*	link_type;

		Val							value_field;
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

	struct rb_tree_base_iterator
	{
		typedef rb_tree_node::base_ptr		base_ptr;
		typedef bidirectional_iterator_tag	iterator_tag;
		typedef ptrdiff_t					difference_type;

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
	};

	template<typename Val, typename Ref, typename Ptr>
	struct rb_tree_iterator : public rb_tree_base_iterator
	{
		typedef Val												value_type;
		typedef Ref												reference;
		typedef Ptr												pointer;
		typedef rb_tree_iterator<Val, Val&, Val*>				iterator;
		typedef rb_tree_iterator<Val, const Val&, const Val*>	const_iterator;
		typedef rb_tree_iterator<Val, Ref, Ptr>					Self;
		typedef rb_tree_iterator<Val>*							Link_type;

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

	void	rb_tree_rotate_left(rb_tree_node* x, rb_tree_node* root) {
		rb_tree_node*	y = x->right;
		x->right = y->left;
		if (y->left != 0) {
			y->left->parent = x;
			y->parent = x->parent;
		}
		if (x == root) {
			root = y;
		} else if (x == x->parent->left) {
			x->parent->left = y;
		} else {
			x->parent->right = y;
		}
		x->parent->right = y;
		y->left = x;
		x->parent = y;
	}

	// note: gcc mirror rbtree 분석이 먼저 필요함;

} // namespace ft


#endif

