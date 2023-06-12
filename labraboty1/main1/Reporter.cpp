#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct employee
{
    int num;
    char name[10];
    double hours;
};

int main(int argc, char* argv[])
{
    if (argc != 4)
    {
        cerr << "Error: Argument exception (must be equal 4)" << std::endl;
        return 1;
    }

    string binaryFileName = argv[1];
    string reportFileName = argv[2];
    double payPerHour = stod(argv[3]);

    ifstream binaryFile(binaryFileName, ios::binary);
    if (!binaryFile)
    {
        cerr << "Error opening binary file " << binaryFileName << endl;
        return 1;
    }

    ofstream reportFile(reportFileName);
    if (!reportFile)
    {
        cerr << "Error opening report file " << reportFileName << endl;
        return 1;
    }

    reportFile << "Report based on binary file " << binaryFileName << endl;
    reportFile << "Employee \tName\tHours\tSalary" << endl;

    employee employee;
    while (binaryFile.read((char*)&employee, sizeof(employee)))
    {
        double salary = employee.hours * payPerHour;
        reportFile << employee.num << "\t\t" << employee.name << "\t" << employee.hours << "\t" << salary << endl;
    }

    binaryFile.close();
    reportFile.close();

    return 0;
}
