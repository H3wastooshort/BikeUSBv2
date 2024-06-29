enum chg_speed_t {
  CHG_2W5,
  CHG_5W,
  CHG_7W5
};

bool level_2W5_possible() {
  if (config.min_speed_2W5 <= 0) return false;
  return AC_frequency > config.min_speed_2W5;
}

bool level_5W_possible() {
  if (config.min_speed_5W <= 0) return false;
  return AC_frequency > config.min_speed_5W;
}

bool level_7W5_possible() {
  if (config.min_speed_7W5 <= 0) return false;
  return AC_frequency > config.min_speed_7W5;
}

bool level_possible(chg_speed_t level) {
  switch (uint8_t(level)) {
    case CHG_2W5: return level_2W5_possible();
    case CHG_5W: return level_5W_possible();
    case CHG_7W5: return level_7W5_possible();
  }
  return false;
}
