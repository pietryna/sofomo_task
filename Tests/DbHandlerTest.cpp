#include <gtest/gtest.h>

#include "../DbHandler/DbHandler.hpp"
#include "Sqlite3_stubs.hpp"

using testing::_;
using testing::Return;
using testing::DoAll;
using testing::SetArgPointee;
using testing::StrEq;
using testing::InSequence;

class DbHandlerConstructionTest : public testing::Test {

    void TearDown() override {
        EXPECT_CALL(sqlite3Mock, sqlite3_close_v2(_)).Times(1);
    }
};

TEST_F(DbHandlerConstructionTest, DbHandlerCration_success_emptyDb) {
    sqlite3 *dummyDb;
    sqlite3_stmt *dummyStmt;
    {
        InSequence sq;
        EXPECT_CALL(sqlite3Mock,
                    sqlite3_open_v2(StrEq("geolocDataDb.db"), _, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
                                    nullptr))
            .WillOnce(DoAll(SetArgPointee<1>(dummyDb), Return(SQLITE_OK)));
        EXPECT_CALL(sqlite3Mock, sqlite3_prepare_v2(
            dummyDb,
            StrEq("SELECT count(*) FROM sqlite_master WHERE type='table' AND name NOT LIKE 'sqlite_%';"),
            -1,
            _,
            nullptr)).WillOnce(DoAll(SetArgPointee<3>(dummyStmt), Return(SQLITE_OK)));
        EXPECT_CALL(sqlite3Mock, sqlite3_step(dummyStmt)).WillOnce(Return(SQLITE_OK));
        EXPECT_CALL(sqlite3Mock, sqlite3_column_int(dummyStmt, 0)).WillOnce(Return(0));
        EXPECT_CALL(sqlite3Mock, sqlite3_finalize(dummyStmt)).Times(1);
        EXPECT_CALL(sqlite3Mock,
                    sqlite3_exec(dummyDb,
                                 StrEq("CREATE TABLE \"GeoData\" ( ID INTEGER PRIMARY KEY AUTOINCREMENT, URL TEXT, IP "
                                       "TEXT, Longitude TEXT, Latitude TEXT);"),
                                 nullptr,
                                 nullptr,
                                 nullptr))
            .WillOnce(Return(SQLITE_OK));
    }

    std::unique_ptr<DbHandler> dbHandler = std::make_unique<DbHandler>();
}

TEST_F(DbHandlerConstructionTest, DbHandlerCration_success_notEmptyDb) {
    sqlite3 *dummyDb;
    sqlite3_stmt *dummyStmt;
    {
        InSequence sq;
        EXPECT_CALL(sqlite3Mock,
                    sqlite3_open_v2(StrEq("geolocDataDb.db"), _, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
                                    nullptr))
            .WillOnce(DoAll(SetArgPointee<1>(dummyDb), Return(SQLITE_OK)));
        EXPECT_CALL(sqlite3Mock, sqlite3_prepare_v2(
            dummyDb,
            StrEq("SELECT count(*) FROM sqlite_master WHERE type='table' AND name NOT LIKE 'sqlite_%';"),
            -1,
            _,
            nullptr)).WillOnce(DoAll(SetArgPointee<3>(dummyStmt), Return(SQLITE_OK)));
        EXPECT_CALL(sqlite3Mock, sqlite3_step(dummyStmt)).WillOnce(Return(SQLITE_OK));
        EXPECT_CALL(sqlite3Mock, sqlite3_column_int(dummyStmt, 0)).WillOnce(Return(1));
        EXPECT_CALL(sqlite3Mock, sqlite3_finalize(dummyStmt)).Times(1);
        EXPECT_CALL(sqlite3Mock,
                    sqlite3_prepare_v2(dummyDb, StrEq("SELECT count(*) FROM sqlite_master WHERE type='table' "
                                                      "AND name=?;"), -1, _, nullptr))
            .WillOnce(DoAll(SetArgPointee<3>(dummyStmt), Return(SQLITE_OK)));
        EXPECT_CALL(sqlite3Mock, sqlite3_bind_text(dummyStmt, 1, StrEq("GeoData"), -1, SQLITE_STATIC))
            .WillOnce(Return(SQLITE_OK));
        EXPECT_CALL(sqlite3Mock, sqlite3_step(dummyStmt)).WillOnce(Return(SQLITE_ROW));
        EXPECT_CALL(sqlite3Mock, sqlite3_column_int(dummyStmt, 0)).WillOnce(Return(1));
        EXPECT_CALL(sqlite3Mock, sqlite3_finalize(dummyStmt)).Times(1);
    }

    std::unique_ptr<DbHandler> dbHandler = std::make_unique<DbHandler>();
}

