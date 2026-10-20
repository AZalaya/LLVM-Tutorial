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
#include "logger.hpp"

#include <cstddef>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace FrontEnd {

Parser::Parser(const std::vector<Token>& tokens)
    : mTokens(tokens)
{
    GetNextToken(); // Get initial token
}

std::vector<std::unique_ptr<AST::ASTNode>> Parser::Parse()
{
    std::vector<std::unique_ptr<AST::ASTNode>> ast_nodes;

    while (mCurrentToken->mType != TokenType::eEOF) {
        switch (mCurrentToken->mType) 
        {
        case TokenType::eDEFINITION:
            ast_nodes.emplace_back(std::move(ParseDefinition()));

            if (ast_nodes.back() == nullptr) {
                Logger::GetLogger().Log(LogLevel::eERROR, "Failed to parse function definition");
                ast_nodes.pop_back();
                GetNextToken();
            }
            else {
                Logger::GetLogger().Log(LogLevel::eINFO, "Successfully parsed function definition");
            }

            break;

        case TokenType::eEXTERN:
            ast_nodes.emplace_back(std::move(ParseExtern()));

            if (ast_nodes.back() == nullptr) {
                Logger::GetLogger().Log(LogLevel::eERROR, "Failed to parse function declaration");
                ast_nodes.pop_back();
                GetNextToken();
            }
            else {
                Logger::GetLogger().Log(LogLevel::eINFO, "Successfully parsed function declaration");
            }

            break;

        case TokenType::eEOL:
            GetNextToken();

            break;

        default:
            ast_nodes.emplace_back(std::move(ParseAnonymousExpression()));

            if (ast_nodes.back() == nullptr) {
                Logger::GetLogger().Log(LogLevel::eERROR, "Failed to parse anonymous expression");
                ast_nodes.pop_back();
                GetNextToken();
            }
            else {
                Logger::GetLogger().Log(LogLevel::eINFO, "Successfully parsed anonymous expression");
            }

            break;
        }
    }

    return ast_nodes;
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

BinaryOperators Parser::GetBinaryOperatorPrecedence() const
{
    std::unordered_map<TokenType, BinaryOperators>::const_iterator it =
        TOKEN_TO_OPERATOR.find(mCurrentToken->mType);

    if (it != TOKEN_TO_OPERATOR.end()) {
        return it->second;
    }

    return BinaryOperators::eUNKNOWN;
}

std::unique_ptr<AST::FunctionDefinitionNode> Parser::ParseDefinition()
{
    GetNextToken();

    std::unique_ptr<AST::FunctionDeclarationNode> declaration = ParseDeclaration();
    if (declaration == nullptr) {
        return nullptr;
    }

    if (mCurrentToken->mType != TokenType::eCURLY_BRACE_START) {
        Logger::GetLogger().Log(LogLevel::eERROR, "Expected '{' after function declaration");
        return nullptr;
    }

    GetNextToken();

    std::vector<std::unique_ptr<AST::ASTNode>> expressions;
    while (mCurrentToken->mType != TokenType::eCURLY_BRACE_FINISH && mCurrentToken->mType != TokenType::eEOF) {
        std::unique_ptr<AST::ASTNode> statement = ParseExpression();

        if (statement != nullptr) {
            expressions.emplace_back(std::move(statement));
        }
        else {
            return nullptr;
        }
    }

    if (mCurrentToken->mType != TokenType::eCURLY_BRACE_FINISH) {
        Logger::GetLogger().Log(LogLevel::eERROR, "Expected '}' after function body");
        return nullptr;
    }

    GetNextToken(); // Consume the closing brace

    return std::make_unique<AST::FunctionDefinitionNode>(std::move(declaration), std::move(expressions));
}

std::unique_ptr<AST::FunctionDeclarationNode> Parser::ParseExtern()
{
    GetNextToken();

    return ParseDeclaration();
}

std::unique_ptr<AST::FunctionDefinitionNode> Parser::ParseAnonymousExpression()
{
    std::unique_ptr<AST::ASTNode> statement = ParseExpression();
    if (statement == nullptr) {
        return nullptr;
    }
    
    std::vector<std::unique_ptr<AST::ASTNode>> expressions;
    expressions.emplace_back(std::move(statement));

    std::unique_ptr<AST::FunctionDeclarationNode> declaration = 
        std::make_unique<AST::FunctionDeclarationNode>("_anon_expr", std::vector<std::string>{});

    return std::make_unique<AST::FunctionDefinitionNode>(std::move(declaration), std::move(expressions));
}

std::unique_ptr<AST::FunctionDeclarationNode> Parser::ParseDeclaration()
{
    if (mCurrentToken->mType != TokenType::eIDENTIFIER) {
        Logger::GetLogger().Log(LogLevel::eERROR, "Expected function name");
        return nullptr;
    }

    std::string function_name = mCurrentToken->mValue;

    GetNextToken();

    if (mCurrentToken->mType != TokenType::ePARENTHESIS_START) {
        Logger::GetLogger().Log(LogLevel::eERROR, "Expected '(' in function prototype");
        return nullptr;
    }

    GetNextToken();

    std::vector<std::string> arguments;
    while (mCurrentToken->mType == TokenType::eIDENTIFIER) {
        arguments.emplace_back(mCurrentToken->mValue);
        
        GetNextToken();
    }

    if (mCurrentToken->mType != TokenType::ePARENTHESIS_FINISH) {
        Logger::GetLogger().Log(LogLevel::eERROR, "Expected ')' in function prototype");
        return nullptr;
    }

    GetNextToken();

    return std::make_unique<AST::FunctionDeclarationNode>(function_name, arguments);
}

std::unique_ptr<AST::ASTNode> Parser::ParseExpression()
{
    std::unique_ptr<AST::ASTNode> lhs = ParsePrimaryExpression();

    if (lhs == nullptr) {
        return nullptr;
    }

    return ParseBinaryExpression(static_cast<BinaryOperators>(0), std::move(lhs));
}

std::unique_ptr<AST::ASTNode> Parser::ParsePrimaryExpression()
{
    switch (mCurrentToken->mType) 
    {
    case TokenType::eNUMBER:
        return ParseNumber();

    case TokenType::ePARENTHESIS_START:
        return ParseParenthesis();

    case TokenType::eIDENTIFIER:
        return ParseIdentifier();

    case TokenType::eIF:
        return ParseIfStatement();

    default:
        Logger::GetLogger().Log(LogLevel::eERROR, "Unknown token provided");
        return nullptr;
    }
}

std::unique_ptr<AST::ASTNode> Parser::ParseBinaryExpression(BinaryOperators precedence, std::unique_ptr<AST::ASTNode> lhs)
{
    while (true) {
        BinaryOperators lhs_precedence = GetBinaryOperatorPrecedence();

        if (lhs_precedence < precedence) {
            return lhs;
        }

        GetNextToken();

        std::unique_ptr<AST::ASTNode> rhs = ParsePrimaryExpression();

        if (rhs == nullptr) {
            return nullptr;
        }

        BinaryOperators rhs_precedence = GetBinaryOperatorPrecedence();

        if (lhs_precedence < rhs_precedence) {
            rhs = ParseBinaryExpression(lhs_precedence, std::move(rhs));

            if (rhs == nullptr) {
                return nullptr;
            }
        }

        lhs = std::make_unique<AST::BinaryOpNode>(lhs_precedence, std::move(lhs), std::move(rhs));
    }

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

    std::unique_ptr<AST::ASTNode> node = ParseExpression();

    if (mCurrentToken->mType != TokenType::ePARENTHESIS_FINISH) {
        Logger::GetLogger().Log(LogLevel::eERROR, "Expected ')'");
        return nullptr;
    }

    GetNextToken();

    return std::move(node);
}

std::unique_ptr<AST::ASTNode> Parser::ParseIdentifier()
{
    const std::string& name = mCurrentToken->mValue;

    GetNextToken();

    if (mCurrentToken->mType != TokenType::ePARENTHESIS_START) {
        return std::make_unique<AST::IdentifierNode>(name);
    }

    GetNextToken();

    std::vector<std::unique_ptr<AST::ASTNode>> arguments;

    if (mCurrentToken->mType != TokenType::ePARENTHESIS_FINISH) {
        while (true) {
            std::unique_ptr<AST::ASTNode> argument = ParseExpression();
            
            if (argument == nullptr) {
                return nullptr;
            }

            arguments.emplace_back(std::move(argument));    

            if (mCurrentToken->mType == TokenType::ePARENTHESIS_FINISH) {
                break;
            }

            if (mCurrentToken->mType != TokenType::eCOMMA) {
                Logger::GetLogger().Log(LogLevel::eERROR, "Expected ')' or ',' in argument list");
                return nullptr;
            }

            GetNextToken();
        }
    }

    GetNextToken();

    return std::make_unique<AST::FunctionCallNode>(name, std::move(arguments));
}

std::unique_ptr<AST::ASTNode> Parser::ParseIfStatement()
{
    GetNextToken();

    std::unique_ptr<AST::ASTNode> condition = ParseExpression();
    if (condition == nullptr) {
        Logger::GetLogger().Log(LogLevel::eERROR, "No valid condition provided for if statement");
        return nullptr;
    }

    if (mCurrentToken->mType != TokenType::eCURLY_BRACE_START) {
        Logger::GetLogger().Log(LogLevel::eERROR, "Expected '{' after if condition");
        return nullptr;
    }

    GetNextToken();

    std::vector<std::unique_ptr<AST::ASTNode>> then_branch;
    while (mCurrentToken->mType != TokenType::eCURLY_BRACE_FINISH && mCurrentToken->mType != TokenType::eEOF) {
        std::unique_ptr<AST::ASTNode> statement = ParseExpression();

        if (statement == nullptr) {
            return nullptr;
        }

        then_branch.emplace_back(std::move(statement));
    }

    if (mCurrentToken->mType != TokenType::eCURLY_BRACE_FINISH) {
        Logger::GetLogger().Log(LogLevel::eERROR, "Expected '}' after if block");
        return nullptr;
    }

    GetNextToken();

    std::vector<std::unique_ptr<AST::ASTNode>> else_branch;
    if (mCurrentToken->mType == TokenType::eELSE) {
        GetNextToken();

        if (mCurrentToken->mType != TokenType::eCURLY_BRACE_START) {
            Logger::GetLogger().Log(LogLevel::eERROR, "Expected '{' after else");
            return nullptr;
        }

        GetNextToken();

        while (mCurrentToken->mType != TokenType::eCURLY_BRACE_FINISH && mCurrentToken->mType != TokenType::eEOF) {
            std::unique_ptr<AST::ASTNode> statement = ParseExpression();

            if (statement == nullptr) {
                return nullptr;
            }

            else_branch.emplace_back(std::move(statement));
        }

        if (mCurrentToken->mType != TokenType::eCURLY_BRACE_FINISH) {
            Logger::GetLogger().Log(LogLevel::eERROR, "Expected '}' after else block");
            return nullptr;
        }

        GetNextToken();
    }

    return std::make_unique<AST::IfStatementNode>(std::move(condition), std::move(then_branch), std::move(else_branch));
}


} // namespace FrontEnd