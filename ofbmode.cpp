#include "ofbmode.h"

OFBMode::OFBMode(uint numWordsInBlock, uint numWordsInKey) :
  ModeOfOp(numWordsInBlock, numWordsInKey) {
  uniqueIV(1);
}

std::string OFBMode::encrypt(const std::string plaintxt) {


  return "";  // TODO.
}

std::string OFBMode::decrypt(const std::string ciphertxt) {

  return "";  // TODO.
}
