#include <iostream>
#include <fstream>
#include <Windows.h>
#include <tchar.h>
#include <algorithm>
#include <string>




struct employee {
    int num;
    char name[10];
    double hours;
};

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Error: Argument exception (must be equal 3)" << std::endl;
        return 1;
    }

    STARTUPINFO si; // information about windows(console)
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    PROCESS_INFORMATION pi; // information about process, for controll it
    ZeroMemory(&pi, sizeof(pi));

    std::string command = "Creator.exe " + std::string(argv[1]) + std::string(" ") + std::string(argv[2]);
    TCHAR commandLine[MAX_PATH];
    _tcscpy_s(commandLine, MAX_PATH, command.c_str());

    if (!CreateProcess(NULL, commandLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
    {
        std::cerr << "Error creating process" << std::endl;
        return 1;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);

    std::ifstream input(argv[1], std::ios::binary);
    if (!input) {
        std::cerr << "Error: Could not open file for reading" << std::endl;
        return 1;
    }

    employee record;
    std::cout << std::endl;
    std::cout << "Records :" << std::endl;
    while (input.read(reinterpret_cast<char*>(&record), sizeof(record))) {
        std::cout << "ID: " << record.num << std::endl;
        std::cout << "Name: " << record.name << std::endl;
        std::cout << "Hours: " << record.hours << std::endl;
        std::cout << std::endl;
    }
    
    std::cout << "Write name for Report file : " << std::endl;
    std::string reportFile;
    std::cin >> reportFile;
    double salary;
    std::cout << "Write salary : " << std::endl;
    std::cin >> salary;
    input.close();

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    command = "Reporter.exe " + std::string(argv[1]) + std::string(" ") + reportFile + std::string(" ") + std::to_string(salary);
    _tcscpy_s(commandLine, MAX_PATH, command.c_str());

    if (!CreateProcess(NULL, commandLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
    {
        std::cerr << "Error creating process" << std::endl;
        return 1;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);

    input.open(reportFile);
    if (!input) {
        std::cerr << "Error: Could not open reporter file for reading" << std::endl;
        return 1;
    }

    std::string line;
    std::cout << std::endl;
    while (std::getline(input, line))
    {
        std::cout << line << std::endl;
    }
    return 0;
}
