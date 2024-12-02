#ifndef SOFOMO_TESTS_MOCKS_SQLITE3_STUBS_HPP_
#define SOFOMO_TESTS_MOCKS_SQLITE3_STUBS_HPP_

#include "sqlite3.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

extern "C"
{
struct Sqlite3Mock {
    MOCK_METHOD(SQLITE_API int, sqlite3_open_v2, (const char *, sqlite3 **, int, const char *));
    MOCK_METHOD(SQLITE_API int, sqlite3_close_v2, (sqlite3 * db));
    MOCK_METHOD(SQLITE_API int, sqlite3_prepare_v2, (sqlite3 * , const char *, int, sqlite3_stmt **, const char **));
    MOCK_METHOD(SQLITE_API int, sqlite3_column_int, (sqlite3_stmt * , int));
    MOCK_METHOD(SQLITE_API int, sqlite3_step, (sqlite3_stmt * ));
    MOCK_METHOD(SQLITE_API int, sqlite3_exec, (sqlite3 * , const char *, sqlite3_callback, void *, char **));
    MOCK_METHOD(SQLITE_API int, sqlite3_finalize, (sqlite3_stmt * ));
    MOCK_METHOD(SQLITE_API int, sqlite3_bind_text, (sqlite3_stmt * , int, const char *, int, void (*)(void *)));
    MOCK_METHOD(SQLITE_API const char *, sqlite3_errmsg, (sqlite3 * ));
    MOCK_METHOD(SQLITE_API const unsigned char*, sqlite3_column_text, (sqlite3_stmt * , int));
};

static Sqlite3Mock sqlite3Mock;

SQLITE_API int sqlite3_open_v2(const char *filename, sqlite3 **ppDb, int flags, const char *zVfs) {
    return sqlite3Mock.sqlite3_open_v2(filename, ppDb, flags, zVfs);
}
SQLITE_API int sqlite3_prepare_v2(sqlite3 *db,
                                  const char *zSql,
                                  int nBytes,
                                  sqlite3_stmt **ppStmt,
                                  const char **pzTail) {
    return sqlite3Mock.sqlite3_prepare_v2(db, zSql, nBytes, ppStmt, pzTail);
}
SQLITE_API int sqlite3_column_int(sqlite3_stmt *pStmt, int i) {
    return sqlite3Mock.sqlite3_column_int(pStmt, i);
}
SQLITE_API int sqlite3_step(sqlite3_stmt *pStmt) {
    return sqlite3Mock.sqlite3_step(pStmt);
}
SQLITE_API int sqlite3_exec(sqlite3 *db, const char *zSql, sqlite3_callback xCallback, void *pArg, char **pzErrMsg) {
    return sqlite3Mock.sqlite3_exec(db, zSql, xCallback, pArg, pzErrMsg);
}
SQLITE_API int sqlite3_finalize(sqlite3_stmt *pStmt) {
    return sqlite3Mock.sqlite3_finalize(pStmt);
}
SQLITE_API int sqlite3_bind_text(
    sqlite3_stmt *pStmt,
    int i,
    const char *zData,
    int nData,
    void (*xDel)(void *)
) {
    return sqlite3Mock.sqlite3_bind_text(pStmt, i, zData, nData, xDel);
}
SQLITE_API const char *sqlite3_errmsg(sqlite3 *db) {
    return sqlite3Mock.sqlite3_errmsg(db);
}
SQLITE_API const unsigned char *sqlite3_column_text(sqlite3_stmt *pStmt, int i) {
    return sqlite3Mock.sqlite3_column_text(pStmt, i);
}
SQLITE_API int sqlite3_close_v2(sqlite3 *db) {
    return sqlite3Mock.sqlite3_close_v2(db);
}

}

#endif //SOFOMO_TESTS_MOCKS_SQLITE3_STUBS_HPP_
