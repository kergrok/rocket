#include <vector>
#include <string>
#include "Mesh.h"
#include "Dense"

using namespace std;
using namespace Eigen;

void Mesh::initialize()
{
  Buildvoisins();
  ReadParameter();
}

void Mesh::compute()
{

}
