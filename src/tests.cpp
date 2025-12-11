#include "parser_errors.hpp"
#include "eval_errors.hpp"
#include "symbol_table.hpp"
#include "syntax_tree.hpp"
#include "tokens.hpp"
#include "parser.hpp"
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace clex {
    std::vector<Token> tokenize(const std::string& input);
}


class Test {
  private:
    std::string m_name;
    std::string m_input;
    clex::SymbolTable m_available_symbols;
    std::optional<double> m_expected_result;
  public:
    Test(std::string&& name, std::string&& input) 
      : m_name(std::move(name)), m_input(std::move(input)), m_available_symbols(), m_expected_result() {};
    Test(std::string&& name, std::string&& input, double result) 
      : m_name(std::move(name)), m_input(std::move(input)), m_available_symbols(), m_expected_result(result) {};
    Test(std::string&& name, std::string&& input, clex::SymbolTable&& symbols) 
      : m_name(std::move(name)), m_input(std::move(input)), m_available_symbols(std::move(symbols)), m_expected_result() {};
    Test(std::string&& name, std::string&& input, clex::SymbolTable&& symbols, double result) 
      : m_name(std::move(name)), m_input(std::move(input)), m_available_symbols(std::move(symbols)), m_expected_result(result) {};

    void run() noexcept {
        std::cout << ">>> EJECUTANDO TEST: " << m_name << '\n';
        std::cout << ">>> ENTRADA: `" << m_input << "`\n";
        auto toks = clex::tokenize(m_input);
        for(const clex::Token& tok : toks) {
            if(tok.type() == clex::TokenType::ERROR_TOKEN) {
                std::cerr << "\tERROR LÉXICO DETECTADO\n"
                          << "\tSe ha detectado un token de error. Deteniendo ejecución del test.\n";
                return;
            }
        }
        auto parser = clex::Parser(std::move(toks));
        std::optional<clex::Statement> stmt;
        try {
            stmt = parser.parse_next_statement();
        } catch(const clex::ParserError& err) {
            std::cerr << "\tERROR SINTÁCTICO DETECTADO\n"
                      << "\tEl parser ha detectado un error en el token " << err.problem_token() 
                      << ". El mensaje de error es:\n\t\t";
            err.print_to(std::cerr);
            std::cerr << "\tDeteniendo ejecución del test.\n";
            return;
        }
        if(stmt->is_expression() && m_expected_result.has_value()) {
            double expr_val;
            try {
                expr_val = stmt->ref_as_expression().evaluate(m_available_symbols);
            } catch(const clex::EvalError& err) {
                std::cerr << "\tERROR DE EVALUACIÓN DETECTADO\n"
                          << "\tSe ha detectado un error al evaluar la (sub)expresión " << err.problem_expr()
                          << ". El mensaje de error es:\n\t\t";
                err.print_to(std::cerr);
                std::cerr << "\tDeteniendo ejecución del test.\n";
                return;
            }
            if(expr_val == *m_expected_result) {
                std::cout << "Test ejecutado con éxito: El valor de la expresión es igual al esperado (" << expr_val << ").\n";
            } else if(std::abs(expr_val - *m_expected_result) < 1e-5) {
                std::cout << "Test ejecutado con éxito parcial: La diferencia entre el valor de la expresión (" << expr_val 
                          << ") y el esperado (" << *m_expected_result << ") es lo suficientemente pequeña para suponer"
                          << "que puede ser debido a errores de redondeo.\n";
            } else {
                std::cout << "Test ejecutado y fallado: El valor de la expresión (" << expr_val << ") es distinto del esperado ("
                          << *m_expected_result << ").\n";
            }
        } else if(stmt->is_assignment() && !m_expected_result.has_value()) {
            try {
                stmt->ref_as_assignment().execute(m_available_symbols);
            } catch(const clex::EvalError& err) {
                std::cerr << "\tERROR DE EVALUACIÓN DETECTADO\n"
                          << "\tSe ha detectado un error al evaluar la (sub)expresión " << err.problem_expr()
                          << ". El mensaje de error es:\n\t\t";
                err.print_to(std::cerr);
                std::cerr << "\tDeteniendo ejecución del test.\n";
                return;
            }
            std::cout << "Test ejecutado con éxito. La asignación no ha dado errores.\n";
        } else {
            std::cerr << "\tEl test esperaba una asignación y ha recibido una expresión de la entrada, o viceversa.\n"
                      << "\tDeteniendo ejecución del test.\n";
            return;
        }
    }
};

int main(int argc, char** argv) {
    std::vector<Test> tests {
        Test {
            "Suma simple",
            "2 + 2",
            4
        },
        Test {
            "Suma complicada",
            "1 + 2 + 3 + 4 + 5",
            15
        },
        Test {
            "Expresión compuesta",
            "(3 + 4) * 5 - 6 / 2^2",
            33.5
        },
        Test {
            "Variables",
            "(a + 1 - b * c) / d",
            clex::SymbolTable::from_map({{"a", 7}, {"b", 3}, {"c", 2}, {"d", 0.5}}),
            4
        },
        Test {
            "Asignación", 
            "a = 2 + 2 * b",
            clex::SymbolTable::from_map({{"b", 3}})
        },
        Test {
            "Operadores unarios",
            "+-(2 - -2)*+3",
            -12
        },
        Test {
            "Error 1: Tokens inválidos",
            "a = 2 + @"
        },
        Test {
            "Error 2: Error de sintaxis",
            "5 + 3 * a - ^ (2",
        },
        Test {
            "Error 3: Variables sin definir",
            "(1 + a * b) / c",
            clex::SymbolTable::from_map({{"a", 5}, {"b", 2}}),
            0
        },
        Test {
            "Error 4: División por cero", 
            "a = 3 / (1 - 1)"
        },
        Test {
            "Error 5: Resultado no real",
            "i = (0 - 1) ^ 0.5"
        }
    };
    
    if(argc > 1) {
        if(std::string(argv[1]) == "all") {
            std::cout << "===== EJECUTANDO TODOS LOS TESTS =====\n";
            for(Test& test : tests) {
                test.run();
                std::cout << "======================================\n";
            }
        } else {
            for(int i = 1; i < argc; i++) {
                size_t test_idx = std::atoll(argv[i]);
                if(test_idx >= tests.size()) {
                    std::cerr << "Número de test " << test_idx << " inválido. Por favor, introduzca un número de 0 a " << tests.size() - 1 << ".\n";
                    exit(-1);
                }
                std::cout << "===== EJECUTANDO TEST " << test_idx << " =====\n";
                tests[test_idx].run();
            }
            std::cout << "=============================\n";
        }
    } else {
        std::cerr << "Error de argumento. Sintaxis esperada: \n"
                  << "\t" << argv[0] << " <núm test 1> <núm test 2> <...> <núm test N>: Ejecuta los tests con números puestos como argumentos.\n"
                  << "\t" << argv[0] << " all: Ejecuta todos los tests.\n";
        exit(-1);
    }

}