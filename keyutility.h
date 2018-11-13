// TODO[Anya].

public class KeyUtility {
 public:
  KeyUtility();  // dev/rand things.

  // Returns a 4-byte word representing the key column used by AddRoundKey().
  uint32_t expandKey(const uint8_t[] key, uint wordinkey, uint index);

 private:
  // TODO: read about key storage in memory.
  union key {
    uint8_t[16] key128;
    uint8_t[24] key192;
    uint8_t[32] key256;
  }

  // Performs the S-box substitution on each of the bytes in the input word.
  // Would call sbox() 4 times.
  uint32_t subWord(const uint32_t w);

  // Takes a 4-byte word [a0,a1,a2,a3] and returns [a1,a2,a3,a0].
  uint32_t rotWord(const uint32_t w);

  // Returns a word [x^{n-1},{00},{00},{00}].
  uint32_t rcon(const uint32_t n);
};
