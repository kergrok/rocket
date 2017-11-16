// normal_distribution
#include <iostream>
#include <string>
#include <random>

using namespace std;


double alea(double median, double sigma)
{

  random_device generator;
  normal_distribution<double> distribution(median,sigma);

  double number = distribution(generator);/* Nombre généré suivant loi normale (median,sigma)*/

  return number

}
