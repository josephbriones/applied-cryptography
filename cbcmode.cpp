#include "cbcmode.h"
//#include "aes.h"
//#include "modeofop.h"
//typedef std::vector<uint8_t> Block;

CBCMode::CBCMode(uint numBytesInBlock, uint numWordsInKey) :
  ModeOfOp(numBytesInBlock, numWordsInKey) {
  unpredictableIV();
}

std::string CBCMode::encrypt(const std::string plaintxt) {
  /*std::vector<Block> cipher;
  std::vector<Block> plain = textToBlocks(plaintxt);
  pad(&plain);
  vector<Block>::iterator itr;
  Block temp = IV;
  AES loopaes;
  for(itr = plain.begin(); itr < plain.end(); itr++){
    temp = temp ^ *itr;
    temp = loopaes.encrypt(temp);
    cipher.push_back(temp);
  }
  std::string ciphertxt = blocksToText(&cipher);
  return ciphertxt;*/
  return "";  // TODO.
}

std::string CBCMode::decrypt(const std::string ciphertxt) {
  /*std::vector<Block> plain;
  std::vector<Block> cipher = textToBlocks(ciphertxt);
  vector<Block>::iterator itr;
  AES loopaes;
  Block temp = IV;
  Block x;
  for(itr = cipher.begin(); itr < cipher.end(); itr++){
    x = loopaes.decrypt(*itr) ^ temp;
    plain.push_back(x);
    temp = *itr;
  }
  invpad(&plain);
  std::string plaintxt = blocksToText(&plain);
  return plaintxt;*/
  return "";  // TODO.
}
