#ifndef SOFOMO_DBHANDLER_HPP
#define SOFOMO_DBHANDLER_HPP

#include <string>
#include "sqlite3.h"
#include "DatabaseItem.hpp"

class DbHandler {
public:
    DbHandler();
    ~DbHandler();

    void insertGeoLocData(DatabaseItem item);
    DatabaseItem fetchGeoLocData(const std::string& ip);
    void removeGeoLocData(const std::string &ip);

private:
    bool isExistingGeoLocData(const std::string& ip);
    const std::string DatabaseName;
    sqlite3* Database = nullptr;
};

#endif//SOFOMO_DBHANDLER_HPP
