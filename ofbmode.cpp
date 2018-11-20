#include "ofbmode.h"

OFBMode::OFBMode(uint numBytesInBlock, uint numWordsInKey) :
  ModeOfOp(numBytesInBlock, numWordsInKey) {
  uniqueIV();
}

std::string OFBMode::encrypt(const std::string plaintxt) {
  std::vector<Block> cipher;
  std::vector<Block> plain = textToBlocks(plaintxt);
  Block temp = IV;
  Block x;

  for (Block block : plain) {
    temp = aes->encrypt(temp);
    for (uint i = 0; i < block.size(); ++i) {
      x[i] = temp[i] ^ block[i];
    }
    cipher.push_back(x);
  }

  return blocksToText(cipher);
}

std::string OFBMode::decrypt(const std::string ciphertxt) {
  std::vector<Block> plain;
  std::vector<Block> cipher = textToBlocks(ciphertxt);
  Block temp = IV;
  Block x;

  for (Block block : cipher) {
    temp = aes->encrypt(temp);
    for (uint i = 0; i < block.size(); ++i) {
      x[i] = temp[i] ^ block[i];
    }
    plain.push_back(x);
  }

  return blocksToText(plain);
}
