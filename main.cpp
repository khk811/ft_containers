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

void	testSet(void){
	// 이터레이터 순환 테스트 해야함;
	ft::vector<int>	input(12, 42);
	std::cout << "curr vector size(): " << input.size() << std::endl;
	ft::set<int>	test(input.begin(), input.end());

	ft::set<int>::iterator	it_begin = test.begin();
	ft::set<int>::iterator	it_end = test.end();
	std::cout << "curr test size(): " << test.size() << std::endl;
	size_t	safety = 0;
	for (; it_begin != it_end; it_begin++) {
		std::cout << "it[" << safety++ << "]: " << *(it_begin) << std::endl;
		if (safety >= test.size()) {
			break ;
		}
	}

	ft::set<int>::reverse_iterator	rit_begin = test.rbegin();
	ft::set<int>::reverse_iterator	rit_end = test.rend();
	safety = 0;
	for (; rit_begin != rit_end; rit_begin++) {
		std::cout << "rit[" << safety++ << "]: " << *(rit_begin) << std::endl;
		if (safety >= test.size()) {
			break ;
		}
	}

}

int	main(void) {
	// testVector();
	testSet();
	std::cout << "\n\n===Check Memory leaks===\n" << std::endl;
	system("leaks ft_containers");
	return 0;
}
