using pdo_validate_callback_t = bool (*)(uint8_t*, size_t);

void do_pd_msg_resp(uint8_t* msg, size_t len) {
  switch (0) {
    default:
      pd.do_other_msg_resp(msg, len);
      break;
  }
}

enum pd_src_state_t {
  SRC_OFF,           //do nothing
  SRC_DETACHED,      //waiting for device sink attach
  SRC_ADVERTIZE,     //advertizing capabilites to device
  SRC_WAIT,          //got a GoodCRC packet, stop advertising
  SRC_STARTING_PSU,  //waiting to send PS_RDY
  SRC_ACTIVE         //pdo has been selected
} src_state;

const uint32_t pdo_2W5 = PDStack_SRC::make_fixed_pdo(5000, 500);
const uint32_t pdo_5W = PDStack_SRC::make_fixed_pdo(5000, 1000);
const uint32_t pdo_7W5 = PDStack_SRC::make_fixed_pdo(5000, 1500);

void run_pd_src_sm() {
  switch (src_state) {
    case SRC_OFF:  // do nothing
      break;

    case SRC_DETACHED:
      if (pd.attach_src()) {
        src_state = SRC_ADVERTIZE;
      }
      break;

    case SRC_ADVERTIZE:
      {
        static uint32_t last_adv = 0;
        if (millis() - last_adv > 100) {
          union {
            uint32_t pdo;
            uint8_t buf[4];
          };

          switch (msm_state) {
            default:
              return;
              break;

            case MSM_SWITCH_2W5:
            case MSM_2W5:
              pdo = pdo_2W5;
              break;

            case MSM_SWITCH_5W:
            case MSM_5W:
              pdo = pdo_5W;
              break;

            case MSM_SWITCH_7W5:
            case MSM_7W5:
              pdo = pdo_7W5;
              break;
          }  //

          pd.send_data_msg(PDM_Source_Capabilities, buf, sizeof(buf));
        }
      }
      break;

    case SRC_WAIT:
      break;

    case SRC_STARTING_PSU:
      if (!digitalRead(OUTPUT_GOOD_PIN)) {
        //more stuff here
        src_state = SRC_ACTIVE;
        return;
      }
      break;

    case SRC_ACTIVE:
      //TODO: check for disconnect
      break;
  }
}

void fusb_int() {
  uint32_t i = fusb.get_interrupts();
  if (i & FUSB_I_COMP_CHNG) {  //on at/detach
    if (src_state == SRC_DETACHED) src_state = SRC_ADVERTIZE;
    else src_state == SRC_DETACHED;  //TODO this is very bad! check with find_cc() if device is really gone
  }
}

void on_message(uint8_t* msg, size_t len) {
  if (PDStack::is_data_msg(msg, len)) switch (PDStack::get_data_msg_type(msg, len)) {
      default: pd.do_other_msg_resp(msg, len); break;
    }

  else switch (PDStack::get_ctrl_msg_type(msg, len)) {
      case PDM_GoodCRC:
        if (src_state == SRC_WAIT) src_state = SRC_STARTING_PSU;
        break;
      default: pd.do_other_msg_resp(msg, len); break;
    }
}

void run_pd() {
  if (!digitalRead(FUSB_INT_PIN)) fusb_int();
}
