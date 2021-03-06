#ifndef CBCMODE_H
#define CBCMODE_H

#include "modeofop.h"

// DCL60-CPP
class CBCMode : public ModeOfOp {
 public:
  // Constructor which takes as input its block cipher's # of words per block
  // and the # of words per key.
  CBCMode(unsigned int numWordsInBlock, unsigned int numWordsInKey);

  // Default destructor that calls the parent destructor.
  ~CBCMode();

  // Override functions for encrypting or decrypting some text according to the
  // cipher block chaining mode of operation.
  void encrypt(const std::string plaintxt) override final;
  std::string decrypt() override final;
};

#endif  // CBCMODE_H
