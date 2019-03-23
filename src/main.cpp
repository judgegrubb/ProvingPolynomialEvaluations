#include "poly_eval.h"
#include <NTL/ZZ.h>
#include <NTL/ZZ_p.h>

using namespace std;
using namespace NTL;

int main () {
  // test:
  
  // d <- 1000
  int d = 1000;
  
  // Compute (pk, vk) <- KGen(1024,d)
  KeyPair pkvk = KGen(1024, d);

  // Choose a random (univariate) polynomial f of degree d
  
  // Choose a random point t <- ZZ_q (get from pk?)
  
  // Compute y <- f(t) mod q


  // Run pi <- P(pk, f, t)
  Proof pi = P(pkvk.pk, 0, 0);

  // Run b <- V(vk, f, t, f(t), pi)
  int b = V(pkvk.vk, 0, 0, 0, pi);
  cout << b << "\n";
  
  return 0;
}
