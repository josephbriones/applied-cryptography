#include "cbcmode.h"

CBCMode::CBCMode(uint numBytesInBlock, uint numWordsInKey) :
  ModeOfOp(numBytesInBlock, numWordsInKey) {
  unpredictableIV();
}

std::string CBCMode::encrypt(const std::string plaintxt) {


  return "";  // TODO.
}

std::string CBCMode::decrypt(const std::string ciphertxt) {

  return "";  // TODO.
}
