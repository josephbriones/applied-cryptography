#include <cassert>
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
  unsigned int numWordsInKey = std::stoul(argv[2]);
  std::string modeStr = argv[3];

  // Instantiate the AES and mode of operation objects.
  ModeOfOp * mode;
  if (modeStr.compare("cbc") == 0) {
    mode = new CBCMode(4, numWordsInKey);
    std::cout << "Using CBC Mode." << std::endl;
  } else if (modeStr.compare("ofb") == 0) {
    mode = new OFBMode(4, numWordsInKey);
    std::cout << "Using OFB Mode." << std::endl;
  } else if (modeStr.compare("ctr") == 0) {
    mode = new CTRMode(4, numWordsInKey);
    std::cout << "Using CTR Mode." << std::endl;
  } else {
    std::cout << "[mode of op] must be one of \'cbc\', \'ofb\', or \'ctr\'."
              << std::endl;
    exit(EXIT_FAILURE);
  }

  std::cout << "Encrypting..." << std::endl;
  mode->encrypt(text);
  std::cout << "Decrypting..." << std::endl;
  std::string plainText = mode->decrypt();

  assert(text.compare(plainText) == 0);
  std::cout << "Original text and decrypted plaintext match.\n" << std::endl;

  delete mode;
}
