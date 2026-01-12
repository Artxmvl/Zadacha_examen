#ifndef DATABASE_CONNECTION_H
#define DATABASE_CONNECTION_H

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <libpq-fe.h>

template <typename T>
class DatabaseConnection {
private:
    PGconn* conn;
    
public:
    DatabaseConnection(const std::string& connStr) {
        conn = PQconnectdb(connStr.c_str());
        if (PQstatus(conn) != CONNECTION_OK) {
            std::cerr << "Connection failed: " << PQerrorMessage(conn) << std::endl;
            PQfinish(conn);
            conn = nullptr;
        }
    }
    
    ~DatabaseConnection() {
        if (conn) {
            PQfinish(conn);
        }
    }
    
    bool isConnected() const {
        return conn != nullptr && PQstatus(conn) == CONNECTION_OK;
    }
    
    PGresult* executeQuery(const std::string& query) {
        if (!conn) return nullptr;
        return PQexec(conn, query.c_str());
    }
    
    bool beginTransaction() {
        return executeQuery("BEGIN") != nullptr;
    }
    
    bool commit() {
        return executeQuery("COMMIT") != nullptr;
    }
    
    bool rollback() {
        return executeQuery("ROLLBACK") != nullptr;
    }
};

#endif
