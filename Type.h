#include <vector>
#include <string>
#include "Dense"

class Part
{
  private:
     // Coordonnees de la particule
     Eigen::Vector2d _coor;
     // Vitesse de la particule
     Eigen::Vector3d _velo;
     // Numéro de la maille
     int _ref;
  public:
    // Constructeur
    Part(Eigen::Vector2d coor,Eigen::Vector3d velo, int ref);
    // Récupérer coor
    Eigen::Vector2d Getcoor() {return _coor;};
    // Récupère la vitesse
    Eigen::Vector3d Getvelo() {return _velo;};
    // Modifier coor
    void Modifycoor(Eigen::Vector2d coor) { _coor = coor;};
    // Modifier velo
    void Modifyvelo(Eigen::Vector3d velo) { _velo = velo;};
    // Récupérer la référence du point
    int Getref(){return _ref;};
    // Modifier Ref
    void Modifyref(int ref) {_ref = ref;};
};

/*******************  Stocke les particules présentes et les prop physiques de la maille  *******************/

class Maille
{
private:
   // Indices des particules présentent
   Eigen::VectorXi _indices;
   // Propriétés : Densité Vitesse Température
   Eigen::Vector3d _prop;
public:
  // Constructeur
  Maille(Eigen::VectorXi indices,Eigen::Vector3d prop);
  // Récupérer indices
  Eigen::VectorXi Getindices() {return _indices;};
  // Récupère la densité
  double Getdensity() {return _prop[0];};
  //Récupère vitesse moyenne
  double Getaverage() {return _prop[1];};
  // Récupère la température
  double Gettemp() {return _prop[2];};
  // Modifier densité
  void Modifydensity(double prop) { _prop[0] = prop;};
  // Modifier vitesse moyenne
  void Modifyaverage(double prop) { _prop[1] = prop;};
  // Modifier temp
  void Modifytemp(double prop) { _prop[2] = prop;};
};

class Quad
{
  private:
    // Indices des 4 sommets composant le quad
    Eigen::Vector4i _v;
    // Tag associe au quad
    int _ref;
  public:
    // Constructeur
    Quad(Eigen::Vector4i v, int ref);
    // Récupérer le quad
    Eigen::Vector4i Getquad(){return _v;};
    // Récupérer la référence du quad
    int Getref(){return _ref;};
};

class Point
{
  private:
     // Coordonnees du point ;
     Eigen::Vector2d _coor;
     // Tag associe au point
     int _ref;
  public:
    // Constructeur
    Point(Eigen::Vector2d coor, int ref);
    // Récupérer coor
    Eigen::Vector2d Getcoor() {return _coor;};
    // Récupérer la référence du point
    int Getref(){return _ref;};
};

class Edge
{
  private:
    // Indices des 2 sommets composant l'arete
    Eigen::Vector2i _v;
    // Tag associe a l'arete
    int _ref;
  public:
    // Constructeur
    Edge(Eigen::Vector2i v, int ref);
    // Récupérer l'arête
    Eigen::Vector2i Getedge(){return _v;};
    // Récupérer la référence de l'arête
    int Getref(){return _ref;};
};