class DbHandlerApiTest : public testing::Test {
    void SetUp() override {
        sqlite3_stmt *dummyStmt;
        {
            InSequence sq;
            EXPECT_CALL(sqlite3Mock,
                        sqlite3_open_v2(StrEq("geolocDataDb.db"), _, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
                                        nullptr))
                .WillOnce(DoAll(SetArgPointee<1>(dummyDb), Return(SQLITE_OK)));
            EXPECT_CALL(sqlite3Mock, sqlite3_prepare_v2(
                dummyDb,
                StrEq("SELECT count(*) FROM sqlite_master WHERE type='table' AND name NOT LIKE 'sqlite_%';"),
                -1,
                _,
                nullptr)).WillOnce(DoAll(SetArgPointee<3>(dummyStmt), Return(SQLITE_OK)));
            EXPECT_CALL(sqlite3Mock, sqlite3_step(dummyStmt)).WillOnce(Return(SQLITE_OK));
            EXPECT_CALL(sqlite3Mock, sqlite3_column_int(dummyStmt, 0)).WillOnce(Return(1));
            EXPECT_CALL(sqlite3Mock, sqlite3_finalize(dummyStmt)).Times(1);
            EXPECT_CALL(sqlite3Mock,
                        sqlite3_prepare_v2(dummyDb, StrEq("SELECT count(*) FROM sqlite_master WHERE type='table' "
                                                          "AND name=?;"), -1, _, nullptr))
                .WillOnce(DoAll(SetArgPointee<3>(dummyStmt), Return(SQLITE_OK)));
            EXPECT_CALL(sqlite3Mock, sqlite3_bind_text(dummyStmt, 1, StrEq("GeoData"), -1, SQLITE_STATIC))
                .WillOnce(Return(SQLITE_OK));
            EXPECT_CALL(sqlite3Mock, sqlite3_step(dummyStmt)).WillOnce(Return(SQLITE_ROW));
            EXPECT_CALL(sqlite3Mock, sqlite3_column_int(dummyStmt, 0)).WillOnce(Return(1));
            EXPECT_CALL(sqlite3Mock, sqlite3_finalize(dummyStmt)).Times(1);
        }
        dbHandler = std::make_unique<DbHandler>();

        ON_CALL(sqlite3Mock, sqlite3_errmsg(dummyDb)).WillByDefault(Return("Error"));
    };
    void TearDown() override {
        EXPECT_CALL(sqlite3Mock, sqlite3_close_v2(dummyDb)).Times(1);
    }
 protected:
    std::unique_ptr<DbHandler> dbHandler;
    sqlite3 *dummyDb;
};

TEST_F(DbHandlerApiTest, insertGeoLocData_OK_new) {
    std::string url{"test.com"}, ip{"1.2.3.4"}, longitude{"18.13456"}, latitude{"52.3245"};

    {
        InSequence s;
        /* Check if it's existing and return false */
        sqlite3_stmt *dummyStmt;
        EXPECT_CALL(sqlite3Mock,
                    sqlite3_prepare_v2(dummyDb,
                                       StrEq("SELECT 1 FROM GeoData WHERE IP = ? LIMIT 1;"),
                                       -1, _, nullptr))
            .WillOnce(DoAll(SetArgPointee<3>(dummyStmt), Return(SQLITE_OK)));
        EXPECT_CALL(sqlite3Mock, sqlite3_bind_text(dummyStmt, 1, StrEq(ip.c_str()), -1, SQLITE_STATIC))
            .WillOnce(Return(SQLITE_OK));
        EXPECT_CALL(sqlite3Mock, sqlite3_step(dummyStmt)).WillOnce(Return(SQLITE_DONE));
        EXPECT_CALL(sqlite3Mock, sqlite3_finalize(dummyStmt)).Times(1);

        /* Insert when not existing */
        EXPECT_CALL(sqlite3Mock,
                    sqlite3_prepare_v2(dummyDb,
                                       StrEq("INSERT INTO GeoData (URL, IP, Longitude, Latitude) VALUES (?,?,?,?);"),
                                       -1, _, nullptr))
            .WillOnce(DoAll(SetArgPointee<3>(dummyStmt), Return(SQLITE_OK)));
        EXPECT_CALL(sqlite3Mock, sqlite3_bind_text(dummyStmt, 1, StrEq(url.c_str()), -1, SQLITE_STATIC))
            .WillOnce(Return(SQLITE_OK));
        EXPECT_CALL(sqlite3Mock, sqlite3_bind_text(dummyStmt, 2, StrEq(ip.c_str()), -1, SQLITE_STATIC))
            .WillOnce(Return(SQLITE_OK));
        EXPECT_CALL(sqlite3Mock, sqlite3_bind_text(dummyStmt, 3, StrEq(longitude.c_str()), -1, SQLITE_STATIC))
            .WillOnce(Return(SQLITE_OK));
        EXPECT_CALL(sqlite3Mock, sqlite3_bind_text(dummyStmt, 4, StrEq(latitude.c_str()), -1, SQLITE_STATIC))
            .WillOnce(Return(SQLITE_OK));
        EXPECT_CALL(sqlite3Mock, sqlite3_step(dummyStmt)).WillOnce(Return(SQLITE_DONE));
        EXPECT_CALL(sqlite3Mock, sqlite3_finalize(dummyStmt)).Times(1);
    }

    ASSERT_EQ(true, dbHandler->insertGeoLocData({url, ip, longitude, latitude}));
}

