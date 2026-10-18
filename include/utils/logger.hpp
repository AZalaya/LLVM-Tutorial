/**
 * @file logger.hpp
 * @author Andoni Zalaya (andoni.zalaya.c@gmail.com)
 * @brief Header file for the Logger class singleton.
 * @version 0.1
 * @date 2026-09-23
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include "definitions.hpp"

#include <string>

class Logger
{
private:
    /**
     * @brief Construct a new Logger object
     * 
     */
    Logger() = default;

    /**
     * @brief Destroy the Logger object
     * 
     */
    ~Logger() = default;

public:
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

public:
    /**
     * @brief Get the Logger object
     * 
     * @return Logger& Singleton logger reference
     */
    static const Logger& GetLogger();

    /**
     * @brief Log new message.
     * 
     * @param level Message level.
     * @param message Message string to print.
     */
    void Log(LogLevel level, const std::string& message) const;
};

#endif