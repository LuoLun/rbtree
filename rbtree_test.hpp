#pragma once

#include "rbtree.hpp"

#include <string>

template<typename V>
static int black_node_count(rbnode<V> *n);
template<typename V>
std::string to_json_string(V *value);
template<typename V>
std::string *to_json_string(rbnode<V> *n);