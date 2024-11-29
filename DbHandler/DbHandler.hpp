#ifndef SOFOMO_DBHANDLER_HPP
#define SOFOMO_DBHANDLER_HPP

#include <string>
#include "sqlite3.h"
#include "DatabaseItem.hpp"

class DbHandler {
 public:
    DbHandler();
    ~DbHandler();

    bool insertGeoLocData(DatabaseItem item);
    std::pair<bool, DatabaseItem> fetchGeoLocData(const std::string &ip);
    bool removeGeoLocData(const std::string &ip);
    bool isExistingGeoLocData(const std::string &ip);

 private:
    const std::string DatabaseName;
    sqlite3 *Database = nullptr;
};

#endif//SOFOMO_DBHANDLER_HPP
