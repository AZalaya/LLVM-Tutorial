/**
 * @file parser.hpp
 * @author Andoni Zalaya (andoni.zalaya.c@gmail.com)
 * @brief Header file for the Parser class. 
 * @version 0.1
 * @date 2026-09-23
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#ifndef PARSER_HPP
#define PARSER_HPP

#include "tokens.hpp"
#include "AST.hpp"

#include <memory>
#include <vector>

namespace FrontEnd {

    class Parser
    {
    public:
        /**
         * @brief Construct a new Parser object
         * 
         * @param tokens Vector of tokens from Lexer.
         */
        Parser(const std::vector<Token>& tokens);

        /**
         * @brief Destroy the Parser object
         * 
         */
        ~Parser() = default;

        /**
         * @brief Parse the token list associated with this Parser object.
         * 
         * @return std::unique_ptr<AST::ASTNode> 
         */
        std::unique_ptr<AST::ASTNode> Parse();

    private:
        /**
         * @brief Get the next token from the tokens list
         * 
         */
        void GetNextToken();

        /**
         * @brief Parse an expression.
         * 
         * @return std::unique_ptr<AST::ASTNode> Pointer to node in the AST.
         */
        std::unique_ptr<AST::ASTNode> ParseExpresion();

        /**
         * @brief Parse a numeric value.
         * 
         * @return std::unique_ptr<AST::ASTNode> Pointer to number node.
         */
        std::unique_ptr<AST::ASTNode> ParseNumber();

        /**
         * @brief Parse an expresion thats in between parenthesis.
         * 
         * @return std::unique_ptr<AST::ASTNode> Pointer to node in the AST.
         */
        std::unique_ptr<AST::ASTNode> ParseParenthesis();

        /**
         * @brief Parse an identifier.
         * 
         * @return std::unique_ptr<AST::ASTNode> Pointer to a variable node if the identifier was a variable.
         * @return std::unique_ptr<AST::ASTNode> Pointer to a call node if the identifier was a function call.
         */
        std::unique_ptr<AST::ASTNode> ParseIdentifier();

    private:
        const std::vector<Token>& mTokens;
        const Token* mCurrentToken;

    };

} // namespace FrontEnd

#endif
