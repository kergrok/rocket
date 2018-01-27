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
     *  \brief Obtains coordinates
     *
     *  This function lets you obtain the scalar coordinates of a particule.
     *
     *  \return Vector including the x and y components of the coordinates
     */
     std::vector<double> Getcoor() {return _coor;};
    /*!
     *  \brief Obtains velocity
     *
     *  \return Vector including the x, y and z components of the velocity
     */
    std::vector<double> Getvelo() {return _velo;};
    /*!
     *  \brief Modifies coordinates of the particle
     *
     *  \param coor : new coordinates, gived to attribute _coor
     */
    void Modifycoor(std::vector<double> coor) { _coor = coor;};
    /*!
     *  \brief Modifies velocity of the particle
     *
     *  \param velo : new velocity, gived to attribute _velo
     */
    void Modifyvelo(std::vector<double> velo) { _velo = velo;};
    /*!
     *  \brief Obtains reference
     *
     *  \return Integer corresponding to the reference of the particle
     */
    int Getref(){return _ref;};
    /*!
     *  \brief Modifies reference
     *
     *  \param ref : new reference of the particle, gived to attribute _ref
     */
    void Modifyref(int ref) {_ref = ref;};
};

/*! \class Maille
   * \brief Class representing a mesh
   *
   *  This class stores and manages the properties of a mesh (density, average velocity, temperature, surface, neighbour meshes, reference)
   * as well as particles presented in this mesh.
   */

class Maille
{
private:
   // Indices des particules présentes
   std::vector<int> _indices;/*!< Particule indices present in the mesh*/
   // Propriétés : Densité Vitesse Température
   std::vector<double> _prop;/*!< Mesh properties (density, average velocity, temperature)*/
   // Surface de la maille
   std::vector<double> _average;/*!< Mesh average velocity*/
   // Surface de la maille
   double _surf;/*!< The surface of the mesh*/
   // Voisins de la maille
   std::vector<int> _voisins;/*!< The neighbours of the mesh*/
   // Ref de la maille : 0 si elle est au centre du maillage, 1, 2, 3 ou 4 si
   // elle est sur un bord
   int _ref;
public:
  // Constructeur
  Maille(std::vector<int> indices,std::vector<double> prop,std::vector<double> average,double surf,std::vector<int> voisins);
  // Récupérer indices
  /*!
   *  \brief Obtains reference of the particles in the mesh
   *
   *  \return Vector containing integers corresponding to each references of the particles
   */
  std::vector<int>  Getindices() {return _indices;};
  // Récupère la densité
  /*!
   *  \brief Obtains density in the mesh
   *
   *  \return Double corresponding to the mesh particule density
   */
  double Getdensity() {return _prop[0];};
  //Récupère vitesse moyenne
  /*!
   *  \brief Obtains average velocity
   *
   *  \return Vector of 3 double corresponding to the average velocity of the mesh (in cartesian)
   */
  std::vector<double> Getaverage() {return _average;};
  // Récupère la température
  /*!
   *  \brief Obtains temperature
   *
   *  \return Double corresponding to the temperature of the mesh
   */
  double Gettemp() {return _prop[1];};
  // Récupère la Surface
  /*!
   *  \brief Obtains surface
   *
   *  \return Double corresponding to the surface of the mesh
   */
  double Getsurf() {return _surf;};
  // Récupère les voisins
  /*!
   *  \brief Obtains neighbours
   *
   *  \return Vector of 4 integer, corresponding to the 4 neighbours of the mesh
   */
  std::vector<int> Getvoisins() {return _voisins;};
  // Modifier densité
  /*!
   *  \brief Modifies the density
   *
   *  \param prop : double which is the new density, gived to attribute _prop[0]
   */
  void Modifydensity(double prop) { _prop[0] = prop;};
  // Modifier vitesse moyenne
  /*!
   *  \brief Modifies the average velocity
   *
   *  \param average : new average velocity (vector of 3 doubles), gived to attribute _average
   */
  void Modifyaverage(std::vector<double> average) { _average = average;};
  // Modifier température
  /*!
   *  \brief Modifies the temperature
   *
   *  \param prop : new temperature (double), gived to attribute _prop[1]
   */
  void Modifytemp(double prop) { _prop[1] = prop;};
  // Modifier la Surface
  /*!
   *  \brief Modifies the surface
   *
   *  \param surf : new surface (double), gived to attribute _surf
   */
  void Modifysurf(double surf){_surf = surf;};
  // Modifier le ième voisin avec la ref de voisin
  // - /!\ i doit être compris entre 0 et 3 /!\ -
  /*!
   *  \brief Modify a mesh's neighbour
   *
   *  \param voisin : new neighbour (integer), gived to attribute _voisins
   *  \param i : numerous of the neighbour (0 for the first one found, 3 for the last one, has to be between 0 and 3)
   */
  void Modifyvoisins(int voisin, int i){_voisins[i] = voisin;};
  // Maj de la taille
  /*!
   *  \brief Updates _indices size
   *
   *  This function changes the size of the attribute vector _indices to 0.
   *
   */
  void Majindices(){_indices.resize(0);};
  // Modifier les indices des particules présentes dans la maille
  /*!
   *  \brief Adds a particle in _indices
   *
   *  This function adds the indice of a particule in the class mesh
   *
   *  \param indice : new indice (integer), added to attribute vector _indices
   */
  void Addindice(int indice){_indices.push_back(indice);};
  // Modifier Ref
  /*!
   *  \brief Modifies reference
   *
   *  \param ref : new reference (integer) of the mesh, gived to attribute _ref
   */
  void Modifyref(int ref) {_ref = ref;};
  /*!
   *  \brief Obtains reference
   *
   *  \return Integer corresponding to the reference of the mesh
   */
  int Getref() {return _ref;};
};

