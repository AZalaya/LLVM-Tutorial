/**
 * @file main.cpp
 * @author Andoni Zalaya (andoni.zalaya.c@gmail.com)
 * @brief Main entry point for the CKal compiler application.
 * @version 0.1
 * @date 2026-09-13
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "logger.hpp"
#include "lexer.hpp"
#include "parser.hpp"

#include <llvm/IR/Value.h>
#include <llvm/Support/raw_ostream.h>

#include <iostream>
#include <fstream>

int main(int argc, char** argv) 
{
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <source_file>" << std::endl;
        return 1;
    }

    std::string sourceFile = argv[1];
    std::ifstream file(sourceFile);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << sourceFile << std::endl;
        return 1;
    }

    std::string sourceCode((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    // Tokenize the source code
    std::vector<FrontEnd::Token> tokens = FrontEnd::Lexer::Lexer::Tokenize(sourceCode);

#if defined(DEBUG)
    // Print the tokens
    for (const FrontEnd::Token& token : tokens) {
        Logger::GetLogger().Log(LogLevel::eDEBUG, token.ToString());
    }
#endif

    std::vector<std::unique_ptr<FrontEnd::AST::ASTNode>> ast_nodes = 
        FrontEnd::Parser(tokens).Parse();

#if defined(DEBUG)
    // Print the AST nodes
    for (const std::unique_ptr<FrontEnd::AST::ASTNode>& node : ast_nodes) {
        Logger::GetLogger().Log(LogLevel::eDEBUG, node->ToString());
        node->CodeGeneration()->print(llvm::outs());
    }
#endif

    return 0;
}