TEST_F(DbHandlerApiTest, insertGeoLocData_OK_existing) {
    std::string url{"test.com"}, ip{"1.2.3.4"}, longitude{"18.13456"}, latitude{"52.3245"};

    {
        InSequence s;
        /* Check if it's existing and return false */
        sqlite3_stmt *dummyStmt;
        EXPECT_CALL(sqlite3Mock,
                    sqlite3_prepare_v2(dummyDb,
                                       StrEq("SELECT 1 FROM GeoData WHERE IP = ? LIMIT 1;"),
                                       -1, _, nullptr))
            .WillOnce(DoAll(SetArgPointee<3>(dummyStmt), Return(SQLITE_OK)));
        EXPECT_CALL(sqlite3Mock, sqlite3_bind_text(dummyStmt, 1, StrEq(ip.c_str()), -1, SQLITE_STATIC))
            .WillOnce(Return(SQLITE_OK));
        EXPECT_CALL(sqlite3Mock, sqlite3_step(dummyStmt)).WillOnce(Return(SQLITE_ROW));
        EXPECT_CALL(sqlite3Mock, sqlite3_finalize(dummyStmt)).Times(1);
    }

    ASSERT_EQ(true, dbHandler->insertGeoLocData({url, ip, longitude, latitude}));
}

TEST_F(DbHandlerApiTest, insertGeoLocData_OK_stepFailed) {
    std::string url{"test.com"}, ip{"1.2.3.4"}, longitude{"18.13456"}, latitude{"52.3245"};

    {
        InSequence s;
        /* Check if it's existing and return false */
        sqlite3_stmt *dummyStmt;
        EXPECT_CALL(sqlite3Mock,
                    sqlite3_prepare_v2(dummyDb,
                                       StrEq("SELECT 1 FROM GeoData WHERE IP = ? LIMIT 1;"),
                                       -1, _, nullptr))
            .WillOnce(DoAll(SetArgPointee<3>(dummyStmt), Return(SQLITE_OK)));
        EXPECT_CALL(sqlite3Mock, sqlite3_bind_text(dummyStmt, 1, StrEq(ip.c_str()), -1, SQLITE_STATIC))
            .WillOnce(Return(SQLITE_OK));
        EXPECT_CALL(sqlite3Mock, sqlite3_step(dummyStmt)).WillOnce(Return(SQLITE_DONE));
        EXPECT_CALL(sqlite3Mock, sqlite3_finalize(dummyStmt)).Times(1);

        /* Insert when not existing */
        EXPECT_CALL(sqlite3Mock,
                    sqlite3_prepare_v2(dummyDb,
                                       StrEq("INSERT INTO GeoData (URL, IP, Longitude, Latitude) VALUES (?,?,?,?);"),
                                       -1, _, nullptr))
            .WillOnce(DoAll(SetArgPointee<3>(dummyStmt), Return(SQLITE_OK)));
        EXPECT_CALL(sqlite3Mock, sqlite3_bind_text(dummyStmt, 1, StrEq(url.c_str()), -1, SQLITE_STATIC))
            .WillOnce(Return(SQLITE_OK));
        EXPECT_CALL(sqlite3Mock, sqlite3_bind_text(dummyStmt, 2, StrEq(ip.c_str()), -1, SQLITE_STATIC))
            .WillOnce(Return(SQLITE_OK));
        EXPECT_CALL(sqlite3Mock, sqlite3_bind_text(dummyStmt, 3, StrEq(longitude.c_str()), -1, SQLITE_STATIC))
            .WillOnce(Return(SQLITE_OK));
        EXPECT_CALL(sqlite3Mock, sqlite3_bind_text(dummyStmt, 4, StrEq(latitude.c_str()), -1, SQLITE_STATIC))
            .WillOnce(Return(SQLITE_OK));
        EXPECT_CALL(sqlite3Mock, sqlite3_step(dummyStmt)).WillOnce(Return(SQLITE_ERROR));
        EXPECT_CALL(sqlite3Mock, sqlite3_finalize(dummyStmt)).Times(1);
    }

    ASSERT_EQ(false, dbHandler->insertGeoLocData({url, ip, longitude, latitude}));
}

