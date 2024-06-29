using pdo_validate_callback_t = bool (*)(uint8_t*, size_t);

void do_src_msg_resp(uint8_t* msg, size_t len) {
  switch (0) {
    default:
      send_ctrl_msg(PDM_Not_Supported);
      break;
  }
}

enum pd_src_state_t {
  SRC_ADVERTIZE,     //advertizing capabilites to device
  SRC_WAIT,          //got a GoodCRC packet, stop advertising
  SRC_STARTING_PSU,  //waiting to send PS_RDY
  SRC_PDO_ACTIVE     //pdo has been selected
} src_state;

void fusb_isr() {

}
