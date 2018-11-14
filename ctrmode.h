#include "modeofop.h"

class CTRMode : public ModeOfOp {
 public:
  // Constructor which takes as input a pointer to an AES object (to be used as
  // CTR's block cipher) and the # of bytes per block.
  CTRMode(AES * aes, uint blockSize);

  // Override functions for encrypting or decrypting some text according to the
  // counter mode of operation.
  std::string encrypt(const std::string plaintxt) override final;
  std::string decrypt(const std::string ciphertxt) override final;
};
