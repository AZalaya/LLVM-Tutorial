/**
 * @file main.cpp
 * @author Andoni Zalaya (andoni.zalaya.c@gmail.com)
 * @brief Main entry point for the lexer application. This program reads a source file, tokenizes its content using the Lexer class, and prints the resulting tokens to the console.
 * @version 0.1
 * @date 2026-09-13
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "lexer.hpp"

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

#if defined(DEBUG) || defined(VERBOSE)
    // Print the tokens
    for (const FrontEnd::Token& token : tokens) {
        std::cout << token.ToString() << std::endl; // Assuming Token has a toString() method
    }
#endif

    return 0;
}