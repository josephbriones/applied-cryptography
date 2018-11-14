// Utility functions for computing algebraic functions for AES.

#include <array>
#include <cmath>
#include <vector>

/* Function signatures and documentation. */

// Given an array of Boolean values representing the 0/1 coefficients [pi] of a
// polynomial p(x) = pnx^n + p(n-1)x^(n-1) + ... + p1x + p0, this function
// returns p(x) mod m(x) = x^8 + x^4 + x^3 + 1 (the irreducible polynomial for
// AES) as a byte (uint8_t).
uint8_t reducemodm(std::vector<bool> p);

// Given a byte (uint8_t) b expressing a polynomial b(x) in GF(2^8), this
// function returns a byte (uint8_t) expressing the polynomial x * b(x).
uint8_t xtimes(const uint8_t b);

// Given two vectors of Boolean values representing the 0/1 coefficients of
// polynomials p1(x) and p2(x), this function returns the 0/1 coefficients outf
// p1(x) * p2(x).
std::vector<bool> polytimes(const std::vector<bool> p1,
                            const std::vector<bool> p2);

// Given two bytes (uint8_t) b1 and b2 expressing polynomials b1(x) and b2(x) in
// GF(2^8), this function returns a byte (uint8_t) expressing the polynomial
// b1(x) * b2(x) mod m(x), where m(x) is the irreducible polynomial for AES.
uint8_t bytetimes(const uint8_t b1, const uint8_t b2);

// TODO: word multiplication?

// Given a byte (uint8_t) b expressing a polynomial b(x) in GF(2^8), this
// function returns a byte (uint8_t) expressing the multiplicative inverse
// b-1(x) of b(x) mod m(x), where m(x) is the irreducible polynomial for AES.
uint8_t multinvmodm(const uint8_t b);

// Performs the S-Box substitution of (i) finding the multiplicative inverse and
// (ii) applying the AES affine transformation.
uint8_t sbox(const uint8_t b);

// Given a byte (uint8_t), this function returns an array of Boolean values
// representing the 0/1 coefficients [bi] of the corresponding polynomial b(x) =
// b7x^7 + b6x^6 + ... + b1x + b0.
std::array<bool, 8> bytetopoly(uint8_t b);

// Given an array of Boolean values representing the 0/1 coefficients [bi] of a
// polynomial b(x) = b7x^7 + b6x^6 + ... + b1x + b0, this function returns the
// corresponding byte (uint8_t).
uint8_t polytobyte(std::array<bool, 8> p);


/* Function implementations. */

uint8_t reducemodm(std::vector<bool> p) {
  const std::array<bool, 9> m {1, 1, 0, 1, 1, 0, 0, 0, 1};

  while (1) {
    // First, get the maximum degree of p with a 1 coefficient.
    uint maxdeg = 0;
    for (uint i = 0; i < p.size(); ++i) {
      maxdeg = p[i] ? i : maxdeg;
    }

    // If the maximum degree of p with a 1 coefficient is < 8, then we've found
    // the remainder. Otherwise, do a round of long division.
    if (maxdeg < 8) {
      break;
    } else {
      for (uint i = 0; i < m.size(); ++i) {
        if (m[i]) {
          p[maxdeg - 8 + i] = !p[maxdeg - 8 + i];
        }
      }
    }
  }

  // Copy the 8 least-significant bits of p as the remainder r and return.
  std::array<bool, 8> r;
  for (uint i = 0; i < r.size(); ++i) {
    r[i] = p[i];
  }
  return polytobyte(r);
}

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

std::vector<bool> polytimes(const std::vector<bool> p1,
                            const std::vector<bool> p2) {
  std::cout << std::endl << "p1 = [";
  for (bool bit : p1) {
    std::cout << bit;
  }
  std::cout << "]" << std::endl;
  std::cout << "p2 = [";
  for (bool bit : p2) {
    std::cout << bit;
  }
  std::cout << "]" << std::endl;

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

  std::cout << "prod = [";
  for (bool bit : prod) {
    std::cout << bit;
  }
  std::cout << "]" << std::endl << std::endl;

  return prod;
}

uint8_t bytetimes(const uint8_t b1, const uint8_t b2) {
  // First, we need to make polynomial representations of the bytes.
  std::array<bool, 8> temp1 = bytetopoly(b1);
  std::array<bool, 8> temp2 = bytetopoly(b2);

  std::vector<bool> p1;
  p1.assign(std::begin(temp1), std::end(temp1));
  std::vector<bool> p2;
  p2.assign(std::begin(temp2), std::end(temp2));

  // Next, multiply them together.
  std::vector<bool> prod = polytimes(p1, p2);

  // Now reduce the product modulo m(x), the irreducible polynomial for AES.
  return reducemodm(prod);
}

