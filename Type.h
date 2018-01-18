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
     std::vector<double> _coor;  /*!< Particule's coordinates*/
     // Vitesse de la particule
     std::vector<double> _velo; /*!< Particule's velocity*/
     // Numéro de la maille
     int _ref; /*!< Particule's reference*/
  public:
    // Constructeur
    Part( std::vector<double> coor, std::vector<double> velo, int ref);
    /*!
     *  \brief Obtaining coordinates
     *
     *  This function lets you obtain the scalar coordinates of a particule.
     *
     *  \return Vector including the x and y components of the coordinates
     */
     std::vector<double> Getcoor() {return _coor;};
    /*!
     *  \brief Obtaining velocity
     *
     *  This function lets you obtain the velocity of a particule.
     *
     *  \return Vector including the x, y and z components of the velocity
     */
    std::vector<double> Getvelo() {return _velo;};
    /*!
     *  \brief Modifying coordinates
     *
     *  This function lets you modify the coordinates of a particule.
     *
     *  \param coor : new coordinates, gived to attribute _coor
     */
    void Modifycoor(std::vector<double> coor) { _coor = coor;};
    /*!
     *  \brief Modifying velocity
     *
     *  This function lets you modify the velocity of a particule.
     *
     *  \param velo : new velocity, gived to attribute _velo
     */
    void Modifyvelo(std::vector<double> velo) { _velo = velo;};
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
   std::vector<double> _prop;
   // Surface de la maille
   std::vector<double> _average;
   // Surface de la maille
   double _surf;
   // Voisins de la maille
   std::vector<int> _voisins;
   // Ref de la maille : 0 si elle est au centre du maillage, 1, 2, 3 ou 4 si
   // elle est sur un bord
   int _ref;
public:
  // Constructeur
  Maille(std::vector<int> indices,std::vector<double> prop,std::vector<double> average,double surf,std::vector<int> voisins);
  // Récupérer indices
  std::vector<int>  Getindices() {return _indices;};
  // Récupère la densité
  double Getdensity() {return _prop[0];};
  //Récupère vitesse moyenne
  std::vector<double> Getaverage() {return _average;};
  // Récupère la température
  double Gettemp() {return _prop[1];};
  // Récupère la Surface
  double Getsurf() {return _surf;};
  // Récupère les voisins
  std::vector<int> Getvoisins() {return _voisins;};
  // Modifier densité
  void Modifydensity(double prop) { _prop[0] = prop;};
  // Modifier vitesse moyenne
  void Modifyaverage(std::vector<double> average) { _average = average;};
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
    std::vector<int> _p;
    // Indices des 4 arêtes composant le quad
    std::vector<int> _v;
    // Coord. des mileux des quad
    std::vector<double> _center;
    //Norme des arêtes
    std::vector<std::vector<double>> _normale;
    //les vecteurs milieu-edge milieu du quad
    std::vector<std::vector<double>> _mid_norm;
    //milieu des edges
    std::vector<std::vector<double>> _mid_ed;
    // Tag associe au quad
    int _ref;
  public:
    // Constructeur
    Quad(std::vector<int> p, std::vector<int> v, int ref);
    // Récupérer les sommets du quad
    std::vector<int> Getquadp(){return _p;};
    // Récupérer les arêtes du quad
    std::vector<int> Getquadv(){return _v;};
    // Modifie les coordonnées du centre
    void Modify_center(std::vector<double> center){_center = center;};
    // Modifie les normales
    void Modify_normale(std::vector<std::vector<double>> normale){_normale = normale;};
    // Modifie les vecteurs milieu-edge milieu du quad
    void Modify_mid_norm(std::vector<std::vector<double>> mid_norm){_mid_norm = mid_norm;};

    void Modify_mid_ed(std::vector<std::vector<double>> mid_ed){_mid_ed = mid_ed;};
    // Récupérer la référence du quad
    int Getref(){return _ref;};
    // Modifie les num des aretes du quad
    void Modifyv(int i, int j){_v[j] = i;};
    //Donne les coord. des centres des quadrilatères
    std::vector<double> Getcenter(){return _center;};
    //Donne les normales des quadrilatères
    std::vector<std::vector<double>> Getnormale(){return _normale;};
    //Donne les vecteurs milieu-edge milieu du quad
    std::vector<std::vector<double>> Getmidnorm(){return _mid_norm;};
    //Donne le milieu des edges
    std::vector<std::vector<double>> Getmiddle(){return _mid_ed;};
};

class Point
{
  private:
     // Coordonnees du point ;
     std::vector<double> _coor;
     // Tag associe au point
     int _ref;
  public:
    // Constructeur
    Point(std::vector<double> coor, int ref);
    // Récupérer coor
    std::vector<double> Getcoor() {return _coor;};
    // Récupérer la référence du point
    int Getref(){return _ref;};
};

class Edge
{
  private:
    // Indices des 2 sommets composant l'arete
    std::vector<int> _v;
    // Tag associe a l'arete
    int _ref;
  public:
    // Constructeur
    Edge(std::vector<int> v, int ref);
    // Récupérer l'arête
    std::vector<int> Getedge(){return _v;};
    // Récupérer la référence de l'arête
    int Getref(){return _ref;};
    // Modifie la référence
    void Modifyref(int i){_ref = i;};
};
