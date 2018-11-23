#ifndef MODEOFOP_H
#define MODEOFOP_H

#include <string>
#include <set>
#include <vector>

#include "aes.h"

class ModeOfOp {
 public:
  // Constructor which takes as input its block cipher's # of words per block
  // and the # of words per key.
  ModeOfOp(unsigned int numWordsInBlock, unsigned int numWordsInKey);

  // Destructor which garbage collects the AES object and saves the used
  // initialization vectors to file.
  virtual ~ModeOfOp();

  // Pure virtual functions for encrypting or decrypting some text; must be
  // overridden by child classes.
  virtual std::string encrypt(const std::string plaintxt) = 0;
  virtual std::string decrypt(const std::string ciphertxt) = 0;

 protected:
  // Convenience typedef for representing blocks as vectors of bytes (uint8_t),
  // ordered from most to least significant byte. The correct size is
  // maintainted by the mode of operation's "blockSize".
  typedef std::vector<uint8_t> Block;

  // Properties.
  unsigned int numWordsInBlock;  // Block size, in # of words.
  std::vector<uint32_t> key;     // Key for the block cipher.
  AES * aes;                     // Pointer to AES block cipher.
  std::set<Block> usedIVs;       // Container of all previously used IVs.
  Block IV;                      // Current IV.

  // Utility functions for loading and saving the list of previously used
  // initialization vectors.
  void loadIVs(const std::string fname);
  void saveIVs(const std::string fname);

  // Functions for generating unpredictable or unique initialization vectors.
  // Unpredictable IVs are obtained through random number generation. Unique IVs
  // are also obtained through random number generation, but ensure that none
  // of IV, IV+1, IV+2, ..., IV+(numIVs-1) have ever been used before.
  void unpredictableIV();
  void uniqueIV(unsigned int numIVs);

  // Functions for changing text into a vector of blocks and back.
  std::vector<Block> textToBlocks(std::string text);
  std::string blocksToText(const std::vector<Block> blocks);

  // Given a vector of blocks, pad() performs PKCS#7 padding (i.e., it appends
  // X = blockSize - (|text| mod blockSize) copies of X, unless X = 0, in which
  // case blockSize copies of blockSize are appended instead). invPad() simply
  // does the opposite, removing any padding.
  void pad(std::vector<Block> * blocks);
  void invPad(std::vector<Block> * blocks);

  // Utility function for getting random bytes and words from /dev/random. The
  // number of bytes or words to create is passed as input.
  std::vector<uint8_t> randBytes(const unsigned int numBytes);
  std::vector<uint32_t> randWords(const unsigned int numWords);
};

#endif  // MODEOFOP_H
