#include "modeofop.h"

class CBCMode : public ModeOfOp {
 public:
  // Constructor which takes as input a pointer to an AES object (to be used as
  // CBC's block cipher) and the # of bytes per block.
  CBCMode(AES * aes, uint blockSize);

  // Override functions for encrypting or decrypting some text according to the
  // cipher block chaining mode of operation.
  std::string encrypt(const std::string plaintxt) override final;
  std::string decrypt(const std::string ciphertxt) override final;
};
