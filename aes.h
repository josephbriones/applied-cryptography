class AES {
 public:
  typedef State uint8_t[4][4];

  // TODO.
  AES();

  // TODO.
  encrypt(KeyUtility * keyutil);
  decrypt(KeyUtility * keyutil);

 private:
  State state;

  // TODO: Joseph.
  void subBytes(const State &state);

  // TODO: Joseph.
  void shiftRows(const State &state);

  // TODO[Josh]: This is going to need a temp that we'll need to delete().
  void mixColumns(const State &state);

  // TODO[Anya]: calls expandKey() and xor() to XOR the current key schedule with the
  // columns of the state.
  void addRoundKey(const State &state, const uint round);

  // TODO: Joseph.
  void invShiftRows(const State &state);

  // TODO: Joseph.
  void invSubBytes(const State &state);

  // TODO: Josh.
  void invMixColumns(const State &state);
};
