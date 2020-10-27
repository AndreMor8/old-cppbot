#ifndef UTIL_HPP
#define UTIL_HPP
#include <string>
#include <vector>
std::vector<std::string> split(std::string str, std::string splitBy);
std::string date(std::int64_t timestamp);
#endif