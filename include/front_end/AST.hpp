/**
 * @file AST.hpp
 * @author Andoni Zalaya (andoni.zalaya.c@gmail.com)
 * @brief Abstract Syntax Tree classes for representing the structure of the parsed code.
 * @version 0.1
 * @date 2026-09-13
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#ifndef AST_HPP
#define AST_HPP

#include "definitions.hpp"

#include <string>
#include <memory>
#include <vector>

namespace FrontEnd {
namespace AST {

    class ASTNode 
    {
    public:
        /**
         * @brief Destroy the ASTNode object.
         * 
         */
        virtual ~ASTNode() = default;

        /**
         * @brief Prints the AST node.
         * 
         */
        virtual std::string ToString() const = 0;

    };

    class NumberNode : public ASTNode 
    {
    public:
        /**
         * @brief Construct a new NumberNode object.
         * 
         * @param value The value of the number node.
         */
        NumberNode(const std::string& value);

        /**
         * @brief Prints the number node.
         * 
         */
        std::string ToString() const override;

        template <typename T = std::size_t>
        T getValue() const;

    private:
        std::size_t mValue;
        NumericType mNumericType;
        
    };

    class VariableNode : public ASTNode 
    {
    public:
        /**
         * @brief Construct a new VariableNode object.
         * 
         * @param name The name of the variable node.
         */
        VariableNode(const std::string& name);

        /**
         * @brief Prints the variable node.
         * 
         */
        std::string ToString() const override;

    private:
        std::string mName;

    };

    class BinaryOpNode : public ASTNode 
    {
    public:
        /**
         * @brief Construct a new BinaryOpNode object.
         *
         * @param op The operator of the binary operation.
         * @param left The left operand of the binary operation.
         * @param right The right operand of the binary operation.
         */
        BinaryOpNode(BinaryOperators op, std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right);

        /**
         * @brief Prints the binary operation node.
         * 
         */
        std::string ToString() const override;

    private:
        BinaryOperators mOp;
        std::unique_ptr<ASTNode> mLeft;
        std::unique_ptr<ASTNode> mRight;
    };

    class IfStatementNode : public ASTNode 
    {
    public:
        /**
         * @brief Construct a new IfStatementNode object.
         * 
         * @param condition The condition of the if statement.
         * @param then_branch The statements to execute if the condition is true.
         * @param else_branch The statements to execute if the condition is false.
         */
        IfStatementNode(std::unique_ptr<ASTNode> condition, std::vector<std::unique_ptr<ASTNode>> then_branch, std::vector<std::unique_ptr<ASTNode>> else_branch);

        /**
         * @brief Prints the if statement node.
         * 
         */
        std::string ToString() const override;

    private:
        std::unique_ptr<ASTNode> mCondition;
        std::vector<std::unique_ptr<ASTNode>> mThenBranch;
        std::vector<std::unique_ptr<ASTNode>> mElseBranch;
    };

    class FunctionCallNode : public ASTNode 
    {
    public:
        /**
         * @brief Construct a new FunctionCallNode object. 
         * @param name The name of the function.
         * @param arguments The arguments of the function.
         */
        FunctionCallNode(const std::string& name, std::vector<std::unique_ptr<ASTNode>> arguments);

        /**
         * @brief Prints the function call node.
         * 
         */
        std::string ToString() const override;

    private:
        std::string mName;
        std::vector<std::unique_ptr<ASTNode>> mArguments;
    };

    class FunctionDeclarationNode : public ASTNode 
    {
    public:
        /**
         * @brief Construct a new Function Declaration Node object
         * 
         * @param name The name of the function.
         * @param arguments The arguments of the function.
         */
        FunctionDeclarationNode(const std::string& name, std::vector<std::string> arguments);

        /**
         * @brief Prints the function declaration node.
         * 
         */
        std::string ToString() const override;

    private:
        std::string mName;
        std::vector<std::string> mArguments;

    };

    class FunctionDefinitionNode : public ASTNode 
    {
    public:
        /**
         * @brief Construct a new Function Definition Node object
         * 
         * @param declaration The function declaration node.
         * @param body The body of the function.
         */
        FunctionDefinitionNode(std::unique_ptr<FunctionDeclarationNode> declaration, std::vector<std::unique_ptr<ASTNode>> body);

        /**
         * @brief Prints the function definition node.
         * 
         */
        std::string ToString() const override;

    private:
        std::unique_ptr<FunctionDeclarationNode> mDeclaration;
        std::vector<std::unique_ptr<ASTNode>> mBody;
    };

} // namespace AST
} // namespace FrontEnd

#endif // AST_HPP