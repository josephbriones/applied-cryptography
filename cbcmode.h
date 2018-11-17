#ifndef CBCMODE_H
#define CBCMODE_H

#include "modeofop.h"

class CBCMode : public ModeOfOp {
 public:
  // Constructor which takes as input its block cipher's # of bytes per block
  // and the # of words per key.
  CBCMode(uint numBytesInBlock, uint numWordsInKey);

  // TODO: need a destructor to call the parent destructor.

  // Override functions for encrypting or decrypting some text according to the
  // cipher block chaining mode of operation.
  std::string encrypt(const std::string plaintxt) override final;
  std::string decrypt(const std::string ciphertxt) override final;
};

#endif  // CBCMODE_H
