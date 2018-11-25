#include <cassert>


#include "aes.h"
#include "algebra.h"

AES::AES(std::vector<uint32_t> * key) :
  key(key) {
  assert(key->size() == 4 || key->size() == 6 || key->size() == 8);
}

std::vector<uint8_t> AES::encrypt(std::vector<uint8_t> plainBlock) {
  // Load the input block into AES's state.
  for (unsigned int row = 0; row < plainBlock.size() / 4; ++row) {
    for (unsigned int col = 0; col < plainBlock.size() / 4; ++col) {
      state[row][col] = plainBlock[row + 4*col];
    }
  }
 
  // Perform the forward AES cipher.
  addRoundKey(0);
  
  for (unsigned int round = 1; round < (key->size() + 6) - 1; ++round) {
    subBytes();
    shiftRows();
    mixColumns();
    addRoundKey(round);
  }
  subBytes();
  shiftRows();
  addRoundKey(key->size() + 6);

  // Unpack AES's state into the output block.
  std::vector<uint8_t> cipherBlock;
  for (unsigned int col = 0; col < 4; ++col) {
    for (unsigned int row = 0; row < 4; ++row) {
      cipherBlock.push_back(state[row][col]);
    }
  }

  return cipherBlock;
}

std::vector<uint8_t> AES::decrypt(std::vector<uint8_t> cipherBlock) {
  // Load the input block into AES's state.
  for (unsigned int row = 0; row < cipherBlock.size() / 4; ++row) {
    for (unsigned int col = 0; col < cipherBlock.size() / 4; ++col) {
      state[row][col] = cipherBlock[row + 4*col];
    }
  }

  // Perform the inverse AES cipher.
  addRoundKey(key->size() + 6);
  for (unsigned int round = (key->size() + 6) - 1; round > 0; --round) {
    invShiftRows();
    invSubBytes();
    addRoundKey(round);
    invMixColumns();
  }
  invShiftRows();
  invSubBytes();
  addRoundKey(0);

  // Unpack AES's state into the output block.
  std::vector<uint8_t> plainBlock;
  for (unsigned int col = 0; col < 4; ++col) {
    for (unsigned int row = 0; row < 4; ++row) {
      plainBlock.push_back(state[row][col]);
    }
  }

  return plainBlock;
}

void AES::subBytes() {
  for (unsigned int row = 0; row < 4; ++row) {
    for (unsigned int col = 0; col < 4; ++col) {
      state[row][col] = Algebra::sbox(state[row][col]);
    }
  }
}

void AES::invSubBytes() {
  for (unsigned int row = 0; row < 4; ++row) {
    for (unsigned int col = 0; col < 4; ++col) {
      state[row][col] = Algebra::invSbox(state[row][col]);
    }
  }
}

void AES::shiftRows() {
  for (unsigned int row = 0; row < 4; ++row) {
    uint8_t r[4] = {state[row][0], state[row][1], state[row][2], state[row][3]};

    // Cyclic shift each element 'row' elements to the left.
    for (unsigned int col = 0; col < 4; ++col) {
      state[row][col] = r[(col + row) % 4];
    }
  }
}

void AES::invShiftRows() {
  for (unsigned int row = 0; row < 4; ++row) {
    uint8_t r[4] = {state[row][0], state[row][1], state[row][2], state[row][3]};

    // Cyclic shift each element 'row' elements to the right.
    for (unsigned int col = 0; col < 4; ++col) {
      state[row][col] = r[(col - row + 4) % 4];
    }
  }
}

void AES::mixColumns() {
  for (unsigned int col = 0; col < 4; ++col) {
    // Copy the current column into a temporary array of bytes.
    uint8_t s[4] = {state[0][col], state[1][col], state[2][col], state[3][col]};
    
    // Multiply s(x) by a(x) = {03}x^3+ {01}x^2 + {01}x + {02}.
   
    state[0][col] = Algebra::bytetimes(0x02, s[0]) ^ Algebra::bytetimes(0x03, s[1])
                    ^ s[2] ^ s[3];
    state[1][col] = s[0] ^ Algebra::bytetimes(0x02, s[1])
                    ^ Algebra::bytetimes(0x03, s[2]) ^ s[3];
    state[2][col] = s[0] ^ s[1] ^ Algebra::bytetimes(0x02, s[2])
                    ^ Algebra::bytetimes(0x03, s[3]);
    state[3][col] = Algebra::bytetimes(0x03, s[0]) ^ s[1] ^ s[2]
                    ^ Algebra::bytetimes(0x02, s[3]);
  }
}

