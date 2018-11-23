#include <fstream>

#include "modeofop.h"

ModeOfOp::ModeOfOp(unsigned int numWordsInBlock, unsigned int numWordsInKey) :
  numWordsInBlock(numWordsInBlock) {
  key = randWords(numWordsInKey);
  aes = new AES(&key);
  loadIVs("data/old_ivs");
}

ModeOfOp::~ModeOfOp() {
  delete aes;
  saveIVs("data/old_ivs");
}

void ModeOfOp::loadIVs(std::string fname) {
  std::fstream IVreader(fname, std::fstream::in | std::fstream::binary);
  if (IVreader) {
    usedIVs.clear();
    Block usedIV;
    uint8_t byte = 0;
    while (IVreader.read(reinterpret_cast<char*>(&byte), sizeof(byte))) {
      usedIV.push_back(byte);
      if (usedIV.size() == 4 * numWordsInBlock) {  // 4 bytes = 1 word.
        usedIVs.push_back(usedIV);
        usedIV.clear();
      }
    }

    IVreader.close();
  } else {
    // Couldn't open file for reading. Fail without warning.
    exit(EXIT_FAILURE);
  }
}

void ModeOfOp::saveIVs(std::string fname) {
  std::fstream IVwriter(fname, std::fstream::out | std::fstream::binary |
                               std::fstream::trunc);
  if (IVwriter) {
    for (Block usedIV : usedIVs) {
      for (uint8_t byte : usedIV) {
        IVwriter.write(reinterpret_cast<char*>(&byte), sizeof(byte));
        if (IVwriter.bad()) {
          // Writing failed. Fail without warning.
          exit(EXIT_FAILURE);
        }
      }
    }

    IVwriter.close();
  } else {
    // Couldn't open file for writing. Fail without warning.
    exit(EXIT_FAILURE);
  }
}

void ModeOfOp::unpredictableIV() {
  // TODO: add this to the list of used IVs.
  IV = randBytes(4 * numWordsInBlock);
}

void ModeOfOp::uniqueIV(unsigned int numIVs) {
  // TODO.
}

std::vector<ModeOfOp::Block> ModeOfOp::textToBlocks(const std::string text) {
  std::vector<Block> blocks;

  // TODO.

  return blocks;
}

std::string ModeOfOp::blocksToText(const std::vector<ModeOfOp::Block>& blocks) {
  std::string str = "";
  for (Block block : blocks) {
    // TODO.
  }

  return str;
}

void ModeOfOp::pad(std::vector<Block>& blocks) {
  Block& lastBlock = blocks[blocks.size()-1];
  if(lastBlock.size() < 16){
    for(unsigned int i = lastBlock.size(); i<16; ++i){
      lastBlock.push_back(16-lastBlock.size());
    }
  }
  else{
    Block paddingBlock;
    for(unsigned int i = 0; i<16; ++i){
      paddingBlock.push_back(0x10);
    }
    blocks.push_back(paddingBlock);
  }
}

void ModeOfOp::invPad(std::vector<Block>& blocks) {
  Block& lastBlock = blocks[blocks.size()-1];
  int paddingLength = lastBlock[lastBlock.size()-1];
  for(unsigned int i = 0; i<paddingLength; ++i){
    lastBlock.pop_back();
  }
}

std::vector<uint8_t> ModeOfOp::randBytes(const unsigned int numBytes) {
  std::vector<uint8_t> rBytes;

  // Open /dev/random and read words from it one at a time.
  std::fstream devrand("/dev/random", std::fstream::in | std::fstream::binary);
  if (devrand) {
    for (unsigned int i = 0; i < numBytes; ++i) {
      uint8_t rByte = 0;
      devrand.read(reinterpret_cast<char*>(&rByte), sizeof(rByte));
      if (devrand) {
        // Reading succeeded. Add the new random word to the list.
        rBytes.push_back(rByte);
      } else {
        // Reading failed. Fail without warning.
        exit(EXIT_FAILURE);
      }
    }

    devrand.close();
  } else {
    // Couldn't open /dev/random for reading. Fail without warning.
    exit(EXIT_FAILURE);
  }

  return rBytes;
}

std::vector<uint32_t> ModeOfOp::randWords(const unsigned int numWords) {
  std::vector<uint32_t> rWords;

  // Get 4 * #words random bytes, then collect them into words.
  std::vector<uint8_t> rBytes = randBytes(4 * numWords);
  for (unsigned int i = 0; i < rBytes.size(); i += 4) {
    uint32_t rWord = 0;
    for (unsigned int j = 0; j < 4; ++j) {
      rWord = rWord << 8;
      rWord += rBytes[i + j];
    }
    rWords.push_back(rWord);
  }

  return rWords;
}
