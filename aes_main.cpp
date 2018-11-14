// Main driver for AES under the various modes of operation.

// TODO: includes.
#include <iostream>
#include <string>

#include "aes.h"
#include "algebra.h"

int main(int argc, char* argv[]) {
  // Argument checking.
  if (argc != 4) {
    std::cout << "Usage: " << argv[0] << " [text to encrypt] [#words in key]"
              << " [mode of op]" << std::endl;
    exit(EXIT_FAILURE);
  }

  // Argument processing.
  std::string text = argv[1];
  uint nk = std::stoul(argv[2]);
  std::string modeStr = argv[3];

  // Instantiate the AES and mode of operation objects.
  AES * aes = new AES(nk);
  // TODO: instantiate ModeOfOp after deciding how inheritance will work.

  // TODO: encrypt the plaintext according to the mode of operation.
  // TODO: output the ciphertext.
  // TODO: decrypt the ciphertext according to the mode of operation.
  // TODO: output the plaintext.
}
