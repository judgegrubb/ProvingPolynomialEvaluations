#include "poly_eval.h"
#include <NTL/ZZ.h>
#include <NTL/ZZ_p.h>
#include <NTL/ZZ_pX.h>

using namespace std;
using namespace NTL;

int main () {
  // test:
  
  // d <- 1000
  long d = 1000;
  
  // Compute (pk, vk) <- KGen(1024,d)
  KeyPair pkvk = KGen(1024, d);

  // get q back out
  ZZ q = (pkvk.pk.p - 1) / 2;

  // f's coefficients and our points are in ZZ_q
  ZZ_p::init(q);

  // Choose a random (univariate) polynomial f of degree d
  ZZ_pX f = random_ZZ_pX(d+1);
  
  // Choose a random point t <- ZZ_q (get from pk?)
  ZZ_p t = random_ZZ_p();

  // Compute y <- f(t) mod q
  ZZ_p y = eval(f, t);

  // Run pi <- P(pk, f, t)
  Proof pi = P(pkvk.pk, f, t);

  // Run b <- V(vk, f, t, f(t), pi)
  int b = V(pkvk.vk, f, t, y, pi);
  cout << b << "\n";
  
  return 0;
}
