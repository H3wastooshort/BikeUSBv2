enum msm_states_s {
  //initial
  MSM_START = 0,
  MSM_GOT_AC,

  //pre-charging (transient)
  MSM_DO_POWER_DOWN,
  MSM_DO_POWER_UP,
  //pre-charging
  MSM_POWERED_DOWN,

  //digital (transient)
  MSM_SWITCH_2W5,
  MSM_SWITCH_5W,
  MSM_SWITCH_7W5,
  //digital
  MSM_2W5,
  MSM_5W,
  MSM_7W5,

  MSM_SWITCH_DUMB_MODE,
  MSM_DUMB_MODE,

  //misc
  MSM_CALIBRATION,
  MSM_CONFIGURATION,
  MSM_NO_CAL

} msm_state = MSM_START;

void msm_change_state(msm_states_s new_state) {
  printDebug<uint8_t>(DBG_MSM_STATE, msm_state);
  msm_state = new_state;
  printDebug<uint8_t>(DBG_MSM_STATE, msm_state);
}
