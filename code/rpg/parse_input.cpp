#include "../util/logger.cpp"
#include <charconv>
#include <cstdio>
#include <string_view>

struct ParsedRoll {
  int src = 0;  // source index (1-based)
  int roll = 0; // natural roll
  int mod = 0;  // optional modifier
  int tar = 0;  // target index (1-based)
};

// Pure helper: parses string_view into int, returns true on success, false on
// failure
inline bool parse_int(std::string_view sv, int &out) {
  auto res = std::from_chars(sv.data(), sv.data() + sv.size(), out);
  return res.ec == std::errc();
}

inline bool err_log_parse_rolls(int rollEntry, std::string_view token,
                                const char *field = nullptr) {
  if (field) {
    (LogLevel::ERROR, "Error parsing field '%s' in roll entry #%d: '%.*s'\n",
     field, rollEntry, static_cast<int>(token.size()), token.data());
  } else {
    std::fprintf(stderr, "Error parsing roll entry #%d: '%.*s'\n", rollEntry,
                 static_cast<int>(token.size()), token.data());
  }
  return false;
}

// Parse multiple rolls
// Returns true if all rolls parsed successfully, false on first error
bool parse_rolls(const char *input, ParsedRoll *out, int max_rolls,
                 int &num_entries) {
  num_entries = 0;
  int src_counter = 1;

  const char *p = input;
  while (*p && num_entries < max_rolls) {
    ParsedRoll roll{};
    roll.src = src_counter++;

    // Save full token for error messages
    const char *end = p;
    while (*end && *end != ',')
      ++end;
    std::string_view token(p, end - p);

    // Helper lambda: parse field or log error
    auto parse = [&](std::string_view sv, int &field,
                     const char *name) -> bool {
      if (!parse_int(sv, field)) {
        return err_log_parse_rolls(num_entries, sv, name);
      }
      return true;
    };

    // Parse optional source
    size_t colon = token.find(':');
    if (colon != std::string_view::npos) {
      if (!parse(token.substr(0, colon), roll.src, "src"))
        return false;
      token = token.substr(colon + 1);
    }

    // Find optional modifier and target
    size_t plus = token.find('+');
    size_t dash = token.find('-');

    // Try parsing modifier
    if (plus != std::string_view::npos) {
      if (!parse(token.substr(0, plus), roll.roll, "roll"))
        return false;
      std::string_view after_plus = token.substr(plus + 1);

      // Parse mod until tar if present
      if (dash != std::string_view::npos) {
        // With a target we grab mod from after_plus until dash
        if (!parse(after_plus.substr(0, dash - (plus + 1)), roll.mod, "mod"))
          return false;
        // Then get dash
        if (!parse(after_plus.substr((dash - (plus + 1)) + 1), roll.tar, "tar"))
          return false;
      } else {
        // No tar present, just mod till the end
        if (!parse(after_plus, roll.mod, "mod"))
          return false;
        roll.tar = (roll.src == 1) ? 2 : 1;
      }
    } else if (dash != std::string_view::npos) {
      if (!parse(token.substr(0, dash), roll.roll, "roll"))
        return false;
      if (!parse(token.substr(dash + 1), roll.tar, "tar"))
        return false;
    } else {
      if (!parse(token, roll.roll, "roll"))
        return false;
      roll.tar = (roll.src == 1) ? 2 : 1;
    }

    out[num_entries++] = roll;
    p = (*end == ',') ? end + 1 : end;
  }

  return true;
}