TEST_F(DbHandlerApiTest, insertGeoLocData_OK_bindFailed) {
    std::string url{"test.com"}, ip{"1.2.3.4"}, longitude{"18.13456"}, latitude{"52.3245"};

    {
        InSequence s;
        /* Check if it's existing and return false */
        sqlite3_stmt *dummyCheck, *dummyInsert;
        EXPECT_CALL(sqlite3Mock,
                    sqlite3_prepare_v2(dummyDb,
                                       StrEq("SELECT 1 FROM GeoData WHERE IP = ? LIMIT 1;"),
                                       -1, _, nullptr))
            .WillOnce(DoAll(SetArgPointee<3>(dummyCheck), Return(SQLITE_OK)));
        EXPECT_CALL(sqlite3Mock, sqlite3_bind_text(dummyCheck, 1, StrEq(ip.c_str()), -1, SQLITE_STATIC))
            .WillOnce(Return(SQLITE_OK));
        EXPECT_CALL(sqlite3Mock, sqlite3_step(dummyCheck)).WillOnce(Return(SQLITE_DONE));
        EXPECT_CALL(sqlite3Mock, sqlite3_finalize(dummyCheck)).Times(1);

        /* Insert when not existing */
        EXPECT_CALL(sqlite3Mock,
                    sqlite3_prepare_v2(dummyDb,
                                       StrEq("INSERT INTO GeoData (URL, IP, Longitude, Latitude) VALUES (?,?,?,?);"),
                                       -1, _, nullptr))
            .WillOnce(DoAll(SetArgPointee<3>(dummyInsert), Return(SQLITE_OK)));
        EXPECT_CALL(sqlite3Mock, sqlite3_bind_text(dummyInsert, 1, StrEq(url.c_str()), -1, SQLITE_STATIC))
            .WillOnce(Return(SQLITE_ERROR));
        EXPECT_CALL(sqlite3Mock, sqlite3_step(dummyInsert)).WillOnce(Return(SQLITE_DONE));
        EXPECT_CALL(sqlite3Mock, sqlite3_finalize(dummyInsert)).Times(1);
    }

    ASSERT_EQ(false, dbHandler->insertGeoLocData({url, ip, longitude, latitude}));
}

TEST_F(DbHandlerApiTest, insertGeoLocData_OK_preparedFailed) {
    std::string url{"test.com"}, ip{"1.2.3.4"}, longitude{"18.13456"}, latitude{"52.3245"};

    {
        InSequence s;
        /* Check if it's existing and return false */
        sqlite3_stmt *dummyStmt;
        EXPECT_CALL(sqlite3Mock,
                    sqlite3_prepare_v2(dummyDb,
                                       StrEq("SELECT 1 FROM GeoData WHERE IP = ? LIMIT 1;"),
                                       -1, _, nullptr))
            .WillOnce(DoAll(SetArgPointee<3>(dummyStmt), Return(SQLITE_OK)));
        EXPECT_CALL(sqlite3Mock, sqlite3_bind_text(dummyStmt, 1, StrEq(ip.c_str()), -1, SQLITE_STATIC))
            .WillOnce(Return(SQLITE_OK));
        EXPECT_CALL(sqlite3Mock, sqlite3_step(dummyStmt)).WillOnce(Return(SQLITE_DONE));
        EXPECT_CALL(sqlite3Mock, sqlite3_finalize(dummyStmt)).Times(1);

        /* Insert when not existing */
        EXPECT_CALL(sqlite3Mock,
                    sqlite3_prepare_v2(dummyDb,
                                       StrEq("INSERT INTO GeoData (URL, IP, Longitude, Latitude) VALUES (?,?,?,?);"),
                                       -1, _, nullptr))
            .WillOnce(DoAll(SetArgPointee<3>(dummyStmt), Return(SQLITE_ERROR)));
        EXPECT_CALL(sqlite3Mock, sqlite3_bind_text(dummyStmt, 1, StrEq(url.c_str()), -1, SQLITE_STATIC))
            .WillOnce(Return(SQLITE_OK));
        EXPECT_CALL(sqlite3Mock, sqlite3_bind_text(dummyStmt, 2, StrEq(ip.c_str()), -1, SQLITE_STATIC))
            .WillOnce(Return(SQLITE_OK));
        EXPECT_CALL(sqlite3Mock, sqlite3_bind_text(dummyStmt, 3, StrEq(longitude.c_str()), -1, SQLITE_STATIC))
            .WillOnce(Return(SQLITE_OK));
        EXPECT_CALL(sqlite3Mock, sqlite3_bind_text(dummyStmt, 4, StrEq(latitude.c_str()), -1, SQLITE_STATIC))
            .WillOnce(Return(SQLITE_OK));
        EXPECT_CALL(sqlite3Mock, sqlite3_step(dummyStmt)).WillOnce(Return(SQLITE_DONE));
        EXPECT_CALL(sqlite3Mock, sqlite3_finalize(dummyStmt)).Times(1);
    }

    ASSERT_EQ(false, dbHandler->insertGeoLocData({url, ip, longitude, latitude}));
}

