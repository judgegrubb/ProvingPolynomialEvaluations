#include "poly_eval.h"

#include <NTL/ZZ.h>
#include <NTL/ZZ_p.h>

using namespace NTL;

// input:
//    lambda => security parameter
//    d => bound on degree of polynomials to be proven
// output:
//    (pk, vk):
//        pk => proving key (p, g0, h0, ... gd, hd)
//        vk => verification key (p, g, s, a)
KeyPair KGen(int lambda, int d) {
  // choose random prime p of lamda bits such that q = (p - 1)/2 is also prime
  ZZ q = GenGermainPrime_ZZ(lambda);
  ZZ p = 2 * q + 1;
  
  // set ZZ_p for future uses
  ZZ_p::init(p);
  
  // Sample a random generator g in the subgroup of quadratic residues of ZZ_p*
  // i.e. sample w <- ZZ_p* and compute g <- w^2 mod p
  ZZ_p w = random_ZZ_p();
  ZZ_p g = sqr(w);

  // Sample random s <- ZZ_q* and alpha <- ZZ_q*
  
  // For i = 0 to d compute g_i <- g^(s^i)) mod p and h_i <- g^(alpha * s^i) mod p
  
  // pk <- (p, g_0, h_0, ..., g_d, h_d) and vk <- (p, g, s, alpha)
  
  // return (pk, sk)
  PublicKey pk;
  pk.p = p;
  VerifKey vk;
  KeyPair pkvk;
  pkvk.pk = pk;
  pkvk.vk = vk;
  return pkvk;
}

// input:
//    pk => proving key
//    f => univariate polynomial f(X) (coefficients in ZZ_q)
//    t => point in ZZ_q
// output:
//    pi => (pi_1, pi_2)
//    pi_1 => 
//    pi_2 =>
Proof P(PublicKey pk, int f, int t) {
  Proof pi;
  return pi;
}


// input:
//    vk => verification key
//    f => univariate polynomial f(X) (coefficients in ZZ_q)
//    t => point in ZZ_q
//    y => point in ZZ_q
//    pi => proof (pi_1, pi_2)
// output:
//    1 => accept proof
//    0 => reject proof
int V(VerifKey vk, int f, int t, int y, Proof pi) {
  return 0;
}
