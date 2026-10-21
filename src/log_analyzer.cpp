#include "log_analyzer.hpp"

#include <algorithm>
#include <cctype>

namespace logtool {
namespace {

std::string upper(std::string_view value) {
    std::string result(value);
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char ch) {
        return static_cast<char>(std::toupper(ch));
    });
    return result;
}

std::string trimLeft(std::string value) {
    const auto pos = value.find_first_not_of(" \t");
    if (pos == std::string::npos) {
        return {};
    }
    value.erase(0, pos);
    return value;
}

}  // namespace

void Summary::add(Severity severity) noexcept {
    ++total;
    ++counts[static_cast<std::size_t>(severity)];
}

std::size_t Summary::count(Severity severity) const noexcept {
    return counts[static_cast<std::size_t>(severity)];
}

std::string_view toString(Severity severity) noexcept {
    switch (severity) {
        case Severity::Debug: return "DEBUG";
        case Severity::Info: return "INFO";
        case Severity::Warning: return "WARNING";
        case Severity::Error: return "ERROR";
        case Severity::Critical: return "CRITICAL";
        case Severity::Unknown: return "UNKNOWN";
        case Severity::Count: break;
    }
    return "UNKNOWN";
}

std::optional<Severity> severityFromString(std::string_view text) {
    const auto value = upper(text);
    if (value == "DEBUG") return Severity::Debug;
    if (value == "INFO") return Severity::Info;
    if (value == "WARN" || value == "WARNING") return Severity::Warning;
    if (value == "ERROR") return Severity::Error;
    if (value == "CRITICAL" || value == "FATAL") return Severity::Critical;
    if (value == "UNKNOWN") return Severity::Unknown;
    return std::nullopt;
}

LogEntry parseLine(std::string line) {
    LogEntry entry;
    entry.raw = line;

    std::size_t searchFrom = 0;
    while (true) {
        const auto open = line.find('[', searchFrom);
        if (open == std::string::npos) break;
        const auto close = line.find(']', open + 1);
        if (close == std::string::npos) break;

        const auto token = std::string_view(line).substr(open + 1, close - open - 1);
        if (const auto severity = severityFromString(token)) {
            entry.severity = *severity;
            entry.message = trimLeft(line.substr(close + 1));
            return entry;
        }
        searchFrom = close + 1;
    }

    entry.message = line;
    return entry;
}

Summary summarize(const std::vector<LogEntry>& entries) noexcept {
    Summary summary;
    for (const auto& entry : entries) {
        summary.add(entry.severity);
    }
    return summary;
}

std::vector<LogEntry> filterBySeverity(
    const std::vector<LogEntry>& entries,
    Severity severity) {
    std::vector<LogEntry> result;
    for (const auto& entry : entries) {
        if (entry.severity == severity) {
            result.push_back(entry);
        }
    }
    return result;
}

}  // namespace logtool