TEST_F(DbHandlerApiTest, fetchGeoLocData_OK) {
    std::string url{"test.com"}, ip{"1.2.3.4"}, longitude{"18.13456"}, latitude{"52.3245"};

    {
        InSequence s;
        sqlite3_stmt *dummyStmt;
        EXPECT_CALL(sqlite3Mock,
                    sqlite3_prepare_v2(dummyDb,
                                       StrEq("SELECT URL, IP, Longitude, Latitude FROM GeoData WHERE IP = ?;"),
                                       -1, _, nullptr))
            .WillOnce(DoAll(SetArgPointee<3>(dummyStmt), Return(SQLITE_OK)));
        EXPECT_CALL(sqlite3Mock, sqlite3_bind_text(dummyStmt, 1, StrEq(ip.c_str()), -1, SQLITE_STATIC))
            .WillOnce(Return(SQLITE_OK));
        EXPECT_CALL(sqlite3Mock, sqlite3_step(dummyStmt)).WillOnce(Return(SQLITE_ROW));
        EXPECT_CALL(sqlite3Mock, sqlite3_column_text(dummyStmt, 0))
            .WillOnce(Return(reinterpret_cast<const unsigned char *>(url.c_str())));
        EXPECT_CALL(sqlite3Mock, sqlite3_column_text(dummyStmt, 2))
            .WillOnce(Return(reinterpret_cast<const unsigned char *>(longitude.c_str())));
        EXPECT_CALL(sqlite3Mock, sqlite3_column_text(dummyStmt, 3))
            .WillOnce(Return(reinterpret_cast<const unsigned char *>(latitude.c_str())));
        EXPECT_CALL(sqlite3Mock, sqlite3_finalize(dummyStmt)).Times(1);
    }
    auto [found, item] = dbHandler->fetchGeoLocData(ip);
    ASSERT_EQ(true, found);
    ASSERT_EQ(url, item.getUrl());
    ASSERT_EQ(ip, item.getIp());
    ASSERT_EQ(longitude, item.getLongitude());
    ASSERT_EQ(latitude, item.getLatitude());
}

TEST_F(DbHandlerApiTest, fetchGeoLocData_notFound) {
    std::string url{"test.com"}, ip{"1.2.3.4"}, longitude{"18.13456"}, latitude{"52.3245"};

    {
        InSequence s;
        sqlite3_stmt *dummyStmt;
        EXPECT_CALL(sqlite3Mock,
                    sqlite3_prepare_v2(dummyDb,
                                       StrEq("SELECT URL, IP, Longitude, Latitude FROM GeoData WHERE IP = ?;"),
                                       -1, _, nullptr))
            .WillOnce(DoAll(SetArgPointee<3>(dummyStmt), Return(SQLITE_OK)));
        EXPECT_CALL(sqlite3Mock, sqlite3_bind_text(dummyStmt, 1, StrEq(ip.c_str()), -1, SQLITE_STATIC))
            .WillOnce(Return(SQLITE_OK));
        EXPECT_CALL(sqlite3Mock, sqlite3_step(dummyStmt)).WillOnce(Return(SQLITE_DONE));
        EXPECT_CALL(sqlite3Mock, sqlite3_finalize(dummyStmt)).Times(1);
    }
    auto [found, item] = dbHandler->fetchGeoLocData(ip);
    ASSERT_EQ(false, found);
    ASSERT_EQ("", item.getUrl());
    ASSERT_EQ(ip, item.getIp());
    ASSERT_EQ("", item.getLongitude());
    ASSERT_EQ("", item.getLatitude());
}

TEST_F(DbHandlerApiTest, fetchGeoLocData_bindFailed) {
    std::string url{"test.com"}, ip{"1.2.3.4"}, longitude{"18.13456"}, latitude{"52.3245"};

    {
        InSequence s;
        sqlite3_stmt *dummyStmt;
        EXPECT_CALL(sqlite3Mock,
                    sqlite3_prepare_v2(dummyDb,
                                       StrEq("SELECT URL, IP, Longitude, Latitude FROM GeoData WHERE IP = ?;"),
                                       -1, _, nullptr))
            .WillOnce(DoAll(SetArgPointee<3>(dummyStmt), Return(SQLITE_OK)));
        EXPECT_CALL(sqlite3Mock, sqlite3_bind_text(dummyStmt, 1, StrEq(ip.c_str()), -1, SQLITE_STATIC))
            .WillOnce(Return(SQLITE_ERROR));
        EXPECT_CALL(sqlite3Mock, sqlite3_step(dummyStmt)).WillOnce(Return(SQLITE_DONE));
        EXPECT_CALL(sqlite3Mock, sqlite3_finalize(dummyStmt)).Times(1);
    }
    auto [found, item] = dbHandler->fetchGeoLocData(ip);
    ASSERT_EQ(false, found);
    ASSERT_EQ("", item.getUrl());
    ASSERT_EQ(ip, item.getIp());
    ASSERT_EQ("", item.getLongitude());
    ASSERT_EQ("", item.getLatitude());
}

TEST_F(DbHandlerApiTest, fetchGeoLocData_prepareFailed) {
    std::string url{"test.com"}, ip{"1.2.3.4"}, longitude{"18.13456"}, latitude{"52.3245"};

    {
        InSequence s;
        sqlite3_stmt *dummyStmt;
        EXPECT_CALL(sqlite3Mock,
                    sqlite3_prepare_v2(dummyDb,
                                       StrEq("SELECT URL, IP, Longitude, Latitude FROM GeoData WHERE IP = ?;"),
                                       -1, _, nullptr))
            .WillOnce(DoAll(SetArgPointee<3>(dummyStmt), Return(SQLITE_ERROR)));
        EXPECT_CALL(sqlite3Mock, sqlite3_bind_text(dummyStmt, 1, StrEq(ip.c_str()), -1, SQLITE_STATIC))
            .WillOnce(Return(SQLITE_ERROR));
        EXPECT_CALL(sqlite3Mock, sqlite3_step(dummyStmt)).WillOnce(Return(SQLITE_DONE));
        EXPECT_CALL(sqlite3Mock, sqlite3_finalize(dummyStmt)).Times(1);
    }
    auto [found, item] = dbHandler->fetchGeoLocData(ip);
    ASSERT_EQ(false, found);
    ASSERT_EQ("", item.getUrl());
    ASSERT_EQ(ip, item.getIp());
    ASSERT_EQ("", item.getLongitude());
    ASSERT_EQ("", item.getLatitude());
}

