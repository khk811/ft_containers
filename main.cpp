#include "vector.hpp"
#include "iostream"

void	test(void) {
	ft::vector<int>	test(12, 42);

	std::cout << "instance test size: " << test.size() << std::endl;
	test[0] = 42;
	std::cout << "test[0]: " << test[0] << std::endl;
	std::cout << "test[1]: " << test[1] << std::endl;
	std::cout << "test[2]: " << test[2] << std::endl;
	std::cout << *(test.begin()) << std::endl;
	std::cout << *(test.end()) << std::endl;
}

int	main(void) {
	test();
	std::cout << "\n\n===Check Memory leaks===\n" << std::endl;
	system("leaks ft_containers");
	return 0;
}
