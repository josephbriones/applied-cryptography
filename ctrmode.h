#ifndef CTRMODE_H
#define CTRMODE_H

#include "modeofop.h"

class CTRMode : public ModeOfOp {
 public:
  // Constructor which takes as input its block cipher's # of words per block
  // and the # of words per key.
  CTRMode(unsigned int numWordsInBlock, unsigned int numWordsInKey);

  // TODO: need a destructor to call the parent destructor.
 ~CTRMode();
  // Override functions for encrypting or decrypting some text according to the
  // counter mode of operation.
  std::string encrypt(const std::string plaintxt) override final;
  std::string decrypt(const std::string ciphertxt) override final;
};

#endif  // CTRMODE_H
