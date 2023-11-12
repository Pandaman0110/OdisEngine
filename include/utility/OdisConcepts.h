#ifndef ODIS_CONCEPTS_H
#define ODIS_CONCEPTS_H

#include <string>
#include <concepts>

template <typename T>
concept KeyType = std::convertible_to<T, const std::string&>;


#endif
