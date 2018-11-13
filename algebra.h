// Utility functions for computing algebraic functions for AES.

#include <array>
#include <cmath>

/* Function signatures and documentation. */

// Given a byte (uint8_t) b expressing a polynomial b(x) in GF(2^8), this
// function returns a byte (uint8_t) expressing the polynomial x * b(x).
uint8_t xtimes(const uint8_t b);

// Given two bytes (uint8_t) b1 and b2 expressing polynomials b1(x) and b2(x) in
// GF(2^8), this function returns a byte (uint8_t) expressing the polynomial
// b1(x) * b2(x) mod m(x), where m(x) = x^8 + x^4 + x^3 + 1 is the irreducible
// polynomial for AES.
uint8_t bytetimes(const uint8_t b1, const uint8_t b2);

// TODO: word multiplication?

// Performs the S-Box substitution of (i) finding the multiplicative inverse and
// (ii) applying the AES affine transformation.
uint8_t sbox(const uint8_t b);

// Given a byte (uint8_t), this function returns an array of Boolean values
// representing the 0/1 coefficients [bi] of the corresponding polynomial b(x) =
// b7x^7 + b6x^6 + ... + b1x + b0.
std::array<bool, 8> bytetopoly(uint8_t b);

// Given an array of Boolean values presenting the 0/1 coefficients [bi] of a
// polynomial b(x) = b7x^7 + b6x^6 + ... + b1x + b0, this function returns the
// corresponding byte (uint8_t).
uint8_t polytobyte(std::array<bool, 8> p);


/* Function implementations. */

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

uint8_t bytetimes(const uint8_t b1, const uint8_t b2) {
  // First, we need to make polynomial representations of the bytes.
  std::array<bool, 8> p1 = bytetopoly(b1);
  std::array<bool, 8> p2 = bytetopoly(b2);

  // Multiply the polynomials together with 0/1 coefficients.
  std::array<bool, 15> prod;
  prod.fill(0);
  for (uint i = 0; i < p1.size(); ++i) {
    if (p1[i]) {
      for (uint j = 0; j < p2.size(); ++j) {
        if (p2[j]) {
          prod[i + j] = !prod[i + j];
        }
      }
    }
  }

  // Now reduce the following polynomial modulo m(x) = x^8 + x^4 + x^3 + x + 1,
  // the irreducible polynomial for AES. We're computing the remainder r in the
  // polynomial division d / m = q + r, where d = product.
  const std::array<bool, 9> m {1, 1, 0, 1, 1, 0, 0, 0, 1};
  std::array<bool, 15> d = prod;

  while (1) {
    // First, get the maximum degree of d with a 1 coefficient.
    uint maxdeg = 0;
    for (uint i = 0; i < d.size(); ++i) {
      if (d[i]) {
        maxdeg = i;
      }
    }

    // If the maximum degree of d with a 1 coefficient is < 8, then we've found
    // the remainder. Otherwise, do a round of long division.
    if (maxdeg < 8) {
      break;
    } else {
      for (uint i = 0; i < m.size(); ++i) {
        if (m[i]) {
          d[maxdeg - 8 + i] = !d[maxdeg - 8 + i];
        }
      }
    }
  }

  // Copy the 8 least-significant bits of d as the remainder r and return.
  std::array<bool, 8> r;
  for (uint i = 0; i < r.size(); ++i) {
    r[i] = d[i];
  }

  return polytobyte(r);
}

uint8_t sbox(const uint8_t b) {
  // First, calculate the multiplicative inverse of the byte b in GF(2^8).

  // Then apply the S-box affine transformation over GF(2).

  return 0;  // TODO: update.
}

std::array<bool, 8> bytetopoly(uint8_t b) {
  std::array<bool, 8> result;
  result.fill(0);
  for (uint i = 0; i < 8; ++i) {
    result[7 - i] = (b >= 128);
    b = b << 1;
  }

  return result;
}

uint8_t polytobyte(std::array<bool, 8> p) {
  uint8_t result = 0;
  for (uint i = 0; i < 8; ++i) {
    if (p[i]) {
      result += pow(2, i);
    }
  }

  return result;
}
