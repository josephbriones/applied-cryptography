#include "modeofop.h"

ModeOfOp::ModeOfOp(uint numBytesInBlock, uint numWordsInKey) :
  numBytesInBlock(numBytesInBlock) {
  key = randWords(numWordsInKey);
  aes = new AES(&key);
}

void ModeOfOp::unpredictableIV() {
  // TODO.
}

void ModeOfOp::uniqueIV() {
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

std::vector<uint32_t> ModeOfOp::randWords(uint numWords) {
  // TODO: see below code.
}

/*int randomData = open("/dev/random", O_RDONLY);
if (randomData < 0)
{
  // something went wrong
  exit(EXIT_FAILURE);
}
else
{
  char myRandomData[50];
  size_t randomDataLen = 0;
  while (randomDataLen < sizeof myRandomData)
  {
      ssize_t result = read(randomData, myRandomData + randomDataLen, (sizeof myRandomData) - randomDataLen);
      if (result < 0)
      {
          // something went wrong
          exit(EXIT_FAILURE);
      }
      randomDataLen += result;
  }
  close(randomData);
}*/
