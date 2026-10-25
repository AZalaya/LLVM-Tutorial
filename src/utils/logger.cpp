/**
 * @file logger.cpp
 * @author Andoni Zalaya (andoni.zalaya.c@gmail.com)
 * @brief Implementation file for the Logger class singleton. Used to print different kind of messages from the program.
 * @version 0.1
 * @date 2026-09-23
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "logger.hpp"

const Logger& Logger::GetLogger()
{
    static Logger logger;

    return logger;
}
