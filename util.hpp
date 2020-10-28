#ifndef UTIL_HPP
#define UTIL_HPP
#include <string>
#include <vector>
std::vector<std::string> split(std::string str, std::string splitBy);
std::string join(std::vector<std::string> arr, std::string joinwith);
std::string reverse_string(std::string line);
std::string date(std::int64_t timestamp);
#endif