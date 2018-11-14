#include "modeofop.h"

class OFBMode : public ModeOfOp {
 public:
  // Constructor which takes as input a pointer to an AES object (to be used as
  // OFB's block cipher) and the # of bytes per block.
  OFBMode(AES * aes, uint blockSize);

  // Override functions for encrypting or decrypting some text according to the
  // output feedback mode of operation.
  std::string encrypt(const std::string plaintxt) override final;
  std::string decrypt(const std::string ciphertxt) override final;
};
