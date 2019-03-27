# A Protocol for Proving Polynomial Evaluations

Implementation of a coding challenge I'm working on for an internship position.
Implements a not-very-good verified computing protocol.

Fun opportunity to brush up on my C++ knowledge! 

Requires [Victor Shoup's Library for Number Theory](https://www.shoup.net/ntl/)
and all the prereqs involved there.

To run:
```
cd src
make
./main
```

To clean up executables:
```
cd src
make clean
```

Design decisions:
- Built as a very basic C++ library. View `poly_eval.cpp` for the used data
  structures.
- In order to avoid computationally complex exponentation, but retain valid
  exponents, many calculations `X`, s.t. `g^X mod p` was needed, were
  calculated as `X mod (p-1)` as per [Euler's Theorem and the Euler
  phi-function](https://math.stackexchange.com/questions/491576/simplifying-large-exponents-in-modular-arithmetic-like-1007-in-41007-pmod)
  (e.g. calculating `s^i` for `g^(s^i)`.)
