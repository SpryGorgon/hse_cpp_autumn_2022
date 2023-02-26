#include "avl.hpp"

template<class T>
CustomAllocator<T>::CustomAllocator() : begin(nullptr), size(0), capacity(0) {}

template<class T>
T* CustomAllocator<T>::allocate(std::size_t size)
{
	if (size == 0u)
	{
		return nullptr;
	}

	if (this->size + size <= this->capacity)
	{
		this->size += size;
		return begin + (this->size - size);
	}

	std::size_t new_capacity = this->size + size;
	T* new_begin = new T[new_capacity];
	for (std::size_t i = 0; i < this->size; i++) new_begin[i] = std::move(*(begin + i));
	begin = new_begin;
	this->size += size;
	capacity = new_capacity;
	return begin + (this->size - size);
}

template<class T>
void CustomAllocator<T>::deallocate(T* memory_ptr, std::size_t size)
{
	if ((!memory_ptr) || (size == 0u))
	{
		return;
	}

	delete[] begin;
}

template<class T>
Iterator<T>::Iterator(T* ptr, bool reverse) : ptr_(ptr), reverse(reverse) {}

template<class T>
bool Iterator<T>::operator==(const Iterator<T>& rhs) const
{
	return ptr_ == rhs.ptr_;
}

template<class T>
bool Iterator<T>::operator!=(const Iterator<T>& rhs) const
{
	return !(*this == rhs);
}

template<class T>
Iterator<T>::reference Iterator<T>::operator*() const
{
	return *ptr_;
}

template<class T>
Iterator<T>& Iterator<T>::findNext()
{
	if (ptr_ != nullptr)
	{
		auto c = ptr_;
		auto p = c->par, r = c->right;
		if (!r)
		{
			if (p->left == c) c = p;
			else
			{
				//auto key = c->key;
				while (c->par && c->par->right == c) c = c->par;
				if (c->par) c = c->par;
				else c = nullptr;
			}
		}
		else
		{
			c = r;
			while (c->left) c = c->left;
		}
		ptr_ = c;
	}
	return *this;
}

template<class T>
Iterator<T>& Iterator<T>::findPrev()
{
	if (ptr_ != nullptr)
	{
		auto c = ptr_;
		auto p = c->par, l = c->left;
		if (!l)
		{
			if (p->right == c) c = p;
			else
			{
				//auto key = c->key;
				while (c->par && c->par->left == c) c = c->par;
				if (c->par) c = c->par;
				else c = nullptr;
			}
		}
		else
		{
			c = l;
			while (c->right) c = c->right;
		}
		ptr_ = c;
	}
	return *this;
}

template<class T>
Iterator<T>& Iterator<T>::operator++()
{
	return (reverse ? findPrev() : findNext());
}

template<class T>
Iterator<T>& Iterator<T>::operator--()
{
	return (reverse ? findNext() : findPrev());
}


template<class Key, class T>
Node<Key, T>::Node(Key k, T v) : key(k), value(v), height_(1), left(nullptr), right(nullptr), par(nullptr) {}
template<class Key, class T>
Node<Key, T>::~Node() = default;

template<class Key, class T>
uint8_t Node<Key, T>::height(Node * p)
{
	return p ? p->height_ : 0;
}

template<class Key, class T>
int Node<Key, T>::bfactor(Node * p)
{
	return height(p->right) - height(p->left);
}

template<class Key, class T>
void Node<Key, T>::fixheight(Node * p)
{
	uint8_t hl = height(p->left);
	uint8_t hr = height(p->right);
	p->height_ = (hl > hr ? hl : hr) + 1;
}

template<class Key, class T>
Node<Key, T>* Node<Key, T>::rotateright(Node * p) // правый поворот вокруг p
{
	Node* q = p->left;
	p->left = q->right; if (p->left) p->left->par = p;
	q->right = p; q->par = p->par; q->right->par = q;
	fixheight(p);
	fixheight(q);
	return q;
}

template<class Key, class T>
Node<Key, T>* Node<Key, T>::rotateleft(Node * q) // левый поворот вокруг q
{
	Node* p = q->right;
	q->right = p->left; if (q->right) q->right->par = q;
	p->left = q; p->par = q->par; p->left->par = p;
	fixheight(q);
	fixheight(p);
	return p;
}

template<class Key, class T>
Node<Key, T>* Node<Key, T>::balance(Node * p) // балансировка узла p
{
	fixheight(p);
	if (bfactor(p) == 2)
	{
		if (bfactor(p->right) < 0)
			p->right = rotateright(p->right);
		return rotateleft(p);
	}
	if (bfactor(p) == -2)
	{
		if (bfactor(p->left) > 0)
			p->left = rotateleft(p->left);
		return rotateright(p);
	}
	return p; // балансировка не нужна
}

template<class Key, class T>
Node<Key, T>* Node<Key, T>::insert(Node * p, Key & k, T & v) // вставка ключа k в дерево с корнем p
{
	if (!p) return new Node(k, v);
	if (k < p->key)
	{
		p->left = insert(p->left, k, v);
		if (p->left) p->left->par = p;
	}
	else
	{
		p->right = insert(p->right, k, v);
		if (p->right) p->right->par = p;
	}
	return balance(p);
}

