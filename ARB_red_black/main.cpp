
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

enum Color { RED, BLACK };

struct Node {
    int valeur;
    Color couleur;
    Node *gauche;
    Node *droite;
    Node *parent;
};

struct RedBlackTree {
    Node *racine;
};

// Creation d'un noeud
Node* creerNoeud(int valeur) {
    Node *nouveau = new Node;

    nouveau->valeur = valeur;
    nouveau->couleur = RED;
    nouveau->gauche = NULL;
    nouveau->droite = NULL;
    nouveau->parent = NULL;

    return nouveau;
}

// Recuperer la couleur
Color couleurNoeud(Node *noeud) {
    if (noeud == NULL)
        return BLACK;

    return noeud->couleur;
}

// Rotation gauche
void rotationGauche(RedBlackTree &arbre, Node *x) {
    Node *y = x->droite;

    x->droite = y->gauche;

    if (y->gauche != NULL)
        y->gauche->parent = x;

    y->parent = x->parent;

    if (x->parent == NULL)
        arbre.racine = y;
    else if (x == x->parent->gauche)
        x->parent->gauche = y;
    else
        x->parent->droite = y;

    y->gauche = x;
    x->parent = y;
}

// Rotation droite
void rotationDroite(RedBlackTree &arbre, Node *y) {
    Node *x = y->gauche;

    y->gauche = x->droite;

    if (x->droite != NULL)
        x->droite->parent = y;

    x->parent = y->parent;

    if (y->parent == NULL)
        arbre.racine = x;
    else if (y == y->parent->gauche)
        y->parent->gauche = x;
    else
        y->parent->droite = x;

    x->droite = y;
    y->parent = x;
}

// Correction apres insertion
void corrigerInsertion(RedBlackTree &arbre, Node *z) {
    while (z != arbre.racine &&
           couleurNoeud(z->parent) == RED) {

        Node *parent = z->parent;
        Node *grandParent = parent->parent;

        if (parent == grandParent->gauche) {
            Node *oncle = grandParent->droite;

            if (couleurNoeud(oncle) == RED) {
                parent->couleur = BLACK;
                oncle->couleur = BLACK;
                grandParent->couleur = RED;
                z = grandParent;
            }
            else {
                if (z == parent->droite) {
                    z = parent;
                    rotationGauche(arbre, z);
                }

                z->parent->couleur = BLACK;
                z->parent->parent->couleur = RED;

                rotationDroite(arbre, z->parent->parent);
            }
        }
        else {
            Node *oncle = grandParent->gauche;

            if (couleurNoeud(oncle) == RED) {
                parent->couleur = BLACK;
                oncle->couleur = BLACK;
                grandParent->couleur = RED;
                z = grandParent;
            }
            else {
                if (z == parent->gauche) {
                    z = parent;
                    rotationDroite(arbre, z);
                }

                z->parent->couleur = BLACK;
                z->parent->parent->couleur = RED;

                rotationGauche(arbre, z->parent->parent);
            }
        }
    }

    arbre.racine->couleur = BLACK;
}

// Insertion
void insertion(RedBlackTree &arbre, int valeur) {
    Node *z = creerNoeud(valeur);

    Node *y = NULL;
    Node *x = arbre.racine;

    while (x != NULL) {
        y = x;

        if (z->valeur < x->valeur)
            x = x->gauche;
        else
            x = x->droite;
    }

    z->parent = y;

    if (y == NULL)
        arbre.racine = z;
    else if (z->valeur < y->valeur)
        y->gauche = z;
    else
        y->droite = z;

    corrigerInsertion(arbre, z);
}

// Recherche
Node* recherche(Node *racine, int valeur) {
    Node *courant = racine;

    while (courant != NULL && courant->valeur != valeur) {
        if (valeur < courant->valeur)
            courant = courant->gauche;
        else
            courant = courant->droite;
    }

    return courant;
}

// Trouver le minimum
Node* minimum(Node *noeud) {
    while (noeud->gauche != NULL)
        noeud = noeud->gauche;

    return noeud;
}

// Remplacement d'un noeud
void transplanter(RedBlackTree &arbre, Node *u, Node *v) {
    if (u->parent == NULL)
        arbre.racine = v;
    else if (u == u->parent->gauche)
        u->parent->gauche = v;
    else
        u->parent->droite = v;

    if (v != NULL)
        v->parent = u->parent;
}