TEST_F(DbHandlerApiTest, removeGeoLocData_OK) {
    std::string ip{"1.2.3.4"};

    {
        InSequence s;
        sqlite3_stmt *dummyStmt;

        EXPECT_CALL(sqlite3Mock,
                    sqlite3_prepare_v2(dummyDb,
                                       StrEq("SELECT 1 FROM GeoData WHERE IP = ? LIMIT 1;"),
                                       -1, _, nullptr))
            .WillOnce(DoAll(SetArgPointee<3>(dummyStmt), Return(SQLITE_OK)));
        EXPECT_CALL(sqlite3Mock, sqlite3_bind_text(dummyStmt, 1, StrEq(ip.c_str()), -1, SQLITE_STATIC))
            .WillOnce(Return(SQLITE_OK));
        EXPECT_CALL(sqlite3Mock, sqlite3_step(dummyStmt)).WillOnce(Return(SQLITE_ROW));
        EXPECT_CALL(sqlite3Mock, sqlite3_finalize(dummyStmt)).Times(1);

        EXPECT_CALL(sqlite3Mock,
                    sqlite3_prepare_v2(dummyDb,
                                       StrEq("DELETE FROM GeoData WHERE IP = ?;"),
                                       -1, _, nullptr))
            .WillOnce(DoAll(SetArgPointee<3>(dummyStmt), Return(SQLITE_OK)));
        EXPECT_CALL(sqlite3Mock, sqlite3_bind_text(dummyStmt, 1, StrEq(ip.c_str()), -1, SQLITE_STATIC))
            .WillOnce(Return(SQLITE_OK));
        EXPECT_CALL(sqlite3Mock, sqlite3_step(dummyStmt)).WillOnce(Return(SQLITE_DONE));
        EXPECT_CALL(sqlite3Mock, sqlite3_finalize(dummyStmt)).Times(1);
    }

    ASSERT_EQ(true, dbHandler->removeGeoLocData(ip));
}

TEST_F(DbHandlerApiTest, removeGeoLocData_stepFailed) {
    std::string ip{"1.2.3.4"};

    {
        InSequence s;
        sqlite3_stmt *dummyStmt;

        EXPECT_CALL(sqlite3Mock,
                    sqlite3_prepare_v2(dummyDb,
                                       StrEq("SELECT 1 FROM GeoData WHERE IP = ? LIMIT 1;"),
                                       -1, _, nullptr))
            .WillOnce(DoAll(SetArgPointee<3>(dummyStmt), Return(SQLITE_OK)));
        EXPECT_CALL(sqlite3Mock, sqlite3_bind_text(dummyStmt, 1, StrEq(ip.c_str()), -1, SQLITE_STATIC))
            .WillOnce(Return(SQLITE_OK));
        EXPECT_CALL(sqlite3Mock, sqlite3_step(dummyStmt)).WillOnce(Return(SQLITE_ROW));
        EXPECT_CALL(sqlite3Mock, sqlite3_finalize(dummyStmt)).Times(1);

        EXPECT_CALL(sqlite3Mock,
                    sqlite3_prepare_v2(dummyDb,
                                       StrEq("DELETE FROM GeoData WHERE IP = ?;"),
                                       -1, _, nullptr))
            .WillOnce(DoAll(SetArgPointee<3>(dummyStmt), Return(SQLITE_OK)));
        EXPECT_CALL(sqlite3Mock, sqlite3_bind_text(dummyStmt, 1, StrEq(ip.c_str()), -1, SQLITE_STATIC))
            .WillOnce(Return(SQLITE_OK));
        EXPECT_CALL(sqlite3Mock, sqlite3_step(dummyStmt)).WillOnce(Return(SQLITE_ERROR));
        EXPECT_CALL(sqlite3Mock, sqlite3_finalize(dummyStmt)).Times(1);
    }

    ASSERT_EQ(false, dbHandler->removeGeoLocData(ip));
}

