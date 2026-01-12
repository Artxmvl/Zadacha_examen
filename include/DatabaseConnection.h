#ifndef DATABASE_CONNECTION_H
#define DATABASE_CONNECTION_H

#include <string>
#include <vector>
#include <memory>
#include <iostream>
using namespace std;

template <typename T>
class DatabaseConnection {
private:
    string connStr;
    bool connected;
    bool inTransaction;
    
public:
    DatabaseConnection(const string& connStr) 
        : connStr(connStr), connected(false), inTransaction(false) {
        connect();
    }
    
    virtual ~DatabaseConnection() {
        disconnect();
    }
    
    void connect() {
        cout << "Connecting to database..." << endl;
        connected = true;
    }
    
    void disconnect() {
        if (connected) {
            cout << "Disconnecting..." << endl;
            connected = false;
        }
    }
    
    bool isConnected() const { return connected; }
    
    vector<vector<T>> executeQuery(const string& query) {
        if (!connected) {
            cerr << "Error: Not connected!" << endl;
            return vector<vector<T>>();
        }
        cout << "Query: " << query << endl;
        return vector<vector<T>>();
    }
    
    bool executeNonQuery(const string& query) {
        if (!connected) {
            cerr << "Error: Not connected!" << endl;
            return false;
        }
        cout << "Execute: " << query << endl;
        return true;
    }
    
    bool beginTransaction() {
        inTransaction = true;
        cout << "Transaction started" << endl;
        return true;
    }
    
    bool commitTransaction() {
        inTransaction = false;
        cout << "Transaction committed" << endl;
        return true;
    }
    
    bool rollbackTransaction() {
        inTransaction = false;
        cout << "Transaction rolled back" << endl;
        return true;
    }
    
    bool createFunction(const string& name, const string& body) {
        cout << "Function " << name << " created" << endl;
        return executeNonQuery(body);
    }
    
    bool createTrigger(const string& name, const string& body) {
        cout << "Trigger " << name << " created" << endl;
        return executeNonQuery(body);
    }
    
    bool getTransactionStatus() const { return inTransaction; }
};

#endif