template<class Key, class T>
Node<Key, T>* Node<Key, T>::findmin(Node * p) // поиск узла с минимальным ключом в дереве p 
{
	return p->left ? findmin(p->left) : p;
}

template<class Key, class T>
Node<Key, T>* Node<Key, T>::findmax(Node * p) // поиск узла с максимальным ключом в дереве p 
{
	return p->right ? findmax(p->right) : p;
}

template<class Key, class T>
Node<Key, T>* Node<Key, T>::removemin(Node * p) // удаление узла с минимальным ключом из дерева p
{
	if (p->left == nullptr)
		return p->right;
	p->left = removemin(p->left);
	if (p->left) p->left->par = p;
	return balance(p);
}

template<class Key, class T>
Node<Key, T>* Node<Key, T>::remove(Node * p, Key & k) // удаление ключа k из дерева p
{
	if (!p) return nullptr;
	if (k < p->key)
	{
		p->left = remove(p->left, k);
		if (p->left) p->left->par = p;
	}
	else if (k > p->key)
	{
		p->right = remove(p->right, k);
		if (p->right) p->right->par = p;
	}
	else //  k == p->key 
	{
		Node* q = p->left;
		Node* r = p->right;
		Node* p_par = p->par;
		delete p;
		if (!r)
		{
			if (q) q->par = p_par;
			return q;
		}
		Node* min = findmin(r);
		min->right = removemin(r); if (min->right) min->right->par = min;
		min->left = q; if (min->left) min->left->par = min;
		return balance(min);
	}
	return balance(p);
}

template<class Key, class T>
Node<Key, T>* Node<Key, T>::find(Node * p, Key & k)
{
	if (!p) return nullptr;
	if (k < p->key) return find(p->left, k);
	else if (k > p->key) return find(p->right, k);
	else return p;
}


//using NodeT = Node<Key, T>;
//using node = NodeT*;
//using key_ref = Key&;
//using val_ref = T&;
//using iterator = Iterator<NodeT>;
template<class K, class T, class C, class A>
avl<K, T, C, A>::avl() : root(nullptr), sz(0) {}
//template<class K, class T, class C, class A>
//avl<K, T, C, A>::avl(node n) : root(n), sz(n->sz) {}
//avl(avl& other) = default;
template<class K, class T, class C, class A>
avl<K, T, C, A>::~avl()
{
	clear();
	delete root;
}

template<class K, class T, class C, class A>
avl<K, T, C, A>::val_ref avl<K, T, C, A>::operator[](const key_ref k)
{
	node n = NodeT::find(root, k);
	if (n == nullptr) throw std::out_of_range("Key " + std::to_string(k) + " is out of range");
	return n->value;
}

template<class K, class T, class C, class A>
avl<K, T, C, A>::val_ref avl<K, T, C, A>::at(const key_ref k)
{
	try
	{
		val_ref v = (*this)[k];
		return v;
	}
	catch (const std::out_of_range& e)
	{
		throw std::out_of_range("Key " + std::to_string(k) + " is out of range");
	}
}

template<class K, class T, class C, class A>
void avl<K, T, C, A>::insert(const key_ref k, const val_ref v)
{
	root = NodeT::insert(root, k, v);
	sz++;
}

template<class K, class T, class C, class A>
void avl<K, T, C, A>::erase(const key_ref k)
{
	root = NodeT::remove(root, k);
	sz--;
}

template<class K, class T, class C, class A>
avl<K, T, C, A>::iterator avl<K, T, C, A>::find(const T v)
{
	iterator begin_it = begin(),
		end_it = end();
	while (begin_it != end_it)
	{
		if ((*begin_it).value == v) break;
		++begin_it;
	}
	return begin_it;
}

template<class K, class T, class C, class A>
bool avl<K, T, C, A>::contains(const T v)
{
	iterator it = this->find(v);
	return (it != nullptr);
}

template<class K, class T, class C, class A>
bool avl<K, T, C, A>::empty() const
{
	return NodeT::height(root) == 0;
}

template<class K, class T, class C, class A>
uint32_t avl<K, T, C, A>::size() const
{
	return sz;
}

template<class K, class T, class C, class A>
void avl<K, T, C, A>::clear()
{
	while (NodeT::height(root)) root = NodeT::remove(root, root->key);
}

template<class K, class T, class C, class A>
avl<K, T, C, A>::iterator avl<K, T, C, A>::begin() noexcept
{
	return iterator(NodeT::findmin(root));
}

template<class K, class T, class C, class A>
avl<K, T, C, A>::iterator avl<K, T, C, A>::end() noexcept
{
	return nullptr;
}

template<class K, class T, class C, class A>
avl<K, T, C, A>::iterator avl<K, T, C, A>::rbegin() noexcept
{
	return iterator(NodeT::findmax(root), true);
}

template<class K, class T, class C, class A>
avl<K, T, C, A>::iterator avl<K, T, C, A>::rend() noexcept
{
	return nullptr;
}