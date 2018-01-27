/**
 * \file alea.cpp
 * \author L.Bouet - S.Bouchex - A.Antimes - C.Gouin
 * \version 1.0
 * \date 25 janvier 2018
 *
 */

// normal_distribution
#include <iostream>
#include <string>
#include <random>
#include "Mesh.h"

using namespace std;


double Mesh::alea(double median, double sigma)
{

  random_device generator;
  normal_distribution<double> distribution(median,sigma);

  double number = distribution(generator);/* Nombre généré suivant loi normale (median,sigma)*/

  return number;

}
