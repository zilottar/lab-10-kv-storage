// Copyright 2020 <>

#pragma once
#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <thread>
#include <boost/filesystem.hpp>
#include <boost/log/trivial.hpp>
#include <boost/unordered_map.hpp>
#include <rocksdb/db.h>
#include <rocksdb/slice.h>
#include <rocksdb/options.h>
#include <boost/thread.hpp>
#include <boost/program_options.hpp>
#include <mutex>
#include <vector>
#include <list>
#include <picosha2.hpp>
#include <constants.hpp>

using FContainer =
        std::list<std::unique_ptr<rocksdb::ColumnFamilyHandle>>;
using FDescriptorContainer =
        std::vector<rocksdb::ColumnFamilyDescriptor>;
using FHandlerContainer =
        std::list<std::unique_ptr<rocksdb::ColumnFamilyHandle>>;
using StrContainer = boost::unordered_map<std::string, std::string>;
namespace po = boost::program_options;

class DBHashCreator {
public:
    DBHashCreator(std::string &new_path, std::string &path,
            std::size_t &threadCount,
            int logLVL) :
            _new_path(new_path),
            _path(path),
            _logLVL(logLVL),  _threadCountHash(threadCount){
      options.create_if_missing = true;
      startThreads();
    }

    FDescriptorContainer getFamilyDescriptors();

    FHandlerContainer openDB(const FDescriptorContainer &);

    StrContainer getStrs(rocksdb::ColumnFamilyHandle *);

    void getHash(rocksdb::ColumnFamilyHandle *, StrContainer);

    void startHash(FHandlerContainer *, std::list<StrContainer> *);

    void startThreads();

private:
    rocksdb::Options options;
    std::string _new_path;
    std::string _path;
    int _logLVL;
    std::unique_ptr<rocksdb::DB> _db, _new_db;
    std::size_t _threadCountHash;
    std::recursive_mutex _mutex;
};

