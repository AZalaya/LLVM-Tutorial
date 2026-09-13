/**
 * @file AST.cpp
 * @author Andoni Zalaya (andoni.zalaya.c@gmail.com)
 * @brief Implementation file for the Abstract Syntax Tree (AST) classes, which represent the structure of the parsed code.
 * @version 0.1
 * @date 2026-09-13
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "front_end/AST.hpp"

#include <iostream>
#include <memory>

namespace FrontEnd {
namespace AST {

    // NUMberNode implementation ===================================

    NumberNode::NumberNode(double value)
        : mValue(value) 
    {}

    void NumberNode::print() const 
    {
        std::cout << "NumberNode: " << mValue << std::endl;
    }

    // VariableNode implementation ===================================

    VariableNode::VariableNode(const std::string& name)
        : mName(name) 
    {}

    void VariableNode::print() const 
    {
        std::cout << "VariableNode: " << mName << std::endl;
    }

    // BinaryOpNode implementation ===================================

    BinaryOpNode::BinaryOpNode(char op, std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right)
        : mOp(op), mLeft(std::move(left)), mRight(std::move(right)) 
    {}

    void BinaryOpNode::print() const 
    {
        std::cout << "BinaryOpNode: " << mOp << std::endl;
        if (mLeft) {
            std::cout << "Left: ";
            mLeft->print();
        }
        if (mRight) {
            std::cout << "Right: ";
            mRight->print();
        }
    }

    // FunctionCallNode implementation ===================================

    FunctionCallNode::FunctionCallNode(const std::string& name, std::vector<std::unique_ptr<ASTNode>> arguments)
        : mName(name), mArguments(std::move(arguments)) 
    {}

    void FunctionCallNode::print() const 
    {
        std::cout << "FunctionCallNode: " << mName << std::endl;
        for (const std::unique_ptr<ASTNode>& arg : mArguments) {
            if (arg) {
                std::cout << "Argument: ";
                arg->print();
            }
        }
    }

    // FunctionDeclarationNode implementation ===================================

    FunctionDeclarationNode::FunctionDeclarationNode(const std::string& name, std::vector<std::string> arguments)
        : mName(name), mArguments(std::move(arguments)) 
    {}

    void FunctionDeclarationNode::print() const 
    {
        std::cout << "FunctionDeclarationNode: " << mName << std::endl;
        for (const std::string& arg : mArguments) {
            std::cout << "Argument: " << arg << std::endl;
        }
    }

    // FunctionDefinitionNode implementation ===================================

    FunctionDefinitionNode::FunctionDefinitionNode(std::unique_ptr<FunctionDeclarationNode> declaration, std::unique_ptr<ASTNode> body)
        : mDeclaration(std::move(declaration)), mBody(std::move(body)) 
    {}

    void FunctionDefinitionNode::print() const 
    {
        std::cout << "FunctionDefinitionNode: " << std::endl;
        if (mDeclaration) {
            std::cout << "Declaration: ";
            mDeclaration->print();
        }
        if (mBody) {
            std::cout << "Body: ";
            mBody->print();
        }
    }

} // namespace AST
} // namespace FrontEnd