#ifndef MODEOFOP_H
#define MODEOFOP_H

#include <string>
#include <vector>

#include "aes.h"

class ModeOfOp {
 public:
  // Constructor which takes as input its block cipher's # of bytes per block
  // and the # of words per key.
  ModeOfOp(uint numBytesInBlock, uint numWordsInKey);

  // TODO: need a destructor to garbage collect the AES object.

  // Pure virtual functions for encrypting or decrypting some text; must be
  // overridden by child classes.
  virtual std::string encrypt(const std::string plaintxt) = 0;
  virtual std::string decrypt(const std::string ciphertxt) = 0;

 protected:
  // Convenience typedef for representing blocks as vectors of bytes (uint8_t).
  // The correct size is maintainted by the mode of operation's "blockSize".
  typedef std::vector<uint8_t> Block;

  // Properties.
  uint numBytesInBlock;
  std::vector<uint32_t> key;
  AES * aes;
  Block IV;

  // Functions for generating unpredictable or unique initialization vectors.
  // Unpredictable IVs are obtained through random number generation, while
  // uniqueIVs are started at a random seed and incremented in large enough
  // step sizes to guarantee uniqueness. (TODO: may need to fix latter).
  void unpredictableIV();
  void uniqueIV();

  // Functions for changing text into a vector of blocks and back.
  std::vector<Block> textToBlocks(const std::string text);
  std::string blocksToText(const std::vector<Block>& blocks);

  // Given a vector of blocks, pad() performs PKCS#7 padding (i.e., it appends
  // X = blockSize - (|text| mod blockSize) copies of X, unless X = 0, in which
  // case blockSize copies of blockSize are appended instead). invPad() simply
  // does the opposite, removing any padding.
  void pad(const std::vector<Block>& blocks);
  void invPad(const std::vector<Block>& blocks);

  // Utility function for getting random words from /dev/random. The number of
  // words to create is passed as input.
  std::vector<uint32_t> randWords(uint numWords);
};

#endif  // MODEOFOP_H
