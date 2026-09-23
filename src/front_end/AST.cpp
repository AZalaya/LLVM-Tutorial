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

#include "AST.hpp"

#include <iostream>
#include <memory>
#include <string>

namespace FrontEnd {
namespace AST {

    // NUMberNode implementation ===================================

    NumberNode::NumberNode(const std::string& value)
    : mValue(0)
    , mNumericType(NumericType::eUNKNOWN)
    {
        std::size_t type = 0;

        if (value.at(value.size() - 1) == 'u' || value.at(value.size() - 1) == 'U' ||
            value.at(value.size() - 2) == 'u' || value.at(value.size() - 2) == 'U') {
            type |= UNSIGNED;
        }
        else {
            type |= SIGNED;
        }

        std::size_t point = value.find('.');

        if (point != std::string::npos) {
            if (value.back() == 'f' || value.back() == 'F') {
                type |=  FLOATING_POINT | SIZE_32;
                float tempValue = std::stof(value);
                std::memcpy(&mValue, &tempValue, sizeof(tempValue));
            } 
            else if (value.back() == 'd' || value.back() == 'D') {
                type |= FLOATING_POINT | SIZE_64;
                double tempValue = std::stof(value);
                std::memcpy(&mValue, &tempValue, sizeof(tempValue));
            } 
            else {
                type |= FLOATING_POINT | SIZE_64; // Default to FLOAT64 for floating-point values
                double tempValue = std::stof(value);
                std::memcpy(&mValue, &tempValue, sizeof(tempValue));
            }
        } 
        else if (value.back() == 'f' || value.back() == 'F') {
            type |=  FLOATING_POINT | SIZE_32;
            float tempValue = std::stof(value);
            std::memcpy(&mValue, &tempValue, sizeof(tempValue));
        } 
        else if (value.back() == 'd' || value.back() == 'D') {
            type |= FLOATING_POINT | SIZE_64;
            double tempValue = std::stof(value);
            std::memcpy(&mValue, &tempValue, sizeof(tempValue));
        } 
        else if (value.back() == 'l' || value.back() == 'L') {
            type |= INTEGER | SIZE_64;
            long tempValue = std::stol(value);
            std::memcpy(&mValue, &tempValue, sizeof(tempValue));
        } 
        else if (value.back() == 'i' || value.back() == 'I') {
            type |= INTEGER | SIZE_32;
            int tempValue = std::stoi(value);
            std::memcpy(&mValue, &tempValue, sizeof(tempValue));
        } 
        else if (value.back() == 's' || value.back() == 'S') {
            type |= INTEGER | SIZE_16;
            short tempValue = static_cast<short>(std::stoi(value));
            std::memcpy(&mValue, &tempValue, sizeof(tempValue));
        } 
        else if (value.back() == 'c' || value.back() == 'C') {
            type |= INTEGER | SIZE_8;
            char tempValue = static_cast<char>(std::stoi(value));
            std::memcpy(&mValue, &tempValue, sizeof(tempValue));
        } 
        else {
            type |= INTEGER | SIZE_32; // Default to INT32 for integer values
            int tempValue = std::stoi(value);
            std::memcpy(&mValue, &tempValue, sizeof(tempValue));
        }

        mNumericType = static_cast<NumericType>(type);
    }

    void NumberNode::print() const 
    {
        std::cout << "NumberNode: " << mValue << std::endl;
    }

    std::size_t NumberNode::getValue() const
    {
        return mValue;
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