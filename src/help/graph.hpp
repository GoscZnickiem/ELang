#ifndef _ELC_HELP_GRAPH_
#define _ELC_HELP_GRAPH_

#include "help/heap.hpp"
#include <list>
#include <map>
#include <stdexcept>
#include <string>
#include <utility>

template <typename T, typename Compare>
class Graph {
public:
	struct Node;
private:
	std::map<std::string, Node> nodes;
	using MapIterator = std::map<std::string, Node>::iterator;
	struct MapIteratorCompare {
		Compare comp;
		bool operator()(const MapIterator& a, const MapIterator& b) const {
			return comp(a->second, b->second);
		}
	};
	using HeapType = Heap<MapIterator, MapIteratorCompare>;
	HeapType availability;

public:
	struct Node {
		std::list<Node*> deps;
		std::list<std::pair<typename HeapType::Node*, typename std::list<Node*>::iterator>> refs;
		HeapType* heap;
		HeapType::Node* heapPos;
		T value;

		void addDependence(Node &node) {
			deps.push_back(&node);
			node.refs.emplace_back(this, deps.end());
			heap->increaseKey(heapPos);
		};

		explicit Node(const T& v, HeapType* h) : heap(h), value(v) {}
		explicit Node(T&& v, HeapType* h) : heap(h), value(std::move(v)) {}

		~Node() {
			for(auto [node, it] : refs) {
				node->value->refs.erase(it);
				heap->decreaseKey(node);
			}
		}
		Node(const Node &) = delete;
		Node(Node &&) = delete;
		Node &operator=(const Node &) = delete;
		Node &operator=(Node &&) = delete;
	};

	void addNode(const std::string& key, T&& value) {
		auto [it, success] = nodes.try_emplace(key, std::move(value), availability);
		if(!success) throw std::runtime_error("Co ty robisz gosciu");
		it->second.heapPos = availability.emplace(it);
	}

	void addNode(const std::string& key, T& value) {
		addNode(key, std::move(value));
	}

	template <typename Container>
	requires std::same_as<typename Container::value_type, std::string>
	void addNode(const std::string& key, T&& value, const Container& dependencies = {}) {
		addNode(key, std::move(value));
		for(const auto& v : dependencies) {
			nodes[key].addDependence(nodes.at(v));
		}
	}

	template <typename Container>
	requires std::same_as<typename Container::value_type, std::string>
	void addNode(const std::string& key, T& value, const Container& dependencies = {}) {
		addNode(key, std::move(value), dependencies);
	}

	T& available() {
		return *availability.top();
	}

	void pop() {
		auto& it = availability.top();
		availability.pop();
		nodes.erase(it);
	}

	Node& operator[](const std::string& key) {
		return nodes[key];
	}
};

#endif // !_ELC_HELP_GRAPH_
