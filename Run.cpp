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

Mesh::Displacement()
{
  Eigen::Vector2d new_coor;
  Eigen::Vector3d vitesse;
  bool in_domain;

  for (int i = 0; i < _part.size() ; i++) {

    new_coor = _part[i].Getcoor();
    coor = _part[i].Getcoor();
    vitesse = _part[i].Getvelo();
    new_coor[0]+=vitesse[0]*_dt;
    new_coor[1]+=vitesse[1]*_dt;
    _part[i].Modifycoor(new_coor);
    in_domain = Find_Maille(i);
    if(not(in_domain))
    {
      find_impact(i,coor,new_coor);
    }
  }
}


Mesh::find_impact(int i, Vector2d coor, Vector2d new_coor)
{

  double a1,c1,a2,c2;
  int ref_maille;
  Vector4i ref_edges;
  int impact_edge;
  double distance_a_parcourir,distance_parcourue,theta1,theta2,theta3;
  Vector2d coorS1,coorS2,coorImpact,vector_edge,vector_deplacement;
  Vector2i sommets;

  // Coefficients de la droite trajet de la particule
  a1=(coor[1]-new_coor[1])/(coor[0]-new_coor[0]);
  c1=coor[1]-coor[0]*a1;

  //Maille dans laquelle est la particule
  ref_maille = _part[i].Getref();
  // Numéro des aretes de la maille
  ref_edges = _mquad[ref_maille].Getquadv();

  // ATTENTION EN CAS DE 2 ARETES DE BORD----------------------------------------------------------------------------------
  // On cherche par quelle arete sort la particule
  for(int i=0,i<4,i++)
  {
    if( _medge[ref_edges[i]].Getref() == _border_tag_stay_in) // FAIRE BORDER_TAG
    {
      impact_edge = ref_edges[i];
    }
  }

  // Coordonnées des sommets de l'arete par laquelle sort la particule
  sommets = _medge[impact_edge].Getedge();
  coorS1 = _mpoint[sommets[0]].Getcoor();
  coorS2 = _mpoint[sommets[1]].Getcoor();

  //Coefficient de la droite passant par l'arete
  a2 = (coorS1[1]-coorS2[1])/(coorS1[0]-coorS2[0]);
  c2 = coorS1[1]-coorS1[0]*a2;

  // Coordonnées de l'impact
  coorImpact[0] = (c2-c1)/(a1-a2);
  coorImpact[1] = a1*coorImpact[0]+c1;

  //Vecteur représentant l'arete, IL VA DE LA GAUCHE VERS LA DROITE
  if(coorS2[0]<coorS1[0])
  {
    vector_edge[0]=coorS1[0]-coorS2[0];
    vector_edge[1]=coorS1[1]-coorS2[1];
  }
  else
  {
    vector_edge[0]=coorS2[0]-coorS1[0];
    vector_edge[1]=coorS2[1]-coorS1[1];
  }

  // Vecteur déplacement
  vector_deplacement[0]=coorImpact[0]-coor[0];
  vector_deplacement[1]=coorImpact[1]-coor[1];

  // Calcul de distances
  distance_a_parcourir=Norme_entre(new_coor,coor);
  distance_parcourue=Norme_entre(coor,coorImpact);


  // !!!!!!!!!!!!!!!!!!!!!!!!!!       VERIFICATION IMPERATIVE         !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


  // Angles: entre l'arete et la trajectoire, l'arete et l'horizontale, de la trajectoire après rebond

  // Ces trois lignes sont à vérifier proprement !!!!
  // Theta2 est forcément entre -pi/2 et pi/2, le vecteur allant de la gauche vers la droite

  theta1=acos(produit_scalaire(vector_deplacement,vector_edge)/Norme(vector_edge)/distance_parcourue);
  theta2=atan(vector_edge[1]/vector_edge[0]);
  theta3=theta1+theta2;

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!






  //Valeur absolue de la vitesse
  vector3d Velo, NewVelo;
  double VeloAbs;

  Velo=_part[i].Getvelo();
  VeloAbs=sqrt(Velo[0]**2+Velo[1]**2);

  //Calcul des nouvelles composantes de la vitesse
  NewVelo[0] = VeloAbs*cos(Theta3);
  NewVelo[1] = VeloAbs*sin(Theta3);
  NewVelo[2] = Velo[2];

  //Modification de la vitesse
  _part[i].Modifyvelo(NewVelo);

  //Calcul des nouvelles coordonnées
  new_coor[0]=coorImpact[0]+(distance_a_parcourir-distance_parcourue)*cos(Theta3);
  new_coor[1]=coorImpact[1]+(distance_a_parcourir-distance_parcourue)*sin(Theta3);

  // Modification des coordonnées
  _part[i].Modifycoor(new_coor);


}

double Norme_entre(Vector2d Vec1, Vector2d Vec2)
{
  return sqrt((Vec1[0]-Vec2[0])**2+(Vec1[1]-Vec2[1])**2);
}

double Norme(Vector2d Vec)
{
  return sqrt(Vec[0]**2+Vec[1]**2)
}

double produit_scalaire(Vector2d Vec1, Vector2d Vec2)
{
  return Vec1[1]*Vec2[1]+Vec1[0]*Vec2[0]
}
