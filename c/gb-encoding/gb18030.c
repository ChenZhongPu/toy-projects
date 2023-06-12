#include "gb18030.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef enum GB_State GB_State;
// eleven states
enum GB_State {
  VALID,
  MORE,       // it can be either double-byte or quad-byte
  B_ONE_MORE, // it is a double-byte character
  TWO_MORE,   // it is a quad-byte character
  Q_ONE_MORE, // it is a quad-byte character
  BYTE_84,
  BYTE_84_31,
  BYTE_E3,
  BYTE_E3_32,
  BYTE_E3_32_9A,
  INVALID,
};
static const GB_State State_Transitions[11][256] = {
    [VALID] =
        {
            [0x00 ... 0x7F] = VALID,
            [0x80] = INVALID,
            [0x81 ... 0x83] = MORE,
            [0x84] = BYTE_84,
            [0x85 ... 0x8F] = B_ONE_MORE,
            [0x90 ... 0xE2] = MORE,
            [0xE3] = BYTE_E3,
            [0xE4 ... 0xFE] = B_ONE_MORE,
            [0xFF] = INVALID,
        },
    [MORE] =
        {
            [0x00 ... 0x2F] = INVALID,
            [0x30 ... 0x39] = TWO_MORE,
            [0x40 ... 0x7E] = VALID, // it is a double-byte
            [0x7F] = INVALID,
            [0x80 ... 0xFE] = VALID, // it is a double-byte
            [0xFF] = INVALID,
        },
    [B_ONE_MORE] =
        {
            [0x00 ... 0x39] = INVALID,
            [0x40 ... 0x7E] = VALID, // it is a double-byte
            [0x7F] = INVALID,
            [0x80 ... 0xFE] = VALID, // it is a double-byte
            [0xFF] = INVALID,
        },
    [TWO_MORE] =
        {
            [0x00 ... 0x80] = INVALID,
            [0x81 ... 0xFE] = Q_ONE_MORE,
            [0xFF] = INVALID,
        },
    [Q_ONE_MORE] =
        {
            [0x00 ... 0x2F] = INVALID,
            [0x30 ... 0x39] = VALID, // it is a quad-byte
            [0x3A ... 0xFF] = INVALID,
        },
    [BYTE_84] =
        {
            [0x00 ... 0x2F] = INVALID,
            [0x30] = TWO_MORE,
            [0x31] = BYTE_84_31,
            [0x32 ... 0x39] = TWO_MORE,
            [0x40 ... 0x7E] = VALID, // it is a double-byte
            [0x7F] = INVALID,
            [0x80 ... 0xFE] = VALID, // it is a double-byte
            [0xFF] = INVALID,
        },
    [BYTE_84_31] =
        {
            [0x00 ... 0x80] = INVALID,
            [0x81 ... 0xA4] = Q_ONE_MORE,
            [0xA5 ... 0xFF] = INVALID,
        },
    [BYTE_E3] =
        {
            [0x00 ... 0x2F] = INVALID,
            [0x30 ... 0x31] = TWO_MORE,
            [0x32] = BYTE_E3_32,
            [0x33 ... 0x39] = INVALID,
            [0x40 ... 0x7E] = VALID, // it is a double-byte
            [0x7F] = INVALID,
            [0x80 ... 0xFE] = VALID, // it is a double-byte
            [0xFF] = INVALID,
        },
    [BYTE_E3_32] =
        {
            [0x00 ... 0x80] = INVALID,
            [0x81 ... 0x99] = Q_ONE_MORE,
            [0x9A] = BYTE_E3_32_9A,
            [0x9B ... 0xFF] = INVALID,
        },
    [BYTE_E3_32_9A] =
        {
            [0x00 ... 0x2F] = INVALID,
            [0x30 ... 0x35] = VALID, // it is a quad-byte
            [0x36 ... 0xFF] = INVALID,
        },
    [INVALID] = {[0x00 ... 0xFF] = INVALID},
};

