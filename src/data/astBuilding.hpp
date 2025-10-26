#ifndef _ELC_DATA_ASTBUILDING_
#define _ELC_DATA_ASTBUILDING_

#include "symbols.hpp"
#include <coroutine>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace elc::ast::build {

struct VirtualSymbol;

using Context = std::unordered_map<std::string, VirtualSymbol>;

struct SymbolBuilder {
	struct promise_type {
		struct final_awaiter {
			[[nodiscard]] bool await_ready() const noexcept;
			void await_suspend(std::coroutine_handle<promise_type> h) const noexcept;
			void await_resume() const noexcept;
		};

		SymbolBuilder get_return_object() noexcept;

		std::suspend_never initial_suspend() noexcept;
		final_awaiter final_suspend() noexcept;

		void return_void() noexcept;
		void unhandled_exception();
	};

	std::coroutine_handle<promise_type> handle;

	explicit SymbolBuilder(std::coroutine_handle<promise_type> h);
	SymbolBuilder(SymbolBuilder&& other) noexcept;
	SymbolBuilder& operator=(SymbolBuilder&& other) noexcept;
	SymbolBuilder(const SymbolBuilder&);
	SymbolBuilder& operator=(const SymbolBuilder&);
	~SymbolBuilder() = default;

	static thread_local Context context;
};

struct VirtualSymbol {
	std::optional<SymbolBuilder> builder;
	std::vector<std::coroutine_handle<>> waiters;

	std::optional<Symbol> symbol;
};

struct VirtualSymbolAwait {
    VirtualSymbol* symbol;

    [[nodiscard]] bool await_ready() const noexcept;
    void await_suspend(std::coroutine_handle<> h) const;
    [[nodiscard]] Symbol* await_resume() const;
};

}

#endif // !_ELC_DATA_ASTBUILDING_
