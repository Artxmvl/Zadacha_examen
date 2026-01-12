#ifndef REPORT_GENERATOR_H
#define REPORT_GENERATOR_H

#include <string>
#include <vector>
#include <fstream>
using namespace std;

class ReportGenerator {
public:
    static void generateAuditReport(const string& filename);
    static void generateOrderHistoryReport(const string& filename);
    static void generateUserActivityReport(const string& filename);
    
private:
    static void writeCSVHeader(ofstream& file, const vector<string>& headers);
    static void writeCSVRow(ofstream& file, const vector<string>& data);
};

#endif
