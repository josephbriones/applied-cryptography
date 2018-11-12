#include "algebra.h"

word rcon(const uint n) {
  uint8_t b = 2;
  for (uint i = 0; i < n-1; ++i) {
    b = xtimes(b);
  }
}
