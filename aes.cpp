#include "aes.h"
#include "algebra.h"
#include "keyutility.h"

AES::AES(unsigned int nk)
{
  //keyutil = new KeyUtility(nk);
}

void AES::encrypt(){
    // Cipher encryption scheme reference in Figure 5 of standard documentation
    addRoundKey(0);
    for(int i = 1; i <(nk+6)-1; i++){
        subBytes();
        shiftRows();
        mixColumns();
        addRoundKey(i);
    }
    subBytes();
    shiftRows();
    addRoundKey(nk+6);
}
void AES::decrypt(){
    // Cipher decryption scheme reference in Figure 12 of standard documentation
    addRoundKey(nk+6);
    for(int i = (nk+6)-1; i > 0; i++){
        invShiftRows();
        invSubBytes();
        addRoundKey(i);
        invMixColumns();
    }
    invSubBytes();
    invShiftRows();
    addRoundKey(0);
}

void AES::subBytes() {
    // Perform a independent byte substitution using the substitution table (S-Box)
    for(int i = 0; i<4; ++i){
        for(int j = 0; j<4; ++j){
            state[i][j] = algebra.sbox(state[i][j]); 
        }
    }
}

void AES::invSubBytes() {
    // Perform a independent byte substitution using the inverse substitution 
    // table (inverse S-Box).
    for(int i = 0; i<4; ++i){
        for(int j = 0; j<4; ++j){
            state[i][j] = algebra.invSbox(state[i][j]); 
        }
    }
}

void AES::shiftRows() {
    // Shift each row left cyclically by an offset equal to row number.
    for(int i = 0; i<4; ++i){
        uint8_t n[4] = {state[i][0], state[i][1], state[i][2], state[i][3]};
        for(int j = 0; j<4; ++j){
            state[i][j] = n[j+i % 4]; //(j+i) since shift LEFT
        }
    }
}

void AES::invShiftRows() {
    // Shift each row right cyclically by an offset equal to row number.
    for(int i = 0; i<4; ++i){
        uint8_t n[4] = {state[i][0], state[i][1], state[i][2], state[i][3]};
        for(int j = 0; j<4; ++j){
            state[i][j] = n[abs(j+i) % 4]; //(j-i) since shift RIGHT
        }
    }
}
void AES::mixColumns() {
    // Perform column-by-column computation through polynomial multiplication.
    // See Section 5.6
    for(int i = 0; i<4; ++i){
        uint8_t n[4] = {state[i][0], state[i][1], state[i][2], state[i][3]};
        // Each hexadecimal was precomputed into int
        state[i][0] = algebra.bytetimes(n[0], 2) ^ algebra.bytetimes(n[1], 3) ^ n[2] ^ n[3];
        state[i][1] = n[0] ^ algebra.bytetimes(n[1], 2) ^ algebra.bytetimes(n[2], 3) ^ n[3];
        state[i][2] = n[0] ^ n[1] ^ algebra.bytetimes(n[2], 2) ^ algebra.bytetimes(n[3], 3);
        state[i][3] = algebra.bytetimes(n[0], 3) ^ n[1] ^ n[2] ^ algebra.bytetimes(n[3], 2);
    }
}

void AES::invMixColumns(){
    // Perform column-by-column computation through polynomial multiplication.
    // See Section 5.10
    for(int i = 0; i<4; ++i){
        uint8_t n[4] = {state[i][0], state[i][1], state[i][2], state[i][3]};
        // Each hexadecimal was precomputed into int
        state[i][0] = algebra.bytetimes(n[0], 14) ^ algebra.bytetimes(n[1], 11) ^ algebra.bytetimes(n[2], 13) ^ algebra.bytetimes(n[3], 9);
        state[i][1] = algebra.bytetimes(n[0], 9) ^ algebra.bytetimes(n[1], 14) ^ algebra.bytetimes(n[2], 11) ^ algebra.bytetimes(n[3], 13);
        state[i][2] = algebra.bytetimes(n[0], 13) ^ algebra.bytetimes(n[1], 9) ^ algebra.bytetimes(n[2], 14) ^ algebra.bytetimes(n[3], 11);
        state[i][3] = algebra.bytetimes(n[0], 11) ^ algebra.bytetimes(n[1], 13) ^ algebra.bytetimes(n[2], 9) ^ algebra.bytetimes(n[3], 14);
    }
}

void AES::addRoundKey(const uint round) {
  KeyUtility k;
  //how to access the union key
  uint8_t n[4][4] = state;
  for(int i = 0; i<state.size(); ++i){
    for(int j = 0; j<state[i].size(); ++j){
      //discuss the arguments to pass in expandKey
      uint32_t wtemp = k.expandKey()
      //how do you xor the word with a column
      uint8_t b[4];
      b[0] = (wtemp & 0x000000ff);
      b[1] = (wtemp & 0x0000ff00) >> 8;
      b[2] = (wtemp & 0x00ff0000) >> 16;
      b[3] = (wtemp & 0xff000000) >> 24;
      n[i][j] = state[i][j] ^ b[j];
    }
  }
  state = n;
  n.clear();
}
