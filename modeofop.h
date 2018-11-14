#include <string>
#include <vector>

#include "aes.h"

class ModeOfOp {
 public:
  // Constructor which takes as input a pointer to an AES object (to be used as
  // this mode of operation's block cipher) and the # of bytes per block.
  ModeOfOp(AES * aes, uint blockSize);

  // Pure virtual functions for encrypting or decrypting some text; must be
  // overridden by child classes.
  virtual std::string encrypt(const std::string plaintxt) = 0;
  virtual std::string decrypt(const std::string ciphertxt) = 0;

 private:
  // Convenience typedef for representing blocks as vectors of bytes (uint8_t).
  // The correct size is maintainted by the mode of operation's "blockSize".
  typedef std::vector<uint8_t> Block;

  // Properties.
  AES * aes;
  uint blockSize;
  Block IV;

  // Pure virtual function for generating the initialization vector; must be
  // overridden by child classes.
  virtual void generateIV() = 0;

  // Given a vector of blocks, pad() performs PKCS#7 padding (i.e., it appends
  // X = blockSize - (|text| mod blockSize) copies of X, unless X = 0, in which
  // case blockSize copies of blockSize are appended instead). invPad() simply
  // does the opposite, removing any padding.
  void pad(const std::vector<Block>& blocks);
  void invPad(const std::std::vector<Block>& blocks);
};
