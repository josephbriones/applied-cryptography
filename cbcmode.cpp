#include "cbcmode.h"

CBCMode::CBCMode(AES * aes, uint blockSize) :
  ModeOfOp(aes, blockSize) {
  IV = unpredictableIV();
}

std::string encrypt(const std::string plaintxt) {


  return "";  // TODO.
}

std::string decrypt(const std::string ciphertxt) {

  return "";  // TODO.
}
