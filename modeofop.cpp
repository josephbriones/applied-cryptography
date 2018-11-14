#include "modeofop.h"

ModeOfOp::ModeOfOp(AES * aes, uint blockSize) :
  aes(aes),
  blockSize(blockSize) {}

ModeOfOp::Block ModeOfOp::unpredictableIV() {
  // TODO.
}

ModeOfOp::Block ModeOfOp::uniqueIV() {
  // TODO.
}

void ModeOfOp::pad(const std::vector<Block>& blocks) {
  // TODO.
}

void ModeOfOp::invPad(const std::vector<Block>& blocks) {
  // TODO.
}
