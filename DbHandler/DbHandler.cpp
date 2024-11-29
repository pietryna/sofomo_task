#include "DbHandler.hpp"
#include <QDebug>

DbHandler::DbHandler()
    : DatabaseName("geolocDataDb.db") {
    if (SQLITE_OK != sqlite3_open_v2(DatabaseName.c_str(), &Database, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
                                     nullptr)) {
        qInfo() << "Can't open database: " << sqlite3_errmsg(Database);
    }
    sqlite3_stmt *isEmpty;
    if (SQLITE_OK != sqlite3_prepare_v2(Database, std::string("SELECT count(*) FROM sqlite_master WHERE type='table' "
                                                              "AND name NOT LIKE 'sqlite_%';").c_str(), -1, &isEmpty,
                                        nullptr)) {
        qInfo() << "Failed to prepare isEmpty statement: " << sqlite3_errmsg(Database);
    }
    if (SQLITE_OK != sqlite3_step(isEmpty)) {
        qInfo() << "Failed to step isEmpty statement: " << sqlite3_errmsg(Database);
    }
    auto count = sqlite3_column_int(isEmpty, 0);
    sqlite3_finalize(isEmpty);
    if (0 == count) {
        qInfo() << "Database is empty.";
        if (SQLITE_OK != sqlite3_exec(Database,
                                      std::string(
                                          "CREATE TABLE \"GeoData\" ( ID INTEGER PRIMARY KEY AUTOINCREMENT, URL TEXT, IP "
                                          "TEXT, Longitude TEXT, Latitude TEXT);").c_str(),
                                      nullptr,
                                      nullptr,
                                      nullptr)) {
            qInfo() << "Failed to prepare create statement: " << sqlite3_errmsg(Database);
        }
    } else {
        bool exists = false;
        qInfo() << "Database not empty.";
        sqlite3_stmt *containGeoDataTable;
        if (SQLITE_OK
            != sqlite3_prepare_v2(Database, std::string("SELECT count(*) FROM sqlite_master WHERE type='table' "
                                                        "AND name=?;").c_str(), -1, &containGeoDataTable,
                                  nullptr)) {
            qInfo() << "Failed to prepare containGeoDataTable statement: " << sqlite3_errmsg(Database);
        }
        if (SQLITE_OK != sqlite3_bind_text(containGeoDataTable, 1, "GeoData", -1, SQLITE_STATIC)) {
            qInfo() << "Failed to bind containGeoDataTable statement: " << sqlite3_errmsg(Database);
        }

        if (sqlite3_step(containGeoDataTable) == SQLITE_ROW) {
            auto count = sqlite3_column_int(containGeoDataTable, 0);
            exists = (count > 0);
        } else {
            qInfo() << "Failed to step statement: " << sqlite3_errmsg(Database);
        }
        sqlite3_finalize(containGeoDataTable);
        if (exists) {
            qInfo() << "Database somewhat valid.";
        } else {
            qInfo() << "Database invalid!";
            // TODO: Close db connection, remove file and reopen fresh db.
        }
    }

}
DbHandler::~DbHandler() {
    sqlite3_close_v2(Database);
}
bool
DbHandler::insertGeoLocData(DatabaseItem item) {
    bool result = true;
    if (!isExistingGeoLocData(item.getIp())) {
        sqlite3_stmt *insert;
        if (SQLITE_OK
            != sqlite3_prepare_v2(Database,
                                  std::string("INSERT INTO GeoData (URL, IP, Longitude, Latitude) VALUES (?,?,"
                                              "?,?);").c_str(),
                                  -1,
                                  &insert,
                                  nullptr)) {
            qInfo() << "Failed to prepare insert statement: " << sqlite3_errmsg(Database);
            result = false;
        }
        if (
            (SQLITE_OK != sqlite3_bind_text(insert, 1, item.getUrl().c_str(), -1, SQLITE_STATIC)) ||
                (SQLITE_OK != sqlite3_bind_text(insert, 2, item.getIp().c_str(), -1, SQLITE_STATIC)) ||
                (SQLITE_OK != sqlite3_bind_text(insert, 3, item.getLongitude().c_str(), -1, SQLITE_STATIC)) ||
                (SQLITE_OK != sqlite3_bind_text(insert, 4, item.getLatitude().c_str(), -1, SQLITE_STATIC))
            ) {
            qInfo() << "Failed to bind insert parameters: " << sqlite3_errmsg(Database);
            result = false;
        }
        if (SQLITE_DONE != sqlite3_step(insert)) {
            qInfo() << "Failed execute statement: " << sqlite3_errmsg(Database);
            result = false;
        }
        sqlite3_finalize(insert);
    } else {
        qInfo() << "Data for IP: " << item.getIp() << " is already existing in database.";
    }
    return result;
}

