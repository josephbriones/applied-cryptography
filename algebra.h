// Utility functions for computing algebraic functions for AES.

// Function signatures.

// TODO.
// TODO: do some kind of polynomial multiplication, both for bytes and words.

// Given a byte (uint8_t) b expressing a polynomial b(x) in GF(2^8), this
// function returns a byte (uint8_t) expressing the polynomial x * b(x).
uint8_t xtimes(const uint8_t b);

// Performs the affine transformation for the S-Box.
uint8_t sbox(const uint8_t in);


// Function implementations.

uint8_t xtimes(const uint8_t b) {
  uint8_t m = 27;
  uint8_t result;

  // If the most-significant bit of b is a 1, then multiplying by x will make
  // this polynomial too large for a byte (uint8_t). So it must be reduced using
  // the irreducible polynomial for AES, m(x) = x^8 + x^4 + x^3 + 1.
  if (b >= 128) {
    result = (b << 1) ^ m;  // Multiply by x and reduce by m.
  } else {
    result = b << 1;  // Multiply by x.
  }

  return result;
}
