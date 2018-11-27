#include "cbcmode.h"
#include <iostream>

CBCMode::CBCMode(unsigned int numWordsInBlock, unsigned int numWordsInKey) :
  ModeOfOp(numWordsInBlock, numWordsInKey) {
  unpredictableIV();
}

CBCMode::~CBCMode() {}

void CBCMode::encrypt(const std::string plaintxt) {
  // DEBUG
  std::cout << std::endl << "CBC ENCRYPT:\nPlain Text: \"" << plaintxt << "\""
            << std::endl;

  std::vector<Block> cipher;
  std::vector<Block> plain = textToBlocks(plaintxt);

  // DEBUG
  std::cout << "Plain Blocks:" << std::endl;
  for (Block block : plain) {
    std::cout << "[";
    for (uint8_t byte : block) {
      std::cout << std::hex << +byte << ",";
    }
    std::cout << "]" << std::endl;
  }

  pad(&plain);

  // DEBUG
  std::cout << "Padded Plain Blocks:" << std::endl;
  for (Block block : plain) {
    std::cout << "[";
    for (uint8_t byte : block) {
      std::cout << std::hex << +byte << ",";
    }
    std::cout << "]" << std::endl;
  }

  Block temp = IV;
  cipher.push_back(IV);
  for (Block block : plain) {
    for (unsigned int i = 0; i < temp.size(); ++i) {
      temp[i] = temp[i] ^ block[i];
    }
    temp = aes->encrypt(temp);
    cipher.push_back(temp);
  }

  // DEBUG
  std::cout << "Padded Cipher Blocks:" << std::endl;
  for (Block block : cipher) {
    std::cout << "[";
    for (uint8_t byte : block) {
      std::cout << std::hex << +byte << ",";
    }
    std::cout << "]" << std::endl;
  }

  // DEBUG:
  std::string result = blocksToText(cipher);
  std::cout << std::dec << "Should have a result of length " << +result.length()
            << std::endl << std::endl;

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

  // DEBUG
  std::cout << "Padded Cipher Blocks:" << std::endl;
  for (Block block : cipher) {
    std::cout << "[";
    for (uint8_t byte : block) {
      std::cout << std::hex << +byte << ",";
    }
    std::cout << "]" << std::endl;
  }
  std::cout << "IV:" << std::endl;
  std::cout << "[";
  for (uint8_t byte : temp) {
    std::cout << std::hex << +byte << ",";
  }
  std::cout << "]" << std::endl;

  for (Block block : cipher) {
    // DEBUG
    std::cout << std::endl << "Debugging block [";
    for (uint8_t byte : block) {
      std::cout << std::hex << +byte << ",";
    }
    std::cout << "]" << std::endl;

    d = aes->decrypt(block);
    for (unsigned int i = 0; i < temp.size(); ++i) {
      x.push_back(d[i] ^ temp[i]);
    }
    plain.push_back(x);
    temp = block;
    x.clear();
  }

  // DEBUG
  std::cout << "Padded Plain Blocks:" << std::endl;
  for (Block block : plain) {
    std::cout << "[";
    for (uint8_t byte : block) {
      std::cout << std::hex << +byte << ",";
    }
    std::cout << "]" << std::endl;
  }

  invPad(&plain);

  // DEBUG
  std::cout << "Plain Blocks:" << std::endl;
  for (Block block : plain) {
    std::cout << "[";
    for (uint8_t byte : block) {
      std::cout << std::hex << +byte << ",";
    }
    std::cout << "]" << std::endl;
  }

  return blocksToText(plain);
}
