#ifndef OFBMODE_H
#define OFBMODE_H

#include "modeofop.h"

class OFBMode : public ModeOfOp {
 public:
  // Constructor which takes as input its block cipher's # of words per block
  // and the # of words per key.
  OFBMode(uint numWordsInBlock, uint numWordsInKey);

  // TODO: need a destructor to call the parent destructor.

  // Override functions for encrypting or decrypting some text according to the
  // output feedback mode of operation.
  std::string encrypt(const std::string plaintxt) override final;
  std::string decrypt(const std::string ciphertxt) override final;
};

#endif  // OFBMODE_H
