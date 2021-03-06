#pragma once

#include <stdio.h>
#include <any>
#include <string>
#include <unordered_map>
#include <vector>

struct Block;
struct Statement;

namespace nyx {
enum ValueType { Int, Double, String, Bool, Null };

struct Function {
    explicit Function() = default;

    std::string name;
    std::vector<std::string> params;
    Block* block{};
};

struct Value {
    explicit Value() = default;
    explicit Value(nyx::ValueType type) : type(type) {}
    explicit Value(nyx::ValueType type, std::any data)
        : type(type), data(std::move(data)) {}

    template <int _NyxType>
    inline bool isType();

    template <typename _CastingType>
    inline _CastingType value_cast();

    Value operator+(Value rhs);
    Value operator-(Value rhs);
    Value operator*(Value rhs);
    Value operator/(Value rhs);
    Value operator%(Value rhs);

    Value operator&&(Value rhs);
    Value operator||(Value rhs);

    Value operator==(Value rhs);
    Value operator!=(Value rhs);
    Value operator>(Value rhs);
    Value operator>=(Value rhs);
    Value operator<(Value rhs);
    Value operator<=(Value rhs);

    Value operator&(Value rhs);
    Value operator|(Value rhs);

    nyx::ValueType type{};
    std::any data;
};

struct Variable {
    explicit Variable() = default;

    std::string name;
    Value value;
};

struct LocalContext {
    explicit LocalContext() = default;

    std::vector<Variable*> vars;
};

struct GlobalContext : public LocalContext {
    explicit GlobalContext();

    std::vector<Function*> funcs;
    std::vector<Statement*> stmts;
    std::unordered_map<std::string,
                       Value (*)(GlobalContext*, std::vector<Value>)>
        builtin;
};

template <int _NyxType>
inline bool Value::isType() {
    return this->type == _NyxType;
}

template <typename _CastingType>
inline _CastingType Value::value_cast() {
    return std::any_cast<_CastingType>(data);
}
}  // namespace nyx
