enum chg_speed_t {
  CHG_2W5,
  CHG_5W,
  CHG_7W5
};

bool level_cmp(const uint16_t thresh, const int8_t hyst) {
  if (thresh == 0) return false;
  if (hyst == 0) return AC_frequency > thresh;
  else return AC_frequency > (thresh + (hyst * FREQ_HYST * float(thresh)));
}

bool level_2W5_possible(const int8_t hyst = 0) {
  return level_cmp(config.min_speed_2W5, hyst);
}

bool level_5W_possible(const int8_t hyst = 0) {
  return level_cmp(config.min_speed_5W, hyst);
}

bool level_7W5_possible(const int8_t hyst = 0) {
  return level_cmp(config.min_speed_7W5, hyst);
}

bool level_possible(const chg_speed_t level) {
  switch (uint8_t(level)) {
    case CHG_2W5: return level_2W5_possible();
    case CHG_5W: return level_5W_possible();
    case CHG_7W5: return level_7W5_possible();
  }
  return false;
}
