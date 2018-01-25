/**
 * \file Mesh.h
 * \brief Definition of Mesh class
 * \author L.Bouet - S.Bouchex - A.Antimes - C.Gouin
 * \version 1.0
 * \date 26 janvier 2018
 *
 *
 *
 */

#include <vector>
#include <string>
#include "Type.h"

/*! \class Mesh
   * \brief Class containing all function of the computing
   *
   *  This class stores and manages every step of the calculation. All classes of Type.h are regrouped in this class.
   */

class Mesh
{
  private:
    std::vector<Point> _mpoint; /*!< Vector containing all mesh point*/
    std::vector<Quad> _mquad; /*!< Vector containing all mesh quadrilaterals*/
    std::vector<Part> _part; /*!< Vector containing all particles*/
    std::vector<Maille> _maille; /*!< Vector containing all mesh properties*/
    std::vector<Edge> _medge; /*!< Vector containing all mesh edges*/
    std::string _meshname; /*!< Mesh name*/
    int _N;
    double _T; /*!< Temperature*/
    double _Ma; /*!< Mach number*/
    double _gamma; /*!< Gamma */
    double _rho; /*!< Density*/
    std::vector<bool> _TF; /*!< Test on particles*/
    double _omega; /*!< Numerical weight*/
    double _Temps_final; /*!< Final times*/
    double _dt; /*!< Time step*/
    double _vitesse_max; /*!< Maximum velocity to respect the CFL*/
    double _Mp; /*!< Nitrogen particle mass*/  // masse d'une particule de N2
    double _tau; /*!< Characteristic time*/ // temps caractéristique pour les injections
    //Surface du domaine
    double _surf_tot; /*!< Domain surface*/
    // Longueur des arêtes
    std::vector<double> _lenghts; /*!< Vector containing all the lenghts of egdes*/
    // Les quadrilatères collés aux arêtes
    // Si _edg_Q2[i] == -1 alors c'est une arête au bord
    std::vector<int> _edg_Q1 /*!< Indicator for the egdes : if its value is -1 it's a border edge*/, _edg_Q2 /*!< Indicator for the egdes : if its value is -1 it's a border edge*/;
    std::string _methode; /*!< Type of rebound*/
    double _C; /*!< Condition for the charasteristic time*/


  public:
    // Constructeur
    Mesh(std::string meshname);
    // ------------------ Lecture du maillage  ---------------------
    /*!
     *  \brief Reading the mesh
     *
     *  This function reads the mesh (.mesh) and builds the vectors _mpoints, _medge and _mquad.
     *
     */
    void readmesh();
    // ------------ Construction des arêtes à l'intérieur du domaine -----------
    /*!
     *  \brief Building the edges of the mesh
     */
    void BuildEdges();
    // ------------------ Conversion cartésien <-> cylindrique -----------------
    /*!
     *  \brief Converting cartesian coordinates in cylindrical coordinates
     *
     *  \param x first component of the cartesian coordinates
     *  \param y second component of the cartesian coordinates
     *
     *  \return Vector including the r and theta components of the cylindrical coordinates
     *
     */
    std::vector<double> Convert(double x, double y);
    /*!
     *  \brief Converting cylindrical coordinates in cartesian coordinates
     *
     *  \param r first component of the cylindrical coordinates
     *  \param theta second component of the cylindrical coordinates
     *
     *  \return Vector including the x and y components of the cartesian coordinates
     *
     */
    std::vector<double> Convertinv(double r, double theta);
    double Angle(int i);
    // ------------------ Calcul de la longueur des arêtes -----------------
    /*!
     *  \brief Building the vector _lenght
     *
     * This function builds the lenght of each edges of the mesh
     *
     */
    void Buildlenghts();
    // ------------------ Calcul des surfaces des quad ---------------------
    /*!
     *  \brief Building the vector _surf
     *
     * This function builds the surfaces of each quadrilaterals of the mesh
     *
     */
    void Buildsurfaces();
    // ----------- Calcul des propriétés dans la maille i --------------
    /*!
     *  \brief Computing all properties
     *
     * This function computes the properties of an element of _maille
     *
     * \param i corresponds at the number of the element of maille
     *
     */
    void Calc_prop(int i);
    // ------- Déplace toutes les particules pour un pas de temps -------
    /*!
     *  \brief Computing the displacement of every particles
     *
     * This function calculates the new coordinates of each particles using their velocities for one time step
     *
     */
    void Displacement();
    // ---------------- Calcul dt grâce à la "CFL" --------------------
    /*!
     *  \brief Calculing dt using the CFL
     */
    void CFL();
    // ---------- Construction des voisins pour chaque maille --------------
    /*!
     *  \brief Building the neighbours of each quadrilaterals of the mesh
     *
     * This function builds the vector _voisins for each elements of the class Maille
     *
     */
    void Buildvoisins();
    /*!
     *  \brief Finding if a a particle belongs at the domain
     *
     * \param i corresponds to the particle to test
     *
     * \return TRUE if the particle is in the domain
     * \return FALSE else
     *
     */
    bool Find_Maille(int i);

    void find_impact(int i, std::vector<double> coor, std::vector<double> new_coor);

    bool is_in(int maille, std::vector<double> Position);

    void Create_particules(int maille, int arete, std::vector<int> Edges);
    // --------------- Fait entrer les particules ----------------
    void Create_in_Flow();
    // ------------- Ecriture des arêtes dans un fichier --------------
    void WriteEdgesAndAssociatedQuad();

    void write(std::string Nom_Fichier, std::string Nom_Fichier1, std::string Nom_Fichier2, std::string Nom_Fichier3, std::string Nom_Fichier4);
    // Lit les parametres dans un fichier
    void ReadParameter();
    // Initialisation du Problem
    void initialize();
    // boucle en tps du Problem
    void compute();

    double alea(double a,double b);

    void MajMailleParticule();

    double Norme_entre(std::vector<double> Vec1, std::vector<double> Vec2);

    double Norme(std::vector<double> Vec);
    // ---------- Calcul temps caractéristique pour les injections ---------
    void Create_tau();
    // Collision pour l'ensemble des particules
    void collision();
    //------Calcul de la norme, du milieu et de la normale de chaque arêtes------
    void Build_Center_Norm();

    bool is_CFL_respected(std::vector<double> Vitesse);

};
