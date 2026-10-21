#pragma once

#include <array>
#include <cstddef>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace logtool {

enum class Severity : std::size_t {
    Debug = 0,
    Info,
    Warning,
    Error,
    Critical,
    Unknown,
    Count,
};

struct LogEntry {
    Severity severity{Severity::Unknown};
    std::string message;
    std::string raw;
};

struct Summary {
    std::array<std::size_t, static_cast<std::size_t>(Severity::Count)> counts{};
    std::size_t total{0};

    void add(Severity severity) noexcept;
    [[nodiscard]] std::size_t count(Severity severity) const noexcept;
};

[[nodiscard]] std::string_view toString(Severity severity) noexcept;
[[nodiscard]] std::optional<Severity> severityFromString(std::string_view text);
[[nodiscard]] LogEntry parseLine(std::string line);
[[nodiscard]] Summary summarize(const std::vector<LogEntry>& entries) noexcept;
[[nodiscard]] std::vector<LogEntry> filterBySeverity(
    const std::vector<LogEntry>& entries,
    Severity severity);

}  // namespace logtool
