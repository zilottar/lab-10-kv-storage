// Copyright 2018 Your Name <your_email>

#include <gtest/gtest.h>
#include <DBHashCreator.hpp>	
#include <constants.hpp>	
#include <logs.hpp>	

int main(int argc, char **argv) {	
    std::string logLVL;	
    std::size_t threadCount;	
    std::string pathToNewFile;	
    std::string pathToFile;	
    po::options_description desc("short description");	
    desc.add_options()	
            ("help,h", "0 помощи")	
            ("log_level",	
             po::value<std::string>(&logLVL)->default_value(DEFAULT_LOG_LEVEL),	
             "level logging")	
            ("thread_count",	
                 po::value<size_t>(&threadCount)->	
                     default_value(DEFAULT_THREAD_HASH),	
             "count of threads")	
            ("output",	
                 po::value<std::string>(&pathToNewFile)->	
                     default_value(DEFAULT_OUTPUT_PATH),	
             "path result")	
        ("input",	
                 po::value<std::string>(&pathToFile), "input path");	

    po::variables_map vm;	
    try {	
        po::store(po::parse_command_line(argc, argv, desc), vm);	
        po::notify(vm);	
    }	

    catch (po::error &e) {	
        std::cout << e.what() << std::endl;	
        std::cout << desc << std::endl;	
        return 1;	
    }	
    if (vm.count("help")) {	
        std::cout << desc << std::endl;	
        return 1;	
    }	

    if (!vm.count("input")) {	
        std::cout << "error: bad format" << std::endl << desc << std::endl;	
        return 1;	
    }	
    logs::logInFile();	
    DBHashCreator db(pathToNewFile, pathToFile,	
                     threadCount, log_lvls.at(logLVL));	
    return 0;	
}	

TEST(Example, EmptyTest) {
EXPECT_TRUE(true);
}