TEST_F(DbHandlerApiTest, removeGeoLocData_bindFailed) {
    std::string ip{"1.2.3.4"};

    {
        InSequence s;
        sqlite3_stmt *dummyStmt;

        EXPECT_CALL(sqlite3Mock,
                    sqlite3_prepare_v2(dummyDb,
                                       StrEq("SELECT 1 FROM GeoData WHERE IP = ? LIMIT 1;"),
                                       -1, _, nullptr))
            .WillOnce(DoAll(SetArgPointee<3>(dummyStmt), Return(SQLITE_OK)));
        EXPECT_CALL(sqlite3Mock, sqlite3_bind_text(dummyStmt, 1, StrEq(ip.c_str()), -1, SQLITE_STATIC))
            .WillOnce(Return(SQLITE_OK));
        EXPECT_CALL(sqlite3Mock, sqlite3_step(dummyStmt)).WillOnce(Return(SQLITE_ROW));
        EXPECT_CALL(sqlite3Mock, sqlite3_finalize(dummyStmt)).Times(1);

        EXPECT_CALL(sqlite3Mock,
                    sqlite3_prepare_v2(dummyDb,
                                       StrEq("DELETE FROM GeoData WHERE IP = ?;"),
                                       -1, _, nullptr))
            .WillOnce(DoAll(SetArgPointee<3>(dummyStmt), Return(SQLITE_OK)));
        EXPECT_CALL(sqlite3Mock, sqlite3_bind_text(dummyStmt, 1, StrEq(ip.c_str()), -1, SQLITE_STATIC))
            .WillOnce(Return(SQLITE_ERROR));
        EXPECT_CALL(sqlite3Mock, sqlite3_step(dummyStmt)).WillOnce(Return(SQLITE_ERROR));
        EXPECT_CALL(sqlite3Mock, sqlite3_finalize(dummyStmt)).Times(1);
    }

    ASSERT_EQ(false, dbHandler->removeGeoLocData(ip));
}

TEST_F(DbHandlerApiTest, removeGeoLocData_prepareFailed) {
    std::string ip{"1.2.3.4"};

    {
        InSequence s;
        sqlite3_stmt *dummyStmt;

        EXPECT_CALL(sqlite3Mock,
                    sqlite3_prepare_v2(dummyDb,
                                       StrEq("SELECT 1 FROM GeoData WHERE IP = ? LIMIT 1;"),
                                       -1, _, nullptr))
            .WillOnce(DoAll(SetArgPointee<3>(dummyStmt), Return(SQLITE_OK)));
        EXPECT_CALL(sqlite3Mock, sqlite3_bind_text(dummyStmt, 1, StrEq(ip.c_str()), -1, SQLITE_STATIC))
            .WillOnce(Return(SQLITE_OK));
        EXPECT_CALL(sqlite3Mock, sqlite3_step(dummyStmt)).WillOnce(Return(SQLITE_ROW));
        EXPECT_CALL(sqlite3Mock, sqlite3_finalize(dummyStmt)).Times(1);

        EXPECT_CALL(sqlite3Mock,
                    sqlite3_prepare_v2(dummyDb,
                                       StrEq("DELETE FROM GeoData WHERE IP = ?;"),
                                       -1, _, nullptr))
            .WillOnce(DoAll(SetArgPointee<3>(dummyStmt), Return(SQLITE_ERROR)));
        EXPECT_CALL(sqlite3Mock, sqlite3_bind_text(dummyStmt, 1, StrEq(ip.c_str()), -1, SQLITE_STATIC))
            .WillOnce(Return(SQLITE_ERROR));
        EXPECT_CALL(sqlite3Mock, sqlite3_step(dummyStmt)).WillOnce(Return(SQLITE_ERROR));
        EXPECT_CALL(sqlite3Mock, sqlite3_finalize(dummyStmt)).Times(1);
    }

    ASSERT_EQ(false, dbHandler->removeGeoLocData(ip));
}

TEST_F(DbHandlerApiTest, removeGeoLocData_notExistingd) {
    std::string ip{"1.2.3.4"};

    {
        InSequence s;
        sqlite3_stmt *dummyStmt;

        EXPECT_CALL(sqlite3Mock,
                    sqlite3_prepare_v2(dummyDb,
                                       StrEq("SELECT 1 FROM GeoData WHERE IP = ? LIMIT 1;"),
                                       -1, _, nullptr))
            .WillOnce(DoAll(SetArgPointee<3>(dummyStmt), Return(SQLITE_OK)));
        EXPECT_CALL(sqlite3Mock, sqlite3_bind_text(dummyStmt, 1, StrEq(ip.c_str()), -1, SQLITE_STATIC))
            .WillOnce(Return(SQLITE_OK));
        EXPECT_CALL(sqlite3Mock, sqlite3_step(dummyStmt)).WillOnce(Return(SQLITE_DONE));
        EXPECT_CALL(sqlite3Mock, sqlite3_finalize(dummyStmt)).Times(1);
    }

    ASSERT_EQ(true, dbHandler->removeGeoLocData(ip));
}

TEST_F(DbHandlerApiTest, isExistingGeoLocData_existing) {
    std::string ip{"1.2.3.4"};

    {
        InSequence s;
        sqlite3_stmt *dummyStmt;

        EXPECT_CALL(sqlite3Mock,
                    sqlite3_prepare_v2(dummyDb,
                                       StrEq("SELECT 1 FROM GeoData WHERE IP = ? LIMIT 1;"),
                                       -1, _, nullptr))
            .WillOnce(DoAll(SetArgPointee<3>(dummyStmt), Return(SQLITE_OK)));
        EXPECT_CALL(sqlite3Mock, sqlite3_bind_text(dummyStmt, 1, StrEq(ip.c_str()), -1, SQLITE_STATIC))
            .WillOnce(Return(SQLITE_OK));
        EXPECT_CALL(sqlite3Mock, sqlite3_step(dummyStmt)).WillOnce(Return(SQLITE_ROW));
        EXPECT_CALL(sqlite3Mock, sqlite3_finalize(dummyStmt)).Times(1);
    }

    ASSERT_EQ(true, dbHandler->isExistingGeoLocData(ip));
}

