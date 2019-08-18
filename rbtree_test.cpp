#include "rbtree_test.hpp"

#include "rbtree.hpp"
#include <cassert>
#include <cstdio>
#include <string>
#include <memory>
#include <cstring>
#include <typeinfo>

using std::string;
using std::strlen;

template<typename V>
void check_rbtree(rbnode<V> *root);
template<typename V>
void check_no_adjacency_red_node(rbnode<V> *n);
template<typename V>
void check_no_adjacency_red_node(rbnode<V> *n, rbnode<V> *child);

void test_insert1() {
	const int testVal = 1;
	const int val = testVal;
	rbnode<const int> *n = insert((rbnode<const int> *)nullptr, &testVal);
	assert(n != nullptr);
	assert(*n->p_value == testVal);
}

void test_insert2() {
	auto *n = new rbnode<const int>(50);
	for (int i = 0; i < 10; i++) {
		const int val = i;
		insert<const int>(root(n), val);

		check_rbtree(root(n));
	}

	// print
	string *json_string = to_json_string(root(n));
	printf("%s", json_string->c_str());
	delete json_string;

	//check_rbtree(root(n));
}

//static string NullStr("null");

template<typename V>
string *to_json_string(rbnode<V> *n) {
	if (n == nullptr) return new string("null");

	string s_value = to_json_string(n->p_value);
	string s_color = to_json_string(n->color);
	string *s_left = to_json_string(n->p_left);
	string *s_right = to_json_string(n->p_right);

	char *_template = "{ \
		\"value\": %s, \
		\"color\": %s, \
		\"left\": %s, \
		\"right\": %s \
		}";

	char *s = new char[s_value.size() + s_color.size() + s_left->size() + s_right->size() + strlen(_template)];
	sprintf(s, _template, 
		s_value.c_str(),
		s_color.c_str(),
		s_left->c_str(),
		s_right->c_str()
	);
	if (s_left != nullptr) delete s_left;
	if (s_right != nullptr) delete s_right;
	return new string(s);
}

int digitCount(int i) {
	if (i < 0) i = -i;
	if (i == 0) return 1;
	int c = 0;
	while (i != 0) {
		i /= 10;
		c++;
	}
	return c;
}

template<typename V> struct JsonConverts { 
	static string convert(V *value) {
		return '"' + *value + '"';
	}
};
template<> struct JsonConverts<const int> { 
	static string convert(const int *value) {
		int val = *value;
		char *s = new char[digitCount(val) + 1];
		sprintf(s, "%d", *value);
		return string(s);
	}
};
//template<V> JsonConverts<V> { 
//	typedef string Type; 
//	//typedef Type *RawType;
//
//	static Type convert(V *value) {
//		return '"' + *value + '"';
//	}
//};

template<typename V>
string to_json_string(V *value) {
	return JsonConverts<V>::convert(value);
}

string to_json_string(rb_color color) {
	if (color == black) {
		return "\"black\"";
	}
	else {
		return "\"red\"";
	}
}

template<typename V>
void check_rbtree(rbnode<V> *root) {
	// 1. The color of root of red-black tree is black.
	assert(root == nullptr || root->color == black);

	// 2. There is no adjacency red node in the tree.
	check_no_adjacency_red_node(root);

	// 3. The number of each path from root to leaf has same number of black nodes.
	check_black_node_count(root);
}

template<typename V>
void check_black_node_count(rbnode<V> *n) {
	if (n == nullptr) return;

	int l_count = black_node_count(n->p_left);
	int r_count = black_node_count(n->p_right);
	assert(l_count == r_count);
}

template<typename V>
int black_node_count(rbnode<V> *n) {
	if (n == nullptr) return 1;

	int cur_count = n->color == black ? 1 : 0;
	int l_count = black_node_count(n->p_left) + cur_count;
	int r_count = black_node_count(n->p_right) + cur_count;
	assert(l_count == r_count);
	return l_count;
}

template<typename V>
void check_no_adjacency_red_node(rbnode<V> *n) {
	if (n == nullptr) return;
	check_no_adjacency_red_node(n, n->p_left);
	check_no_adjacency_red_node(n, n->p_right);
}

template<typename V>
void check_no_adjacency_red_node(rbnode<V> *n, rbnode<V> *child) {
	assert(n != nullptr); // Parent node must not be null
	rb_color n_color = n->color;
	rb_color child_color = child == nullptr ? black : child->color;
	assert(!(n_color == red && child_color == red));

	check_no_adjacency_red_node(child);
}

int main(int argc, char **argv) {
	test_insert1();
	test_insert2();

	printf("All test finied.");
	getchar();

	return 0;
}
