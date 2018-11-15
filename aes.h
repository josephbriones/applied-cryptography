// Main AES driver class that encrypts/decrypts from input

class AES {
  public:

  // AES algorithm’s operations are performed on a two-dimensional array of bytes 
  // called the State. State consists of four rows with 4 columns of bytes.
  typedef uint8_t State [4][4];

  // Constructor for AES. Given an unsigned int nk, representing the length of the
  // encryption key, this function creates an AES object that can be used by various
  // modes of operations.
  AES(unsigned int nk);

  // Performs the encrption on the initial State that is initialized by the
  // mode of operation. Calls a series of operations that manipulate the internals
  // of a State.
  void encrypt();

  // Performs decryption of final State after perviously being encrypted. Calls
  // a series of operations that performs operations to un-do pervious encryption
  // of the interal of State. 
  void decrypt();

 private:

  // Initialization of State
  State state;

  // References to utility classes to call needed functions. 
  KeyUtility keyutil;
  Algebra algebra;
  
  // Storage of the length of encrpytion key
  unsigned int nk;

  // The SubBytes() transformation is a non-linear byte substitution that operates 
  // independently on each byte of the State using a substitution table (S-box).
  void subBytes();

  // ShiftRows() transformation, the bytes in the last three rows of the State are
  // cyclically shifted over different numbers of bytes (offsets). The offset 
  // shifts each row LEFT.
  void shiftRows();

  // MixColumns() transformation operates on the State column-by-column, treating 
  // each column as a four-term polynomial, s(x). This polynomial is multiplied
  // modulo x^4 + 1 by a(x) = {03}x^3+ {01}x^2 + {01}x + {02}.
  void mixColumns();

  // TODO[Anya]: calls expandKey() and xor() to XOR the current key schedule with the
  // columns of the state.
  void addRoundKey(const unsigned int round);

  // invShiftRows() is inverse of the ShiftRows() transformation.
  void invShiftRows();

  // InvSubBytes() is the inverse of the byte substitution transformation, in 
  // which the inverse Sbox is applied to each byte of the State..
  void invSubBytes();

  // InvMixColumns() is the inverse of the MixColumns(). 
  // a-1(x) = {0b}x^3+ {0d}x^2+ {09}x + {0e}.
  void invMixColumns();

};
