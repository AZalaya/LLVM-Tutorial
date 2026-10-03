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

    std::string NumberNode::ToString() const 
    {
        return "NumberNode: " + std::to_string(mValue) + '\n';
    }

    std::size_t NumberNode::getValue() const
    {
        return mValue;
    }

    // VariableNode implementation ===================================

    VariableNode::VariableNode(const std::string& name)
        : mName(name) 
    {}

    std::string VariableNode::ToString() const 
    {
        return "VariableNode: " + mName + '\n';
    }

    // BinaryOpNode implementation ===================================

    BinaryOpNode::BinaryOpNode(BinaryOperators op, std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right)
        : mOp(op), mLeft(std::move(left)), mRight(std::move(right)) 
    {}

    std::string BinaryOpNode::ToString() const 
    {
        std::string result = "BinaryOpNode: " + std::string(BINARY_OPERATOR_TO_STRING.at(mOp)) + '\n';
        if (mLeft) {
            result += mLeft->ToString();
        }
        if (mRight) {
            result += mRight->ToString();
        }
        
        return result;
    }

    // FunctionCallNode implementation ===================================

    FunctionCallNode::FunctionCallNode(const std::string& name, std::vector<std::unique_ptr<ASTNode>> arguments)
        : mName(name), mArguments(std::move(arguments)) 
    {}

    std::string FunctionCallNode::ToString() const 
    {
        std::string result = "FunctionCallNode: " + mName + '\n';
        for (const std::unique_ptr<ASTNode>& arg : mArguments) {
            if (arg) {
                result += arg->ToString();
            }
        }
        
        return result;
    }

    // FunctionDeclarationNode implementation ===================================

    FunctionDeclarationNode::FunctionDeclarationNode(const std::string& name, std::vector<std::string> arguments)
        : mName(name), mArguments(std::move(arguments)) 
    {}

    std::string FunctionDeclarationNode::ToString() const 
    {
        std::string result = "FunctionDeclarationNode: " + mName + '\n';
        for (const std::string& arg : mArguments) {
            result += "Argument: " + arg + '\n';
        }
        
        return result;
    }

    // FunctionDefinitionNode implementation ===================================

    FunctionDefinitionNode::FunctionDefinitionNode(std::unique_ptr<FunctionDeclarationNode> declaration, std::unique_ptr<ASTNode> body)
        : mDeclaration(std::move(declaration)), mBody(std::move(body)) 
    {}

    std::string FunctionDefinitionNode::ToString() const 
    {
        std::string result = "FunctionDefinitionNode: \n";
        if (mDeclaration) {
            result += mDeclaration->ToString();
        }
        if (mBody) {
            result += mBody->ToString();
        }

        return result;
    }

} // namespace AST
} // namespace FrontEnd