#include "vector.hpp"

void	test(void) {
	ft::vector<int>	test;

	std::cout << "instance test size: " << test.size() << std::endl;
	// test[0] = 42;
	// std::cout << "test[0]: " << test[0] << std::endl;
}

int	main(void) {
	test();
	std::cout << "\n\n===Check Memory leaks===\n" << std::endl;
	system("leaks ft_containers");
	return 0;
}
