#ifndef UTIL_HPP
#define UTIL_HPP
#include <aegis.hpp>
#include <string>
#include <vector>
#include <future>
std::vector<char> to_char_vector(std::string input);
std::vector<std::string> vector_slice(std::vector<std::string> arr, int i);
std::string string_round(float var);
std::future<std::string> get_cpu_usage();
std::int16_t to_i16(std::string str, int length);
std::int64_t to_number(std::string str, int length);
std::vector<std::string> split(std::string str, std::string splitBy);
std::string join(std::vector<std::string> arr, std::string joinwith);
std::string reverse_string(std::string line);
std::string date(std::int64_t timestamp);
aegis::snowflake get_snowflake(const std::string name, aegis::guild &_guild) noexcept;
#endif