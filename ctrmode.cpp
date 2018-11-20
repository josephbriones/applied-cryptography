#include "ctrmode.h"

CTRMode::CTRMode(uint numBytesInBlock, uint numWordsInKey) :
  ModeOfOp(numBytesInBlock, numWordsInKey) {

}

std::string CTRMode::encrypt(const std::string plaintxt) {
  std::vector<Block> cipher;
  std::vector<Block> plain = textToBlocks(plaintxt);
  uniqueIV(plain.size());
  Block ctr = IV;
  Block x,temp;

  for (Block block : plain) {
    temp = aes->encrypt(ctr);
    ctr++;
    for (uint i = 0; i < block.size(); ++i) {
      x[i] = temp[i] ^ block[i];
    }
    cipher.push_back(x);
  }
  cipher.push_back(IV);
  return blocksToText(cipher);
}

std::string CTRMode::decrypt(const std::string ciphertxt) {
  std::vector<Block> plain;
  std::vector<Block> cipher = textToBlocks(ciphertxt);
  Block ctr = cipher.pop_back();
  Block x,temp;

  for (Block block : cipher) {
    temp = aes->encrypt(ctr);
    ctr++;
    for (uint i = 0; i < block.size(); ++i) {
      x[i] = temp[i] ^ block[i];
    }
    plain.push_back(x);
  }

  return blocksToText(plain);
}
