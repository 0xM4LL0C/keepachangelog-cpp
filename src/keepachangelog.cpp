#include "keepachangelog.hpp"

#include <regex>
#include <semver.hpp>
#include <sstream>
#include <string>
#include <unordered_map>
#include <variant>

namespace keepachangelog {

Changelog Changelog::parse(const std::string& input) {
  Changelog changelog;

  std::istringstream stream(input);
  std::string line;

  Entry current_entry;
  std::optional<Change> current_change;
  bool in_entry = false;

  std::regex header_regex(R"(^## \[([^\]]+)\](?: - (\d{4}-\d{2}-\d{2}))?)");
  std::regex section_regex(R"(^###\s+(.+))");
  std::regex bullet_regex(R"(^[-*]\s+(.*))");

  static const std::unordered_map<std::string, ChangeType> section_map = {
      {"Added", ChangeType::ADDED},
      {"Changed", ChangeType::CHANGED},
      {"Deprecated", ChangeType::DEPRECATED},
      {"Removed", ChangeType::REMOVED},
      {"Fixed", ChangeType::FIXED},
      {"Security", ChangeType::SECURITY},
  };

  while (std::getline(stream, line)) {
    std::smatch match;

    if (std::regex_match(line, match, header_regex)) {
      if (in_entry) {
        if (current_change) {
          current_entry.changes.push_back(*current_change);
          current_change.reset();
        }
        changelog.entries.push_back(std::move(current_entry));
        current_entry = Entry{};
      }

      std::string version_str = match[1];
      std::string date_str = match.size() > 2 ? match[2].str() : "";

      if (version_str == "Unreleased") {
        current_entry.version = version_str;
      } else {
        semver::version<> v;
        if (!semver::parse(version_str, v)) {
          throw ParseError("Invalid version: " + version_str);
        }
        current_entry.version = v;
      }

      if (!date_str.empty()) {
        std::istringstream date_stream(date_str);
        std::chrono::sys_days date_days;
        date_stream >> std::chrono::parse("%F", date_days);
        if (date_stream.fail()) {
          throw ParseError("Can't parse date: " + date_str);
        }
        current_entry.date = std::chrono::year_month_day{date_days};
      }

      in_entry = true;
    }

    else if (std::regex_match(line, match, section_regex)) {
      if (!in_entry) {
        throw ParseError("Section found outside of entry: " + line);
      }
      if (current_change) {
        current_entry.changes.push_back(*current_change);
      }

      std::string section_name = match[1];
      auto it = section_map.find(section_name);
      if (it == section_map.end()) {
        throw ParseError("Unknown section: " + section_name);
      }

      current_change = Change{.type = it->second, .items = {}};
    }

    else if (std::regex_match(line, match, bullet_regex)) {
      if (!current_change) {
        throw ParseError("Change item found outside of section: " + line);
      }
      current_change->items.push_back(match[1]);
    }
  }

  if (in_entry) {
    if (current_change) {
      current_entry.changes.push_back(*current_change);
    }
    changelog.entries.push_back(std::move(current_entry));
  }

  return changelog;
}

std::string Changelog::unparse() {
  std::stringstream out;

  for (const auto& entry : this->entries) {
    out << "## ";
    out << "[";
    if (std::holds_alternative<std::string>(entry.version)) {
      out << std::get<std::string>(entry.version);
    } else if (std::holds_alternative<semver::version<>>(entry.version)) {
      out << std::get<semver::version<>>(entry.version).to_string();
    }
    out << "]";

    if (entry.date.has_value()) {
      out << " - ";
      auto date = entry.date.value();
      out << date.year() << "-" << std::setw(2) << std::setfill('0')
          << static_cast<unsigned>(date.month()) << "-" << std::setw(2)
          << std::setfill('0') << static_cast<unsigned>(date.day());
    }

    out << "\n\n";

    for (const auto& change : entry.changes) {
      out << "### " << to_string(change.type) << "\n\n";

      for (const auto& item : change.items) {
        out << "- " << item << "\n";
      }

      out << "\n";
    }
  }
  return out.str();
}

std::string to_string(const ChangeType& change_type) {
  switch (change_type) {
    case ChangeType::ADDED:
      return "Added";
    case ChangeType::CHANGED:
      return "Changed";
    case ChangeType::DEPRECATED:
      return "Deprecated";
    case ChangeType::FIXED:
      return "Fixed";
    case ChangeType::REMOVED:
      return "Removed";
    case ChangeType::SECURITY:
      return "Security";
    default:
      throw ParseError("Unknown ChangeType");
  }
}

}  // namespace keepachangelog
