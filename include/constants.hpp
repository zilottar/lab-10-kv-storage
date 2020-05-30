// Copyright 2020 <LinkIvan333>
#include <map>
#include <string>
#pragma once
const std::size_t DEFAULT_THREAD_HASH = boost::thread::hardware_concurrency();
const char DEFAULT_LOG_LEVEL[] = "error";
const char DEFAULT_OUTPUT_PATH[] = "/home/ivan/DB/new_db";
const std::map <std::string, int> log_lvls =
    {{"info", 0}, {"warning", 1}, {"error", 2}};

