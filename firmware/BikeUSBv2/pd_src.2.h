//TODO: move parts of this into PD_Friend some day...

using pdo_validate_callback_t = bool (*)(uint8_t*, size_t);

void do_pd_msg_resp(uint8_t* msg, size_t len) {
  switch (0) {
    default:
      pd.do_other_msg_resp(msg, len);
      break;
  }
}
void send_source_cap() {
  uint32_t pdo = 0;
  switch (msm_state) {
    default:
      return;
      break;

    case MSM_SWITCH_2W5:
    case MSM_2W5:
      pdo = PDStack_SRC::make_fixed_pdo(5000, 500);
      break;

    case MSM_SWITCH_5W:
    case MSM_5W:
      pdo = PDStack_SRC::make_fixed_pdo(5000, 1000);
      break;

    case MSM_SWITCH_7W5:
    case MSM_7W5:
      pdo = PDStack_SRC::make_fixed_pdo(5000, 1500);
      break;
  }  //
  printDebug(DBG_PDO, pdo);
  pd.send_data_msg(PDDM_Source_Capabilities, &pdo, 1);
}

enum pd_src_state_t {
  SRC_OFF,           //do nothing
  SRC_DETACHED,      //waiting for device sink attach
  SRC_ADVERTIZE,     //advertizing capabilites to device
  SRC_WAIT,          //got a GoodCRC packet, stop advertising, wait for accept/reject
  SRC_STARTING_PSU,  //waiting to send PS_RDY
  SRC_ACTIVE         //pdo has been selected
} src_state = SRC_OFF;

bool src_state_changed = false;
void src_change_state(pd_src_state_t new_state) {
  //printDebug<uint8_t>(DBG_MSM_STATE, src_state);
  src_state = new_state;
  src_state_changed = true;
  printDebug<uint8_t>(DBG_SRC_STATE, src_state);
}

bool src_check_power() {
  if (!isPowerGood()) {
    pd.reset();
    pd.detach();
    src_change_state(SRC_DETACHED);
    return false;
  }
  return true;
}

bool src_level_change() {  //change state if safe and return true, else return false
  switch (src_state) {
    case SRC_ACTIVE:
      src_change_state(SRC_ADVERTIZE);
      return true;
      break;
    case SRC_OFF:
      src_change_state(SRC_DETACHED);
      return true;
      break;
    case SRC_DETACHED:
      return true;
      break;

    default:
      return false;
      break;
  }
}

void run_pd_src_sm() {
  static uint32_t last_adv = 0;
  switch (src_state) {
    case SRC_OFF:
      if (src_state_changed) {
        pd.detach();
        setPowerOutput(false);
      }
      break;

    case SRC_DETACHED:
      if (src_state_changed) {
        setPowerOutput(true);
      }
      if (isPowerGood())  //do nothing while power is bad
        if (pd.attach_src()) {
          setPowerOutput(true);  //if pulldowns detected, turn on boost converter
          src_change_state(SRC_ADVERTIZE);
        }
      break;

    case SRC_ADVERTIZE:
      if (millis() - last_adv > 100) {
        last_adv = millis();
        send_source_cap();
      }
      src_check_power();
      break;

    case SRC_WAIT:                       //wait for accept/reject
      if (millis() - last_adv > 5000) {  //something went wrong
        printDebug(DBG_PD_ERR, 0x00);
        pd.reset();
        src_change_state(SRC_DETACHED);
      }
      src_check_power();
      break;

    case SRC_STARTING_PSU:
      if (src_state_changed) setPowerOutput(true);  //just in case it wasnt on earlier
      if (isPowerGood()) {
        //more stuff here
        pd.send_ctrl_msg(PDCM_PS_RDY);
        src_change_state(SRC_ACTIVE);
      }
      break;

    case SRC_ACTIVE:
      src_check_power();
      //TODO: check for disconnect
      break;
  }
  src_state_changed = false;
}

void on_message(uint8_t* msg, size_t len) {
  if (PDStack::is_data_msg(msg, len)) switch (PDStack::get_data_msg_type(msg, len)) {
      case PDDM_Request:
        pd.send_ctrl_msg(PDCM_Accept);  //not checking lol (at least for now)
        src_change_state(SRC_STARTING_PSU);
        break;
      default: pd.do_other_msg_resp(msg, len); break;
    }

  else switch (PDStack::get_ctrl_msg_type(msg, len)) {
      case PDCM_Get_Source_Cap: send_source_cap(); break;
      case PDCM_GoodCRC:
        if (src_state == SRC_ADVERTIZE) src_change_state(SRC_WAIT);
        break;
      default: pd.do_other_msg_resp(msg, len); break;
    }
}

void fusb_int() {
  uint32_t interrupts = fusb.get_interrupts();
  printDebug(DBG_FUSB_INT, interrupts);
  fusb.clear_interrupts();
  if (interrupts & FUSB_I_COMP_CHNG) {  //on at/detach
    if (fusb.find_cc_source() == 0) src_change_state(SRC_DETACHED);
  }

  if (interrupts & FUSB_I_ACTIVITY)
    if (fusb.rxb_state() == 0) {  //TODO: this does not seem right
      uint8_t buf[256];
      pd.read_msg(buf, sizeof(buf));
      on_message(buf, sizeof(buf));
    }
}

void run_pd() {
  if (!digitalRead(FUSB_INT_PIN)) fusb_int();
  run_pd_src_sm();
}
