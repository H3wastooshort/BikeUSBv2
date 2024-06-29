void run_msm() {
  switch (msm_state) {
    case MSM_START:
      {
        //conf mode
        if (!AC_new_meas_flag) {  //wait for AC measurement
          if (millis() > 5000) {  //after 5 seconds with no ac waveform, time out and decide it's DC => conf mode
            config_setup();
            msm_change_state(MSM_CONFIGURATION);
          }
          return;
        } else msm_change_state(MSM_GOT_AC);
      }


    case MSM_GOT_AC:
      {
        //cal mode
        if (config.calibrate_mode != 0) {
          calibration_setup();
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
          if (level_7W5_possible()) msm_change_state(MSM_SWITCH_7W5);
          else if (level_5W_possible()) msm_change_state(MSM_SWITCH_5W);
          else if (level_2W5_possible()) msm_change_state(MSM_SWITCH_2W5);
        }
      }
      break;
    //pre-charging
    case MSM_POWERED_DOWN:
      {
        if (level_2W5_possible()) msm_change_state(MSM_DO_POWER_UP);
      }
      break;

      //digital
    case MSM_2W5:
      {
        if (!level_2W5_possible()) msm_change_state(MSM_DO_POWER_DOWN);
        else if (level_5W_possible()) msm_change_state(MSM_SWITCH_5W);
      }
      break;
    case MSM_5W:
      {
        if (!level_2W5_possible()) msm_change_state(MSM_DO_POWER_DOWN);
        else if (!level_5W_possible()) msm_change_state(MSM_SWITCH_2W5);
        else if (level_7W5_possible()) msm_change_state(MSM_SWITCH_7W5);
      }
      break;
    case MSM_7W5:
      {
        if (!level_2W5_possible()) msm_change_state(MSM_DO_POWER_DOWN);
        else if (!level_5W_possible()) msm_change_state(MSM_SWITCH_2W5);
        else if (!level_7W5_possible()) msm_change_state(MSM_SWITCH_5W);
      }
      break;

    case MSM_CONFIGURATION: config_loop(); break;
    case MSM_CALIBRATION: calibration_loop(); break;
  }
}
