#include "poly_eval.h"

#include <NTL/ZZ_p.h>
#include <vector>

using namespace NTL;

// input:
//    lambda => security parameter
//    d => bound on degree of polynomials to be proven
// output:
//    (pk, vk):
//        pk => proving key (p, g0, h0, ... gd, hd)
//        vk => verification key (p, g, s, a)
KeyPair KGen(int lambda, long d) {
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
  ZZ_p::init(q);
  ZZ_p s = random_ZZ_p();
  ZZ_p a = random_ZZ_p();

  ZZ_p::init(p);

  std::vector<ZZ_p> gList;
  std::vector<ZZ_p> hList;

  // For i = 0 to d compute g_i <- g^(s^i)) mod p and h_i <- g^(alpha * s^i) mod p
  for (int i = 0; i < d; i++) {
    gList.push_back(power(g,rep(power(s,i)))); // g_i <- g^(s^i)
    hList.push_back(power(g,rep(a*power(s,i)))); // h_i <- g^(a * s^i)
  }

  // pk <- (p, gList, hList) and vk <- (p, g, s, alpha)
  PublicKey pk { p, gList, hList};
  VerifKey vk = { p, g, s, a};
  
  // return (pk, sk)
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
Proof P(PublicKey pk, ZZ_pX f, ZZ_p t) {
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
int V(VerifKey vk, ZZ_pX f, ZZ_p t, ZZ_p y, Proof pi) {
  ZZ_p::init(vk.p);
  ZZ_p h_f = power(vk.g, rep(eval(f,vk.s)));
 
  ZZ_p neg_y;
  negate(neg_y, y);

  if (power(pi.pi_1,rep(vk.s - t)) == h_f * power(vk.g, rep(neg_y)) 
      && power(pi.pi_1, rep(vk.a)) == pi.pi_2) {
    return 1;
  }

  return 0;
}