uint8_t multinvmodm(const uint8_t b) {
  if (b == 0) {
    return 0;
  } else {
    std::vector<bool> lhs = {1, 1, 0, 1, 1, 0, 0, 0, 1};  // m(x).
    std::array<bool, 8> rhs = bytetopoly(b);  // b(x).
    std::vector<std::array<bool, 8>> quots;
    std::vector<std::array<bool, 8>> rems;
    std::vector<std::array<bool, 8>> auxs;
    auxs.push_back({0, 0, 0, 0, 0, 0, 0, 0});
    auxs.push_back({1, 0, 0, 0, 0, 0, 0, 0});

    // DEBUG:
    std::cout << "lhs = [";
    for (bool bit : lhs) {
      std::cout << bit;
    }
    std::cout << "]" << std::endl;
    std::cout << "rhs = [";
    for (bool& bit : rhs) {
      std::cout << bit;
    }
    std::cout << "]" << std::endl << std::endl;

    std::array<bool, 8> q;
    q.fill(0);
    std::array<bool, 8> r;
    r.fill(0);
    std::array<bool, 8> a;
    a.fill(0);
    do {
      // Get the maximum degree of the divisor.
      uint maxdeg = 0;
      for (uint i = 0; i < rhs.size(); ++i) {
        maxdeg = rhs[i] ? i : maxdeg;
      }

      // DEBUG:
      std::cout << "maxdeg = " << maxdeg << std::endl;

      // Compute the quotient of lhs / rhs.
      for (uint i = 0; i < lhs.size(); ++i) {
        if (lhs[i] && (i >= maxdeg)) {
          q[i - maxdeg] = !q[i - maxdeg];
        }
      }
      quots.push_back(q);

      // DEBUG:
      std::cout << "q = [";
      for (bool& bit : q) {
        std::cout << bit;
      }
      std::cout << "]" << std::endl;

      q.fill(0);

      // Compute the remainder r = lhs - (q * rhs).
      std::vector<bool> qVec;
      qVec.assign(std::begin(quots.back()), std::end(quots.back()));
      std::vector<bool> rhsVec;
      rhsVec.assign(std::begin(rhs), std::end(rhs));
      auto prod = polytimes(qVec, rhsVec);

      // DEBUG:
      std::cout << "prod = [";
      for (bool bit : prod) {
        std::cout << bit;
      }
      std::cout << "]" << std::endl;

      for (uint i = 0; i < r.size(); ++i) {
        r[i] = lhs[i] ^ prod[i];
      }
      rems.push_back(r);

      // DEBUG:
      std::cout << "r = [";
      for (bool& bit : r) {
        std::cout << bit;
      }
      std::cout << "]" << std::endl;

      r.fill(0);

      // Next, if we are beyond step 1, compute the auxiliary information.
      // BOOKMARK: pick up here, with fixing the multiplication below.
      uint step = quots.size() - 1;
      if (step > 1) {
        a = bytetopoly(polytobyte(auxs[step - 2]) ^
                       bytetimes(polytobyte(auxs[step - 1]),
                                 polytobyte(quots[step - 2])));
        auxs.push_back(a);
        a.fill(0);
      }

      // Prepare the next step.
      lhs.resize(8);  // Make the lhs 8 bits long.
      lhs.assign(std::begin(rhs), std::end(rhs));

      std::cout << "lhs = [";
      for (bool bit : lhs) {
        std::cout << bit;
      }
      std::cout << "]" << std::endl;

      rhs = rems.back();

      std::cout << "rhs = [";
      for (bool& bit : rhs) {
        std::cout << bit;
      }
      std::cout << "]" << std::endl;

      break;  // DEBUG;
    } while (polytobyte(rhs) != 0);

    // Calculate the multiplicative inverse.
    const uint k = auxs.size();
    return polytobyte(auxs[k - 1])
           ^ bytetimes(polytobyte(auxs[k]), polytobyte(quots[k - 1]));
  }
}

uint8_t sbox(const uint8_t b) {
  // First, calculate the multiplicative inverse of the byte b in GF(2^8) using
  // the Extended Euclidean algorithm for polynomials.
  auto inv = bytetopoly(multinvmodm(b));

  // Then apply the S-box affine transformation over GF(2).
  std::array<bool, 8> affine;
  for (uint i = 0; i < inv.size(); ++i) {
    affine[i] = inv[i] ^ inv[(i + 4) % 8] ^ inv[(i + 5) % 8] ^ inv[(i + 6) % 8]
                ^ inv[(i + 7) % 8];
  }

  return polytobyte(affine) ^ 99;
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
