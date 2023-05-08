#include "gb18030.h"

bool isNotSupplementary(uint8_t b2, uint8_t b3, uint8_t b4) {
  const size_t value = 0x10FFFF - 0x10000 - (0xE3 - 0x90) * 12600;
  return (b2 - 0x30) * 1260 + (b3 - 0x81) * 10 + (b4 - 0x30) > value;
}

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
        if (byte == 0x84 && ((byte2 << 16) + (byte3 << 8) + byte4) > 0x31A4A9) {
          return false; // reserved
        }
        if (byte == 0xE3 && isNotSupplementary(byte2, byte3, byte4)) {
          return false; // reserved
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
