#include <vector>
#include <string>
#include "Mesh.h"
#include "Dense"

using namespace std;
using namespace Eigen;

void Mesh::initialize()
{
  ReadParameter();
  Buildvoisins();
  CFL();
  Create_in_Flow();
}

void Mesh::compute()
{
  double t;
  t=0.;
  while (t<_Temps_final)
  {
    Displacement();
    do (int i=0; i<_maille.size();i++)
    {
      Calc_prop(i);
    }
    Create_in_Flow(); 
    t+=_dt;
  }
}
