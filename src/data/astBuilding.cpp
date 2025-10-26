#include "astBuilding.hpp"
#include <exception>

namespace elc::ast::build {

// NOLINTBEGIN(readability-convert-member-functions-to-static)

bool SymbolBuilder::promise_type::final_awaiter::await_ready() const noexcept { return false; }
void SymbolBuilder::promise_type::final_awaiter::await_suspend(std::coroutine_handle<promise_type> h) const noexcept {
	h.destroy();
}
void SymbolBuilder::promise_type::final_awaiter::await_resume() const noexcept {}

SymbolBuilder SymbolBuilder::promise_type::get_return_object() noexcept { 
	return SymbolBuilder{std::coroutine_handle<promise_type>::from_promise(*this)}; 
}

std::suspend_never SymbolBuilder::promise_type::initial_suspend() noexcept { return {}; }
SymbolBuilder::promise_type::final_awaiter SymbolBuilder::promise_type::final_suspend() noexcept { return {}; }
void SymbolBuilder::promise_type::return_void() noexcept {}

void SymbolBuilder::promise_type::unhandled_exception() { 
	auto e = std::current_exception();
}
// NOLINTEND(readability-convert-member-functions-to-static)

SymbolBuilder::SymbolBuilder(std::coroutine_handle<promise_type> h) : handle(h) {}

SymbolBuilder::SymbolBuilder(SymbolBuilder&& other) noexcept = default;
SymbolBuilder& SymbolBuilder::operator=(SymbolBuilder&& other) noexcept = default;
SymbolBuilder::SymbolBuilder(const SymbolBuilder& other) = default;
SymbolBuilder& SymbolBuilder::operator=(const SymbolBuilder& other) = default;

bool VirtualSymbolAwait::await_ready() const noexcept {
	return symbol->symbol.has_value();
}

void VirtualSymbolAwait::await_suspend(std::coroutine_handle<> h) const {
	symbol->waiters.push_back(h);
}

Symbol* VirtualSymbolAwait::await_resume() const {
	return &*symbol->symbol;
}

}
