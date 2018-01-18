//lc ne sert plus à rien mais on en a besoin pour definir les points
lc = 1;
//On definit ici la valeur des rayons du domaine
r_ext = 3;
r_int = 0.5;
//Correspond au nombre de mailles radiales moins 1 (correspond au nbre d'aretes)
nbre_mailles_radiales = 90;
//De même pour le nombre de couche
nbre_couches = 120;

//Definition des points de la geometrie
Point(10) = {r_ext,0,0,lc};
Point(20) = {0,0,0,lc};
Point(30) = {0,r_ext,0,lc};
Point(40) = {r_int,0,0,lc};
Point(50) = {0,r_int,0,lc};

//Definition des lignes
Line(1) = {10,40};
Circle(2) = {40,20,50};
Line(3) = {50,30};
Circle(4) = {30,20,10};

//Definition du contour a partir des lignes
Line Loop (5) = {1,2,3,4};
//Definition de la surface a mailler
Ruled Surface (6) = {5};

// Structuration du maillage
Transfinite Line {1,3} = nbre_couches;
Transfinite Line {2,4} = nbre_mailles_radiales;
Transfinite Surface 6;
Recombine Surface 6;
