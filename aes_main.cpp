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
  } else if (modeStr.compare("ofb") == 0) {
    mode = new OFBMode(4, numWordsInKey);
  } else if (modeStr.compare("ctr") == 0) {
    mode = new CTRMode(4, numWordsInKey);
  } else {
    std::cout << "[mode of op] must be one of \'cbc\', \'ofb\', or \'ctr\'."
              << std::endl;
    exit(EXIT_FAILURE);
  }

  std::string encryptedText = mode->encrypt(text);
  std::cout << "Encrypted Text: " << encryptedText << std::endl;
  std::string decryptedText = mode->decrypt(encryptedText);
  std::cout << "Decrypted Text: " << decryptedText << std::endl;

  delete mode;
}
