#include "gb18030.h"
#include <stdbool.h>

bool check_by_range(char *data, size_t len) {
  size_t i = 0;
  while (i < len) {
    uint8_t byte = (uint8_t)data[i];
    if (byte <= 0x7F) {
      // it is ASCII (single byte)
      i++;
    } else if (byte >= 0x81 && byte <= 0xFE) {
      // double byte or four byte
      if (i + 1 >= len) {
        return false; // too short
      }
      uint8_t byte2 = (uint8_t)data[i + 1];
      if (byte2 >= 0x40 && byte2 <= 0xFE && byte2 != 0x7F) {
        // double byte
        i += 2; // note that for double-byte, undefined has been mapped to PUA.
      } else if (byte2 >= 0x30 && byte2 <= 0x39) {
        // may be four byte
        if (i + 3 >= len) {
          return false; // too short
        }
        if ((byte >= 0x85 && byte <= 0x8F) || (byte >= 0xE4 && byte <= 0xFE)) {
          return false; // reserved, user-defined
        }
        uint8_t byte3 = (uint8_t)data[i + 2];
        uint8_t byte4 = (uint8_t)data[i + 3];
        if (byte == 0x84 && ((byte2 << 16) + (byte3 << 8) + byte4) > 0x31A439) {
          return false; // reserved
        }
        if (byte == 0xE3 && ((byte2 << 16) + (byte3 << 8) + byte4) > 0x329A35) {
          return false;
        }
        i += 3;
      } else {
        return false;
      }
    } else {
      return false;
    }
  }
  return true;
}

// eleven states
typedef enum GB_State GB_State;
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

const uint8_t State_Transitions[11][256] = {
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

inline GB_State update_state(uint8_t byte, GB_State *state) {
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
