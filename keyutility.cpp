#include "algebra.h"

word KeyUtility::rcon(const uint n) {
  byte b = 2;
  for (uint i = 0; i < n-1; ++i) {
    b = xtimes(b);
  }

}
