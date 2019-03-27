#include "poly_eval.h"

#include <NTL/ZZ_pX.h>
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
  for (int i = 0; i <= d; i++) {
    ZZ temp = PowerMod(rep(s), i, p-1);
    gList.push_back(power(g,temp)); // g_i <- g^(s^i)
    hList.push_back(power(g,rep(a)*temp)); // h_i <- g^(a * s^i)
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

  ZZ q = (pk.p - 1) / 2;

  ZZ_p::init(pk.p);
  ZZ_p pi_1;
  ZZ_p pi_2;
  set(pi_1);
  set(pi_2);

  // calculate psi
  // f(X) - f(t)
  ZZ_p::init(pk.p - 1);
  ZZ_p y = eval(f,t);
  ZZ_pX psi = f - y;
  // X - t
  ZZ_pX denom = ZZ_pX();
  SetX(denom);
  denom = denom - t;
  // f(X) - f(t) / X - t
  psi = psi / denom;

  //ZZ_p::init(pk.p);
  // calculate g^psi(s) and g^(a*psi(s)) from gList and hList
  ZZ_p::init(pk.p);
  for (int i = 0; i <= deg(psi); i++) {
    pi_1 = pi_1 * power(pk.gList.at(i), rep(coeff(psi,i)));
    pi_2 = pi_2 * power(pk.hList.at(i), rep(coeff(psi, i)));
  }

  Proof pi;
  pi.pi_1 = pi_1;
  pi.pi_2 = pi_2;
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
  ZZ_p::init(vk.p-1);

  ZZ temp = rep(eval(f,vk.s));
  ZZ_p::init(vk.p);
  ZZ_p h_f = power(vk.g, temp);

  if (power(pi.pi_1,rep(vk.s) - rep(t)) == h_f * power(vk.g, -1 * rep(y)) 
      && power(pi.pi_1, rep(vk.a)) == pi.pi_2) {
    return 1;
  }

  return 0;
}
