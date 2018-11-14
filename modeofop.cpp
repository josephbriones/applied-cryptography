#include "modeofop.h"

ModeOfOp::ModeOfOp(AES * aes, uint blockSize) :
  aes(aes),
  blockSize(blockSize) {}

void ModeOfOp::pad(const std::vector<Block>& blocks) {
  // TODO.
}

void ModeOfOp::invPad(const std::std::vector<Block>& blocks) {
  // TODO.
}
