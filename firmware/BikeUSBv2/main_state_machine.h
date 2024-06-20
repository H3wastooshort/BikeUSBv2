enum msm_states_s {
  //initial
  MSM_START = 0,
  MSM_GOT_AC,

  //pre-charging (transient)
  MSM_DO_POWER_DOWN,
  MSM_DO_POWER_UP,
  //pre-charging
  MSM_POWERED_DOWN,
  MSM_WAITING_FOR_DEVICE,

  //digital (transient)
  MSM_D_SWITCH_2W5,
  MSM_D_SWITCH_5W,
  MSM_D_SWITCH_7W5,
  //digital
  MSM_D_2W5,
  MSM_D_5W,
  MSM_D_7W5,

  //analog (transient)
  MSM_A_SWITCH_2W5,
  MSM_A_SWITCH_7W5,
  //analog
  MSM_A_2W5,
  MSM_A_7W5,

  //misc
  MSM_CALIBRATION,
  MSM_CONFIGURATION

} msm_state = MSM_START;
