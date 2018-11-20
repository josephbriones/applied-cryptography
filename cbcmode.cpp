#include "cbcmode.h"

CBCMode::CBCMode(uint numBytesInBlock, uint numWordsInKey) :
  ModeOfOp(numBytesInBlock, numWordsInKey) {
  unpredictableIV();
}

std::string CBCMode::encrypt(const std::string plaintxt) {
  std::vector<Block> cipher;
  std::vector<Block> plain = textToBlocks(plaintxt);
  pad(plain);
  Block temp = IV;

  for (Block block : plain) {
    for (uint i = 0; i < temp.size(); ++i) {
      temp[i] = temp[i] ^ block[i];
    }
    temp = aes->encrypt(temp);
    cipher.push_back(temp);
  }

  return blocksToText(cipher);
}

std::string CBCMode::decrypt(const std::string ciphertxt) {
  std::vector<Block> plain;
  std::vector<Block> cipher = textToBlocks(ciphertxt);
  Block temp = IV;
  Block x, d;

  for (Block block : cipher) {
    d = aes->decrypt(block);
    for (uint i = 0; i < temp.size(); ++i) {
      x[i] = d[i] ^ temp[i];
    }
    plain.push_back(x);
    temp = block;
  }

  invpad(plain);
  return blocksToText(plain);
}
