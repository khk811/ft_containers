#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <memory>
#include "iterator"

namespace ft
{
	template <class T, class Allocator = std::allocator<T> > class vector {
	public:
	// types:
	typedef typename Allocator::reference				reference;
	typedef typename Allocator::const_reference 		const_reference;
	typedef pointer										iterator;
	typedef const_pointer								const_iterator;
	typedef std::size_t									size_type;
	typedef std::ptrdiff_t								difference_type;
	typedef T											value_type;
	typedef Allocator									allocator_type;
	typedef typename Allocator::pointer					pointer;
	typedef typename Allocator::const_pointer			const_pointer;
	typedef ft::reverse_iterator<iterator>				reverse_iterator;
	typedef ft::reverse_iterator<const_iterator>		const_reverse_iterator;



	}
} // namespace ft


#endif