void AES::invMixColumns() {
  for (unsigned int col = 0; col < 4; ++col) {
    // Copy the current column into a temporary array of bytes.
    uint8_t s[4] = {state[0][col], state[1][col], state[2][col], state[3][col]};

    // Multiply s(x) by a-1(x) = {0b}x^3+ {0d}x^2+ {09}x + {0e}.
    state[0][col] = Algebra::bytetimes(0x0e, s[0]) ^ Algebra::bytetimes(0x0b, s[1])
                  ^ Algebra::bytetimes(0x0d, s[2]) ^ Algebra::bytetimes(0x09, s[3]);
    state[1][col] = Algebra::bytetimes(0x09, s[0]) ^ Algebra::bytetimes(0x0e, s[1])
                  ^ Algebra::bytetimes(0x0b, s[2]) ^ Algebra::bytetimes(0x0d, s[3]);
    state[2][col] = Algebra::bytetimes(0x0d, s[0]) ^ Algebra::bytetimes(0x09, s[1])
                  ^ Algebra::bytetimes(0x0e, s[2]) ^ Algebra::bytetimes(0x0b, s[3]);
    state[3][col] = Algebra::bytetimes(0x0b, s[0]) ^ Algebra::bytetimes(0x0d, s[1])
                  ^ Algebra::bytetimes(0x09, s[2]) ^ Algebra::bytetimes(0x0e, s[3]);
  }
}

void AES::addRoundKey(const unsigned int round) {
  // First, get the words in the key schedule for this round.
  std::array<uint32_t, 4> roundKeys = expandKey(round);

  for (unsigned int col = 0; col < 4; ++col) {
    // Reconstruct a word from the 4 column bytes.
    uint8_t b[4] = {state[0][col], state[1][col], state[2][col], state[3][col]};
    uint32_t colWord = b[3];
    for (unsigned int i = 1; i < 4; ++i) {
      colWord = (colWord << 8) ^ b[4 - (i + 1)];
    }

    // XOR the column with the corresponding round key.
    colWord = colWord ^ roundKeys[col];

    // Break the updated column into bytes and update the state.
    state[0][col] = (colWord & 0x000000ff);
    state[1][col] = (colWord & 0x0000ff00) >> 8;
    state[2][col] = (colWord & 0x00ff0000) >> 16;
    state[3][col] = (colWord & 0xff000000) >> 24;
  }
}

std::array<uint32_t, 4> AES::expandKey(const unsigned int round) {
  // Need to generate 4 * (round + 1) keys and return the last 4.
  std::vector<uint32_t> keySchedule;

  // First, load all the words from the key into the key schedule.
  for (unsigned int i = 0; i < key->size(); ++i) {
    keySchedule.push_back(key->at(i));
  }

  // Generate the rest of the keys.
  while (keySchedule.size() < 4 * (round + 1)) {
    uint32_t temp = keySchedule.back();
    if (keySchedule.size() % key->size() == 0) {
      temp = subWord(rotWord(temp)) ^ rcon(keySchedule.size() / key->size());
    } else if (key->size() > 6 && (keySchedule.size() % 4) == 0) {
      temp = subWord(temp);
    }

    keySchedule.push_back(keySchedule[keySchedule.size() - key->size()] ^ temp);
  }

  // Extract the last 4 keys and return them.
  std::array<uint32_t, 4> roundKeys;
  for (unsigned int i = 0; i < 4; ++i) {
    roundKeys[4 - (i + 1)] = keySchedule.back();
    keySchedule.pop_back();
  }

  return roundKeys;
}

uint32_t AES::subWord(const uint32_t w) {
  // Break the input word into its 4 bytes.
  uint8_t b[4];
  b[0] = (w & 0x000000ff);
  b[1] = (w & 0x0000ff00) >> 8;
  b[2] = (w & 0x00ff0000) >> 16;
  b[3] = (w & 0xff000000) >> 24;

  // Apply the S-box to each of the 4 bytes.
  for (unsigned int i = 0; i < 4; ++i) {
    b[i] = Algebra::sbox(b[i]);
  }

  // Reconstruct a word from the 4 bytes.
  uint32_t result = b[3];
  for (unsigned int i = 1; i < 4; ++i) {
    result = (result << 8) ^ b[4 - (i + 1)];
  }

  return result;
}

uint32_t AES::rotWord(const uint32_t w) {
  // XOR [0,0,0,a0] with [a1,a2,a3,0] to obtain the rotated word.
  return (w >> 24) ^ (w << 8);
}

uint32_t AES::rcon(const uint32_t r) {
  // Raise x (i.e., {02} = 2) to the power r - 1.
  uint8_t b = 2;
  for (unsigned int i = 1; i <= r - 1; ++i) {
    b = Algebra::xtimes(b);
  }

  uint32_t rcon = b;
  return rcon << 24;
}

