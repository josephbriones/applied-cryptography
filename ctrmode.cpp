#include "ctrmode.h"

CTRMode::CTRMode(uint numBytesInBlock, uint numWordsInKey) :
  ModeOfOp(numBytesInBlock, numWordsInKey) {
  uniqueIV();
}

std::string CTRMode::encrypt(const std::string plaintxt) {


  return "";  // TODO.
}

std::string CTRMode::decrypt(const std::string ciphertxt) {

  return "";  // TODO.
}
