#include "ctrmode.h"

// EXP37-C
CTRMode::CTRMode(unsigned int numWordsInBlock, unsigned int numWordsInKey) :
  ModeOfOp(numWordsInBlock, numWordsInKey) {}

// OOP52-CPP
CTRMode::~CTRMode() {}
// EXP40-C
void CTRMode::encrypt(const std::string plaintxt) {
  std::vector<Block> cipher;
  std::vector<Block> plain = textToBlocks(plaintxt);
  uniqueIV(plain.size());
  Block ctr = IV;
  Block x,temp;

  cipher.push_back(IV);
  for (Block block : plain) {
    temp = aes->encrypt(ctr);
    // CTR50-CPP, INT30-C
    for (unsigned int i = 0; i < block.size(); ++i) {
      x.push_back(temp[i] ^ block[i]);
    }
    cipher.push_back(x);
    x.clear();

    // Get the counter for the next round.
    blockInc(&ctr);
  }

  // Save cipher blocks to file.
  saveBlocks("data/cipher", cipher);
}

std::string CTRMode::decrypt() {
  // Load cipher blocks from file.
  std::vector<Block> plain;
  std::vector<Block> cipher;
  loadBlocks("data/cipher", &cipher);

  Block ctr = cipher.front();
  cipher.erase(cipher.begin());
  Block x,temp;

  for (Block block : cipher) {
    temp = aes->encrypt(ctr);

    // CTR50-CPP, INT30-C
    for (unsigned int i = 0; i < block.size(); ++i) {
      x.push_back(temp[i] ^ block[i]);
    }
    plain.push_back(x);
    x.clear();

    // Get counter for next round.
    blockInc(&ctr);
  }

  // STR50-CPP
  return blocksToText(plain);
}
