#include "ofbmode.h"

OFBMode::OFBMode(unsigned int numWordsInBlock, unsigned int numWordsInKey) :
  ModeOfOp(numWordsInBlock, numWordsInKey) {
  uniqueIV(1);
}

OFBMode::~OFBMode(){
  //calls parent class destructor by default
}

std::string OFBMode::encrypt(const std::string plaintxt) {
  std::vector<Block> cipher;
  std::vector<Block> plain = textToBlocks(plaintxt);
  Block temp = IV;
  Block x;

  for (Block block : plain) {
    temp = aes->encrypt(temp);
    usedIVs.insert(temp);
    for (unsigned int i = 0; i < block.size(); ++i) {
      x[i] = temp[i] ^ block[i];
    }
    cipher.push_back(x);
  }
  cipher.push_back(IV);
  return blocksToText(cipher);
}

std::string OFBMode::decrypt(const std::string ciphertxt) {
  std::vector<Block> plain;
  std::vector<Block> cipher = textToBlocks(ciphertxt);
  Block temp = cipher.back();
  cipher.pop_back();
  Block x;

  for (Block block : cipher) {
    temp = aes->encrypt(temp);
    usedIVs.insert(temp);
    for (unsigned int i = 0; i < block.size(); ++i) {
      x[i] = temp[i] ^ block[i];
    }
    plain.push_back(x);
  }

  return blocksToText(plain);
}
