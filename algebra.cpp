#include <cmath>
#include <iostream>

#include "algebra.h"

uint8_t Algebra::xtimes(const uint8_t b) {
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

uint8_t Algebra::bytetimes(const uint8_t b1, const uint8_t b2) {
  // Simply call the internal version that uses bytePolys.
  return bytetimes(bytetopoly(b1), bytetopoly(b2));
}

uint8_t Algebra::sbox(const uint8_t b) {
  // First, calculate the multiplicative inverse of the byte b in GF(2^8) using
  // the Extended Euclidean algorithm for polynomials.
  bytePoly inv = bytetopoly(multinv(b));

  // Then apply the S-box affine transformation over GF(2).
  bytePoly affine;
  for (uint i = 0; i < inv.size(); ++i) {
    affine[i] = inv[i] ^ inv[(i + 4) % 8] ^ inv[(i + 5) % 8] ^ inv[(i + 6) % 8]
                ^ inv[(i + 7) % 8];
  }

  return polytobyte(affine) ^ 99;
}

uint8_t Algebra::invSbox(const uint8_t b) {
  // First, apply the inverse S-box affine transformation over GF(2).
  bytePoly p = bytetopoly(b);
  bytePoly affine;
  for (uint i = 0; i < affine.size(); ++i) {
    affine[i] = p[(i + 2) % 8] ^ p[(i + 5) % 8] ^ p[(i + 7) % 8];
  }
  uint8_t a = polytobyte(affine) ^ 5;

  // Then return the multiplicative inverse.
  return multinv(a);
}

Algebra::bytePoly Algebra::bytetopoly(uint8_t b) {
  bytePoly result;
  result.fill(0);
  for (uint i = 0; i < 8; ++i) {
    result[7 - i] = (b >= 128);
    b = b << 1;
  }

  return result;
}

uint8_t Algebra::polytobyte(const bytePoly& p) {
  uint8_t result = 0;
  for (uint i = 0; i < 8; ++i) {
    if (p[i]) {
      result += pow(2, i);
    }
  }

  return result;
}

Algebra::bytePoly Algebra::polytobytepoly(const poly& p) {
  bytePoly result;
  result.fill(0);
  for (uint i = 0; i < result.size(); ++i) {
    result[i] = p[i];
  }

  return result;
}

Algebra::poly Algebra::bytepolytopoly(const bytePoly& b) {
  poly result;
  result.assign(std::begin(b), std::end(b));

  return result;
}

Algebra::poly Algebra::irreducible() {
  return {1, 1, 0, 1, 1, 0, 0, 0, 1};
}

uint8_t Algebra::multinv(const uint8_t b) {
  if (b == 0 || b == 1) {
    return b;
  } else {
    poly lhs = irreducible();  // m(x).
    bytePoly rhs = bytetopoly(b);  // b(x).
    std::vector<bytePoly> quots;
    std::vector<bytePoly> rems;
    std::vector<bytePoly> auxs;
    auxs.push_back({0, 0, 0, 0, 0, 0, 0, 0});
    auxs.push_back({1, 0, 0, 0, 0, 0, 0, 0});

    do {
      // Compute the remainder and quotient of lhs / rhs.
      poly rhsP = bytepolytopoly(rhs);
      poly q, r;
      polydiv(lhs, rhsP, &q, &r);
      quots.push_back(polytobytepoly(q));
      rems.push_back(polytobytepoly(r));

      // Next, if we are beyond step 1, compute the auxiliary information.
      uint step = quots.size() - 1;
      if (step > 1) {
        bytePoly a = bytetopoly(polytobyte(auxs[step - 2])
                                ^ bytetimes(auxs[step - 1], quots[step - 2]));
        auxs.push_back(a);
      }

      // Prepare the next step.
      lhs.resize(8);  // Make the lhs 8 bits long.
      lhs.assign(std::begin(rhs), std::end(rhs));
      rhs = rems.back();
    } while (polytobyte(rhs) != 0);

    // Calculate the multiplicative inverse.
    const uint k = auxs.size() - 1;
    return polytobyte(auxs[k - 1]) ^ bytetimes(auxs[k], quots[k - 1]);
  }
}

Algebra::poly Algebra::polytimes(const poly& p1, const poly& p2) {
  // Multiply the polynomials together with 0/1 coefficients.
  std::vector<bool> prod(p1.size() + p2.size() - 1, 0);
  for (uint i = 0; i < p1.size(); ++i) {
    if (p1[i]) {
      for (uint j = 0; j < p2.size(); ++j) {
        if (p2[j]) {
          prod[i + j] = !prod[i + j];
        }
      }
    }
  }

  return prod;
}

void Algebra::polydiv(poly p1, const poly& p2, poly* q, poly* r) {
  q->assign(p1.size(), 0);  // Quotient.
  r->assign(p2.size(), 0);  // Remainder.

  // Get the degree of the divisor (p2), which doesn't change.
  uint deg2 = 0;
  for (uint i = 0; i < p2.size(); ++i) {
    if (p2[i]) {
      deg2 = i;
    }
  }

  // Check for boundary cases.
  if (deg2 == 0) {
    if (p2[0]) {  // The divisor is 1: set quotient (remainder is 0) and return.
      q->assign(std::begin(p1), std::end(p1));
      return;
    } else {  // The divisor is 0: fail without warning.
      exit(EXIT_FAILURE);
    }
  } else {  // No boundary cases, divisor > 1.
    while (1) {
      // Get the degree of the (remaining) dividend.
      uint deg1 = 0;
      for (uint i = 0; i < p1.size(); ++i) {
        if (p1[i]) {
          deg1 = i;
        }
      }

      // If the degree of the (remaining) dividend is smaller than that of the
      // divisor, then division is finished. Set the remainder before returning.
      if (deg1 < deg2) {
        r->assign(std::begin(p1), std::end(p1));
        return;
      } else {
        // Do a round of long division.
        q->at(deg1 - deg2) = 1;
        for (uint i = 0; i <= deg2; ++i) {
          if (p2[i]) {
            p1[deg1 - deg2 + i] = !p1[deg1 - deg2 + i];
          }
        }
      }
    }
  }
}

uint8_t Algebra::bytetimes(const bytePoly& b1, const bytePoly& b2) {
  // First, we need to make polynomial representations of the bytes.
  poly p1 = bytepolytopoly(b1);
  poly p2 = bytepolytopoly(b2);

  // Next, multiply them together.
  poly prod = polytimes(p1, p2);

  // Finally, use division to reduce the product modulo m(x).
  poly q, r;
  polydiv(prod, irreducible(), &q, &r);

  return polytobyte(polytobytepoly(r));
}

void Algebra::printByte(const bytePoly& b, const std::string name) {
  std::cout << name << " = [";
  for (bool bit : b) {
    std::cout << bit;
  }
  std::cout << "]" << std::endl;
}

void Algebra::printPoly(const poly& p, const std::string name) {
  std::cout << name << " = [";
  for (bool bit : p) {
    std::cout << bit;
  }
  std::cout << "]" << std::endl;
}