/*! \class Quad
   * \brief Class representing a quadrilateral
   *
   *  This class stores and manages a quadrilateral of the mesh
   */
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
    /*!
     *  \brief Returns the summets of the quadrilateral
     *
     *  \return Vector of 4 integers containing the references (from the class Point) of the summets
     */
    std::vector<int> Getquadp(){return _p;};
    // Récupérer les arêtes du quad
      /*!
     *  \brief Returns the egdes of the quadrilateral
     *
     *  \return Vector of 4 integers containing the references (from the class Edge) of the edges
     */
    std::vector<int> Getquadv(){return _v;};
    // Modifie les coordonnées du centre
      /*!
     *  \brief Modifies the center of the quadrilateral
     *
     *  \param center : new vector containing the scalar coordinates of the center
     */
    void Modify_center(std::vector<double> center){_center = center;};
    // Modifie les normales
    /*!
   *  \brief Modifies the normales of the each egdes of the quadrilateral
   *
   *  \param Normale : new vector of 4 vectors of double, each containing the normale of an egde
   */
    void Modify_normale(std::vector<std::vector<double>> normale){_normale = normale;};
    // Modifie les vecteurs milieu-edge milieu du quad
    void Modify_mid_norm(std::vector<std::vector<double>> mid_norm){_mid_norm = mid_norm;};
    /*!
   *  \brief Modifies the center of the each egdes of the quadrilateral
   *
   *  \param mid_ed : Vector of 4 vectors of double, each containing the coordinates of the center of an egde
   */
    void Modify_mid_ed(std::vector<std::vector<double>> mid_ed){_mid_ed = mid_ed;};
    // Récupérer la référence du quad
      /*!
     *  \brief Gets the reference of the quadrilateral
     *
     *  \return Integer corresponding to the reference
     */
    int Getref(){return _ref;};
    // Modifie les num des aretes du quad
    /*!
   *  \brief Modifies the references of the edge present in the quadrilateral
   *
   *  \param i : integer corresponding to the new reference of the egde
   *  \param j : numerous of the edge (0 for the first one found, 3 for the last one, has to be between 0 and 3)
   */
    void Modifyv(int i, int j){_v[j] = i;};
    //Donne les coord. des centres des quadrilatères
    /*!
     *  \brief Returns the center coordinates of the quadrilateral
     *
     *  \return Vector of 2 doubles containing the Cartesian coordinates of the center
     */
    std::vector<double> Getcenter(){return _center;};
    //Donne les normales des quadrilatères
    /*!
   *  \brief Returns the normales of the each egdes of the quadrilateral
   *
   *  \return Vector of 4 vectors of double, each containing the normale of an egde
   */
    std::vector<std::vector<double>> Getnormale(){return _normale;};
    //Donne les vecteurs milieu-edge milieu du quad
    std::vector<std::vector<double>> Getmidnorm(){return _mid_norm;};
    //Donne le milieu des edges
    /*!
   *  \brief Returns the center of the each egdes of the quadrilateral
   *
   *  \param Vector of 4 vectors of double, each containing the coordinates of the center of an egde
   */
    std::vector<std::vector<double>> Getmiddle(){return _mid_ed;};
};

/*! \class Point
   * \brief Class representing a point
   *
   *  This class stores and manages the summets of the mesh
   */
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
    /*!
     *  \brief Returns the scalar coordinates of the point
     *
     *  \return Vector of 2 doubles containing the Cartesian coordinates of the summet
     */
    std::vector<double> Getcoor() {return _coor;};
    // Récupérer la référence du point
    /*!
     *  \brief Obtains reference
     *
     *  \return Integer corresponding to the reference of the summet
     */
    int Getref(){return _ref;};
};

/*! \class Edge
   * \brief Class representing an edge
   *
   *  This class stores and manages the egdes of the mesh
   */
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
    /*!
     *  \brief Returns the summets of the egde
     *
     *  \return Vector of 2 integers containing the references (from the class Point) of the summets
     */
    std::vector<int> Getedge(){return _v;};
    // Récupérer la référence de l'arête
    /*!
     *  \brief Obtains reference
     *
     *  \return Integer corresponding to the reference of the edge
     */
    int Getref(){return _ref;};
    // Modifie la référence
    /*!
     *  \brief Modifies reference
     *
     *  \param i : new reference (integer) of the edge, gived to attribute _ref
     */
    void Modifyref(int i){_ref = i;};
};