bool check_by_range(char *data, size_t len) {
  size_t i = 0;
  while (i < len) {
    uint8_t byte = (uint8_t)data[i];
    if (byte <= 0x7F) {
      // it is ASCII (single byte)
      i++;
    } else if (byte == 0x80 || byte == 0xFF) {
      // invalid
      return false;
    } else {
      // double-byte or quad-byte
      if (i + 1 >= len) {
        return false; // too short
      }
      uint8_t byte2 = (uint8_t)data[i + 1];
      if (byte2 >= 0x40 && byte2 <= 0xFE && byte2 != 0x7F) {
        // double-byte
        i += 2;
      } else if (byte2 < 0x30 || byte2 == 0x7F || byte2 == 0xFF) {
        // invalid
        return false;
      } else {
        // byte2 is from 0x30 to 0x39, it is a quad-byte
        if (i + 3 >= len) {
          return false; // too short
        }
        uint8_t byte3 = (uint8_t)data[i + 2];
        if (byte3 < 0x81 || byte3 > 0xFE) {
          return false;
        }
        uint8_t byte4 = (uint8_t)data[i + 3];
        if (byte4 < 0x30 || byte4 > 0x39) {
          return false;
        }
        /* if ((byte >= 0x81) && (byte <= 0x8F) && */
        /*     (byte << 24) + (byte2 << 16) + (byte3 << 8) + byte4 > 0x8431A439)
         * { */
        /*   return false; // out of range */
        /* } */
        // is the following better?
        if (byte > 0x84 && byte <= 0x8F) {
          return false; // out of range
        } else if (byte == 0x84) {
          if (byte2 > 0x31)
            return false;
          else if (byte2 == 0x31) {
            if (byte3 > 0xA4)
              return false;
            else if (byte3 == 0xA4) {
              if (byte4 > 0x39)
                return false;
            }
          }
        }

        /* if ((byte >= 0x90) && */
        /*     (byte << 24) + (byte2 << 16) + (byte3 << 8) + byte4 > 0xE3329A35)
         * { */
        /*   return false; // out of range */
        /* } */
        if (byte > 0xE3)
          return false;
        else if (byte == 0xE3) {
          if (byte2 > 0x32)
            return false;
          else if (byte2 == 0x32) {
            if (byte3 > 0x9A)
              return false;
            else if (byte3 == 0x9A) {
              if (byte4 > 0x35)
                return false;
            }
          }
        }
        i += 3;
      }
    }
  }
  return true;
}

/* bool check_by_range(char *data, size_t len) { */
/*   size_t i = 0; */
/*   while (i < len) { */
/*     uint8_t byte = (uint8_t)data[i]; */
/*     if (byte <= 0x7F) { */
/*       // it is ASCII (single byte) */
/*       i++; */
/*     } else if (byte >= 0x81 && byte <= 0xFE) { */
/*       // double byte or four byte */
/*       if (i + 1 >= len) { */
/*         return false; // too short */
/*       } */
/*       uint8_t byte2 = (uint8_t)data[i + 1]; */
/*       if (byte2 >= 0x40 && byte2 <= 0xFE && byte2 != 0x7F) { */
/*         // double byte */
/*         i += 2; // note that for double-byte, undefined has been mapped to
 * PUA. */
/*       } else if (byte2 >= 0x30 && byte2 <= 0x39) { */
/*         // may be four byte */
/*         if (i + 3 >= len) { */
/*           return false; // too short */
/*         } */
/*         uint8_t byte3 = (uint8_t)data[i + 2]; */
/*         if (byte3 < 0x81 || byte3 > 0xFE) { */
/*           return false; */
/*         } */
/*         uint8_t byte4 = (uint8_t)data[i + 3]; */
/*         if (byte4 < 0x30 || byte4 > 0x39) { */
/*           return false; */
/*         } */
/*         if ((byte >= 0x81) && (byte <= 0x8F) && */
/*             (byte << 24) + (byte2 << 16) + (byte3 << 8) + byte4 > 0x8431A439)
 * { */
/*           return false; // out of range */
/*         } */
/*         if ((byte >= 0x90) && */
/*             (byte << 24) + (byte2 << 16) + (byte3 << 8) + byte4 > 0xE3329A35)
 * { */
/*           return false; // out of range */
/*         } */
/*         i += 3; */
/*       } else { */
/*         return false; */
/*       } */
/*     } else { */
/*       return false; */
/*     } */
/*   } */
/*   return true; */
/* } */

static inline GB_State update_state(uint8_t byte, GB_State *state) {
  *state = State_Transitions[*state][byte];
  return *state;
}

bool check_by_fsm(char *data, size_t len) {
  GB_State state = VALID;
  for (size_t i = 0; i < len; i++) {
    if (update_state((uint8_t)data[i], &state) == INVALID) {
      return false;
    }
  }
  return state == VALID;
}
