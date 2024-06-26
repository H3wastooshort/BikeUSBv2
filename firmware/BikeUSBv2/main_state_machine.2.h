void run_msm() {
  switch (msm_state) {
    case MSM_START:

      //conf mode
      if (!AC_new_meas_flag) {  //wait for AC measurement
        if (millis() > 5000) {  //after 5 seconds with no ac waveform, time out and decide it's DC => conf mode
          config_setup();
          msm_change_state(MSM_CONFIGURATION);
        }
        return;
      } else msm_change_state(MSM_GOT_AC);
      break;


    case MSM_GOT_AC:
      //cal mode
      if (config.calibrate_mode != 0) {
        calibration_setup();
        msm_change_state(MSM_CALIBRATION);
        return;
      } else msm_change_state(MSM_POWERED_DOWN);
      break;


    //pre-charging (transient)
    case MSM_DO_POWER_DOWN:
      setPowerOutput(false);
      pd.detach();
      src_change_state(SRC_OFF);
      msm_change_state(MSM_POWERED_DOWN);
      break;

    case MSM_DO_POWER_UP:
      if (level_7W5_possible()) msm_change_state(MSM_SWITCH_7W5);
      else if (level_5W_possible()) msm_change_state(MSM_SWITCH_5W);
      else if (level_2W5_possible()) msm_change_state(MSM_SWITCH_2W5);
      src_change_state(SRC_DETACHED);
      break;

    //pre-charging
    case MSM_POWERED_DOWN:
      if (level_2W5_possible()) msm_change_state(MSM_DO_POWER_UP);
      break;


    //digital (transient)
    case MSM_SWITCH_2W5:
      src_change_state(SRC_ADVERTIZE);
      msm_change_state(MSM_2W5);
      break;
    case MSM_SWITCH_5W:
      src_change_state(SRC_ADVERTIZE);
      msm_change_state(MSM_5W);
      break;
    case MSM_SWITCH_7W5:
      src_change_state(SRC_ADVERTIZE);
      msm_change_state(MSM_7W5);
      break;

    //digital
    case MSM_2W5:
      if (!level_2W5_possible()) msm_change_state(MSM_DO_POWER_DOWN);
      else if (level_5W_possible()) msm_change_state(MSM_SWITCH_5W);
      break;

    case MSM_5W:
      if (!level_2W5_possible()) msm_change_state(MSM_DO_POWER_DOWN);
      else if (!level_5W_possible()) msm_change_state(MSM_SWITCH_2W5);
      else if (level_7W5_possible()) msm_change_state(MSM_SWITCH_7W5);
      break;

    case MSM_7W5:
      if (!level_2W5_possible()) msm_change_state(MSM_DO_POWER_DOWN);
      else if (!level_5W_possible()) msm_change_state(MSM_SWITCH_2W5);
      else if (!level_7W5_possible()) msm_change_state(MSM_SWITCH_5W);
      break;


    case MSM_CONFIGURATION: config_loop(); break;
    case MSM_CALIBRATION: calibration_loop(); break;
  }
}
