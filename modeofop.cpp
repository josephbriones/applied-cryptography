#include <fstream>

#include "modeofop.h"

ModeOfOp::ModeOfOp(uint numWordsInBlock, uint numWordsInKey) :
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

void ModeOfOp::uniqueIV(uint numIVs) {
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

void ModeOfOp::pad(const std::vector<Block>& blocks) {
  // TODO.
}

void ModeOfOp::invPad(const std::vector<Block>& blocks) {
  // TODO.
}

std::vector<uint8_t> ModeOfOp::randBytes(const uint numBytes) {
  std::vector<uint8_t> rBytes;

  // Open /dev/random and read words from it one at a time.
  std::fstream devrand("/dev/random", std::fstream::in | std::fstream::binary);
  if (devrand) {
    for (uint i = 0; i < numBytes; ++i) {
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

std::vector<uint32_t> ModeOfOp::randWords(const uint numWords) {
  std::vector<uint32_t> rWords;

  // Get 4 * #words random bytes, then collect them into words.
  std::vector<uint8_t> rBytes = randBytes(4 * numWords);
  for (uint i = 0; i < rBytes.size(); i += 4) {
    uint32_t rWord = 0;
    for (uint j = 0; j < 4; ++j) {
      rWord = rWord << 8;
      rWord += rBytes[i + j];
    }
    rWords.push_back(rWord);
  }

  return rWords;
}
