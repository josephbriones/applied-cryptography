#ifndef OFBMODE_H
#define OFBMODE_H

#include "modeofop.h"

class OFBMode : public ModeOfOp {
 public:
  // Constructor which takes as input its block cipher's # of words per block
  // and the # of words per key.
  OFBMode(unsigned int numWordsInBlock, unsigned int numWordsInKey);

  // Default destructor that calls the parent destructor.
  ~OFBMode();

  // Override functions for encrypting or decrypting some text according to the
  // output feedback mode of operation.
  void encrypt(const std::string plaintxt) override final;
  std::string decrypt() override final;
};

#endif  // OFBMODE_H
