/**
 * @file parser.cpp
 * @author Andoni Zalaya (andoni.zalaya.c@gmail.com)
 * @brief Implementation file for the Parser class.
 * @version 0.1
 * @date 2026-09-23
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "parser.hpp"
#include "AST.hpp"
#include "logger.hpp"

#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

namespace FrontEnd {

Parser::Parser(const std::vector<Token>& tokens)
    : mTokens(tokens)
{}

std::unique_ptr<AST::ASTNode> Parser::Parse()
{
    return nullptr;
}


void Parser::GetNextToken()
{
    static size_t current_token_index = static_cast<size_t>(-1);

    if (++current_token_index < mTokens.size()) {
        mCurrentToken = &(mTokens.at(current_token_index));
    }
    else {
        mCurrentToken = nullptr;
    }
}


std::unique_ptr<AST::ASTNode> ParseExpresion()
{
    return nullptr;
}

std::unique_ptr<AST::ASTNode> Parser::ParseNumber()
{
    std::unique_ptr<AST::NumberNode> node =
        std::make_unique<AST::NumberNode>(mCurrentToken->mValue);

    GetNextToken();

    return std::move(node);
}

std::unique_ptr<AST::ASTNode> Parser::ParseParenthesis()
{
    GetNextToken();

    std::unique_ptr<AST::ASTNode> node = ParseExpresion();

    if (mCurrentToken->mValue != ")") {
        Logger::GetLogger().Log(Logger::LogLevel::eERROR, "Expected ')'");
        return nullptr;
    }

    GetNextToken();

    return std::move(node);
}

std::unique_ptr<AST::ASTNode> Parser::ParseIdentifier()
{
    const std::string& name = mCurrentToken->mValue;

    GetNextToken();

    if (mCurrentToken->mValue != "(") {
        return std::make_unique<AST::VariableNode>(name);
    }

    GetNextToken();

    std::vector<std::unique_ptr<AST::ASTNode>> arguments;

    if (mCurrentToken->mValue != ")") {
        while (true) {
            std::unique_ptr<AST::ASTNode> argument = ParseExpresion();
            
            if (argument != nullptr) {
                arguments.emplace_back(std::move(argument));
            }
            else {
                return nullptr;
            }

            if (mCurrentToken->mValue == ")") {
                break;
            }

            if (mCurrentToken->mValue != ",") {
                Logger::GetLogger().Log(Logger::LogLevel::eERROR, "Expected ')' or ',' in argument list");
                return nullptr;
            }

            GetNextToken();
        }
    }

    GetNextToken();

    return std::make_unique<AST::FunctionCallNode>(name, std::move(arguments));
}


} // namespace FrontEnd