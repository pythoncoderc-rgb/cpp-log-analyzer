#include "log_analyzer.hpp"

#include <cassert>
#include <iostream>
#include <vector>

int main() {
    using logtool::Severity;

    const auto info = logtool::parseLine("[2026-10-21 12:00:00] [INFO] Application started");
    assert(info.severity == Severity::Info);
    assert(info.message == "Application started");

    const auto warn = logtool::parseLine("[WARN] Low disk space");
    assert(warn.severity == Severity::Warning);

    const auto critical = logtool::parseLine("[FATAL] Database unavailable");
    assert(critical.severity == Severity::Critical);

    const auto unknown = logtool::parseLine("plain line without severity");
    assert(unknown.severity == Severity::Unknown);

    const std::vector<logtool::LogEntry> entries{info, warn, critical, unknown};
    const auto summary = logtool::summarize(entries);
    assert(summary.total == 4);
    assert(summary.count(Severity::Info) == 1);
    assert(summary.count(Severity::Warning) == 1);
    assert(summary.count(Severity::Critical) == 1);
    assert(summary.count(Severity::Unknown) == 1);

    const auto filtered = logtool::filterBySeverity(entries, Severity::Warning);
    assert(filtered.size() == 1);
    assert(filtered.front().message == "Low disk space");

    assert(logtool::severityFromString("error") == Severity::Error);
    assert(logtool::severityFromString("warning") == Severity::Warning);
    assert(!logtool::severityFromString("trace"));

    std::cout << "log_analyzer_tests: PASS\n";
    return 0;
}
