#ifndef _ELC_HELP_HEAP_
#define _ELC_HELP_HEAP_

#include <array>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <limits>
#include <memory>

template <typename T, typename Compare = std::less<T>, typename Allocator = std::allocator<T>>
class Heap {
public:
	class Node {
		Node *parent = nullptr;
		Node *child = nullptr;
		Node *left = this;
		Node *right = this;
		uint32_t degree = 0;
		bool marked = false;

	public:
		T value;

		explicit Node(const T &v) : value(v) {}
		template <typename... Args>
		explicit Node(Args &&...args) : value(std::forward<Args>(args)...) {}

		friend Heap<T, Compare, Allocator>;
	};

private:
	class Comp {
		friend class Heap<T, Compare, Allocator>;
		Compare compare;
	public:
		bool operator()(const Node* a, const Node* b) const {
			return compare(a->value, b->value);
		}
	};
	Comp comp;

	using NodeAllocType = typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;
	using NodeTraits = std::allocator_traits<NodeAllocType>;
	NodeAllocType nodeAlloc;

	Node* min = nullptr;

	Node* createNode(const T& v) {
		Node* ptr = NodeTraits::allocate(nodeAlloc, 1);
		NodeTraits::construct(nodeAlloc, ptr, v);
		return ptr;
	}

	template <typename... Args>
	Node* createNode(Args&&... args) {
		Node* ptr = NodeTraits::allocate(nodeAlloc, 1);
		NodeTraits::construct(nodeAlloc, ptr, std::forward<Args>(args)...);
		return ptr;
	}

	void addTree(Node* node) {
		node->left = min;
		node->right = min->right;
		node->right->left = node;
		min->right = node;
	}

	void cut(Node* node) {
		Node* parent = node->parent;
		if(parent == nullptr) return;
		if(node->right == node) {
			parent->child = nullptr;
		} else {
			parent->child = node->right;
			node->right->left = node->left;
			node->left->right = node->right;
		}
		parent->degree--;
		node->parent = nullptr;
		node->marked = false;
		addTree(node);
		if(parent->marked) {
			parent->marked = false;
			cut(parent);
		} else {
			parent->marked = true;
		}
	}

	void addChild(Node* node, Node* other) {
		other->parent = node;
		if(node->child) {
			other->left = node->child->left;
			other->right = node->child;
			other->right->left = other;
			other->left->right = other;
		} else {
			node->child = other;
			other->left = other->right = other;
		}
		node->degree++;
	}

	void orphanChildren(Node* node) {
		Node* leftChild = node->child;
		Node* rightChild = leftChild->left;
		for(Node* i = leftChild; i != rightChild; i = i->right) i->parent = nullptr;
		rightChild->parent = nullptr;
		rightChild->right = min->right;
		min->right->left = rightChild;
		leftChild->left = min;
		min->right = leftChild;
	}

	void cleanup(Node* n) {
		constexpr std::size_t deg = std::numeric_limits<std::size_t>::digits;
		std::array<Node*, deg> trees{};

		while(true) {
			if(trees[n->degree] == nullptr) {
				trees[n->degree] = n;
				n = n->right;
			} else {
				Node* m = trees[n->degree];
				if(n == m) { break; }
				trees[n->degree] = nullptr;
				m->left->right = m->right;
				m->right->left = m->left;
				if(comp(n, m)) {
					addChild(n, m);
				} else if(n->right == n) {
					m->left = m;
					m->right = m;
					addChild(m, n);
					n = m;
				} else {
					n->left->right = m;
					m->left = n->left;
					n->right->left = m;
					m->right = n->right;
					addChild(m, n);
					n = m;
				}
			}
		}

		min = n;
		for(Node* m = n->right; m != n; m = m->right) {
			if(comp(m, min)) min = m;
		}
	}

	void destroyNode(Node* node) {
		if(node->child) destroyNode(node->child);

		Node* next = node->right;
		node->left->right = node->right;
		node->right->left = node->left;
		NodeTraits::destroy(nodeAlloc, node);
		NodeTraits::deallocate(nodeAlloc, node, 1);

		if(next != node) destroyNode(next);
	}

public:

	T& top() {
		return min->value;
	}

	Node* push(const T& v) {
		Node* node = createNode(v);
		if(min == nullptr) {
			min = node;
		} else {
			addTree(node);
			if(comp(node, min)) {
				min = node;
			}
		}
		return node;
	}

	template <typename... Args>
	Node* emplace(Args&&... args) {
		Node* node = createNode(std::forward<Args>(args)...);
		if(min == nullptr) {
			min = node;
		} else {
			addTree(node);
			if(comp(node, min)) {
				min = node;
			}
		}
		return node;
	}

	void decreaseKey(Node* node) {
		if(node->parent != nullptr && comp(node, node->parent)) cut(node);
		min = node->parent == nullptr && comp(node, min) ? node : min;
	}

	void increaseKey(Node* node) {
		if(node->child) {
			orphanChildren(node);
			node->child = nullptr;
			cut(node);
		}
		if(min == node) cleanup(min);
	}

	void pop() {
		if(min->child) orphanChildren(min);
		if(min->right != min) {
			min->left->right = min->right;
			min->right->left = min->left;
		} else {
			NodeTraits::destroy(nodeAlloc, min);
			NodeTraits::deallocate(nodeAlloc, min, 1);
			min = nullptr;
			return;
		}
		Node* t = min->left;
		NodeTraits::destroy(nodeAlloc, min);
		NodeTraits::deallocate(nodeAlloc, min, 1);

		cleanup(t);
	}

	[[nodiscard]] bool empty() const {
		return min == nullptr;
	}

	~Heap() {
		if(min != nullptr) destroyNode(min);
	}
	
	Heap() = default;
	Heap(const Heap &) = delete;
	Heap(Heap &&) = delete;
	Heap &operator=(const Heap &) = delete;
	Heap &operator=(Heap &&) = delete;
};

#endif // !_ELC_HELP_HEAP_
