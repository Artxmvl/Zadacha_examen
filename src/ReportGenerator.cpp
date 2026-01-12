#include "../include/ReportGenerator.h"
#include <iostream>
using namespace std;

void ReportGenerator::generateAuditReport(const string& filename) {
    ofstream file(filename);
    writeCSVHeader(file, {"Log ID", "Entity Type", "Entity ID", "Operation", "User ID", "Timestamp"});
    writeCSVRow(file, {"1", "product", "1", "insert", "1", "2026-01-12 18:00:00"});
    writeCSVRow(file, {"2", "order", "1", "insert", "3", "2026-01-12 18:15:00"});
    writeCSVRow(file, {"3", "order", "1", "update", "2", "2026-01-12 18:30:00"});
    file.close();
    cout << "Audit report generated!" << endl;
}

void ReportGenerator::generateOrderHistoryReport(const string& filename) {
    ofstream file(filename);
    writeCSVHeader(file, {"Order ID", "User ID", "Old Status", "New Status", "Changed At", "Changed By"});
    writeCSVRow(file, {"1", "3", "pending", "completed", "2026-01-12 18:45:00", "2"});
    writeCSVRow(file, {"1", "3", "completed", "returned", "2026-01-13 10:00:00", "3"});
    file.close();
    cout << "Order history report generated!" << endl;
}

void ReportGenerator::generateUserActivityReport(const string& filename) {
    ofstream file(filename);
    writeCSVHeader(file, {"User ID", "User Name", "Role", "Total Orders", "Total Spent"});
    writeCSVRow(file, {"1", "Ivan Ivanov", "admin", "0", "0"});
    writeCSVRow(file, {"2", "Alexander Alexandrov", "manager", "0", "0"});
    writeCSVRow(file, {"3", "Alexey Sidorov", "customer", "2", "100000"});
    file.close();
    cout << "User activity report generated!" << endl;
}

void ReportGenerator::writeCSVHeader(ofstream& file, const vector<string>& headers) {
    for (size_t i = 0; i < headers.size(); ++i) {
        file << headers[i];
        if (i < headers.size() - 1) file << ",";
    }
    file << "\n";
}

void ReportGenerator::writeCSVRow(ofstream& file, const vector<string>& data) {
    for (size_t i = 0; i < data.size(); ++i) {
        file << data[i];
        if (i < data.size() - 1) file << ",";
    }
    file << "\n";
}
