#include "cbcmode.h"
#include <iostream>

CBCMode::CBCMode(unsigned int numWordsInBlock, unsigned int numWordsInKey) : 
  ModeOfOp(numWordsInBlock, numWordsInKey) {
  unpredictableIV();
}

CBCMode::~CBCMode(){
  //calls parent class destructor by default
}

std::string CBCMode::encrypt(const std::string plaintxt) {
  
  std::vector<Block> cipher;
  std::vector<Block> plain = textToBlocks(plaintxt);
  pad(&plain);
 
  Block temp = IV;
  for (Block block : plain) {
    for (unsigned int i = 0; i < temp.size(); ++i) {
      temp[i] = temp[i] ^ block[i];
    }
    temp = aes->encrypt(temp); 
    cipher.push_back(temp);
  }
  cipher.push_back(IV);
  return blocksToText(cipher);
}

std::string CBCMode::decrypt(const std::string ciphertxt) {
  std::vector<Block> plain;
  std::vector<Block> cipher = textToBlocks(ciphertxt);
  Block temp = cipher.back();
  cipher.pop_back();
  Block x, d;

  for (Block block : cipher) {
    d = aes->decrypt(block);
    for (unsigned int i = 0; i < temp.size(); ++i) {
      x.push_back(d[i] ^ temp[i]);
    }
    plain.push_back(x);
    temp = block;
    x.clear();
  }
  invPad(&plain);
  return blocksToText(plain);
}
