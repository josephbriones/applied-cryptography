#include "ctrmode.h"

CTRMode::CTRMode(AES * aes, uint blockSize) :
  ModeOfOp(aes, blockSize) {
  IV = uniqueIV();
}

std::string encrypt(const std::string plaintxt) {


  return "";  // TODO.
}

std::string decrypt(const std::string ciphertxt) {

  return "";  // TODO.
}