std::pair<bool, DatabaseItem>
DbHandler::fetchGeoLocData(const std::string &ip) {
    bool found = true;
    std::string url{}, longitude, latitude;
    sqlite3_stmt *fetch;
    if (SQLITE_OK
        != sqlite3_prepare_v2(Database,
                              std::string("SELECT URL, IP, Longitude, Latitude FROM GeoData WHERE IP = ?;").c_str(),
                              -1, &fetch, nullptr)) {
        qInfo() << "Failed to prepare fetch statement: " << sqlite3_errmsg(Database);
        found = false;
    }
    if (SQLITE_OK != sqlite3_bind_text(fetch, 1, ip.c_str(), -1, SQLITE_STATIC)) {
        qInfo() << "Failed to bind fetch parameters: " << sqlite3_errmsg(Database);
        found = false;
    }
    if (SQLITE_ROW == sqlite3_step(fetch)) {
        url = reinterpret_cast<const char *>(sqlite3_column_text(fetch, 0));
        longitude = reinterpret_cast<const char *>(sqlite3_column_text(fetch, 2));
        latitude = reinterpret_cast<const char *>(sqlite3_column_text(fetch, 3));
    } else {
        qInfo() << "No row found for IP: " << ip;
        found = false;
    }
    return {found, DatabaseItem{url, ip, longitude, latitude}};
}

bool
DbHandler::isExistingGeoLocData(const std::string &ip) {
    bool exists = false;
    sqlite3_stmt *check;
    if (SQLITE_OK
        != sqlite3_prepare_v2(Database,
                              std::string("SELECT 1 FROM GeoData WHERE IP = ? LIMIT 1;").c_str(), -1, &check,
                              nullptr)) {
        qInfo() << "Failed to prepare check statement: " << sqlite3_errmsg(Database);
    }
    if (SQLITE_OK != sqlite3_bind_text(check, 1, ip.c_str(), -1, SQLITE_STATIC)) {
        qInfo() << "Failed to bind check parameters: " << sqlite3_errmsg(Database);
    }
    if (SQLITE_ROW == sqlite3_step(check)) {
        exists = true;
    }
    sqlite3_finalize(check);
    return exists;
}

bool
DbHandler::removeGeoLocData(const std::string &ip) {
    bool result = true;
    if (isExistingGeoLocData(ip)) {
        sqlite3_stmt *remove;
        if (SQLITE_OK
            != sqlite3_prepare_v2(Database,
                                  std::string("DELETE FROM GeoData WHERE IP = ?;").c_str(), -1, &remove,
                                  nullptr)) {
            qInfo() << "Failed to prepare remove statement: " << sqlite3_errmsg(Database);
            result = false;
        }
        if (SQLITE_OK != sqlite3_bind_text(remove, 1, ip.c_str(), -1, SQLITE_STATIC)) {
            qInfo() << "Failed to bind remove parameters: " << sqlite3_errmsg(Database);
            result = false;
        }
        if (SQLITE_DONE != sqlite3_step(remove)) {
            qInfo() << "Failed execute remove statement: " << sqlite3_errmsg(Database);
            result = false;
        }
        sqlite3_finalize(remove);
    } else {
        qInfo() << "Row for IP: " << ip << " does not exist in database.";
    }
    return result;
}
