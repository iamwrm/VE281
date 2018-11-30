all:	main.cpp ut.cpp ut.h bot.h bot.cpp
	clang++ -O2 -Wall -o main -std=c++11 main.cpp ut.cpp bot.cpp -g