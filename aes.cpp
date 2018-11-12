/*
 Joseph notes for subByte() :

 // TODO: function for INVERSE sbox calculations. Same as sbox in algebra.h but using figure 14.

 subBytes() transformation is a non-linear byte substitution that operates independently
 on each byte of the State using a substitution table (S-box).

 State consists of four rows of bytes, each containing Nb bytes. each individual byte
 has two indices, with its row number r in the range 0 <= r < 4 and its column number c in the
 range 0 <= c < Nb. s[r, c]

 1. Take the multiplicative inverse in the finite field GF(2^(8)), described in Sec. 4.2; the
 element {00} is mapped to itself.

 Properties: b(x)a(x) + m(x)c(x) = 1
 a(x) • b(x) mod m(x) = 1,
 b^(-1)(x) = a(x) mod m(x) .

 m(t) = x^(8) + x^(4) + x^(3) + x +1,

 byte multInverse(byte b):
 1. byte -> polynomial representation
 2. extended Euclidean algorithm to solve a(x) & c(x)
 3. calculate b^(-1)(x) using a(x) mod m(x)
 4. polynomial representation -> byte
 5. return byte

 2. Apply affine transformation (over GF(2)): Computation in matrix form
 This is a function in algebra.h

 */
void subBytes(const State &state){
    for(int i = 0; i<state.size(); ++i){
        for(int j = 0; j<state[i].size(); ++j){
            state[i][j] = sbox(multInverse(state[i][j])); //multInverse -> sbox
        }
    }
}

void invSubBytes(const State &state){
    for(int i = 0; i<state.size(); ++i){
        for(int j = 0; j<state[i].size(); ++j){
            state[i][j] = multInverse(isbox(state[i][j])); //inverse sbox -> multInverse
        }
    }
}

/*
 Joseph notes for shiftRows() :

 shift(r, Nb) = r

 shiftRows():
     example: row = 2
     [1, 2, 3, 4] -> [3, 4, 1, 2]
     shifts all elements to the LEFT 2

 invShiftRows():
     example: row = 2
     [1, 2, 3, 4] -> [3, 4, 1, 2]
     shifts all elements to the RIGHT 2

 */
void shiftRows(const State &state)
{
    uint8_t n[4][4] = state; //need copy of state since cannot be done inplace
    for(int i = 0; i<state.size(); ++i){
        for(int j = 0; j<state[i].size(); ++j){
            n[i][j] = state[i][(j-i) % state[i].size()]; //(j-i) since shift LEFT
        }
    }
    state = n;
    n.clear(); //clears data in memory. may not be needed
}
void invShiftRows(const State &state)
{
    uint8_t n[4][4] = state; //need copy of state since cannot be done inplace
    for(int i = 0; i<state.size(); ++i){
        for(int j = 0; j<state[i].size(); ++j){
            n[i][j] = state[i][(j+i) % state[i].size()]; //(j+i) since shift RIGHT
        }
    }
    state = n;
    n.clear(); //clears data in memory. may not be needed
}
