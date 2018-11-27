#include "ctrmode.h"

CTRMode::CTRMode(unsigned int numWordsInBlock, unsigned int numWordsInKey) :
  ModeOfOp(numWordsInBlock, numWordsInKey) {}

  CTRMode::~CTRMode(){
    //calls parent class destructor by default
  }

void CTRMode::encrypt(const std::string plaintxt) {
  std::vector<Block> cipher;
  std::vector<Block> plain = textToBlocks(plaintxt);
  uniqueIV(plain.size());
  Block ctr = IV;
  Block x,temp;

  cipher.push_back(IV);
  for (Block block : plain) {
    temp = aes->encrypt(ctr);

    //converting counter to int to increment to next counter
    unsigned int ctrAsInt = 0;
    for (unsigned int i = 1; i <= ctr.size(); ++i) {
      ctrAsInt = ctrAsInt << 8;
      ctrAsInt += ctr[ctr.size() - i];
    }
    ctrAsInt++;

    //converting new counter to block
    Block tempBlock;
    unsigned int tempInt = (ctrAsInt) % (1 << (32 * numWordsInBlock));
    while (tempInt != 0) {
      uint8_t byte = tempInt % 256;
      tempBlock.emplace(tempBlock.begin(), byte);
      tempInt = tempInt / 256;
    }
    //counter updated to next counter for next round
    ctr = tempBlock;

    for (unsigned int i = 0; i < block.size(); ++i) {
      x.push_back(temp[i] ^ block[i]);
    }
    cipher.push_back(x);
    x.clear();
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
    //converting counter to int to increment to next counter
    unsigned int ctrAsInt = 0;
    for (unsigned int i = 1; i <= ctr.size(); ++i) {
      ctrAsInt = ctrAsInt << 8;
      ctrAsInt += ctr[ctr.size() - i];
    }
    ctrAsInt++;

    //converting new counter to block
    Block tempBlock;
    unsigned int tempInt = (ctrAsInt) % (1 << (32 * numWordsInBlock));
    while (tempInt != 0) {
      uint8_t byte = tempInt % 256;
      tempBlock.emplace(tempBlock.begin(), byte);
      tempInt = tempInt / 256;
    }
    //counter updated to next counter for next round
    ctr = tempBlock;

    for (unsigned int i = 0; i < block.size(); ++i) {
      x.push_back(temp[i] ^ block[i]);
    }
    plain.push_back(x);
    x.clear();
  }

  return blocksToText(plain);
}
