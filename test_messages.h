#include "common.h"
#pragma once

void set_up_test_messages(void);
#ifdef MASTER
extern Can_message test_msg[9];
#else
extern Can_message test_msg[17];
#endif
