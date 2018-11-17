#include "ofbmode.h"

OFBMode::OFBMode(uint numBytesInBlock, uint numWordsInKey) :
  ModeOfOp(numBytesInBlock, numWordsInKey) {
  uniqueIV();
}

std::string OFBMode::encrypt(const std::string plaintxt) {


  return "";  // TODO.
}

std::string OFBMode::decrypt(const std::string ciphertxt) {

  return "";  // TODO.
}
