#include "gb18030.h"
#include "naive.h"
#include <criterion/criterion.h>

Test(gb18030, invalid) {
  char data[2][1] = {{0x80}, {0xFF}};
  for (int i = 0; i < 2; i++) {
    cr_assert(check_by_range(data[i], sizeof(data[i])) == false);
    cr_assert(check_by_range(data[i], sizeof(data[i])) ==
              naive_is_gb(data[i], sizeof(data[i])));
  }
}

Test(gb18030, two_byte_tooshort) {
  char data[] = {0x81};
  cr_assert(check_by_range(data, sizeof(data)) == false);
  cr_assert(check_by_range(data, sizeof(data)) ==
            naive_is_gb(data, sizeof(data)));
}

Test(gb18030, four_byte_tooshort) {
  char data[] = {0x90};
  cr_assert(check_by_range(data, sizeof(data)) == false);
  cr_assert(check_by_range(data, sizeof(data)) ==
            naive_is_gb(data, sizeof(data)));

  char data1[] = {0x90, 0x30};
  cr_assert(check_by_range(data1, sizeof(data1)) == false);
  cr_assert(check_by_range(data1, sizeof(data1)) ==
            naive_is_gb(data1, sizeof(data1)));

  char data2[] = {0x90, 0x30, 0x81};
  cr_assert(check_by_range(data2, sizeof(data2)) == false);
  cr_assert(check_by_range(data2, sizeof(data2)) ==
            naive_is_gb(data2, sizeof(data2)));
}

Test(gb18030, two_byte_outrange) {
  char data[3][2] = {{0x81, 0x29}, {0x81, 0x7F}, {0x81, 0xFF}};

  for (int i = 0; i < 3; i++) {
    cr_assert(check_by_range(data[i], sizeof(data[i])) == false);
    cr_assert(check_by_range(data[i], sizeof(data[i])) ==
              naive_is_gb(data[i], sizeof(data[i])));
  }
}

Test(gb18030, four_byte_reserved) {
  char data[3][4] = {{0x85, 0x30, 0x81, 0x30},
                     {0x8F, 0x30, 0x81, 0x30},
                     {0xE4, 0x30, 0x81, 0x30}};
  for (int i = 0; i < 3; i++) {
    cr_assert(check_by_range(data[i], sizeof(data[i])) == false);
    cr_assert(check_by_range(data[i], sizeof(data[i])) ==
              naive_is_gb(data[i], sizeof(data[i])));
  }
}

Test(gb18030, four_byte_userdefined) {
  char data[] = {0xFD, 0x30, 0x81, 0x30};
  cr_assert(check_by_range(data, sizeof(data)) == false);
  cr_assert(check_by_range(data, sizeof(data)) ==
            naive_is_gb(data, sizeof(data)));
}

Test(gb18030, four_byte_bmp) {
  char data[] = {0x84, 0x31, 0xA5, 0x30};
  cr_assert(check_by_range(data, sizeof(data)) == false);
  cr_assert(check_by_range(data, sizeof(data)) ==
            naive_is_gb(data, sizeof(data)));
}

Test(gb18030, four_byte_supp) {
  char data[] = {0xE3, 0x32, 0x9A, 0x36};
  cr_assert(check_by_range(data, sizeof(data)) == false);
  cr_assert(check_by_range(data, sizeof(data)) ==
            naive_is_gb(data, sizeof(data)));
}
