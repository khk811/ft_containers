#include "vector.hpp"
#include <vector>
#include "set.hpp"
#include <set>
#include "iostream"

void	testVector(void) {
	ft::vector<int>	test(12, 42);

	std::cout << "instance test size: " << test.size() << std::endl;
	test[0] = 42;
	std::cout << "test[0]: " << test[0] << std::endl;
	std::cout << "test[1]: " << test[1] << std::endl;
	std::cout << "test[2]: " << test[2] << std::endl;
	std::cout << *(test.begin()) << std::endl;
	std::cout << *(test.end()) << std::endl;
}

template<typename T>
void	printSetWithIterator(ft::set<T> print_target) {
	typename ft::set<T>::iterator	first = print_target.begin();
	typename ft::set<T>::iterator	last = print_target.end();
	size_t	safety = 0;
	size_t	size = print_target.size();

	std::cout << "curr size: " << size << std::endl;
	for (; first != last; first++)
	{
		std::cout << "iterator[" << safety++ << "]: " << *(first) << std::endl;
		if (safety > size) {
			std::cout << "Error: Overflow" << std::endl;
			break ;
		}
	}
}

template<typename T>
void	printSetWithReverseIterator(ft::set<T> print_target) {
	typename ft::set<T>::reverse_iterator	first = print_target.rbegin();
	typename ft::set<T>::reverse_iterator	last = print_target.rend();
	size_t	safety = 0;
	size_t	size = print_target.size();

	std::cout << "curr size: " << size << std::endl;
	for (; first != last; first++)
	{
		std::cout << "reverse_iterator[" << safety++ << "]: " << *(first) << std::endl;
		if (safety > size) {
			std::cout << "Error: Overflow" << std::endl;
			break ;
		}
	}
}

void	testSet(void){
	// 이터레이터 순환 테스트 해야함;
	ft::vector<int>	input(12, 42);
	std::cout << "input size() : " << input.size() << std::endl;
	ft::set<int>	test(input.begin(), input.end());
	ft::set<int>	i_cant;

	i_cant.insert(1);
	i_cant.insert(2);
	i_cant.insert(3);
	i_cant.insert(4);
	i_cant.insert(5);

	printSetWithIterator(i_cant);
	ft::set<int>::iterator b = i_cant.begin();
	ft::set<int>::iterator e = i_cant.end();
	e--;
	int	i = 0;
	for(; e != b; e--) {
		std::cout << *(e) << std::endl;
		i++;
		if (e.node->left) {
			std::cout << "L exist" << std::endl;
		}
		if (i == 10) {
			break;
		}
	}
	printSetWithReverseIterator(i_cant);
	printSetWithIterator(test);
	printSetWithReverseIterator(test);

	std::cout << "\n===\n" << std::endl;
	std::set<int>	standard;
	standard.insert(1);
	standard.insert(2);
	standard.insert(3);
	standard.insert(4);
	standard.insert(5);
	std::cout << "stl max_size: " << standard.max_size() << std::endl;
	std::cout << "ft_set max_size: " << test.max_size() << std::endl;
	std::cout << "\n===\n" << std::endl;
}

int	main(void) {
	// testVector();
	testSet();
	std::cout << "\n\n===Check Memory leaks===\n" << std::endl;
	system("leaks ft_containers");
	return 0;
}
