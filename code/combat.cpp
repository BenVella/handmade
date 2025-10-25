#include <cstdbool>
#include <cstdlib>

#define MAX_ROLLS 16
// ─────────────────────────────────────────────
// Types
// ─────────────────────────────────────────────

// Just a stat holder for original / max or modified
typedef struct {
  int str; // damage, vitality. 0 = unconscious
  int dex; // attack, move
  int wil; // resist, perceive
  int cha; // heal, persuade
} Stat;

// An active fighter containing a reference to their current stats
typedef struct {
  const Stat *stat;
  int poise;    // prevents damage
  int adv_left; // means we crit
  int hp;       // buffer before stat damage
} Fighter;

// A hit, rolled by the attacker towards defender
typedef struct {
  const Fighter* source;
  const Fighter* target; 
  const int roll; // natural
  const int mod; // actual
  const bool is_crit;
} Hit;

// Collection of fighters and their hits towards others
typedef struct {
  int num_opponents;
  Fighter *fighters; // array of opponents
  Hit *hits; // array of hit results
} Exchange;

// ─────────────────────────────────────────────
// Constructors
// ─────────────────────────────────────────────

inline Fighter create_fighter(const Stat &stat) {
  return Fighter{
      .stat = &stat,
      .poise = 4,
      .adv_left = 0,
      .hp = stat.str,
  };
}

inline const Hit create_hit(
    const Fighter &source, const Fighter &target,
    const int roll, const int mod, const bool is_crit
    ) {
  return Hit{
    .source = &source,
    .target = &target,
    .roll = roll,
    .mod = mod,
    .is_crit = is_crit
  };
}

inline Fighter *create_fighter_array(int size) {
  Fighter *fighters = new Fighter[size];
  Stat *stat = new Stat;
  for (int i = 0; i < size; ++i) {
    fighters[i] = create_fighter(*stat);
  }
  return fighters;
}


inline Exchange *create_exchange(int size) {
  return new Exchange{
    size,
    create_fighter_array(size),
    nullptr
  };
}



// ─────────────────────────────────────────────
// API
// ─────────────────────────────────────────────

struct ParsedRoll {
  int source;  // 1-based
  int roll;    // natural roll
  int mod;     // optional modifier (default 0)
  int target;  // 1-based
};



// ─────────────────────────────────────────────
// Combat
// ─────────────────────────────────────────────

Exchange *exhange_create_from_rolls(int size, int* rolls) {
  Exchange *exchange = create_exchange(size);
  // Todo: needs info up front for immutability
  return exchange;
}

// Ues when attacker <=15 nat, defender < 10 and overall mod roll is 5 or greater in favor of attacker
bool is_rolled_crit(
    const Hit &attacker, const Hit &defender,
    int attacker_mod, int defender_mod) {
  return attacker.roll >= 15 && defender.roll < 10 &&
         attacker_mod >= (defender_mod + 5);
}

// Supported formats:
//   - 2 fighters: "15,13" → 2 opposed rolls
//   - 3+ fighters: "15-2,13-1,14-1" → Each roll targets a specific fighter.
//   - Full: "1:15+5-2" → Fighter 1 rolls 15 with +5 modifier vs Fighter 2.
// Source indices are implicitly assigned starting from 1 unless explicitly provided.
// Target indices must be specified for 3 or more fighters.
// Modifier is optional and defaults to 0 if omitted.
int parse_rolls(const char *input, ParsedRoll *out, int max_rolls) {
  int count = 0;
  int source_counter = 1;

  const char *p = input;
  while (*p && count < max_rolls) {
    ParsedRoll roll = {};
    roll.source = source_counter++;

    // Find end of this token
    const char *end = p;
    while (*end && *end != ',') ++end;

    // Copy token to buffer
    char token[32];
    int len = (end - p < 31) ? (end - p) : 31;
    for (int i = 0; i < len; ++i) token[i] = p[i];
    token[len] = '\0';

    // Parse optional source
    char *cursor = token;
    char *colon = strchr(cursor, ':');
    if (colon) {
      *colon = '\0';
      roll.source = strtol(cursor, NULL, 10);
      cursor = colon + 1;
    }

    // Parse roll
    char *plus = strchr(cursor, '+');
    char *dash = strchr(cursor, '-');

    if (plus) {
      *plus = '\0';
      roll.roll = strtol(cursor, NULL, 10);
      cursor = plus + 1;

      if (dash) {
        *dash = '\0';
        roll.mod = strtol(cursor, NULL, 10);
        roll.target = strtol(dash + 1, NULL, 10);
      } else {
        roll.mod = strtol(cursor, NULL, 10);
        roll.target = (roll.source == 1) ? 2 : 1;
      }
    } else {
      if (dash) {
        *dash = '\0';
        roll.roll = strtol(cursor, NULL, 10);
        roll.target = strtol(dash + 1, NULL, 10);
      } else {
        roll.roll = strtol(cursor, NULL, 10);
        roll.target = (roll.source == 1) ? 2 : 1;
      }
    }

    out[count++] = roll;

    // Move to next token
    p = (*end == ',') ? end + 1 : end;
  }

  return count;
}
