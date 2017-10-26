// normal_distribution
#include <iostream>
#include <string>
#include <random>

using namespace std;


int main()
{

  int nrolls=150000;
  double RT=8.314*200;
  double deuxpi=3.14159265358979*2;

  double sigma=RT;
  double median=0;


  random_device generator;
  normal_distribution<double> distribution(median,sigma);

  int p[10]={};
  double sum=0;
  for (int i=0; i<nrolls; ++i) {
    double number = distribution(generator);/* Nombre généré suivant loi normale (median,sigma)*/
    sum=sum+number*number;
  }

  sum=sum/nrolls/RT/RT;

  cout<<"sum="<<sum<<endl;/* Vérification que la loi est bien normale: Doit etre proche de 1 pour nrolls grand*/

  return 0;
}
