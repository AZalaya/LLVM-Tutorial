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

#include "lexer.hpp"

#include "definitions.hpp"

namespace FrontEnd {
namespace Lexer {

    std::vector<Token> Lexer::Tokenize(const std::string& source) 
    {
        std::vector<Token> tokens;

        for (size_t i = 0; i < source.size(); ++i) {
            char currentChar = source[i];

            // [a-zA-Z]*
            if (std::isalpha(currentChar)) {
                std::string identifier;

                while (i < source.size() && std::isalnum(source[i])) {
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
                else if (identifier == "if") {
                    tokens.emplace_back(TokenType::eIF, identifier);
                } 
                else if (identifier == "else") {
                    tokens.emplace_back(TokenType::eELSE, identifier);
                } 
                else {
                    tokens.emplace_back(TokenType::eIDENTIFIER, identifier);
                }
            }
            // [0-9]*[ϵ|\.][0-9]*[ϵ|u|U][ϵ|f|F|d|D|l|L|i|I|s|S|c|C]
            else if (std::isdigit(currentChar) || currentChar == '.') {
                bool dot_used = false;
                std::string number;

                while (i < source.size()) {
                    if (source[i] == '.') {
                        if (dot_used) { 
                            break; 
                        }

                        dot_used = true;
                    }
                    else if (!isdigit(source[i])) {
                        break;
                    }
                    
                    number += source[i];
                    ++i;
                }

                if (source[i] == 'u' || source[i] == 'U') {
                    number += source[i];
                    ++i;
                }

                if (source[i] == 'f' || source[i] == 'F' ||
                    source[i] == 'd' || source[i] == 'D' ||
                    source[i] == 'l' || source[i] == 'L' ||
                    source[i] == 'i' || source[i] == 'I' ||
                    source[i] == 's' || source[i] == 'S' ||
                    source[i] == 'c' || source[i] == 'C') {
                    number += source[i];
                    ++i;
                }

                --i; // Adjust for the extra increment in the loop
                tokens.emplace_back(TokenType::eNUMBER, number);
            } 
            else if (currentChar == '(') {
                tokens.emplace_back(TokenType::ePARENTHESIS_START, std::string(1, currentChar));
            }
            else if (currentChar == ')') {
                tokens.emplace_back(TokenType::ePARENTHESIS_FINISH, std::string(1, currentChar));
            }
            else if (currentChar == '{') {
                tokens.emplace_back(TokenType::eCURLY_BRACE_START, std::string(1, currentChar));
            }
            else if (currentChar == '}') {
                tokens.emplace_back(TokenType::eCURLY_BRACE_FINISH, std::string(1, currentChar));
            }
            else if (currentChar == '[') {
                tokens.emplace_back(TokenType::eSQUARE_BRACKET_START, std::string(1, currentChar));
            }
            else if (currentChar == ']') {
                tokens.emplace_back(TokenType::eSQUARE_BRACKET_FINISH, std::string(1, currentChar));
            }
            else if (currentChar == '<') {
                tokens.emplace_back(TokenType::eLESS_THAN, std::string(1, currentChar));
            }
            else if (currentChar == '+') {
                tokens.emplace_back(TokenType::eSUM, std::string(1, currentChar));
            }
            else if (currentChar == '-') {
                tokens.emplace_back(TokenType::eMINUS, std::string(1, currentChar));
            }
            else if (currentChar == '*') {
                tokens.emplace_back(TokenType::eMULTIPLY, std::string(1, currentChar));
            }
            else if (currentChar == ';') {
                tokens.emplace_back(TokenType::eEOL, std::string(1, currentChar));
            }
            else if (currentChar == ',') {
                tokens.emplace_back(TokenType::eCOMMA, std::string(1, currentChar));
            }
            else if (isspace(currentChar)) {
                // Skip whitespace
                continue;
            }
            else if (currentChar == '#') {
                // Skip comments
                while (i < source.size() && source[i] != '\n' && source[i] != '\r') {
                    ++i;
                }
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