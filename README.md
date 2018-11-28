# applied-cryptography

This repository contains our implementation of the Advanced Encryption Standard (AES) for Arizona State University's Fall 2018 offering of CSE 539.


## Build and Run

Our implementation comes with a `makefile` that will build the project with the correct settings using the `make` command. After building, ensure that a `data` directory exists in the same directory as the project. Our AES implementation can be run using the following command:
```bash
$ ./aes_main [plaintext] [number of words in key] [mode of operation]
```

The supported number of words in the key are `4`, `6`, or `8`. The supported modes of operation are _Cipher Block Chaining_ (`'cbc'`), _Output Feedback_ (`'ofb'`), and _Counter_ (`'ctr'`). The resulting ciphertext is written to a binary file `data/cipher`.

A short test suite can be run using the `make run` command, which will encrypt and decrypt `"helloworldaescpp01234567891011"` using every combination of supported key lengths and modes of operation.
