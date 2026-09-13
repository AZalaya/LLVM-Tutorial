/**
 * @file lexer.cpp
 * @author Andoni Zalaya (andoni.zalaya.c@gmail.com)
 * @brief Implementation file for the lexer class, which is responsible for tokenizing the input source code.
 * @version 0.1
 * @date 2026-09-13
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "front_end/lexer.hpp"

namespace FrontEnd {
namespace Lexer {

    std::vector<Token> Lexer::Tokenize(const std::string& source) 
    {
        std::vector<Token> tokens;

        for (size_t i = 0; i < source.size(); ++i) {
            char currentChar = source[i];

            if (isalpha(currentChar)) {
                std::string identifier;

                while (i < source.size() && isalnum(source[i])) {
                    identifier += source[i];
                    ++i;
                }
                
                --i; // Adjust for the extra increment in the loop

                if (identifier == "def") {
                    tokens.emplace_back(TokenType::eDEFINITION, identifier);
                } 
                else if (identifier == "extern") {
                    tokens.emplace_back(TokenType::eEXTERN, identifier);
                } 
                else {
                    tokens.emplace_back(TokenType::eIDENTIFIER, identifier);
                }
            } 
            else if (isdigit(currentChar)) {
                std::string number;

                while (i < source.size() && (isdigit(source[i]) || source[i] == '.')) {
                    number += source[i];
                    ++i;
                }

                --i; // Adjust for the extra increment in the loop
                tokens.emplace_back(TokenType::eNUMBER, number);
            } 
            else if (currentChar == '#') {
                // Skip comments
                while (i < source.size() && source[i] != '\n' && source[i] != '\r') {
                    ++i;
                }
            } 
            else if (isspace(currentChar)) {
                // Skip whitespace
                continue;
            }
            else {
                tokens.emplace_back(TokenType::eUNKNOWN, std::string(1, currentChar));
            }
        }

        tokens.emplace_back(TokenType::eEOF, "");

        return tokens;
    }

} // namespace Lexer
} // namespace FrontEnd