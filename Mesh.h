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
    std::vector<double> _lenghts;
    // Les quadrilatères collés aux arêtes
    // Si _edg_Q2[i] == -1 alors c'est une arête au bord
    std::vector<int> _edg_Q1, _edg_Q2;
    std::string _methode;
    double _C;


  public:
    // Constructeur
    Mesh(std::string meshname);
    // ------------------ Lecture du maillage  ---------------------
    void readmesh();
    // ------------ Construction des arêtes à l'intérieur du domaine -----------
    void BuildEdges();
    // ------------------ Conversion cartésien <-> cylindrique -----------------
    std::vector<double> Convert(double x, double y);
    std::vector<double> Convertinv(double r, double theta);
    // ------------------ calcul de l'angle d'incidence sur la paroi ----------------
    double Angle(int i);
    // ------------------ Calcul de la longueur des arêtes -----------------
    void Buildlenghts();
    // ------------------ Calcul des surfaces des quad ---------------------
    void Buildsurfaces();
    // ----------- Calcul des propriétés dans la maille i --------------
    void Calc_prop(int i);
    // ------- Déplace toutes les particules pour un pas de temps -------
    void Displacement();
    // ---------------- Calcul dt grâce à la "CFL" --------------------
    void CFL();
    // ---------- Construction des voisins pour chaque maille --------------
    void Buildvoisins();

    bool Find_Maille(int i);

    void find_impact(int i, std::vector<double> coor, std::vector<double> new_coor);

    bool is_in(int maille, std::vector<double> Position);

    void Create_particules(int maille, int arete, std::vector<int> Edges);
    // --------------- Fait entrer les particules ----------------
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
