#include "symbol_table.hpp"
#include "tokens.hpp"
#include <optional>
#include <string>

namespace clex {

SymbolTable::SymbolTable() : m_vars { // Constructor por defecto que incluye valores para constantes utilizadas
    {"pi", 3.14159265358979323846},
    {"euler", 2.71828182845904523536},
    {"phi", 1.61803398874989484820},
    {"eulerMascheroni", 0.57721566490153286060},
} {};

SymbolTable SymbolTable::from_map(std::unordered_map<std::string, double>&& map) {
    SymbolTable output;
    map.merge(output.m_vars); // hacemos un merge de las variables declaradas con las constantes predefinidas 
                              // (manteniendo prioridad de las variables declaradas, por si, por ejemplo, `pi` tiene otro valor)
    output.m_vars = std::move(map);
    return output;
}

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
    m_vars = std::move(SymbolTable().m_vars); // resetea las variables a ser las constantes normales
}

}