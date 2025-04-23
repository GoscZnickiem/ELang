#ifndef _ELC_COMPILER_TYPES_
#define _ELC_COMPILER_TYPES_

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

struct CompiledType {
	CompiledType(const CompiledType &) = default;
	CompiledType(CompiledType &&) = delete;
	CompiledType &operator=(const CompiledType &) = default;
	CompiledType &operator=(CompiledType &&) = delete;
	virtual ~CompiledType() = default;

	bool unified {false};
	virtual void unify();
};

struct IntegerType : public CompiledType {
	enum struct Type {
		Int8, Int16, Int32, Int64,
		Uint8, Uint16, Uint32, Uint64,
		Float16, Float32
	};
	Type type;

	void unify() final;
};

struct StructType : public CompiledType {
	std::string name;
	std::vector<std::unique_ptr<CompiledType>> members;

	void unify() final;
};

struct UnionType : public CompiledType {
	std::string name;
	std::vector<std::unique_ptr<CompiledType>> members;

	void unify() final;
};

struct ArrayType : public CompiledType {
	std::unique_ptr<CompiledType> element;
	std::size_t size;

	void unify() final;
};

struct PointerType : public CompiledType {
	std::unique_ptr<CompiledType> pointed;

	void unify() final;
};

struct FunctionType : public CompiledType {
	// TODO
};

struct AmbiguousType {
	std::vector<CompiledType> possibleTypes;
};


#endif // !_ELC_COMPILER_TYPES_
