/**
 * @file symbol_table.hpp
 * @brief Definición de la tabla de símbolos usada durante la evaluación.
 *
 * Esta clase encapsula el almacenamiento y acceso a variables identificadas
 * por nombre, asociando a cada identificador un valor numérico. Es utilizada
 * principalmente durante la evaluación de expresiones y la ejecución de
 * asignaciones.
 *
 * @author Eduardo Rodríguez, Raúl Gabaldón
 * @date 2025-12
 */
#pragma once

#include "tokens.hpp"
#include <optional>
#include <string>
#include <unordered_map>

namespace clex {

/**
 * @brief Tabla de símbolos para variables.
 *
 * `SymbolTable` mantiene una correspondencia entre nombres de identificadores
 * y valores numéricos (`double`).
 */
class SymbolTable {
  private:  
    std::unordered_map<std::string, double> m_vars; /**< Mapa interno de variables y valores. */

  public:
    /**
     * @brief Constructor por defecto.
     *
     * Construye una tabla de símbolos, que inicialmente solamente reserva nombres para
     * constantes comúnmente utilizadas:
     * - `pi` para la constante π (≈ 3.141592653...)
     * - `euler` para el número de Euler e (≈ 2.7182818284...)
     * - `phi` para el número de oro (= (1 + √5)/2 ≈ 1.6180339887...)
     * - `eulerMascheroni` para la constante de Euler-Mascheroni γ (≈ 0.5772156649...)
     */
    SymbolTable();

    /// Constructor de copia (por defecto).
    SymbolTable(const SymbolTable&) = default;

    /// Constructor de movimiento (por defecto).
    SymbolTable(SymbolTable&&) = default;

    /**
     * @brief Construye una tabla de símbolos a partir de un mapa existente.
     *
     * El mapa pasado como argumento es movido al interior de la tabla de símbolos. Las
     * constantes iniciales señaladas en el constructor por defecto se encuentran también en la
     * tabla construida, excepto si son redeclaradas en `map` (por ejemplo, si se da que `map.at("phi") == 2.0`),
     * entonces la tabla de símbolos devuelta corresponderá el identificador `phi` a `2.0`.
     *
     * @param map Mapa de identificadores a valores numéricos.
     * @return Nueva tabla de símbolos inicializada con los valores proporcionados.
     * @pre Se recomienda que todas las cadenas que sean claves de `map` sean identificadores válidos. El constructor no lanzará ninguna excepción de lo contrario, pero las "variables" inválidas no serán accesibles y malgastarán memoria.
     */
    static SymbolTable from_map(std::unordered_map<std::string, double>&& map) noexcept;

    /**
     * @brief Obtiene el valor asociado a un identificador.
     *
     * Si el identificador no está presente en la tabla, se devuelve un
     * `std::optional` vacío.
     *
     * @param ident Token del identificador a consultar.
     * @return Un `std::optional<double>` con el valor asociado si existe, o vacío en caso contrario.
     * @pre `ident` debe de ser un token de tipo `TokenType::IDENTIFIER`.
     */
    std::optional<double> get(const Token& ident) const noexcept;

    /**
     * @brief Asocia un valor a un identificador.
     *
     * Si el identificador ya existe en la tabla, su valor será sobrescrito.
     *
     * @param ident Token del identificador.
     * @param value Valor numérico a asociar.
     * @pre `ident` debe de ser un token de tipo `TokenType::IDENTIFIER`.
     */
    void set(const Token& ident, double value) noexcept;

    /**
     * @brief Elimina todas las variables almacenadas en la tabla.
     *
     * Tras la llamada, la tabla de símbolos queda con las mismas definiciones que 
     * una `SymbolTable` construida por defecto (las constantes matemáticas comunes).
     */
    void reset() noexcept;
};

} // namespace clex
