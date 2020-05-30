// Copyright 2020 <LinkIvan333>

#include <DBHashCreator.hpp>
#include <logs.hpp>

FHandlerContainer DBHashCreator::openDB
        (const FDescriptorContainer &descriptors) {
    FHandlerContainer handlers;
    std::vector < rocksdb::ColumnFamilyHandle * > newHandles;
    rocksdb::DB *dbStrPtr;
    rocksdb::DB *new_dbStrPtr;

    rocksdb::Status status =
            rocksdb::DB::Open(
                    options,
                    _path,
                    descriptors,
                    &newHandles,
                    &dbStrPtr);
    status = rocksdb::DB::Open(
        options,
        _new_path,
        &new_dbStrPtr);
    if (!status.ok()) std::cerr << status.ToString() << std::endl;
    _db.reset(dbStrPtr);
    _new_db.reset(new_dbStrPtr);
    for (rocksdb::ColumnFamilyHandle *ptr : newHandles) {
        handlers.emplace_back(ptr);
    }
    return handlers;
}

FDescriptorContainer DBHashCreator::getFamilyDescriptors() {
    std::vector <std::string> family;
    FDescriptorContainer descriptors;
    rocksdb::Status status =
            rocksdb::DB::ListColumnFamilies(options,
                                            _path,
                                            &family);
  if (!status.ok()) {
      std::cerr << status.ToString() << std::endl;
    }
    for (const std::string &familyName : family) {
        descriptors.emplace_back(familyName,
                                 rocksdb::ColumnFamilyOptions());
    }
    return descriptors;
}

StrContainer DBHashCreator::getStrs(rocksdb::ColumnFamilyHandle *family) {
    boost::unordered_map <std::string, std::string> dbCase;
    std::unique_ptr <rocksdb::Iterator>
            it(_db->NewIterator(rocksdb::ReadOptions(), family));
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        std::string key = it->key().ToString();
        std::string value = it->value().ToString();
        dbCase[key] = value;
    }
    return dbCase;
}

void DBHashCreator::getHash
        (rocksdb::ColumnFamilyHandle *family, StrContainer strContainer) {
    std::string new_value, value;
    for (const auto & it : strContainer) {
        std::string hash = picosha2::hash256_hex_string(it.first + it.second);
        rocksdb::Status status = _new_db->Put(rocksdb::WriteOptions(),
                                          family,
                                          it.first,
                                          hash);
      status = _new_db->Get(rocksdb::ReadOptions(), it.first, &new_value);
      status = _db->Get(rocksdb::ReadOptions(), it.first, &value);
      if (!status.ok()) std::cerr << status.ToString() << std::endl;
      logs::logInfo(it.first, new_value, value, _logLVL);
    }
}

void DBHashCreator::startHash
        (FHandlerContainer *handlers,
                std::list <StrContainer> *StrContainerList) {
    while (!handlers->empty()) {
        _mutex.lock();
        if (handlers->empty()) {
            _mutex.unlock();
            continue;
        }
        auto &family = handlers->front();
        handlers->pop_front();

        StrContainer strContainer = StrContainerList->front();
        StrContainerList->pop_front();
        _mutex.unlock();
        getHash(family.get(), strContainer);
    }
}

void DBHashCreator::startThreads() {
    auto descriptors = getFamilyDescriptors();
    auto handlers = openDB(descriptors);

    std::list <StrContainer> StrContainerList;

    for (const auto &family : handlers) {
        StrContainerList.push_back(
                getStrs(family.get()));
    }
    boost::thread_group threads;
    for (size_t i = 0; i < _threadCountHash; ++i) {
      threads.add_thread(new boost::thread(&DBHashCreator::startHash,
                            this,
                            &handlers,
                            &StrContainerList));
    }
    threads.join_all();
}
