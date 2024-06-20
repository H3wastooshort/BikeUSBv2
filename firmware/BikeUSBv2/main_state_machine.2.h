void msm_change_state(msm_states_s new_state) {
  printDebug<uint8_t>(DBG_MSM_STATE, msm_state);
  msm_state = new_state;
  printDebug<uint8_t>(DBG_MSM_STATE, msm_state);
}

void msm_run() {
  switch (uint8_t(msm_state)) {
    case MSM_START:
      {
        //conf mode
        if (!AC_new_meas_flag) {                                     //wait for AC measurement
          if (millis() > 5000) msm_change_state(MSM_CONFIGURATION);  //after 5 seconds with no ac waveform, time out and decide it's DC => conf mode
          return;
        } else msm_change_state(MSM_GOT_AC);
      }


    case MSM_GOT_AC:
      {
        //cal mode
        if (config.calibrate_mode != 0) {
          msm_change_state(MSM_CALIBRATION);
          return;
        } else msm_change_state(MSM_POWERED_DOWN);
      }
      break;


    //pre-charging (transient)
    case MSM_DO_POWER_DOWN:
      {

        digitalWrite(OUTPUT_ENABLE_PIN, LOW);
        msm_change_state(MSM_POWERED_DOWN);
      }
      break;
    case MSM_DO_POWER_UP:
      {
        digitalWrite(OUTPUT_ENABLE_PIN, HIGH);
        if (digitalRead(OUTPUT_GOOD_PIN)) {
          //TODO: set FUSB for 2W5
          msm_change_state(MSM_WAITING_FOR_DEVICE);
        }
      }
      break;
    //pre-charging
    case MSM_POWERED_DOWN:
      {
        if (level_2W5_possible()) msm_change_state(MSM_DO_POWER_UP);
      }
      break;
    case MSM_WAITING_FOR_DEVICE:
      {
      }
      break;


      //digital
    case MSM_D_2W5:
      {
        if (!level_2W5_possible()) msm_change_state(MSM_DO_POWER_DOWN);
        else if (level_5W_possible()) msm_change_state(MSM_D_SWITCH_5W);
      }
      break;
    case MSM_D_5W:
      {
        if (!level_2W5_possible()) msm_change_state(MSM_DO_POWER_DOWN);
        else if (!level_5W_possible()) msm_change_state(MSM_D_SWITCH_2W5);
        else if (level_7W5_possible()) msm_change_state(MSM_D_SWITCH_7W5);
      }
      break;
    case MSM_D_7W5:
      {
        if (!level_2W5_possible()) msm_change_state(MSM_DO_POWER_DOWN);
        else if (!level_5W_possible()) msm_change_state(MSM_D_SWITCH_2W5);
        else if (!level_7W5_possible()) msm_change_state(MSM_D_SWITCH_5W);
      }
      break;
  }
}
