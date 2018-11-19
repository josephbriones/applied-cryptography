#include "cbcmode.h"

//typedef std::vector<uint8_t> Block;

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
  std::string plaintxt = blocksToText(&plain);
  return plaintxt;*/
  return "";  // TODO.
}
