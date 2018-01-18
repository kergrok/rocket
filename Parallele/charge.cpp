#include "Mesh.h"
#include <fstream>
#include <iostream>
#include <assert.h>

using namespace std;
using namespace Eigen;


vector<int> Mesh::charge(int me, int Np)
{
  vector<int> sortie(2);
  int N = _part.size();
  int l, r;

  l = N/Np;
  r = N%Np;

  if (me < r) {
    sortie[0] = me*l+me;
    sortie[1] = sortie[0]+l;
  }
  else {
    sortie[0] = me*l+r;
    sortie[1] = sortie[0]+l;
  }
  return sortie;
}
