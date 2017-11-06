#include <vector>
#include <string>
#include "Mesh.h"
#include "Dense"

using namespace std;
using namespace Eigen;

// ---------------- Calcul dt grâce à la "CFL" ------------------
// CFL définie par u * dt << dx
Mesh::CFL(double vit_moy)
{
  // Recherche de la plus petite arête pour définir le dx
  double dx = _lenghts[0];
  for (int i = 1; i < _lenghts.size() ; i++) {
    if (_lenghts[i] > dx) {
      dx = _lenghts[i];
    }
  }

  // Pour que la CFL soit vérifiée, on définit _dt comme suit :
  _dt = dx/(10. * vit_moy);
}


// Déplace toutes les particules pour un pas de temps
// Ne met à jour que la position, la mise à jour de la référence de la maille se fait dans une autre fonction
// En commentaire la fonction pour éffectuer la mise à jour sur plusieurs pas de temps

//Mesh::Displacement(double t_final)
Mesh::Displacement()
{
  //double t = _dt;
  Eigen::Vector2d new_coor;
  Eigen::Vector3d vitesse;

  //while (t < t_final) {
  for (int i = 0; i < _part.size() ; i++) {
    new_coor = _part[i].Getcoor();
    vitesse = _part[i].Getvelo();
    new_coor[0]+=vitesse[0]*_dt;
    new_coor[1]+=vitesse[1]*_dt;
    _part[i].Modifycoor(new_coor);
  }

    //_dt+=_dt;
  //}
}
