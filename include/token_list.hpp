/**
 * @file token_list.hpp
 * @brief Definición de una clase auxiliar para la gestión secuencial de tokens.
 *
 * Esta clase actúa como un contenedor con semántica de flujo sobre una lista
 * de tokens, permitiendo avanzar, inspeccionar el siguiente token y devolver
 * tokens al flujo si es necesario. Está pensada principalmente para ser usada
 * por el analizador sintáctico.
 *
 * @author Eduardo Rodríguez, Raúl Gabaldón
 * @date 2025-12
 */
#pragma once

#include "tokens.hpp"
#include <vector>

namespace clex {

/**
 * @brief Lista de tokens con operaciones de acceso secuencial.
 *
 * `TokenList` encapsula un búfer de tokens y proporciona una interfaz
 * orientada al consumo secuencial, similar a la de un *stream*.
 */
class TokenList {
  private:  
    std::vector<Token> m_tokens; /**< Contenedor interno de tokens. */

  public:
    /**
     * @brief Construye una lista de tokens a partir de un vector.
     *
     * El vector pasado como argumento es movido al interior del objeto,
     * transfiriendo la propiedad de los tokens. Además, si el vector no acaba
     * en un token de tipo fin de archivo (`TokenType::END_OF_FILE`), se inserta
     * dicho token al final.
     *
     * @param tokens Vector de tokens a almacenar.
     */
    TokenList(std::vector<Token>&& tokens);

    /// Constructor de copia (por defecto).
    TokenList(const TokenList&) = default;

    /// Constructor de movimiento (por defecto).
    TokenList(TokenList&&) = default;

    /// Operador de asignación por copia (por defecto).
    TokenList& operator=(const TokenList&) = default;

    /// Operador de asignación por movimiento (por defecto).
    TokenList& operator=(TokenList&&) = default;

    /**
     * @brief Comprueba si se ha alcanzado el final de la lista de tokens.
     *
     * @return `true` si no quedan tokens por consumir, `false` en caso contrario.
     */
    bool at_end() const noexcept;

    /**
     * @brief Obtiene el token actual sin consumirlo.
     *
     * @return Referencia constante al siguiente token disponible.
     */
    const Token& peek() const noexcept;

    /**
     * @brief Consume y devuelve el siguiente token de la lista.
     *
     * Tras la llamada, el estado interno avanza una posición.
     *
     * @return El siguiente token.
     */
    Token next() noexcept;

    /**
     * @brief Devuelve un token al flujo.
     *
     * Inserta el token proporcionado de forma que será el próximo
     * en ser consumido por llamadas posteriores a `next()` o `peek()`.
     * El token pasado como parámetro es movido al búfer interno así que 
     * la referencia queda en un estado indefinido.
     *
     * @param tok Token a devolver al flujo.
     */
    void give_back(Token&& tok) noexcept;
};

} // namespace clex
