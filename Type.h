/**
 * \file Type.h
 * \brief Definition of classes
 * \author L.Bouet - S.Bouchex - A.Antimes - C.Gouin
 * \version 1.0
 * \date 25 janvier 2018
 *
 * Definition of type class :
 *  -> geometry (meshing) : Point ; Edge ; Quad
 *  -> physical (résolution) : Point ; Maille
 *
 */

#include <vector>
#include <string>
#include "Dense"
#include <algorithm>

/*! \class Part
   * \brief Class representing a particle
   *
   *  This class stores and manages the properties of a particle (coordinates, velocity, reference)
   */

class Part
{
  private:
     // Coordonnees de la particule
     Eigen::Vector2d _coor;  /*!< Particule's coordinates*/
     // Vitesse de la particule
     Eigen::Vector3d _velo; /*!< Particule's velocity*/
     // Numéro de la maille
     int _ref; /*!< Particule's reference*/
  public:
    // Constructeur
    Part(Eigen::Vector2d coor,Eigen::Vector3d velo, int ref);
    /*!
     *  \brief Obtaining coordinates
     *
     *  This function lets you obtain the scalar coordinates of a particule.
     *
     *  \return Vector including the x and y components of the coordinates
     */
    Eigen::Vector2d Getcoor() {return _coor;};
    /*!
     *  \brief Obtaining velocity
     *
     *  This function lets you obtain the velocity of a particule.
     *
     *  \return Vector including the x, y and z components of the velocity
     */
    Eigen::Vector3d Getvelo() {return _velo;};
    /*!
     *  \brief Modifying coordinates
     *
     *  This function lets you modify the coordinates of a particule.
     *
     *  \param coor : new coordinates, gived to attribute _coor
     */
    void Modifycoor(Eigen::Vector2d coor) { _coor = coor;};
    /*!
     *  \brief Modifying velocity
     *
     *  This function lets you modify the velocity of a particule.
     *
     *  \param velo : new velocity, gived to attribute _velo
     */
    void Modifyvelo(Eigen::Vector3d velo) { _velo = velo;};
    /*!
     *  \brief Obtaining reference
     *
     *  This function lets you obtain the reference of a particule.
     *
     *  \return Int corresponding to the particule's reference
     */
    int Getref(){return _ref;};
    /*!
     *  \brief Modifying reference
     *
     *  This function lets you modify the reference of a particule.
     *
     *  \param ref : new reference, gived to attribute _ref
     */
    void Modifyref(int ref) {_ref = ref;};
};

/*! \class Maille
   * \brief classe representant une maille
   *
   *  La classe stocke et gère les propriétés d'une maille (densité, vitesse moyenne, température, surface, mailles voisines, référence)
   * ainsi que les particules présentes dans cette maille.
   */

class Maille
{
private:
   // Indices des particules présentes
   std::vector<int> _indices;
   // Propriétés : Densité Vitesse Température
   Eigen::Vector3d _prop;
   // Surface de la maille
   Eigen::Vector3d _average;
   // Surface de la maille
   double _surf;
   // Voisins de la maille
   Eigen::Vector4i _voisins;
   // Ref de la maille : 0 si elle est au centre du maillage, 1, 2, 3 ou 4 si
   // elle est sur un bord
   int _ref;
public:
  // Constructeur
  Maille(std::vector<int> indices,Eigen::Vector3d prop,Eigen::Vector3d average,double surf,Eigen::Vector4i voisins);
  // Récupérer indices
  std::vector<int>  Getindices() {return _indices;};
  // Récupère la densité
  double Getdensity() {return _prop[0];};
  //Récupère vitesse moyenne
  Eigen::Vector3d Getaverage() {return _average;};
  // Récupère la température
  double Gettemp() {return _prop[1];};
  // Récupère la Surface
  double Getsurf() {return _surf;};
  // Récupère les voisins
  Eigen::Vector4i Getvoisins() {return _voisins;};
  // Modifier densité
  void Modifydensity(double prop) { _prop[0] = prop;};
  // Modifier vitesse moyenne
  void Modifyaverage(Eigen::Vector3d average) { _average = average;};
  // Modifier température
  void Modifytemp(double prop) { _prop[1] = prop;};
  // Modifier la Surface
  void Modifysurf(double surf){_surf = surf;};
  // Modifier le ième voisin avec la ref de voisin
  // - /!\ i doit être compris entre 0 et 3 /!\ -
  void Modifyvoisins(int voisin, int i){_voisins[i] = voisin;};
  // Maj de la taille
  void Majindices(){_indices.resize(0);};
  // Modifier les indices des particules présentes dans la maille
  void Addindice(int indice){_indices.push_back(indice);};
  // Modifier Ref
  void Modifyref(int ref) {_ref = ref;};

  int Getref() {return _ref;};
};

class Quad
{
  private:
    // Indices des 4 sommets composant le quad
    Eigen::Vector4i _p;
    // Indices des 4 arêtes composant le quad
    Eigen::Vector4i _v;
    // Coord. des mileux des quad
    Eigen::Vector2d _center;
    //Norme des arêtes
    std::vector<Eigen::Vector2d> _normale;
    //les vecteurs milieu-edge milieu du quad
    std::vector<Eigen::Vector2d> _mid_norm;
    //milieu des edges
    std::vector<Eigen::Vector2d> _mid_ed;
    // Tag associe au quad
    int _ref;
  public:
    // Constructeur
    Quad(Eigen::Vector4i p, Eigen::Vector4i v, int ref);
    // Récupérer les sommets du quad
    Eigen::Vector4i Getquadp(){return _p;};
    // Récupérer les arêtes du quad
    Eigen::Vector4i Getquadv(){return _v;};
    // Modifie les coordonnées du centre
    void Modify_center(Eigen::Vector2d center){_center = center;};
    // Modifie les normales
    void Modify_normale(std::vector<Eigen::Vector2d> normale){_normale = normale;};
    // Modifie les vecteurs milieu-edge milieu du quad
    void Modify_mid_norm(std::vector<Eigen::Vector2d> mid_norm){_mid_norm = mid_norm;};

    void Modify_mid_ed(std::vector<Eigen::Vector2d> mid_ed){_mid_ed = mid_ed;};
    // Récupérer la référence du quad
    int Getref(){return _ref;};
    // Modifie les num des aretes du quad
    void Modifyv(int i, int j){_v[j] = i;};
    //Donne les coord. des centres des quadrilatères
    Eigen::Vector2d Getcenter(){return _center;};
    //Donne les normales des quadrilatères
    std::vector<Eigen::Vector2d> Getnormale(){return _normale;};
    //Donne les vecteurs milieu-edge milieu du quad
    std::vector<Eigen::Vector2d> Getmidnorm(){return _mid_norm;};
    //Donne le milieu des edges
    std::vector<Eigen::Vector2d> Getmiddle(){return _mid_ed;};
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
    // Modifie la référence
    void Modifyref(int i){_ref = i;};
};
