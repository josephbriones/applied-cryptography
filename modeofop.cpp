#include <fstream>

#include "modeofop.h"

ModeOfOp::ModeOfOp(unsigned int numWordsInBlock, unsigned int numWordsInKey) :
  numWordsInBlock(numWordsInBlock) {
  key = randWords(numWordsInKey);
  aes = new AES(&key);
  loadIVs("data/used_ivs");
}

ModeOfOp::~ModeOfOp() {
  delete aes;
  saveIVs("data/used_ivs");
}

void ModeOfOp::loadBlocks(const std::string fname,
                          std::vector<Block> * blocks) {
  std::fstream blockReader(fname, std::fstream::in | std::fstream::binary);
  if (blockReader) {
    Block block;
    uint8_t byte = 0;

    // First, collect all complete blocks.
    while (blockReader.read(reinterpret_cast<char*>(&byte), sizeof(byte))) {
      block.push_back(byte);
      if (block.size() == 4 * numWordsInBlock) {  // 4 bytes = 1 word.
        blocks->push_back(block);
        block.clear();
      }
    }

    // Add the last, possibly incomplete block if it exists.
    if (!block.empty()) {
      blocks->push_back(block);
    }

    blockReader.close();
  }
}

void ModeOfOp::saveBlocks(const std::string fname,
                          const std::vector<Block> blocks) {
  std::fstream blockWriter(fname, std::fstream::out | std::fstream::binary |
                           std::fstream::trunc);
  if (blockWriter) {
   for (Block block : blocks) {
     for (uint8_t byte : block) {
       blockWriter.write(reinterpret_cast<char*>(&byte), sizeof(byte));
       if (blockWriter.bad()) {
         // Writing failed. Fail without warning.
         exit(EXIT_FAILURE);
       }
     }
   }

   blockWriter.close();
  } else {
    // Couldn't open file for writing. Fail without warning.
    exit(EXIT_FAILURE);
  }
}

void ModeOfOp::loadIVs(const std::string fname) {
  std::fstream IVreader(fname, std::fstream::in | std::fstream::binary);
  if (IVreader) {
    usedIVs.clear();
    Block usedIV;
    uint8_t byte = 0;
    while (IVreader.read(reinterpret_cast<char*>(&byte), sizeof(byte))) {
      usedIV.push_back(byte);
      if (usedIV.size() == 4 * numWordsInBlock) {  // 4 bytes = 1 word.
        usedIVs.insert(usedIV);
        usedIV.clear();
      }
    }

    IVreader.close();
  }
}

void ModeOfOp::saveIVs(const std::string fname) {
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
  IV = randBytes(4 * numWordsInBlock);
  usedIVs.insert(IV);
}

void ModeOfOp::uniqueIV(unsigned int numIVs) {
  while (1) {
    IV = randBytes(4 * numWordsInBlock);

    // Check if IV, IV+1, IV+2, ..., IV+(numIVs-1) have ever been used before.
    bool allUnique = true;
    std::vector<Block> candidates;
    Block candidate = IV;
    for (unsigned int i = 0; i < numIVs; ++i) {
      // If IV + i is in the used initialization vectors, break.
      if (usedIVs.find(candidate) != usedIVs.end()) {
        allUnique = false;
        break;
      } else {
        candidates.push_back(candidate);
        blockInc(&candidate);
      }
    }

    // If a unique range has been found, record them and finish.
    if (allUnique) {
      for (Block candidate : candidates) {
        usedIVs.insert(candidate);
      }
      break;
    }
  }
}

std::vector<ModeOfOp::Block> ModeOfOp::textToBlocks(std::string text) {
  std::vector<Block> blocks;
  Block block;

  // First, collect all complete blocks.
  for (unsigned int i = 0; i < text.length(); ++i) {
    block.push_back(*(reinterpret_cast<uint8_t*>(&text[i])));
    if (block.size() == 4 * numWordsInBlock) {
      blocks.push_back(block);
      block.clear();
    }
  }

  // Add the last, possibly incomplete block if it exists.
  if (!block.empty()) {
    blocks.push_back(block);
  }

  return blocks;
}

std::string ModeOfOp::blocksToText(const std::vector<ModeOfOp::Block> blocks) {
  std::string str = "";
  for (Block block : blocks) {
    for (uint8_t byte : block) {
      str += *(reinterpret_cast<char*>(&byte));
    }
  }

  return str;
}

void ModeOfOp::pad(std::vector<Block> * blocks) {
  Block lastBlock = blocks->back();

  // If the last block needs padding, pop it, pad it, and put it back. Otherwise
  // add a whole new block repeating the block size.
  if (lastBlock.size() < 4 * numWordsInBlock) {
    blocks->pop_back();
    uint8_t padAmount = (4 * numWordsInBlock) - lastBlock.size();
    for (unsigned int i = 0; i < padAmount; ++i) {
      lastBlock.push_back(padAmount);
    }
    blocks->push_back(lastBlock);
  } else {
    Block paddingBlock;
    for (unsigned int i = 0; i < 4 * numWordsInBlock; ++i) {
      uint8_t padAmount = 4 * numWordsInBlock;
      paddingBlock.push_back(padAmount);
    }
    blocks->push_back(paddingBlock);
  }
}

void ModeOfOp::invPad(std::vector<Block> * blocks) {
  Block lastBlock = blocks->back();
  uint8_t padAmount = lastBlock.back();
  uint8_t maxPadding = 4 * numWordsInBlock;

  // Remove the last block. If the padding amount was maximum, then the entire
  // last block was padding, so we're done. Otherwise, remove the padding from
  // the last block and put it back.
  blocks->pop_back();
  if (padAmount < maxPadding) {
    for (uint8_t i = 0; i < padAmount; ++i) {
      lastBlock.pop_back();
    }
    blocks->push_back(lastBlock);
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

void ModeOfOp::blockInc(Block * block) {
  // Starting at the least significant byte, add one. If the byte becomes 0,
  // carry the increment to the next most significant bit until either the
  // increment does not result in a carry-over or the entire block carries over.
  for (unsigned int i = 1; i <= block->size(); ++i) {
    unsigned int index = block->size() - i;
    block->at(index)++;
    if (block->at(index) != 0) {
      break;
    }
  }
}
