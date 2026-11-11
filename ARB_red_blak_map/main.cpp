
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <chrono>

using namespace std;

int main() {

    map<int, int> arbre;

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
        arbre.insert({valeurs[i], valeurs[i]});

    auto finInsertion = chrono::high_resolution_clock::now();

    double tempsInsertion =
        chrono::duration<double, nano>(
            finInsertion - debutInsertion
        ).count();

    cout << "\nInsertion terminee !" << endl;
    cout << "Nombre de noeuds inseres : " << arbre.size() << endl;

    // Recherche
    cout << "\nDonner la valeur a rechercher : ";
    cin >> valeurRecherche;

    auto debutRecherche = chrono::high_resolution_clock::now();

    auto resultat = arbre.find(valeurRecherche);

    auto finRecherche = chrono::high_resolution_clock::now();

    double tempsRecherche =
        chrono::duration<double, nano>(
            finRecherche - debutRecherche
        ).count();

    if (resultat != arbre.end())
        cout << "Valeur trouvee !" << endl;
    else
        cout << "Valeur introuvable !" << endl;

    // Suppression
    cout << "\nDonner la valeur a supprimer : ";
    cin >> valeurSuppression;

    auto debutSuppression = chrono::high_resolution_clock::now();

    size_t resultatSuppression = arbre.erase(valeurSuppression);

    auto finSuppression = chrono::high_resolution_clock::now();

    double tempsSuppression =
        chrono::duration<double, nano>(
            finSuppression - debutSuppression
        ).count();

    if (resultatSuppression > 0)
        cout << "Suppression effectuee !" << endl;
    else
        cout << "Valeur introuvable !" << endl;

    // Affichage
    cout << "\nNombre de noeuds : " << arbre.size() << endl;

    if (arbre.empty()) {
        cout << "L'arbre est vide." << endl;
    }
    else {
        cout << "\nLes 10 premiers noeuds :" << endl;

        int compteur = 0;

        for (auto it = arbre.begin();
             it != arbre.end() && compteur < 10;
             ++it) {

            cout << it->first << "  ";
            compteur++;
        }

        cout << "\n\nLes 10 derniers noeuds :" << endl;

        compteur = 0;

        for (auto it = arbre.rbegin();
             it != arbre.rend() && compteur < 10;
             ++it) {

            cout << it->first << "  ";
            compteur++;
        }

        cout << endl;
    }

    // Rapport final
    cout << "\n========== TEMPS D'EXECUTION ==========" << endl;

    cout << "\n--- INSERTION ---" << endl;
    cout << "Nombre de noeuds : " << n << endl;
    cout << "Temps total : " << tempsInsertion << " ns" << endl;

    cout << "Temps moyen par insertion : "
         << tempsInsertion / n << " ns" << endl;

    cout << "\n--- RECHERCHE ---" << endl;
    cout << "Valeur recherchee : " << valeurRecherche << endl;
    cout << "Temps d'execution : " << tempsRecherche << " ns" << endl;

    cout << "\n--- SUPPRESSION ---" << endl;
    cout << "Valeur a supprimer : " << valeurSuppression << endl;
    cout << "Temps d'execution : " << tempsSuppression << " ns" << endl;

    cout << "\n=======================================" << endl;

    return 0;
}
