#include "test_messages.h"
#include "common.h"
#include <stdint.h>

#ifdef MASTER
Can_message test_msg[9];
#else
Can_message test_msg[17];
#endif

void set_up_test_messages(void) {
	/*************************************************************************
	 * Sets up test messages and their content
	 * Input: None
	 * Output: None
	 *************************************************************************/
#ifdef MASTER
	test_msg[0].sid = 0;
	test_msg[0].rtr = 1;
	test_msg[0].ide = 0;
	test_msg[0].rb0 = 0;
	test_msg[0].dlc = 0;

	test_msg[1].sid = 0;
	test_msg[1].rtr = 1;
	test_msg[1].ide = 0;
	test_msg[1].rb0 = 0;
	test_msg[1].dlc = 0;

	test_msg[2].sid = 1601;
	test_msg[2].rtr = 0;
	test_msg[2].ide = 0;
	test_msg[2].rb0 = 0;
	test_msg[2].dlc = 8;
	test_msg[2].byte[0] = 130;
	test_msg[2].byte[1] = 135;
	test_msg[2].byte[2] = 140;
	test_msg[2].byte[3] = 145;
	test_msg[2].byte[4] = 150;
	test_msg[2].byte[5] = 155;
	test_msg[2].byte[6] = 160;
	test_msg[2].byte[7] = 165;

	test_msg[3].sid = 1602;
	test_msg[3].rtr = 0;
	test_msg[3].ide = 0;
	test_msg[3].rb0 = 0;
	test_msg[3].dlc = 8;
	test_msg[3].byte[0] = 170;
	test_msg[3].byte[1] = 175;
	test_msg[3].byte[2] = 180;
	test_msg[3].byte[3] = 185;
	test_msg[3].byte[4] = 190;
	test_msg[3].byte[5] = 195;
	test_msg[3].byte[6] = 200;
	test_msg[3].byte[7] = 205;

	test_msg[4].sid = 1633;
	test_msg[4].rtr = 0;
	test_msg[4].ide = 0;
	test_msg[4].rb0 = 0;
	test_msg[4].dlc = 8;
	test_msg[4].byte[0] = 30;
	test_msg[4].byte[1] = 31;
	test_msg[4].byte[2] = 32;
	test_msg[4].byte[3] = 33;
	test_msg[4].byte[4] = 34;
	test_msg[4].byte[5] = 35;
	test_msg[4].byte[6] = 36;
	test_msg[4].byte[7] = 37;

	test_msg[5].sid = 1634;
	test_msg[5].rtr = 0;
	test_msg[5].ide = 0;
	test_msg[5].rb0 = 0;
	test_msg[5].dlc = 8;
	test_msg[5].byte[0] = 38;
	test_msg[5].byte[1] = 39;
	test_msg[5].byte[2] = 40;
	test_msg[5].byte[3] = 41;
	test_msg[5].byte[4] = 42;
	test_msg[5].byte[5] = 43;
	test_msg[5].byte[6] = 44;
	test_msg[5].byte[7] = 45;


	test_msg[6].sid = 65;
	test_msg[6].rtr = 1;
	test_msg[6].ide = 0;
	test_msg[6].rb0 = 0;
	test_msg[6].dlc = 0;

	test_msg[7].sid = 98;
	test_msg[7].rtr = 1;
	test_msg[7].ide = 0;
	test_msg[7].rb0 = 0;
	test_msg[7].dlc = 0;

	test_msg[8].sid = 97;
	test_msg[8].rtr = 1;
	test_msg[8].ide = 0;
	test_msg[8].rb0 = 0;
	test_msg[8].dlc = 0;
#else
	test_msg[0].sid = 33;
	test_msg[0].rtr = 1;
	test_msg[0].ide = 0;
	test_msg[0].rb0 = 0;
	test_msg[0].dlc = 0;

	test_msg[1].sid = 1632;
	test_msg[1].rtr = 0;
	test_msg[1].ide = 0;
	test_msg[1].rb0 = 0;
	test_msg[1].dlc = 8;
	test_msg[1].byte[0] = 255;
	test_msg[1].byte[1] = 254;
	test_msg[1].byte[2] = 253;
	test_msg[1].byte[3] = 252;
	test_msg[1].byte[4] = 251;
	test_msg[1].byte[5] = 250;
	test_msg[1].byte[6] = 249;
	test_msg[1].byte[7] = 248;

	test_msg[2].sid = 1600;
	test_msg[2].rtr = 0;
	test_msg[2].ide = 0;
	test_msg[2].rb0 = 0;
	test_msg[2].dlc = 8;
	test_msg[2].byte[0] = 247;
	test_msg[2].byte[1] = 246;
	test_msg[2].byte[2] = 245;
	test_msg[2].byte[3] = 244;
	test_msg[2].byte[4] = 243;
	test_msg[2].byte[5] = 242;
	test_msg[2].byte[6] = 241;
	test_msg[2].byte[7] = 240;

	test_msg[3].sid = 1536;
	test_msg[3].rtr = 0;
	test_msg[3].ide = 0;
	test_msg[3].rb0 = 0;
	test_msg[3].dlc = 4;
	test_msg[3].byte[0] = 3;
	test_msg[3].byte[1] = 255;
	test_msg[3].byte[2] = 0;
	test_msg[3].byte[3] = 10;

	test_msg[4].sid = 1536;
	test_msg[4].rtr = 0;
	test_msg[4].ide = 0;
	test_msg[4].rb0 = 0;
	test_msg[4].dlc = 4;
	test_msg[4].byte[0] = 43;
	test_msg[4].byte[1] = 200;
	test_msg[4].byte[2] = 0;
	test_msg[4].byte[3] = 90;

	test_msg[5].sid = 1568;
	test_msg[5].rtr = 0;
	test_msg[5].ide = 0;
	test_msg[5].rb0 = 0;
	test_msg[5].dlc = 8;
	test_msg[5].byte[0] = 0;
	test_msg[5].byte[1] = 1;
	test_msg[5].byte[2] = 2;
	test_msg[5].byte[3] = 3;
	test_msg[5].byte[4] = 4;
	test_msg[5].byte[5] = 5;
	test_msg[5].byte[6] = 6;
	test_msg[5].byte[7] = 7;

	test_msg[6].sid = 1569;
	test_msg[6].rtr = 0;
	test_msg[6].ide = 0;
	test_msg[6].rb0 = 0;
	test_msg[6].dlc = 8;
	test_msg[6].byte[0] = 8;
	test_msg[6].byte[1] = 9;
	test_msg[6].byte[2] = 10;
	test_msg[6].byte[3] = 11;
	test_msg[6].byte[4] = 12;
	test_msg[6].byte[5] = 13;
	test_msg[6].byte[6] = 14;
	test_msg[6].byte[7] = 15;

	test_msg[7].sid = 1570;
	test_msg[7].rtr = 0;
	test_msg[7].ide = 0;
	test_msg[7].rb0 = 0;
	test_msg[7].dlc = 8;
	test_msg[7].byte[0] = 16;
	test_msg[7].byte[1] = 17;
	test_msg[7].byte[2] = 18;
	test_msg[7].byte[3] = 19;
	test_msg[7].byte[4] = 20;
	test_msg[7].byte[5] = 21;
	test_msg[7].byte[6] = 22;
	test_msg[7].byte[7] = 23;

	test_msg[8].sid = 1571;
	test_msg[8].rtr = 0;
	test_msg[8].ide = 0;
	test_msg[8].rb0 = 0;
	test_msg[8].dlc = 8;
	test_msg[8].byte[0] = 24;
	test_msg[8].byte[1] = 25;
	test_msg[8].byte[2] = 26;
	test_msg[8].byte[3] = 27;
	test_msg[8].byte[4] = 28;
	test_msg[8].byte[5] = 29;
	test_msg[8].byte[6] = 30;
	test_msg[8].byte[7] = 31;

	test_msg[9].sid = 1572;
	test_msg[9].rtr = 0;
	test_msg[9].ide = 0;
	test_msg[9].rb0 = 0;
	test_msg[9].dlc = 8;
	test_msg[9].byte[0] = 32;
	test_msg[9].byte[1] = 33;
	test_msg[9].byte[2] = 34;
	test_msg[9].byte[3] = 35;
	test_msg[9].byte[4] = 36;
	test_msg[9].byte[5] = 37;
	test_msg[9].byte[6] = 38;
	test_msg[9].byte[7] = 39;

	test_msg[10].sid = 1573;
	test_msg[10].rtr = 0;
	test_msg[10].ide = 0;
	test_msg[10].rb0 = 0;
	test_msg[10].dlc = 8;
	test_msg[10].byte[0] = 40;
	test_msg[10].byte[1] = 41;
	test_msg[10].byte[2] = 42;
	test_msg[10].byte[3] = 43;
	test_msg[10].byte[4] = 44;
	test_msg[10].byte[5] = 45;
	test_msg[10].byte[6] = 46;
	test_msg[10].byte[7] = 47;

	test_msg[11].sid = 1574;
	test_msg[11].rtr = 0;
	test_msg[11].ide = 0;
	test_msg[11].rb0 = 0;
	test_msg[11].dlc = 8;
	test_msg[11].byte[0] = 48;
	test_msg[11].byte[1] = 49;
	test_msg[11].byte[2] = 50;
	test_msg[11].byte[3] = 51;
	test_msg[11].byte[4] = 52;
	test_msg[11].byte[5] = 53;
	test_msg[11].byte[6] = 54;
	test_msg[11].byte[7] = 55;

	test_msg[12].sid = 1575;
	test_msg[12].rtr = 0;
	test_msg[12].ide = 0;
	test_msg[12].rb0 = 0;
	test_msg[12].dlc = 8;
	test_msg[12].byte[0] = 56;
	test_msg[12].byte[1] = 57;
	test_msg[12].byte[2] = 58;
	test_msg[12].byte[3] = 59;
	test_msg[12].byte[4] = 60;
	test_msg[12].byte[5] = 61;
	test_msg[12].byte[6] = 62;
	test_msg[12].byte[7] = 63;

	test_msg[13].sid = 1576;
	test_msg[13].rtr = 0;
	test_msg[13].ide = 0;
	test_msg[13].rb0 = 0;
	test_msg[13].dlc = 8;
	test_msg[13].byte[0] = 64;
	test_msg[13].byte[1] = 65;
	test_msg[13].byte[2] = 66;
	test_msg[13].byte[3] = 67;
	test_msg[13].byte[4] = 68;
	test_msg[13].byte[5] = 69;
	test_msg[13].byte[6] = 70;
	test_msg[13].byte[7] = 71;

	test_msg[14].sid = 1577;
	test_msg[14].rtr = 0;
	test_msg[14].ide = 0;
	test_msg[14].rb0 = 0;
	test_msg[14].dlc = 8;
	test_msg[14].byte[0] = 72;
	test_msg[14].byte[1] = 73;
	test_msg[14].byte[2] = 74;
	test_msg[14].byte[3] = 75;
	test_msg[14].byte[4] = 76;
	test_msg[14].byte[5] = 77;
	test_msg[14].byte[6] = 78;
	test_msg[14].byte[7] = 79;

	test_msg[15].sid = 1578;
	test_msg[15].rtr = 0;
	test_msg[15].ide = 0;
	test_msg[15].rb0 = 0;
	test_msg[15].dlc = 8;
	test_msg[15].byte[0] = 80;
	test_msg[15].byte[1] = 81;
	test_msg[15].byte[2] = 82;
	test_msg[15].byte[3] = 83;
	test_msg[15].byte[4] = 84;
	test_msg[15].byte[5] = 85;
	test_msg[15].byte[6] = 86;
	test_msg[15].byte[7] = 87;

	test_msg[16].sid = 1536;
	test_msg[16].rtr = 0;
	test_msg[16].ide = 0;
	test_msg[16].rb0 = 0;
	test_msg[16].dlc = 4;
	test_msg[16].byte[0] = 83;
	test_msg[16].byte[1] = 240;
	test_msg[16].byte[2] = 0;
	test_msg[16].byte[3] = 40;
#endif
}