// Correction apres suppression
void corrigerSuppression(RedBlackTree &arbre, Node *x,
                         Node *parent, bool xEstGauche) {

    while (x != arbre.racine &&
           couleurNoeud(x) == BLACK) {

        if (parent == NULL)
            break;

        bool estGauche;

        if (x != NULL)
            estGauche = (x == parent->gauche);
        else
            estGauche = xEstGauche;

        if (estGauche) {
            Node *frere = parent->droite;

            if (couleurNoeud(frere) == RED) {
                frere->couleur = BLACK;
                parent->couleur = RED;

                rotationGauche(arbre, parent);

                frere = parent->droite;
            }

            if (frere == NULL) {
                x = parent;
                parent = x->parent;

                if (parent != NULL)
                    xEstGauche = (x == parent->gauche);

                continue;
            }

            if (couleurNoeud(frere->gauche) == BLACK &&
                couleurNoeud(frere->droite) == BLACK) {

                frere->couleur = RED;

                x = parent;
                parent = x->parent;

                if (parent != NULL)
                    xEstGauche = (x == parent->gauche);
            }
            else {
                if (couleurNoeud(frere->droite) == BLACK) {
                    if (frere->gauche != NULL)
                        frere->gauche->couleur = BLACK;

                    frere->couleur = RED;

                    rotationDroite(arbre, frere);

                    frere = parent->droite;
                }

                frere->couleur = parent->couleur;
                parent->couleur = BLACK;

                if (frere->droite != NULL)
                    frere->droite->couleur = BLACK;

                rotationGauche(arbre, parent);

                x = arbre.racine;
                parent = NULL;
            }
        }
        else {
            Node *frere = parent->gauche;

            if (couleurNoeud(frere) == RED) {
                frere->couleur = BLACK;
                parent->couleur = RED;

                rotationDroite(arbre, parent);

                frere = parent->gauche;
            }

            if (frere == NULL) {
                x = parent;
                parent = x->parent;

                if (parent != NULL)
                    xEstGauche = (x == parent->gauche);

                continue;
            }

            if (couleurNoeud(frere->droite) == BLACK &&
                couleurNoeud(frere->gauche) == BLACK) {

                frere->couleur = RED;

                x = parent;
                parent = x->parent;

                if (parent != NULL)
                    xEstGauche = (x == parent->gauche);
            }
            else {
                if (couleurNoeud(frere->gauche) == BLACK) {
                    if (frere->droite != NULL)
                        frere->droite->couleur = BLACK;

                    frere->couleur = RED;

                    rotationGauche(arbre, frere);

                    frere = parent->gauche;
                }

                frere->couleur = parent->couleur;
                parent->couleur = BLACK;

                if (frere->gauche != NULL)
                    frere->gauche->couleur = BLACK;

                rotationDroite(arbre, parent);

                x = arbre.racine;
                parent = NULL;
            }
        }
    }

    if (x != NULL)
        x->couleur = BLACK;
}

// Suppression
bool suppression(RedBlackTree &arbre, int valeur) {
    Node *z = recherche(arbre.racine, valeur);

    if (z == NULL)
        return false;

    Node *y = z;
    Node *x = NULL;
    Node *parentX = NULL;

    Color couleurOriginale = y->couleur;
    bool xEstGauche = false;

    if (z->gauche == NULL) {
        x = z->droite;
        parentX = z->parent;

        if (parentX != NULL)
            xEstGauche = (z == parentX->gauche);

        transplanter(arbre, z, z->droite);

        if (x != NULL) {
            parentX = x->parent;
            xEstGauche = (parentX != NULL &&
                          x == parentX->gauche);
        }
    }
    else if (z->droite == NULL) {
        x = z->gauche;
        parentX = z->parent;

        if (parentX != NULL)
            xEstGauche = (z == parentX->gauche);

        transplanter(arbre, z, z->gauche);

        if (x != NULL) {
            parentX = x->parent;
            xEstGauche = (parentX != NULL &&
                          x == parentX->gauche);
        }
    }
    else {
        y = minimum(z->droite);

        couleurOriginale = y->couleur;
        x = y->droite;

        if (y->parent == z) {
            parentX = y;
            xEstGauche = false;

            if (x != NULL)
                x->parent = y;
        }
        else {
            parentX = y->parent;
            xEstGauche = (y == parentX->gauche);

            transplanter(arbre, y, y->droite);

            if (x != NULL) {
                parentX = x->parent;
                xEstGauche = (x == parentX->gauche);
            }

            y->droite = z->droite;
            y->droite->parent = y;
        }

        transplanter(arbre, z, y);

        y->gauche = z->gauche;
        y->gauche->parent = y;

        y->couleur = z->couleur;
    }

    delete z;

    if (couleurOriginale == BLACK)
        corrigerSuppression(arbre, x, parentX, xEstGauche);

    return true;
}

// Compter les noeuds
int compterNoeuds(Node *racine) {
    if (racine == NULL)
        return 0;

    return 1 + compterNoeuds(racine->gauche)
             + compterNoeuds(racine->droite);
}

