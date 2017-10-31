#include <iostream>
#include "Dense"
#include "Mesh.h"

int main()
{
  Mesh* mesh = new Mesh("test.mesh");
  mesh->readmesh();
  return 0;
}
