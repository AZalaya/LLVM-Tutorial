/**
 * @file lexer.hpp
 * @author Andoni Zalaya (andoni.zalaya.c@gmail.com)
 * @brief Header file for the lexer class, which is responsible for tokenizing the input source code.
 * @version 0.1
 * @date 2026-09-13
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#ifndef LEXER_HPP
#define LEXER_HPP

#include "tokens.hpp"

#include <vector>

namespace FrontEnd {
namespace Lexer {

    class Lexer 
    {
    private:
        /**
         * @brief Construct a new Lexer object.
         * 
         */
        Lexer() = default;

        /**
         * @brief Destroy the Lexer object.
         * 
         */
        ~Lexer() = default;

    public:
        /**
         * @brief Tokenizes the input source code and returns a vector of tokens.
         * 
         * @param source The input source code to tokenize.
         * @return std::vector<Token> A vector of tokens generated from the input source code.
         */
        static std::vector<Token> Tokenize(const std::string& source);

    };

} // namespace Lexer
} // namespace FrontEnd

#endif // LEXER_HPP