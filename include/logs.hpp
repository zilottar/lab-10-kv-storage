// Copyright 2020 <LinkIvan333>

#pragma once
#include <iostream>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sinks.hpp>
#include <string>

namespace logs{
    void logInFile();
    void logInfo(const std::string &key, const std::string &hash,
                 const std::string &value, const int &lvl);
}
