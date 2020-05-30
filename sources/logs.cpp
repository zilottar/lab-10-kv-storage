// Copyright 2020 <LinkIvan333>

#include <DBHashCreator.hpp>
#include <logs.hpp>
#include <constants.hpp>

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;
namespace sinks = boost::log::sinks;

void logs::logInFile() {
  boost::log::core::get()->add_global_attribute(
      "TimeStamp", boost::log::attributes::local_clock());

  auto sinkFile = boost::log::add_file_log(
      keywords::file_name = "logs/logs.log",
      keywords::rotation_size = 128 * 1024 * 1024,
      keywords::format =
          (boost::log::expressions::stream
  << boost::log::expressions::format_date_time<
  boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S")
  << ": <" << boost::log::trivial::severity << "> "
  << boost::log::expressions::smessage));
  sinkFile->set_filter(logging::trivial::severity >= logging::trivial::info);
  auto sinkConsole = logging::add_console_log(
      std::cout,
      keywords::format =
  (expr::stream << expr::format_date_time<boost::posix_time::ptime>(
      "TimeStamp", "%Y-%m-%d %H:%M:%S")
  << ": <" << logging::trivial::severity << "> "
  << expr::smessage));
  sinkConsole->set_filter(logging::trivial::severity >= logging::trivial::info);

  logging::add_common_attributes();
}


void logs::logInfo(const std::string &key, const std::string &hash,
                   const std::string &value, const int &lvl) {
    switch (lvl){
      case 0:
        BOOST_LOG_TRIVIAL(info) << "Key: " << key << " hash: " << hash
                            << " , where value was " << value << std::endl;
      break;
      case 1:
        BOOST_LOG_TRIVIAL(warning) << "Key: " << key << " hash: " << hash
                            << " , where value was " << value << std::endl;
      break;
      case 2:
        BOOST_LOG_TRIVIAL(error) << "Key: " << key << " hash: " << hash
                            << " , where value was " << value << std::endl;
  }
}
