const char * kuhn_1 = "κόσμε";
const char * kuhn_2_1_1 = "\x00";
uint32_t expect_2_1_1 = 0x00000000;
const char * kuhn_2_1_2 = "\xC2\x80";
uint32_t expect_2_1_2 = 0x00000080;
const char * kuhn_2_1_3 = "\xE0\xA0\x80";
uint32_t expect_2_1_3 = 0x00000800;
const char * kuhn_2_1_4 = "\xF0\x90\x80\x80";
uint32_t expect_2_1_4 = 0x00010000;
const char * kuhn_2_1_5 = "\xF8\x88\x80\x80\x80";
uint32_t expect_2_1_5 = 0x00200000;
const char * kuhn_2_1_6 = "\xFC\x84\x80\x80\x80\x80";
uint32_t expect_2_1_6 = 0x04000000;
const char * kuhn_2_2_1 = "\x7F";
uint32_t expect_2_2_1 = 0x0000007F;
const char * kuhn_2_2_2 = "\xDF\xBF";
uint32_t expect_2_2_2 = 0x000007FF;
const char * kuhn_2_2_3 = "\xEF\xBF\xBF";
uint32_t expect_2_2_3 = 0x0000FFFF;
const char * kuhn_2_2_4 = "\xF7\xBF\xBF\xBF";
uint32_t expect_2_2_4 = 0x001FFFFF;
const char * kuhn_2_2_5 = "\xFB\xBF\xBF\xBF\xBF";
uint32_t expect_2_2_5 = 0x03FFFFFF;
const char * kuhn_2_2_6 = "\xFD\xBF\xBF\xBF\xBF\xBF";
uint32_t expect_2_2_6 = 0x7FFFFFFF;
const char * kuhn_2_3_1 = "\xED\x9F\xBF";
uint32_t expect_2_3_1 = 0x0000D7FF;
const char * kuhn_2_3_2 = "\xEE\x80\x80";
uint32_t expect_2_3_2 = 0x0000E000;
const char * kuhn_2_3_3 = "\xEF\xBF\xBD";
uint32_t expect_2_3_3 = 0x0000FFFD;
const char * kuhn_2_3_4 = "\xF4\x8F\xBF\xBF";
uint32_t expect_2_3_4 = 0x0010FFFF;
const char * kuhn_2_3_5 = "\xF4\x90\x80\x80";
uint32_t expect_2_3_5 = 0x00110000;
const char * kuhn_3_1_1 = "\xC0";
uint32_t expect_3_1_1 = 0x0000;
const char * kuhn_3_1_2 = "\xBF";
const char * kuhn_3_1_3 = "\x80\xBF";
const char * kuhn_3_1_4 = "\x80\xBF\x80";
const char * kuhn_3_1_5 = "\x80\xBF\x80\xBF";
const char * kuhn_3_1_6 = "\x80\xBF\x80\xBF\x80";
const char * kuhn_3_1_7 = "\x80\xBF\x80\xBF\x80\xBF";
const char * kuhn_3_1_8 = "\x80\xBF\x80\xBF\x80\xBF\x80";
const char * kuhn_3_3_1 = "\xC0";
const char * kuhn_3_3_10 = "\xFD\xBF\xBF\xBF\xBF";
uint32_t expect_3_3_10 = 0x7FFFFFFF;
const char * kuhn_3_3_2 = "\xE0\x80";
uint32_t expect_3_3_2 = 0x0000;
const char * kuhn_3_3_3 = "\xF0\x80\x80";
uint32_t expect_3_3_3 = 0x0000;
const char * kuhn_3_3_4 = "\xF8\x80\x80\x80";
uint32_t expect_3_3_4 = 0x0000;
const char * kuhn_3_3_5 = "\xFC\x80\x80\x80\x80";
uint32_t expect_3_3_5 = 0x0000;
const char * kuhn_3_3_6 = "\xDF";
uint32_t expect_3_3_6 = 0x000007FF;
const char * kuhn_3_3_7 = "\xEF\xBF";
uint32_t expect_3_3_7 = 0x0000FFFF;
const char * kuhn_3_3_8 = "\xF7\xBF\xBF";
uint32_t expect_3_3_8 = 0x001FFFFF;
const char * kuhn_3_3_9 = "\xFB\xBF\xBF\xBF";
uint32_t expect_3_3_9 = 0x03FFFFFF;
const char * kuhn_3_5_1 = "\xFE\xFE\xFF\xFF";
const char * kuhn_3_5_2 = "\xFF";
const char * kuhn_3_5_3 = "\xFE\xFE\xFF\xFF";
const char * kuhn_4_1_1 = "\xC0\xAF";
uint32_t expect_4_1_1 = 0x002F;
const char * kuhn_4_1_2 = "\xE0\x80\xAF";
uint32_t expect_4_1_2 = 0x002F;
const char * kuhn_4_1_3 = "\xF0\x80\x80\xAF";
uint32_t expect_4_1_3 = 0x002F;
const char * kuhn_4_1_4 = "\xF8\x80\x80\x80\xAF";
uint32_t expect_4_1_4 = 0x002F;
const char * kuhn_4_1_5 = "\xFC\x80\x80\x80\x80\xAF";
uint32_t expect_4_1_5 = 0x002F;
const char * kuhn_4_2_1 = "\xC1\xBF";
uint32_t expect_4_2_1 = 0x0000007F;
const char * kuhn_4_2_2 = "\xE0\x9F\xBF";
uint32_t expect_4_2_2 = 0x000007FF;
const char * kuhn_4_2_3 = "\xF0\x8F\xBF\xBF";
uint32_t expect_4_2_3 = 0x0000FFFF;
const char * kuhn_4_2_4 = "\xF8\x87\xBF\xBF\xBF";
uint32_t expect_4_2_4 = 0x001FFFFF;
const char * kuhn_4_2_5 = "\xFC\x83\xBF\xBF\xBF\xBF";
uint32_t expect_4_2_5 = 0x03FFFFFF;
const char * kuhn_4_3_1 = "\xC0\x80";
uint32_t expect_4_3_1 = 0x0000;
const char * kuhn_4_3_2 = "\xE0\x80\x80";
uint32_t expect_4_3_2 = 0x0000;
const char * kuhn_4_3_3 = "\xF0\x80\x80\x80";
uint32_t expect_4_3_3 = 0x0000;
const char * kuhn_4_3_4 = "\xF8\x80\x80\x80\x80";
uint32_t expect_4_3_4 = 0x0000;
const char * kuhn_4_3_5 = "\xFC\x80\x80\x80\x80\x80";
uint32_t expect_4_3_5 = 0x0000;
const char * kuhn_5_1_1 = "\xED\xA0\x80";
uint32_t expect_5_1_1 = 0xD800;
const char * kuhn_5_1_2 = "\xED\xAD\xBF";
uint32_t expect_5_1_2 = 0xDB7F;
const char * kuhn_5_1_3 = "\xED\xAE\x80";
uint32_t expect_5_1_3 = 0xDB80;
const char * kuhn_5_1_4 = "\xED\xAF\xBF";
uint32_t expect_5_1_4 = 0xDBFF;
const char * kuhn_5_1_5 = "\xED\xB0\x80";
uint32_t expect_5_1_5 = 0xDC00;
const char * kuhn_5_1_6 = "\xED\xBE\x80";
uint32_t expect_5_1_6 = 0xDF80;
const char * kuhn_5_1_7 = "\xED\xBF\xBF";
uint32_t expect_5_1_7 = 0xDFFF;
const char * kuhn_5_2_1 = "\xED\xA0\x80\xED\xB0\x80";
uint32_t expect_5_2_1 = 0xD800;
const char * kuhn_5_2_2 = "\xED\xA0\x80\xED\xBF\xBF";
uint32_t expect_5_2_2 = 0xD800;
const char * kuhn_5_2_3 = "\xED\xAD\xBF\xED\xB0\x80";
uint32_t expect_5_2_3 = 0xDB7F;
const char * kuhn_5_2_4 = "\xED\xAD\xBF\xED\xBF\xBF";
uint32_t expect_5_2_4 = 0xDB7F;
const char * kuhn_5_2_5 = "\xED\xAE\x80\xED\xB0\x80";
uint32_t expect_5_2_5 = 0xDB80;
const char * kuhn_5_2_6 = "\xED\xAE\x80\xED\xBF\xBF";
uint32_t expect_5_2_6 = 0xDB80;
const char * kuhn_5_2_7 = "\xED\xAF\xBF\xED\xB0\x80";
uint32_t expect_5_2_7 = 0xDBFF;
const char * kuhn_5_2_8 = "\xED\xAF\xBF\xED\xBF\xBF";
uint32_t expect_5_2_8 = 0xDBFF;
const char * kuhn_5_3_1 = "\xEF\xBF\xBE";
uint32_t expect_5_3_1 = 0xFFFE;
const char * kuhn_5_3_2 = "\xEF\xBF\xBF";
uint32_t expect_5_3_2 = 0xFFFF;
const char * kuhn_5_3_3 = "\xEF\xB7\x90\xEF\xB7\x91\xEF\xB7\x92\xEF\xB7\x93\xEF\xB7\x94\xEF\xB7\x95\xEF\xB7\x96\xEF\xB7\x97\xEF\xB7\x98\xEF\xB7\x99\xEF\xB7\x9A\xEF\xB7\x9B\xEF\xB7\x9C\xEF\xB7\x9D\xEF\xB7\x9E\xEF\xB7\x9F\xEF\xB7\xA0\xEF\xB7\xA1\xEF\xB7\xA2\xEF\xB7\xA3\xEF\xB7\xA4\xEF\xB7\xA5\xEF\xB7\xA6\xEF\xB7\xA7\xEF\xB7\xA8\xEF\xB7\xA9\xEF\xB7\xAA\xEF\xB7\xAB\xEF\xB7\xAC\xEF\xB7\xAD\xEF\xB7\xAE\xEF\xB7\xAF";
uint32_t expect_5_3_3 = 0xFDD0;
