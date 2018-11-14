class AES {
 public:
  typedef State uint8_t[4][4];

  // TODO.
  AES(uint nk);

  // TODO.
  encrypt();
  decrypt();

 private:
  State state;
  KeyUtility keyutil;

  // TODO[Joseph].
  void subBytes();

  // TODO[Joseph].
  void shiftRows();

  // TODO[Joseph]: This is going to need a temp that we'll need to delete().
  void mixColumns();

  // TODO[Anya]: calls expandKey() and xor() to XOR the current key schedule with the
  // columns of the state.
  void addRoundKey(const uint round);

  // TODO[Joseph].
  void invShiftRows();

  // TODO[Joseph].
  void invSubBytes();

  // TODO[Joseph].
  void invMixColumns();
};
