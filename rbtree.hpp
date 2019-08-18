#pragma once
#include <cassert>
#include <set>

enum rb_color {red, black};

template<typename V>
struct s_rbnode {
	struct s_rbnode<V> *p_parent = nullptr;
	struct s_rbnode<V> *p_left = nullptr;
	struct s_rbnode<V> *p_right = nullptr;
	const V *const p_value;
	rb_color color = red;

	template<typename U> struct Allocator {
		static U* allocate(U u) {
			return new U(u);
		}
		static U* allocate(U *u) {
			return u;
		}
	};

	s_rbnode(V *p_value):p_value(Allocator<V>::allocate(p_value)) { }
	s_rbnode(V value):p_value(Allocator<V>::allocate(value)) {
		allocated = true;
	}

	~s_rbnode() {
		if (allocated) {
			delete this->p_value;
		}
	}

private:
	bool allocated = false;
};

template<typename V>
using rbnode = s_rbnode<V>;

template<typename V>
rbnode<V> *insert(rbnode<V> *dst, V *p_value) {
	return insert<V>(dst, new rbnode<V>(p_value));
}

template<typename V>
rbnode<V> *insert(rbnode<V> *dst, V value) {
	return insert<V>(dst, new rbnode<V>(value));
}

template<typename V>
rbnode<V> *delete(rbnode<V> *root, V value) {
	// TODO:
	assert(false);
	return nullptr;
}

template<typename V>
rbnode<V> *delete(rbnode<V> *root, v *value) {
	// TODO:
	assert(false);
	return nullptr;
}

/**
	@param dst Ҫ�����Ŀ������
	@param the_new Ҫ������½ڵ�
	@return ����Ŀ���������¸��ڵ�
*/
template<typename V>
rbnode<V> *insert(rbnode<V> *dst, rbnode<V> *the_new) {
	// Can't insert a node on the other tree.
	assert(the_new->p_parent == nullptr);
	
	if (dst == nullptr) {
		insert_case1(the_new);
		return the_new;
	} else {
		if (*the_new->p_value <= *dst->p_value) {
			if (dst->p_left != nullptr) {
				insert(dst->p_left, the_new);
			} else {
				dst->p_left = the_new;
				the_new->p_parent = dst;
				if (the_new->p_parent->color == red) {
					insert_case1(the_new);
				}
			}
		} else if (*the_new->p_value > *dst->p_value) {
			if (dst->p_right != nullptr) {
				insert(dst->p_right, the_new);
			} else {
				dst->p_right = the_new;
				the_new->p_parent = dst;
				if (the_new->p_parent->color == red) {
					insert_case1(the_new);
				}
			}
		}
		return the_new;
	}
}

template<typename V>
void insert_case1(rbnode<V> *n) {
	if (n->p_parent == nullptr) {
		n->color = black;
		return;
	}
	else {
		insert_case2(n);
	}
}

template<typename V>
void insert_case2(rbnode<V> *n) {
	rbnode<V> *p = n->p_parent;
	if (p->color == red && p->p_parent == nullptr) {
		/*if (n == p->p_left) {
			rotate_right(p);
		}
		else {
			rotate_left(p);
		}
		n->color = black;*/
		p->color = black;
	}
	else {
		insert_case3(n);
	}
}

template<typename V>
void insert_case3(rbnode<V> *n) {
	rbnode<V> *p = n->p_parent;
	rbnode<V> *g = p->p_parent;
	if (g == nullptr) {
		p->color = black;
		return;
	} else {
		rbnode<V> *u = slibling(p);
		if (p->color == red
			&& u && u->color == red) {
			p->color = u->color = black;
			insert_case1(g);
		}
		else {
			insert_case4(n);
		}
	}
}

template<typename V>
void insert_case4(rbnode<V> *n) {
	rbnode<V> *p = n->p_parent;
	rbnode<V> *g = p->p_parent;

	if (p == g->p_left && p->color == red
		&& n == p->p_right) {
		rotate_left(p);
		insert_case5(p);
	}
	else if (p == g->p_right && p->color == red
		&& n == p->p_left) {
		rotate_right(p);
		insert_case5(p);
	}
	else {
		insert_case5(n);
	}
}

template<typename V>
void insert_case5(rbnode<V> *n) {
	rbnode<V> *p = n->p_parent;
	rbnode<V> *g = p->p_parent;
	
	if (p->color == red && g->color == black) {
		if (p == g->p_left) {
			rotate_right(g);
		}
		else if (p == g->p_right) {
			rotate_left(g);
		}
		n->color = black;

		insert_case1(p);
	}
}

// -- utils --

template<typename V>
rbnode<V> *slibling(rbnode<V> *n) {
	if (!n->p_parent) return nullptr;
	if (n == n->p_parent->p_left) return n->p_parent->p_right;
	else return n->p_parent->p_left;
}

/**
			p (or null)       p (or null)                                        
		   /                 /                                        
		  n                 r                                        
		/  \      =>       / \                                                     
	   l    r             n   4                                         
	  / \   /\           / \                                              
	 1   2 3  4         l   3
	                   /\      
					  1  2
*/
template<typename V>
void rotate_left(rbnode<V> *n) {
	rbnode<V> *p = n->p_parent;
	rbnode<V> *l = n->p_left;
	rbnode<V> *r = n->p_right;
	
	if (p) {
		if (n == p->p_left) p->p_left = r;
		else p->p_right = r;
	}
	r->p_parent = p;

	n->p_right = r->p_left;
	if (r->p_left) r->p_left->p_parent = n;
	
	n->p_parent = r;
	r->p_left = n;
}

template<typename V> 
void rotate_right(rbnode<V> *n) {
	rbnode<V> *p = n->p_parent;
	rbnode<V> *l = n->p_left;
	rbnode<V> *r = n->p_right;

	if (p) {
		if (n == p->p_left) p->p_left = l;
		else p->p_right = l;
	}
	l->p_parent = p;

	n->p_left = l->p_right;
	if (l->p_right) l->p_right->p_parent = n;

	n->p_parent = l;
	l->p_right = n;
}

template<typename V>
rbnode<V> *root(rbnode<V> *n) {
	std::set<rbnode<V> *> traversed;
	while (n->p_parent != nullptr) {
		if (traversed.find(n) == traversed.end()) {
			traversed.insert(n);
			n = n->p_parent;
		}
		else {
			throw std::runtime_error("Cycling reference!");
		}
	}
	return n;
}