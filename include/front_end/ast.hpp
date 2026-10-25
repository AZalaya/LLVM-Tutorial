/**
 * @file ast.hpp
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

#include <llvm/IR/Value.h>

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
         * @brief Convert the AST node to a string to be printed.
         * 
         */
        virtual void Print() const = 0;

        /**
         * @brief Generates LLVM IR code for the AST node.
         * 
         * @return llvm::Value* The generated LLVM IR value.
         */
        virtual llvm::Value* CodeGeneration() const = 0;

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
         * @brief Gets the value of the number node.
         * 
         * @tparam T The type of the value to return (default is std::size_t).
         * @return T The value of the number node.
         */
        template <typename T = std::size_t>
        T getValue() const;

        /**
         * @brief Gets the numeric type of the number node.
         * 
         * @return NumericType The numeric type of the number node.
         */
        NumericType getNumericType() const;

        /**
         * @brief Convert the number node to a string to be printed.
         * 
         */
        void Print() const override;

        /**
         * @brief Generates LLVM IR code for the number node.
         * 
         * @return llvm::Value* The generated LLVM IR value.
         */
        llvm::Value* CodeGeneration() const override;

    private:
        std::size_t mValue;
        NumericType mNumericType;
        
    };

    class IdentifierNode : public ASTNode 
    {
    public:
        /**
         * @brief Construct a new IdentifierNode object.
         * 
         * @param name The name of the identifier node.
         */
        IdentifierNode(const std::string& name);

        /**
         * @brief Gets the name of the identifier node.
         * 
         * @return std::string The name of the identifier node.
         */
        const std::string& getName() const;

        /**
         * @brief Convert the identifier node to a string to be printed.
         * 
         */
        void Print() const override;
        
        /**
         * @brief Generates LLVM IR code for the identifier node.
         * 
         * @return llvm::Value* The generated LLVM IR value.
         */
        llvm::Value* CodeGeneration() const override;

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
         * @brief Gets the operator of the binary operation.
         * 
         * @return BinaryOperators The operator of the binary operation.
         */
        BinaryOperators getOperator() const;
        
        /**
         * @brief Gets the left operand of the binary operation.
         * 
         * @return const std::unique_ptr<ASTNode>& The left operand of the binary operation.
         */
        const std::unique_ptr<ASTNode>& getLHS() const;
        
        /**
         * @brief Gets the right operand of the binary operation.
         * 
         * @return const std::unique_ptr<ASTNode>& The right operand of the binary operation.
         */
        const std::unique_ptr<ASTNode>& getRHS() const;

        /**
         * @brief Convert the binary operation node to a string to be printed.
         * 
         */
        void Print() const override;

        /**
         * @brief Generates LLVM IR code for the binary operation node.
         * 
         * @return llvm::Value* The generated LLVM IR value.
         */
        llvm::Value* CodeGeneration() const override;

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
         * @brief Gets the condition of the if statement.
         * 
         * @return const std::unique_ptr<ASTNode>& The condition of the if statement.
         */
        const std::unique_ptr<ASTNode>& getCondition() const;

        /**
         * @brief Gets the statements to execute if the condition is true.
         * 
         * @return const std::vector<std::unique_ptr<ASTNode>>& The statements to execute if the condition is true.
         */
        const std::vector<std::unique_ptr<ASTNode>>& getThenBranch() const;

        /**
         * @brief Gets the statements to execute if the condition is false.
         * 
         * @return const std::vector<std::unique_ptr<ASTNode>>& The statements to execute if the condition is false.
         */
        const std::vector<std::unique_ptr<ASTNode>>& getElseBranch() const;

        /**
         * @brief Convert the if statement node to a string to be printed.
         * 
         */
        void Print() const override;
        
        /**
         * @brief Generates LLVM IR code for the if statement node.
         * 
         * @return llvm::Value* The generated LLVM IR value.
         */
        llvm::Value* CodeGeneration() const override;

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
         * @brief Gets the name of the function being called.
         * 
         * @return const std::string& The name of the function.
         */
        const std::string& getName() const;

        /**
         * @brief Gets the arguments of the function call.
         * 
         * @return const std::vector<std::unique_ptr<ASTNode>>& The arguments of the function call.
         */
        const std::vector<std::unique_ptr<ASTNode>>& getArguments() const;

        /**
         * @brief Convert the function call node to a string to be printed.
         * 
         */
        void Print() const override;

        /**
         * @brief Generates LLVM IR code for the function call node.
         * 
         * @return llvm::Value* The generated LLVM IR value.
         */
        llvm::Value* CodeGeneration() const override;

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
         * @brief Gets the name of the function being declared.
         * 
         * @return const std::string& The name of the function.
         */
        const std::string& getName() const;

        /**
         * @brief Gets the arguments of the function declaration.
         * 
         * @return const std::vector<std::string>& The arguments of the function declaration.
         */
        const std::vector<std::string>& getArguments() const;

        /**
         * @brief Convert the function declaration node to a string to be printed.
         * 
         */
        void Print() const override;

        /**
         * @brief Generates LLVM IR code for the function declaration node.
         * 
         * @return llvm::Value* The generated LLVM IR value.
         */
        llvm::Value* CodeGeneration() const override;

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
         * @brief Gets the function declaration node.
         * 
         * @return const std::unique_ptr<FunctionDeclarationNode>& The function declaration node.
         */
        const std::unique_ptr<FunctionDeclarationNode>& getDeclaration() const;
        
        /**
         * @brief Gets the body of the function definition.
         * 
         * @return const std::vector<std::unique_ptr<ASTNode>>& The body of the function definition.
         */
        const std::vector<std::unique_ptr<ASTNode>>& getBody() const;

        /**
         * @brief Convert the function definition node to a string to be printed.
         * 
         */
        void Print() const override;

        /**
         * @brief Generates LLVM IR code for the function definition node.
         * 
         * @return llvm::Value* The generated LLVM IR value.
         */
        llvm::Value* CodeGeneration() const override;

    private:
        std::unique_ptr<FunctionDeclarationNode> mDeclaration;
        std::vector<std::unique_ptr<ASTNode>> mBody;
    };

} // namespace AST
} // namespace FrontEnd

#include "ast.inl"

#endif // AST_HPP