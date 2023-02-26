#include <iostream>
#include <string>
#include <algorithm>
//#include <vector>
//#include <map>

template<class T>
class CustomAllocator
{
private:
	T* begin;
	std::size_t size, capacity;
public:
	CustomAllocator();
	T* allocate(std::size_t size);

	void deallocate(T* memory_ptr, std::size_t size);
};

template<typename T>
class Iterator : public std::iterator<std::bidirectional_iterator_tag, T>
{
private:
	T* ptr_;
	bool reverse;
public:
	using iterator_category = std::forward_iterator_tag;
	using value_type = T;
	using difference_type = std::ptrdiff_t;
	using pointer = T*;
	using reference = T&;
public:
	Iterator(T* ptr, bool reverse = false);
	bool operator==(const Iterator<T>& rhs) const;

	bool operator!=(const Iterator<T>& rhs) const;

	reference operator*() const;

	Iterator& findNext();

	Iterator& findPrev();

	Iterator& operator++();

	Iterator& operator--();
};


template<class Key, class T>
class Node
{
public:
	Key key;
	T value;
	uint8_t height_;
	Node* left;
	Node* right;
	Node* par;
public:
	Node(Key k, T v);
	~Node();

	static uint8_t height(Node* p);

	static int bfactor(Node* p);

	static void fixheight(Node* p);

	static Node* rotateright(Node* p);

	static Node* rotateleft(Node* q);

	static Node* balance(Node* p);

	static Node* insert(Node* p, Key& k, T& v);

	static Node* findmin(Node* p);

	static Node* findmax(Node* p);

	static Node* removemin(Node* p);

	static Node* remove(Node* p, Key& k);

	static Node* find(Node* p, Key& k);
};


template<
	class Key,
	class T,
	class Compare = std::less<Key>,
	class Allocator = std::allocator<std::pair<const Key, T>>
> class avl
{
private:
	using NodeT = Node<Key, T>;
	using node = NodeT*;
	using key_ref = Key&;
	using val_ref = T&;

	node root;
	uint32_t sz;
public:
	using iterator = Iterator<NodeT>;
public:
	avl();
	//avl(node n);
	~avl();

	val_ref operator[](const key_ref k);

	val_ref at(const key_ref k);

	void insert(const key_ref k, const val_ref v);

	void erase(const key_ref k);

	iterator find(const T v);

	bool contains(const T v);

	bool empty() const;

	uint32_t size() const;

	void clear();

	iterator begin() noexcept;

	iterator end() noexcept;

	iterator rbegin() noexcept;

	iterator rend() noexcept;
};

//#include "avl.tpp"