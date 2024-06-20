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