// Parcours infixe
void parcoursInfixe(Node *racine, vector<pair<int, Color>> &valeurs) {
    if (racine == NULL)
        return;

    parcoursInfixe(racine->gauche, valeurs);

    valeurs.push_back({racine->valeur, racine->couleur});

    parcoursInfixe(racine->droite, valeurs);
}

// Affichage
void afficherArbre(Node *racine, int nombre) {
    vector<pair<int, Color>> valeurs;

    parcoursInfixe(racine, valeurs);

    cout << "\nNombre de noeuds : " << nombre << endl;

    if (racine == NULL) {
        cout << "L'arbre est vide." << endl;
        return;
    }

    cout << "Racine : " << racine->valeur
         << " (" << (racine->couleur == RED ? "RED" : "BLACK")
         << ")" << endl;

    cout << "\nLes 10 premiers noeuds :" << endl;

    int limite = min(10, (int)valeurs.size());

    for (int i = 0; i < limite; i++) {
        cout << valeurs[i].first << " ("
             << (valeurs[i].second == RED ? "RED" : "BLACK")
             << ")  ";
    }

    cout << "\n\nLes 10 derniers noeuds :" << endl;

    int debut = max(0, (int)valeurs.size() - 10);

    for (int i = debut; i < (int)valeurs.size(); i++) {
        cout << valeurs[i].first << " ("
             << (valeurs[i].second == RED ? "RED" : "BLACK")
             << ")  ";
    }

    cout << endl;
}

// Liberer la memoire
void libererArbre(Node *racine) {
    if (racine == NULL)
        return;

    libererArbre(racine->gauche);
    libererArbre(racine->droite);

    delete racine;
}

// Programme principal
int main() {
    RedBlackTree arbre;
    arbre.racine = NULL;

    int n;
    int valeurRecherche;
    int valeurSuppression;

    cout << "Donner le nombre de noeuds : ";
    cin >> n;

    if (n <= 0) {
        cout << "Nombre invalide !" << endl;
        return 0;
    }

    // Generation des valeurs aleatoires distinctes
    vector<int> valeurs(n);

    for (int i = 0; i < n; i++)
        valeurs[i] = i + 1;

    mt19937 generateur(time(0));
    shuffle(valeurs.begin(), valeurs.end(), generateur);

    // Insertion
    auto debutInsertion = chrono::high_resolution_clock::now();

    for (int i = 0; i < n; i++)
        insertion(arbre, valeurs[i]);

    auto finInsertion = chrono::high_resolution_clock::now();

    double tempsInsertion =
        chrono::duration<double, milli>(
            finInsertion - debutInsertion
        ).count();

    cout << "\nInsertion terminee !" << endl;
    cout << "Nombre de noeuds inseres : " << n << endl;

    // Recherche
    cout << "\nDonner la valeur a rechercher : ";
    cin >> valeurRecherche;

    auto debutRecherche = chrono::high_resolution_clock::now();

    Node *resultat = recherche(arbre.racine, valeurRecherche);

    auto finRecherche = chrono::high_resolution_clock::now();

    double tempsRecherche =
        chrono::duration<double, nano>(
            finRecherche - debutRecherche
        ).count();

    if (resultat != NULL)
        cout << "Valeur trouvee !" << endl;
    else
        cout << "Valeur introuvable !" << endl;

    // Suppression
    cout << "\nDonner la valeur a supprimer : ";
    cin >> valeurSuppression;

    auto debutSuppression = chrono::high_resolution_clock::now();

    bool resultatSuppression =
        suppression(arbre, valeurSuppression);

    auto finSuppression = chrono::high_resolution_clock::now();

    double tempsSuppression =
        chrono::duration<double, nano>(
            finSuppression - debutSuppression
        ).count();

    if (resultatSuppression)
        cout << "Suppression effectuee !" << endl;
    else
        cout << "Valeur introuvable !" << endl;

    // Affichage
    int nombre = compterNoeuds(arbre.racine);

    afficherArbre(arbre.racine, nombre);

    // Rapport final
    cout << "\n========== TEMPS D'EXECUTION ==========" << endl;

    cout << "\n--- INSERTION ---" << endl;
    cout << "Nombre de noeuds : " << n << endl;
    cout << "Temps total : " << tempsInsertion << " ms" << endl;
    cout << "Temps moyen par insertion : "
         << tempsInsertion * 1000000.0 / n << " ns" << endl;

    cout << "\n--- RECHERCHE ---" << endl;
    cout << "Valeur recherchee : " << valeurRecherche << endl;
    cout << "Temps d'execution : " << tempsRecherche << " ns" << endl;

    cout << "\n--- SUPPRESSION ---" << endl;
    cout << "Valeur a supprimer : " << valeurSuppression << endl;
    cout << "Temps d'execution : " << tempsSuppression << " ns" << endl;

    cout << "\n=======================================" << endl;

    libererArbre(arbre.racine);

    return 0;
}
