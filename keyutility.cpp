#include "algebra.h"

<<<<<<< HEAD
word KeyUtility::rcon(const uint n) {
  byte b = 2;
=======
word rcon(const uint n) {
  uint8_t b = 2;
>>>>>>> c90e81b66dcabd28a3aca7c6a441e841dd6a5b29
  for (uint i = 0; i < n-1; ++i) {
    b = xtimes(b);
  }

}
