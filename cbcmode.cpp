#include "cbcmode.h"

// EXP37-C
CBCMode::CBCMode(unsigned int numWordsInBlock, unsigned int numWordsInKey) :
  ModeOfOp(numWordsInBlock, numWordsInKey) {
  unpredictableIV();
}

// OOP52-CPP
CBCMode::~CBCMode() {}
// EXP40-C
void CBCMode::encrypt(const std::string plaintxt) {
  std::vector<Block> cipher;
  std::vector<Block> plain = textToBlocks(plaintxt);
  pad(&plain);

  Block temp = IV;
  cipher.push_back(IV);
  for (Block block : plain) {
    // CTR50-CPP, INT30-C
    for (unsigned int i = 0; i < temp.size(); ++i) {
      temp[i] = temp[i] ^ block[i];
    }
    temp = aes->encrypt(temp);
    cipher.push_back(temp);
  }

  // Save cipher blocks to file.
  saveBlocks("data/cipher", cipher);
}

std::string CBCMode::decrypt() {
  // Load cipher blocks from file.
  std::vector<Block> plain;
  std::vector<Block> cipher;
  loadBlocks("data/cipher", &cipher);

  Block temp = cipher.front();
  cipher.erase(cipher.begin());
  Block x, d;

  for (Block block : cipher) {
    d = aes->decrypt(block);
    // CTR50-CPP, INT30-C
    for (unsigned int i = 0; i < temp.size(); ++i) {
      x.push_back(d[i] ^ temp[i]);
    }
    plain.push_back(x);
    temp = block;
    x.clear();
  }

  invPad(&plain);

  // STR50-CPP
  return blocksToText(plain);
}
