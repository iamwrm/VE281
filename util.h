#ifndef UTIL_H
#define UTIL_H
#include <getopt.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

class Interface_Property {
       private:
	int verbose_flag = 0;
	std::string method_name;
	void get_ops(int argc, char **argv);

       public:
	Interface_Property(int argc, char **argv)
	{
		get_ops(argc, argv);
	}
	const int & get_verbose_flag()
	{
		return verbose_flag;
	}
	const std::string & get_method_name()
	{
		return method_name;
	}
};


#endif  // !UTIL_H