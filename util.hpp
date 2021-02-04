#ifndef UTIL_HPP
#define UTIL_HPP
#include <string>
#include <vector>
#include <future>
std::vector<std::string> vector_slice(std::vector<std::string> arr, int i);
std::string string_round(float var);
std::future<std::string> get_cpu_usage();
std::int64_t to_number(std::string str, int length);
std::vector<std::string> split(std::string str, std::string splitBy);
std::string join(std::vector<std::string> arr, std::string joinwith);
std::string reverse_string(std::string line);
std::string date(std::int64_t timestamp);
#endif