// Utility functions for computing algebraic functions for AES.

#include <array>
#include <vector>

#include <string>  // DEBUG: Only for debug.

class Algebra {
 public:
  // Given a byte (uint8_t) b expressing a polynomial b(x) in GF(2^8), this
  // function returns a byte (uint8_t) expressing the polynomial x * b(x) mod
  // m(x), the irreducible polynomial for AES.
  static uint8_t xtimes(const uint8_t b);

  // Performs the S-box substitution by (i) finding the multiplicative inverse
  // and (ii) applying the AES affine transformation.
  static uint8_t sbox(const uint8_t b);

  // Performs the inverse S-box substitution by (i) applying the inverse AES
  // affine transformation and then (ii) finding the multiplicative inverse.
  static uint8_t invSbox(const uint8_t b);

 private:
  // Convenience typedefs for working with vectors and arrays as polynomials.
  typedef std::vector<bool> poly;
  typedef std::array<bool, 8> bytePoly;

  // Functions for transforming between a byte (uint8_t) b and its polynomial
  // representation b(x) = b7x^7 + b6x^6 + ... + b1x + b0. An array of Boolean
  // values storing the 0/1 coefficients [bi] is used to represent b(x).
  static bytePoly bytetopoly(uint8_t b);
  static uint8_t polytobyte(const bytePoly& p);

  // Functions for transforming between polys and bytePolys. Note that forming a
  // bytePoly from a poly only takes the first 8 bits.
  static bytePoly polytobytepoly(const poly& p);
  static poly bytepolytopoly(const bytePoly& b);

  // Function for retrieving the irreducible polynomial m(x) = x^8 + x^4 + x^3
  // + x + 1 as a polynomial (std::vector<bool>).
  static poly irreducible();

  // Given a byte (uint8_t) b expressing a polynomial b(x) in GF(2^8), this
  // function returns a byte (uint8_t) expressing the multiplicative inverse
  // b-1(x) of b(x) mod m(x), where m(x) is the irreducible polynomial for AES.
  static uint8_t multinv(const uint8_t b);

  // Abstract algebra utilities needed for computing the multiplicative inverse,
  // above. polytimes() takes two vectors of Boolean values representing the 0/1
  // coefficients of polynomials p1(x) and p2(x) and computes the 0/1
  // coefficients of p1(x) * p2(x). polydiv() takes p1(x) and p2(x) and computes
  // p1(x) / p2(x) = q(x) remainder r(x). bytetimes() is similar to polytimes(),
  // but its inputs are bytes b1 and b2 and it returns b1(x) * b2(x) mod m(x),
  // where m(x) is the irreducible polynomial for AES.
  static poly polytimes(const poly& p1, const poly& p2);
  static void polydiv(poly p1, const poly& p2, poly* q, poly* r);
  static uint8_t bytetimes(const bytePoly& b1, const bytePoly& b2);

  // DEBUG FUNCTIONS.
  static void printByte(const bytePoly& b, const std::string name);
  static void printPoly(const poly& p, const std::string name);
};
