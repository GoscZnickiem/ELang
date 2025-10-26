#include "operators.hpp"
#include <queue>
#include <unordered_map>

namespace elc::ast::op {

PrecedenceLevel::PrecedenceLevel(std::string n, Associativity a) 
	: name(std::move(n)), associativity(a) {}


[[nodiscard]] const std::string& PrecedenceLevel::getName() const {
	return name;
}

[[nodiscard]] Associativity PrecedenceLevel::getAssociativity() const noexcept {
	return associativity;
}

[[nodiscard]] Relation PrecedenceLevel::compare(const PrecedenceLevel& other) const {
	if(reachable(other)) return Relation::LESS;
	if(other.reachable(*this)) return Relation::GREATER;
	return Relation::UNDEFINED;
}

[[nodiscard]] Relation PrecedenceLevel::compare(const PrecedenceLevel& a, const PrecedenceLevel& b) {
	return a.compare(b);
}

void PrecedenceLevel::addRelation(Relation r, PrecedenceLevel& other) {
	if(r == Relation::LESS) {
		lessThan.push_back(&other);
	} else if(r == Relation::GREATER) {
		other.lessThan.push_back(this);
	}
}

void PrecedenceLevel::addRelation(PrecedenceLevel& a, Relation r, PrecedenceLevel& b) {
	a.addRelation(r, b);
}

[[nodiscard]] bool PrecedenceLevel::reachable(const PrecedenceLevel& other) const {
	std::unordered_map<const PrecedenceLevel*, bool> visited;
	std::queue<const PrecedenceLevel*> queue;
	queue.push(this);
	visited[this] = true;

	while(!queue.empty()) {
		const PrecedenceLevel* current = queue.front();
		queue.pop();

		if(current == &other) return true;

		for(const auto* neighbour : lessThan) {
			if(visited[neighbour]) continue;
			queue.push(neighbour);
			visited[neighbour] = true;
		}
	}
	return false;
}

}
