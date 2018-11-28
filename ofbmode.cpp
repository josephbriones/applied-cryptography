#include "ofbmode.h"

// EXP37-C
OFBMode::OFBMode(unsigned int numWordsInBlock, unsigned int numWordsInKey) :
  ModeOfOp(numWordsInBlock, numWordsInKey) {
  uniqueIV(1);
}

// OOP52-CPP
OFBMode::~OFBMode() {}
// EXP40-C
void OFBMode::encrypt(const std::string plaintxt) {
  std::vector<Block> cipher;
  std::vector<Block> plain = textToBlocks(plaintxt);
  Block temp = IV;
  Block x;

  cipher.push_back(IV);
  for (Block block : plain) {
    temp = aes->encrypt(temp);
    usedIVs.insert(temp);
    // CTR50-CPP, INT30-C
    for (unsigned int i = 0; i < block.size(); ++i) {
      x.push_back(temp[i] ^ block[i]);
    }
    cipher.push_back(x);
    x.clear();
  }

  // Save cipher blocks to file.
  saveBlocks("data/cipher", cipher);
}

std::string OFBMode::decrypt() {
  // Load cipher blocks from file.
  std::vector<Block> plain;
  std::vector<Block> cipher;
  loadBlocks("data/cipher", &cipher);

  Block temp = cipher.front();
  cipher.erase(cipher.begin());
  Block x;

  for (Block block : cipher) {
    temp = aes->encrypt(temp);
    usedIVs.insert(temp);
    // CTR50-CPP, INT30-C
    for (unsigned int i = 0; i < block.size(); ++i) {
      x.push_back(temp[i] ^ block[i]);
    }
    plain.push_back(x);
    x.clear();
  }
  // STR50-CPP
  return blocksToText(plain);
}
