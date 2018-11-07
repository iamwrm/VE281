#ifndef UTIL_H
#define UTIL_H
#include <getopt.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

void get_ops(int argc, char **argv, int &verbose_flag, std::string &method_name);

#endif  // !UTIL_H