#include <iostream>
#include <vector>
#include <algorithm>

void print(const std::vector<int> v)
{
	for(std::size_t i = 0; i < v.size(); i++) {
		std::cout << v[i] << " ";
	}
	
	std::cout << std::endl;
}

void removeRange(std::vector<int>& v, const std::vector<int>& elems)
{
	std::vector<int>::iterator it = v.end();
	for (std::size_t i = 0; i < elems.size(); i++) {
		it = std::remove(v.begin(), it, elems[i]);
		// print(v);
		// std::cout << end - v.begin() << std::endl;
	}
	
	v.erase(it, v.end());
}

int main()
{
	std::vector<int> v = { 9, 4, 8, 7, 3, 6, 8, 4, 2 };
	std::vector<int> toDelete = { 4, 8 };
	
	removeRange(v, toDelete);

	print(v);
}