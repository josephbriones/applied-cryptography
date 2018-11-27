#ifndef AES_H
#define AES_H

#include <array>
#include <vector>

class AES {
 public:
  // Constructor which takes as input a pointer to the encryption key (vector of
  // bytes).
  AES(std::vector<uint32_t> * key);

  // Given a block (i.e., a vector of 16 bytes) of plaintext, this function
  // loads the block into the state matrix and performs AES encryption.
  std::vector<uint8_t> encrypt(std::vector<uint8_t> plainBlock);

  // Given a block (i.e., a vector of 16 bytes) of ciphertext, this function
  // loads the block into the state matrix and performs AES decryption. Inverse
  // of encrypt().
  std::vector<uint8_t> decrypt(std::vector<uint8_t> cipherBlock);

 private:
  // Convenience typedef for the AES algorithm’s state, which is a two-
  // dimensional (4x4) array of bytes.
  typedef uint8_t State[4][4];

  // Properties.
  State state;                  // The 4x4 byte state matrix.
  std::vector<uint32_t> * key;  // The 4, 6, or 8 word key.

  // Performs a nonlinear byte substitution of each byte of the state
  // individually using AES's S-box.
  void subBytes();

  // Performs a nonlinear byte substitution of each byte of the state
  // individually using AES's inverse S-box. Inverse of subBytes().
  void invSubBytes();

  // Performs a cyclic left-shift on each row of the state independently. Row i
  // is cyclically shifted i bytes to the left, for i in {0, 1, 2, 3}.
  void shiftRows();

  // Performs a cyclic right-shift on each row of the state independently. Row i
  // is cyclically shifted i bytes to the right, for i in {0, 1, 2, 3}. Inverse
  // of shiftRows().
  void invShiftRows();

  // Performs a transformation on each column of the state independently. Each
  // column is treated as a degree 4 polynomial s(x) over GF(2^8) with
  // coefficients also over GF(2^8). Replaces each column with s(x) * a(x) mod
  // x^4 + 1, where a(x) = {03}x^3+ {01}x^2 + {01}x + {02}.
  void mixColumns();

  // Performs a transformation on each column of the state independently. Each
  // column is treated as a degree 4 polynomial s(x) over GF(2^8) with
  // coefficients also over GF(2^8). Replaces each column with s(x) * a-1(x) mod
  // x^4 + 1, where a-1(x) = {0b}x^3+ {0d}x^2+ {09}x + {0e}. Inverse of
  // mixColumns().
  void invMixColumns();

  // Performs a linear transformation on each column of the state independently.
  // In every round, for each column c in {0, 1, 2, 3}, a new key schedule word
  // w(4 * round + c) is obtained and XORed with column c.
  void addRoundKey(const unsigned int round);

  // Performs key expansion, returning the key schedule words used in
  // addRoundKey() for the given round.
  std::array<uint32_t, 4> expandKey(const unsigned int round);

  // Like subBytes(), this function performs a nonlinear substitution of each of
  // the given word's 4 bytes individually by applying AES's S-box.
  uint32_t subWord(const uint32_t w);

  // Like shiftRows(), this funtion performs a 1-step cyclic left-shift on the
  // given word, transforming [a0,a1,a2,a3] into [a1,a2,a3,a0].
  uint32_t rotWord(const uint32_t w);

  // Computes the round constant word. Given a round number r, this function
  // returns a word [x^(r-1),{00},{00},{00}].
  uint32_t rcon(const uint32_t r);
};

#endif  // AES_H
