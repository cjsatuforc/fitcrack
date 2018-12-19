/**
 * @file Config.cpp
 * @brief Source file for Config
 * @authors Lukas Zobal (zobal.lukas(at)gmail.com)
 * @date 12. 12. 2018
 * @license MIT, see LICENSE
 */

#include <Config.h>

#include <ctime>
#include <iostream>
#include <cstdarg>
#include <iomanip>
#include <sstream>
#include <cstdio>


/** Initialization of static private map */
std::map<std::pair<uint64_t, uint64_t>, std::ifstream* > Tools::m_fd_map;


namespace Config {
    /** Table names, for easy configuration */
    std::string tableNameBenchmark =    "fc_benchmark";
    std::string tableNameJob =          "fc_job";
    std::string tableNamePackage =      "fc_package";
    std::string tableNameHost =         "fc_host";
    std::string tableNameHostActivity = "fc_host_activity";
    std::string tableNameMask =         "fc_mask";
    std::string tableNameSettings =     "fc_settings";
    std::string tableNameDictionary =   "fc_dictionary";
    std::string tableNamePckgDictionary = "fc_package_dictionary";
    std::string tableNameHash =         "fc_hash";

    /** Path to dictionaries */
    std::string dictDir = "/usr/share/collections/dictionaries/";
    std::string markovDir = "/usr/share/collections/markov/";
    std::string rulesDir = "/usr/share/collections/rules/";

    /** Template names*/
    std::string inTemplateFileBench =      "bench_in";
    std::string inTemplateFileMask =       "mask_in";
    std::string inTemplateFileMarkov =     "markov_in";
    std::string inTemplateFileDict =       "dict_in";
    std::string inTemplateFileCombinator = "combinator_in";
    std::string inTemplateFileRule =       "rule_in";
    std::string outTemplateFile =          "app_out";

    /** Some default values */
    char * inTemplatePathBench = nullptr;
    char * inTemplatePathMask = nullptr;
    char * inTemplatePathMarkov = nullptr;
    char * inTemplatePathDict = nullptr;
    char * inTemplatePathCombinator = nullptr;
    char * inTemplatePathRule = nullptr;
    DB_APP * app = nullptr;
    int startTime = 0;
    int seqNo = 0;
    char * appName = nullptr;
    std::string projectDir = "";
}

void Tools::printDebug(const char* format, ...)
{
    va_list argPtr;
    va_start(argPtr, format);
    vfprintf(stderr, format, argPtr);
    va_end(argPtr);
}


void Tools::printDebugTimestamp(const char *format, ...)
{
    std::time_t timeNow = std::time(nullptr);
    std::cerr << std::put_time(std::localtime(&timeNow), "%d.%m.%Y %H:%M:%S ");

    va_list argPtr;
    va_start(argPtr, format);
    vfprintf(stderr, format, argPtr);
    va_end(argPtr);
}


void Tools::printDebugPackage(Config::DebugType type, uint64_t packageId, const char *format, ...)
{
    std::stringstream tmpStream;
    switch (type)
    {
        case Config::DebugType::Log:
            tmpStream << "LOG: ";
            break;
        case Config::DebugType::Warn:
            tmpStream << "WARN: ";
            break;
        case Config::DebugType::Error:
            tmpStream << "ERROR: ";
        default:
            break;
    }

    std::time_t timeNow = std::time(nullptr);
    tmpStream << std::put_time(std::localtime(&timeNow), "%d.%m.%Y %H:%M:%S ");
    tmpStream << "[Package #" << packageId << "] ";

    std::cerr << tmpStream.str();

    va_list argPtr;
    va_start(argPtr, format);
    vfprintf(stderr, format, argPtr);

    if (type == Config::DebugType::Warn)
    {
        FILE * warnFile;
        warnFile = std::fopen((Config::projectDir + "work_generator_warn.log").c_str(), "a");
        if (warnFile != nullptr)
        {
            std::fputs(tmpStream.str().c_str(), warnFile);
            va_start(argPtr, format);
            vfprintf(warnFile, format, argPtr);
            std::fclose(warnFile);
        }
        else
        {
            std::cerr << "Note: Failed to open additional log." << std::endl;
        }
    }
    else if (type == Config::DebugType::Error)
    {
        FILE * errorFile;
        errorFile = std::fopen((Config::projectDir + "work_generator_error.log").c_str(), "a");
        if (errorFile != nullptr)
        {
            std::fputs(tmpStream.str().c_str(), errorFile);
            va_start(argPtr, format);
            vfprintf(errorFile, format, argPtr);
            std::fclose(errorFile);
        }
        else
        {
            std::cerr << "Note: Failed to open additional log." << std::endl;
        }
    }

    va_end(argPtr);
}



void Tools::printDebugHost(Config::DebugType type, uint64_t packageId, uint64_t hostId, const char *format, ...)
{
    std::stringstream tmpStream;
    switch (type)
    {
        case Config::DebugType::Log:
            tmpStream << "LOG: ";
            break;
        case Config::DebugType::Warn:
            tmpStream << "WARN: ";
            break;
        case Config::DebugType::Error:
            tmpStream << "ERROR: ";
        default:
            break;
    }

    std::time_t timeNow = std::time(nullptr);
    tmpStream << std::put_time(std::localtime(&timeNow), "%d.%m.%Y %H:%M:%S ");
    tmpStream << "[Package #" << packageId << "][Host #" << hostId << "] ";

    std::cerr << tmpStream.str();

    va_list argPtr;
    va_start(argPtr, format);
    vfprintf(stderr, format, argPtr);

    if (type == Config::DebugType::Warn)
    {
        FILE * warnFile;
        warnFile = std::fopen((Config::projectDir + "work_generator_warn.log").c_str(), "a");
        if (warnFile != nullptr)
        {
            std::fputs(tmpStream.str().c_str(), warnFile);
            va_start(argPtr, format);
            vfprintf(warnFile, format, argPtr);
            std::fclose(warnFile);
        }
        else
        {
            std::cerr << "Note: Failed to open additional log." << std::endl;
        }
    }
    else if (type == Config::DebugType::Error)
    {
        FILE * errorFile;
        errorFile = std::fopen((Config::projectDir + "work_generator_error.log").c_str(), "a");
        if (errorFile != nullptr)
        {
            std::fputs(tmpStream.str().c_str(), errorFile);
            va_start(argPtr, format);
            vfprintf(errorFile, format, argPtr);
            std::fclose(errorFile);
        }
        else
        {
            std::cerr << "Note: Failed to open additional log." << std::endl;
        }
    }

    va_end(argPtr);
}



void Tools::printDbMap(DbMap & map)
{
    Tools::printDebug("Map contents:\n");
    for (auto & item : map)
        Tools::printDebug("%s : %s\n", item.first.c_str(), item.second.c_str());
}


std::ifstream * Tools::getStream(uint64_t packageId, uint64_t dictId, std::string dictFileName)
{
    /** Search for opened file descriptor */
    for(auto & entry : m_fd_map)
    {
        if(entry.first.first == packageId && entry.first.second == dictId)
            return entry.second;
    }

    /** Open a new one */
    auto newStream = new std::ifstream(Config::dictDir + dictFileName);
    m_fd_map.insert(std::make_pair(std::pair<uint64_t, uint64_t >(packageId, dictId), newStream));
    return newStream;
}


void Tools::releaseFdMemory()
{
    for(auto & fd : m_fd_map)
    {
        if(fd.second)
        {
            fd.second->close();
            free(fd.second);
        }
    }
}
