// TODO[Anya].

public class KeyUtility {
 public:
  KeyUtility();  // dev/rand things.

  // Returns a 4-byte word representing the key column used by AddRoundKey().
  word expandKey(const char[] key, uint roundNum);

 private:
  // TODO: read about key storage in memory.
  union key {
    char[128] key128;
    char[192] key192;
    char[256] key256;
  }

  // Performs the S-box substitution on each of the bytes in the input word.
  // Would call sbox() 4 times.
  word subWord(const word w);

  // Takes a 4-byte word [a0,a1,a2,a3] and returns [a1,a2,a3,a0].
  word rotWord(const word w);

  // Returns a word [x^{n-1},{00},{00},{00}].
  word rcon(const uint n);
};
