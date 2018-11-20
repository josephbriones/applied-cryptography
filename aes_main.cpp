// Main driver for AES under the various modes of operation.

// TODO: includes.
#include <iostream>
#include <string>

#include "cbcmode.h"
#include "ctrmode.h"
#include "ofbmode.h"

int main(int argc, char* argv[]) {
  // Argument checking.
  if (argc != 4) {
    std::cout << "Usage: " << argv[0] << " [text to encrypt] [#words in key]"
              << " [mode of op]" << std::endl;
    exit(EXIT_FAILURE);
  }

  // Argument processing.
  std::string text = argv[1];
  uint numWordsInKey = std::stoul(argv[2]);
  std::string modeStr = argv[3];

  // Instantiate the AES and mode of operation objects.
  ModeOfOp * mode;
  if (modeStr.compare("cbc") == 0) {
    mode = new CBCMode(4, numWordsInKey);
  } else if (modeStr.compare("ofb") == 0) {
    mode = new OFBMode(4, numWordsInKey);
  } else if (modeStr.compare("ctr") == 0) {
    mode = new CTRMode(4, numWordsInKey);
  } else {
    std::cout << "[mode of op] must be one of \'cbc\', \'ofb\', or \'ctr\'."
              << std::endl;
    exit(EXIT_FAILURE);
  }

  // TODO: encrypt the plaintext according to the mode of operation.
  // TODO: output the ciphertext.
  // TODO: decrypt the ciphertext according to the mode of operation.
  // TODO: output the plaintext.

  // TODO: do garbage collection.
}
