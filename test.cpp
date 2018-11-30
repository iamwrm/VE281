#include <iostream>
#include <map>
#include <string>

int main()
{
	std::map<std::string, int> m1;
	m1.emplace(std::make_pair("acd", 2));
	m1.emplace(std::make_pair("1", 2));
	m1.emplace(std::make_pair("ade", 2));
	m1.emplace(std::make_pair("abc", 2));

	for (auto it : m1) {
		std::cout << it.first << std::endl;
	}
}