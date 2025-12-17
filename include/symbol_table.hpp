#pragma once

#include "tokens.hpp"
#include <optional>
#include <string>
#include <unordered_map>

namespace clex {

class SymbolTable {
  private:  
    std::unordered_map<std::string, double> m_vars;
  public:
    SymbolTable();
    SymbolTable(const SymbolTable&) = default;
    SymbolTable(SymbolTable&&) = default;

    static SymbolTable from_map(std::unordered_map<std::string, double>&& map);

    std::optional<double> get(const Token& ident) const noexcept;
    void set(const Token& ident, double value) noexcept;
    void reset() noexcept;
};

}