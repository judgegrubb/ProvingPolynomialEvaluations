#include "poly_eval.h"

#include <NTL/ZZ_pX.h>
#include <vector>

using namespace NTL;

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

  std::vector<ZZ_p> gList;
  std::vector<ZZ_p> hList;

  // For i = 0 to d compute g_i <- g^(s^i)) mod p and h_i <- g^(alpha * s^i) mod p
  ZZ_p::init(p);
  for (int i = 0; i <= d; i++) {
    ZZ spoweri = PowerMod(rep(s), i, p-1); // p is prime so phi(p) = p-1
    gList.push_back(power(g,spoweri)); // g_i <- g^(s^i)
    hList.push_back(power(g,rep(a)*spoweri)); // h_i <- g^(a * s^i)
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

Proof P(PublicKey pk, ZZ_pX f, ZZ_p t) {
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

int V(VerifKey vk, ZZ_pX f, ZZ_p t, ZZ_p y, Proof pi) {
  // exponents need to be mod p-1
  ZZ_p::init(vk.p-1);
  ZZ fofs = rep(eval(f,vk.s));
  
  // rest of calculations done mod p
  ZZ_p::init(vk.p);
  
  // h_f = g^(f(s)) mod p
  ZZ_p h_f = power(vk.g, fofs);

  // check pi_1^(s-t) == h_f * (g^(-y)) mod p and pi_1^(alpha) == pi_2
  if (power(pi.pi_1,rep(vk.s) - rep(t)) == h_f * power(vk.g, -1 * rep(y)) 
      && power(pi.pi_1, rep(vk.a)) == pi.pi_2) {
    // VERIFIED!
    return 1;
  }

  // Bad input :(
  return 0;
}
