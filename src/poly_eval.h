#ifndef _Polynomial_Evaluation_H
#define _Polynomial_Evaluation_H

#include <NTL/ZZ.h>
#include <NTL/ZZ_p.h>
#include <NTL/ZZ_pX.h>
#include <vector>

using namespace NTL;

struct PublicKey {
  ZZ p;
  std::vector<ZZ_p> gList; // (g_0, g_1, ... g_d)
  std::vector<ZZ_p> hList; // (h_0, h_1, ... h_d)
};

struct VerifKey {
  ZZ p;
  ZZ_p g;
  ZZ_p s;
  ZZ_p a;
};

struct KeyPair {
  PublicKey pk;
  VerifKey vk;
};

struct Proof {
  // pi_1
  // pi_2
};

// input:
//    lambda => security parameter
//    d => bound on degree of polynomials to be proven
// output:
//    (pk, vk):
//        pk => proving key (p, g0, h0, ... gd, hd)
//        vk => verification key (p, g, s, a)
KeyPair KGen(int lambda, long d);

// input:
//    pk => proving key
//    f => univariate polynomial f(X) (coefficients in ZZ_q)
//    t => point in ZZ_q
// output:
//    pi => (pi_1, pi_2)
//    pi_1 => 
//    pi_2 =>
Proof P(PublicKey pk, ZZ_pX f, ZZ_p t);

// input:
//    vk => verification key
//    f => univariate polynomial f(X) (coefficients in ZZ_q)
//    t => point in ZZ_q
//    y => point in ZZ_q
//    pi => proof (pi_1, pi_2)
// output:
//    1 => accept proof
//    0 => reject proof
int V(VerifKey vk, ZZ_pX f, ZZ_p t, ZZ_p y, Proof pi);

#endif
