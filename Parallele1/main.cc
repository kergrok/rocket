/**
 * \file main.cc
 * \brief Programme de simulation particulaire.
 * \author L.Bouet - S.Bouchex - A.Antimes - C.Gouin
 * \version 1.0
 * \date 25 janvier 2018
 *
 * Programme modélisant la rentrée atmosphérique d'une navette en milieu raréfié.
 *
 */

#include <iostream>

#include "Mesh.h"

int main()
{
  Mesh* mesh = new Mesh("test.mesh");
  mesh->readmesh();
  mesh->initialize();
  mesh->compute();
  return 0;
}
