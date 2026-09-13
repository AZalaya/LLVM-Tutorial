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
        virtual void print() const = 0;

    };

    class NumberNode : public ASTNode 
    {
    public:
        /**
         * @brief Construct a new NumberNode object.
         * 
         * @param value The value of the number node.
         */
        NumberNode(double value);

        /**
         * @brief Prints the number node.
         * 
         */
        void print() const override;

    private:
        double mValue;
        
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
        void print() const override;

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
        BinaryOpNode(char op, std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right);

        /**
         * @brief Prints the binary operation node.
         * 
         */
        void print() const override;

    private:
        char mOp;
        std::unique_ptr<ASTNode> mLeft;
        std::unique_ptr<ASTNode> mRight;
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
        void print() const override;

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
        void print() const override;

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
        FunctionDefinitionNode(std::unique_ptr<FunctionDeclarationNode> declaration, std::unique_ptr<ASTNode> body);

        /**
         * @brief Prints the function definition node.
         * 
         */
        void print() const override;

    private:
        std::unique_ptr<FunctionDeclarationNode> mDeclaration;
        std::unique_ptr<ASTNode> mBody;
    };

} // namespace AST
} // namespace FrontEnd

#endif // AST_HPP