TEST_F(DbHandlerApiTest, isExistingGeoLocData_notExisting) {
    std::string ip{"1.2.3.4"};

    {
        InSequence s;
        sqlite3_stmt *dummyStmt;

        EXPECT_CALL(sqlite3Mock,
                    sqlite3_prepare_v2(dummyDb,
                                       StrEq("SELECT 1 FROM GeoData WHERE IP = ? LIMIT 1;"),
                                       -1, _, nullptr))
            .WillOnce(DoAll(SetArgPointee<3>(dummyStmt), Return(SQLITE_OK)));
        EXPECT_CALL(sqlite3Mock, sqlite3_bind_text(dummyStmt, 1, StrEq(ip.c_str()), -1, SQLITE_STATIC))
            .WillOnce(Return(SQLITE_OK));
        EXPECT_CALL(sqlite3Mock, sqlite3_step(dummyStmt)).WillOnce(Return(SQLITE_DONE));
        EXPECT_CALL(sqlite3Mock, sqlite3_finalize(dummyStmt)).Times(1);
    }

    ASSERT_EQ(false, dbHandler->isExistingGeoLocData(ip));
}

TEST_F(DbHandlerApiTest, isExistingGeoLocData_stepFailed) {
    std::string ip{"1.2.3.4"};

    {
        InSequence s;
        sqlite3_stmt *dummyStmt;

        EXPECT_CALL(sqlite3Mock,
                    sqlite3_prepare_v2(dummyDb,
                                       StrEq("SELECT 1 FROM GeoData WHERE IP = ? LIMIT 1;"),
                                       -1, _, nullptr))
            .WillOnce(DoAll(SetArgPointee<3>(dummyStmt), Return(SQLITE_OK)));
        EXPECT_CALL(sqlite3Mock, sqlite3_bind_text(dummyStmt, 1, StrEq(ip.c_str()), -1, SQLITE_STATIC))
            .WillOnce(Return(SQLITE_OK));
        EXPECT_CALL(sqlite3Mock, sqlite3_step(dummyStmt)).WillOnce(Return(SQLITE_ERROR));
        EXPECT_CALL(sqlite3Mock, sqlite3_finalize(dummyStmt)).Times(1);
    }

    ASSERT_EQ(false, dbHandler->isExistingGeoLocData(ip));
}

TEST_F(DbHandlerApiTest, isExistingGeoLocData_bindFailed) {
    std::string ip{"1.2.3.4"};

    {
        InSequence s;
        sqlite3_stmt *dummyStmt;

        EXPECT_CALL(sqlite3Mock,
                    sqlite3_prepare_v2(dummyDb,
                                       StrEq("SELECT 1 FROM GeoData WHERE IP = ? LIMIT 1;"),
                                       -1, _, nullptr))
            .WillOnce(DoAll(SetArgPointee<3>(dummyStmt), Return(SQLITE_OK)));
        EXPECT_CALL(sqlite3Mock, sqlite3_bind_text(dummyStmt, 1, StrEq(ip.c_str()), -1, SQLITE_STATIC))
            .WillOnce(Return(SQLITE_ERROR));
        EXPECT_CALL(sqlite3Mock, sqlite3_step(dummyStmt)).WillOnce(Return(SQLITE_ERROR));
        EXPECT_CALL(sqlite3Mock, sqlite3_finalize(dummyStmt)).Times(1);
    }

    ASSERT_EQ(false, dbHandler->isExistingGeoLocData(ip));
}

TEST_F(DbHandlerApiTest, isExistingGeoLocData_prepareFailed) {
    std::string ip{"1.2.3.4"};

    {
        InSequence s;
        sqlite3_stmt *dummyStmt;

        EXPECT_CALL(sqlite3Mock,
                    sqlite3_prepare_v2(dummyDb,
                                       StrEq("SELECT 1 FROM GeoData WHERE IP = ? LIMIT 1;"),
                                       -1, _, nullptr))
            .WillOnce(DoAll(SetArgPointee<3>(dummyStmt), Return(SQLITE_ERROR)));
        EXPECT_CALL(sqlite3Mock, sqlite3_bind_text(dummyStmt, 1, StrEq(ip.c_str()), -1, SQLITE_STATIC))
            .WillOnce(Return(SQLITE_ERROR));
        EXPECT_CALL(sqlite3Mock, sqlite3_step(dummyStmt)).WillOnce(Return(SQLITE_ERROR));
        EXPECT_CALL(sqlite3Mock, sqlite3_finalize(dummyStmt)).Times(1);
    }

    ASSERT_EQ(false, dbHandler->isExistingGeoLocData(ip));
}