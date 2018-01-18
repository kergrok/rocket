#include <vector>
#include <string>
#include "Type.h"


class Mesh
{
  private:
    std::vector<Point> _mpoint;
    std::vector<Quad> _mquad;
    std::vector<Part> _part;
    std::vector<Maille> _maille;
    std::vector<Edge> _medge;
    std::string _meshname;
    int _N;
    double _T;
    double _Ma;
    double _gamma;
    double _rho;
    std::vector<bool> _TF;
    double _omega;
    double _Temps_final;
    double _dt;
    double _vitesse_max;
    double _Mp;  // masse d'une particule de N2
    double _tau; // temps charactéristique pour les injections
    //Surface du domaine
    double _surf_tot;
    // Longueur des arêtes
    Eigen::VectorXd _lenghts;
    // Les quadrilatères collés aux arêtes
    // Si _edg_Q2[i] == -1 alors c'est une arête au bord
    Eigen::VectorXi _edg_Q1, _edg_Q2;
    std::string _methode;


  public:
    // Constructeur
    Mesh(std::string meshname);
    // ------------------ Lecture du maillage  ---------------------
    void readmesh();
    // ------------ Construction des arêtes à l'intérieur du domaine -----------
    void BuildEdges();
    // ------------------ Conversion cartésien <-> cylindrique -----------------
    Eigen::Vector2d Convert(double x, double y);
    Eigen::Vector2d Convertinv(double r, double theta);
    // ------------------ calcul de l'angle d'incidence sur la paroi ----------------
    double Angle(int i);
    // ------------------ Calcul de la longueur des arêtes -----------------
    void Buildlenghts();
    // ------------------ Calcul des surfaces des quad ---------------------
    void Buildsurfaces();
    // ----------- Calcul des propriétés dans la maille i --------------
    void Calc_prop(int i);
    // ------- Déplace toutes les particules pour un pas de temps -------
    void Displacement(int i1, int iN);
    // ---------------- Calcul dt grâce à la "CFL" --------------------
    void CFL();
    // ------------------ Calcul des voisins ---------------------
    void Buildvoisins();

    bool Find_Maille(int i);

    void find_impact(int i, Eigen::Vector2d coor, Eigen::Vector2d new_coor);

    bool is_in(int maille, Eigen::Vector2d Position);

    void Create_particules(int maille, int arete);

    void Create_in_Flow();
    // ------------- Ecriture des arêtes dans un fichier --------------
    void WriteEdgesAndAssociatedQuad();

    void write(std::string Nom_Fichier, std::string Nom_Fichier1, std::string Nom_Fichier2);
    // Lit les parametres dans un fichier
    void ReadParameter();
    // Initialisation du Problem
    void initialize();
    // boucle en tps du Problem
    void compute();

    double alea(double a,double b);

    void MajMailleParticule(int i1, int iN);

    double Norme_entre(Eigen::Vector2d Vec1, Eigen::Vector2d Vec2);

    double Norme(Eigen::Vector2d Vec);

    void Create_tau();
    // Collision pour l'ensemble des particules
    void collision();

    void Build_Center_Norm();

    bool is_CFL_respected(Eigen::Vector3d Vitesse);
    // Répartition des charges entre les processeurs
    std::vector<int> charge(int me, int Np);

    double Get_tau(){return _tau;};
    double Get_dt(){return _dt;};
    double Get_Temps_final(){return _Temps_final;};
    double Get_nb_mailles(){return _maille.size();};
};
