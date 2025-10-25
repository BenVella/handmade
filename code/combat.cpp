#include <cstdlib>
#include <cstdbool>

// ─────────────────────────────────────────────
// Types
// ─────────────────────────────────────────────

typedef struct {
  int str; // damage, vitality. 0 = unconscious
  int dex; // attack, move
  int wil; // resist, perceive
  int cha; // heal, persuade
} Stats;

typedef struct {
  const Stats* stats;
  int poise; // prevents damage
  int adv_left; // means we crit
  int hp; // buffer before stat damage
} Fighter;

typedef struct {
  int roll;
  bool targeted;
  bool is_crit;
} Hit;

typedef struct {
  Fighter* player;
  Fighter* opponents;  // dynamic array of opponents
  int num_opponents;
  Hit* hits;           // dynamic array of hit results
} Exchange;

// ─────────────────────────────────────────────
// Constructors
// ─────────────────────────────────────────────

inline Fighter create_fighter(const Stats& i_stats) {
  return Fighter {
    .stats = &i_stats,
    .poise = 4,
    .adv_left = 0,
    .hp = i_stats.str,
  };
}

inline Fighter* create_fighter_array(int count) {
  Fighter* fighters = new Fighter[count];
  Stats* stats = new Stats;
  for (int i = 0; i < count; ++i) {
    fighters[i] = create_fighter(*stats);
  }
  return fighters;
}

inline Exchange* create_exchange() {
  return new Exchange{nullptr, nullptr, 0, nullptr};
}

inline Exchange* create_exchange(int num_opponents) {
  Exchange* ex = create_exchange();
  ex->num_opponents = num_opponents;
  ex->opponents = new Fighter[num_opponents];
  ex->hits = new Hit[num_opponents];
  return ex;
}

// ─────────────────────────────────────────────
// Combat
// ─────────────────────────────────────────────

void exchange_roll(Exchange& ex, int player_roll) {
  if (!ex.player || !ex.hits || !ex.opponents) return;

  int player_mod = player_roll + ex.player->dex;

  for (int i = 0; i < ex.num_opponents; ++i) {
    int mod_hit = ex.hits[i].roll + ex.opponents[i].dex;
    int delta = mod_hit - player_mod;
    // store or process delta
  }
}

bool is_rolled_crit(const Hit& attacker, const Hit& defender, int attacker_mod, int defender_mod) {
  return attacker.roll >= 15 &&
         defender.roll > 10 &&
         attacker_mod >= (defender_mod + 5);
}
