#pragma once
#include <date/date.h>

#include <optional>
#include <semver.hpp>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

namespace keepachangelog {

struct ParseError : std::runtime_error {
  using std::runtime_error::runtime_error;
};

enum class ChangeType {
  ADDED,
  CHANGED,
  DEPRECATED,
  REMOVED,
  FIXED,
  SECURITY,
};

struct Change {
  ChangeType type;
  std::vector<std::string> items;
};

struct Entry {
  std::variant<std::string, semver::version<>> version;
  std::optional<date::year_month_day> date;
  std::vector<Change> changes;
};

class Changelog {
public:
  std::vector<Entry> entries;

  static Changelog parse(const std::string& input);
  std::string unparse();
};

std::string to_string(const ChangeType& change_type);
}  // namespace keepachangelog
