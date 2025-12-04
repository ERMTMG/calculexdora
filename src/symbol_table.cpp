#include "symbol_table.hpp"
#include "tokens.hpp"
#include <optional>
#include <string>

namespace clex {

std::optional<double> SymbolTable::get(const Token& ident) const noexcept {
    std::string var_name = *ident.get_ident();
    auto itr = m_vars.find(var_name);
    if(itr == m_vars.end()) {
        return {};
    } else {
        return itr->second;
    }
}

void SymbolTable::set(const Token& ident, double value) noexcept {
    std::string var_name = *ident.get_ident();
    m_vars[var_name] = value; // En este caso el operador [] hace exactamente lo que buscamos, 
                              // ya que puede tanto crear variables como cambiar su valor
}

void SymbolTable::reset() noexcept {
    m_vars.clear();
}

}