#include "log_analyzer.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace {

void printUsage(const char* program) {
    std::cout << "Usage: " << program << " <log-file> [--level DEBUG|INFO|WARNING|ERROR|CRITICAL|UNKNOWN]\n";
}

}  // namespace

int main(int argc, char* argv[]) {
    if (argc != 2 && argc != 4) {
        printUsage(argv[0]);
        return 2;
    }

    std::ifstream input(argv[1]);
    if (!input) {
        std::cerr << "Could not open log file: " << argv[1] << '\n';
        return 1;
    }

    std::vector<logtool::LogEntry> entries;
    std::string line;
    while (std::getline(input, line)) {
        entries.push_back(logtool::parseLine(line));
    }

    const auto summary = logtool::summarize(entries);
    std::cout << "Total lines: " << summary.total << '\n';
    for (const auto severity : {
             logtool::Severity::Debug,
             logtool::Severity::Info,
             logtool::Severity::Warning,
             logtool::Severity::Error,
             logtool::Severity::Critical,
             logtool::Severity::Unknown}) {
        std::cout << logtool::toString(severity) << ": " << summary.count(severity) << '\n';
    }

    if (argc == 4) {
        if (std::string(argv[2]) != "--level") {
            printUsage(argv[0]);
            return 2;
        }
        const auto requested = logtool::severityFromString(argv[3]);
        if (!requested) {
            std::cerr << "Unknown level: " << argv[3] << '\n';
            return 2;
        }

        std::cout << "\nFiltered entries (" << logtool::toString(*requested) << "):\n";
        for (const auto& entry : logtool::filterBySeverity(entries, *requested)) {
            std::cout << entry.raw << '\n';
        }
    }

    return 0;
}
