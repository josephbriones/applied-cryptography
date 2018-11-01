// TODO[Josh].

// TODO: Addition operations for bits, bytes, and words.
bit xor(const bit a, const bit b);
byte xor(const byte a, const byte b);
word xor(const word a, const word b);

// TODO.
// TODO: do some kind of polynomial multiplication, both for bytes and words.

// Returns x ({02}) times b in GF(2^8).
byte xtimes(const byte b);

// Performs the affine transformation for the S-Box.
byte sbox(const byte in);
