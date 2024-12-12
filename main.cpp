#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <unordered_map>
#include <chrono>
#include <random>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <unordered_set>
#include <ctime>

using namespace std;

chrono::steady_clock::time_point startReducere;
unordered_map<int, double> produseReducere;
const int durataReducere = 30;

class Produs {
public:
    virtual void descrieProdus() const = 0;
    virtual double calculeazaPret() const = 0;
    virtual bool afiseazaDisponibilitate() const = 0;
    virtual ~Produs() {}
};

class Piese_Auto : public virtual Produs {
public:
    string nume;
    double pret;
    int cantitate;
    Piese_Auto() : nume(""), pret(0.0), cantitate(0) {}
    Piese_Auto(string n, double p, int c) : nume(n), pret(p), cantitate(c) {}
    Piese_Auto(const Piese_Auto& other) : nume(other.nume), pret(other.pret), cantitate(other.cantitate) {}
    ~Piese_Auto() {}

    void descrieProdus() const override {
        cout << "Piesa: " << nume << ", Pret: " << pret << " lei, Cantitate: " << cantitate << endl;
    }

    double calculeazaPret() const override {
        return pret * cantitate;
    }

    bool afiseazaDisponibilitate() const override {
        return cantitate > 0;
    }
};

class ProdusCumparat : public virtual Produs {
public:
    string nume;
    double pret;
    int cantitate;

    ProdusCumparat() : nume(""), pret(0.0), cantitate(0) {}
    ProdusCumparat(string n, double p, int c) : nume(n), pret(p), cantitate(c) {}
    ProdusCumparat(const ProdusCumparat& other) : nume(other.nume), pret(other.pret), cantitate(other.cantitate) {}
    ~ProdusCumparat() {}


    void descrieProdus() const override {
        cout << "Produs: " << nume << ", Pret: " << pret << " lei, Cantitate: " << cantitate << endl;
    }

    double calculeazaPret() const override {
        return pret * cantitate;
    }

    bool afiseazaDisponibilitate() const override {
        return cantitate > 0;
    }

    ProdusCumparat& operator = (const ProdusCumparat& other) {
        if (this != &other) {
            nume = other.nume;
            pret = other.pret;
            cantitate = other.cantitate;
        }
        return *this;
    }

    ProdusCumparat& operator +(const ProdusCumparat& other) {
        if(nume == other.nume) {
            cantitate += other.cantitate;
            pret += other.pret;
        }
        return *this;
    }
};

class ClientProdus : public Piese_Auto, public ProdusCumparat {
public:
    ClientProdus(string n, double p, int c) : Piese_Auto(n, p, c), ProdusCumparat(n, p, c) {}

    void descrieProdus() const override {
        Piese_Auto::descrieProdus();
    }
    double calculeazaPret() const override {
        return Piese_Auto::calculeazaPret();
    }
    bool afiseazaDisponibilitate() const override {
        return Piese_Auto::afiseazaDisponibilitate();
    }
};

class PachetPromotional {
public:
    vector <int> id_uri;
    double pretPachet;
    PachetPromotional() : id_uri({}), pretPachet(0.0) {}
    PachetPromotional(vector<int> id, double pret) : id_uri(id), pretPachet(pret) {}
    PachetPromotional(const PachetPromotional& other) : id_uri(other.id_uri), pretPachet(other.pretPachet) {}
    ~PachetPromotional() {}

    bool operator%(int produsId) const {
        return find(id_uri.begin(), id_uri.end(), produsId) != id_uri.end();
    }
};

class Client {
public:
    string idnp;
    string nume;
    string prenume;
    int numarVizite;
    double sumaTotala;
    bool cardReducere;

    Client(string id, string n, string p, int vizite, double suma)
        : idnp(id), nume(n), prenume(p), numarVizite(vizite), sumaTotala(suma) {}

        void afisareInformatii() const {
        cout << "IDNP: " << idnp << ", Nume: " << nume << ", Prenume: " << prenume << ", Numar vizite: "
        << numarVizite << ", Suma totala: " << sumaTotala << " lei" << endl;
    }
        bool operator>(const Client& other) const {
            return numarVizite > other.numarVizite || (numarVizite == other.numarVizite && sumaTotala > other.sumaTotala);
        }
        bool operator<(const Client& other) const {
        return numarVizite < other.numarVizite || (numarVizite == other.numarVizite && sumaTotala < other.sumaTotala);
    }
        bool operator!() const {
        return numarVizite == 0;
    }
    void actualizareFisier(const string& dateclient) {
        ifstream fisierCitire(dateclient);
        vector<Client> clienti;
        string linie;
        while(getline(fisierCitire, linie)) {
            stringstream ss(linie);
            string idnp, nume, prenume;
            int vizite;
            double suma;
            bool card = false;
            if(getline(ss, idnp, ',') && getline(ss, nume, ',') && getline(ss, prenume, ',') && ss >>
                vizite && ss.ignore(1,',') && ss >> suma) {
                clienti.emplace_back(idnp, nume, prenume, vizite, suma);
                clienti.back().cardReducere = card;
            }
        }
        fisierCitire.close();

        bool clientGasit = false;
        for(auto& client : clienti) {
            if(client.idnp == idnp && client.nume == nume && client.prenume == prenume) {
                client.numarVizite = numarVizite;
                client.sumaTotala = sumaTotala;
                client.cardReducere = cardReducere;
                clientGasit = true;
                break;
            }
        }
        if(!clientGasit) {
            clienti.emplace_back(idnp, nume, prenume, numarVizite, sumaTotala);
            clienti.back().cardReducere = cardReducere;
        }
        ofstream fisierScriere(dateclient);
        for(const auto& client : clienti) {
            fisierScriere << client.idnp << "," << client.nume << "," << client.prenume << "," << client.numarVizite << "," << client.sumaTotala << endl;
        }
        fisierScriere.close();
    }
    bool esteEligibilPentruCardReducere(const Client& client) {
        return client.numarVizite >= 5 && client.sumaTotala >= 4000;
    }

    friend void afisareClient(const Client& client);
    friend bool comparaSumaTotala(const Client& client1, const Client& client2);
};

void afisareClient(const Client &client) {
    cout << "Client: " << client.nume << " " << client.prenume << ", IDNP: " << client.idnp << endl;
    cout << "Numar vizite: " << client.numarVizite << ", Suma totala: " << client.sumaTotala << " lei" << endl;
    cout << "Card de reducere: " << (client.cardReducere ? "Da" : "Nu") << endl;
}

bool comparaSumaTotala(const Client &client1, const Client &client2) {
    return client1.sumaTotala > client2.sumaTotala;
}

vector<Client> citesteClientDinFisier(const string& dateclient) {
    vector<Client> clienti;
    ifstream fisier(dateclient);

    //if(!fisier.is_open()) {
      //  cerr << " Nu s-a putut deschide fisierul: " << dateclient << endl;
        //cerr << "Directorul curent: " << filesystem::current_path() << endl;
        //cerr << "Calea absoluta fisierului: " << filesystem::absolute(dateclient) << endl;
        //return clienti;
    //}

    string linie;
    while(getline(fisier, linie)) {
        stringstream ss(linie);
        string idnp, nume, prenume;
        int vizite;
        double suma;
        if (getline(ss, idnp, ',') && getline(ss, nume, ',') && getline(ss, prenume, ',') && ss >> vizite && ss.ignore(1, ',') && ss >> suma) {
            clienti.emplace_back(idnp, nume, prenume, vizite, suma);
            cout << "Citit client: " << idnp << ", " << nume << ", " << prenume << ", " << vizite << ", " << suma << endl;
        } else {
            cerr << "Eroare la citirea din fisier" << endl;
        }
    }
    if(clienti.empty()) {
        cerr << "Nu s-au citit datele clientilor din fisier" << endl;
    }
    return clienti;
}

void genereazaInvitatieVIP(const Client& client) {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char data[20];
    strftime(data, sizeof(data), "%Y-%m-%d", ltm);

    string invitatie = "Draga " + client.nume + " " + client.prenume + ",\n\n";
    invitatie += "Avem placerea de a va invita la evenimentul nostru exclusiv.\n";
    invitatie += "Nume: " + client.nume + " " + client.prenume + "\n";
    invitatie += "Data: " + string(data) + "\n\n";
    invitatie += "Cu stima,\nEchipa noastra\n";

    string  directory = "D:/POO Seminar/LAB3/cmake-build-debug/invitatii/";

    string fileName = directory + "Invitatie_" + client.idnp + ".txt";
    ofstream fisier(fileName);
    if(fisier.is_open()) {
        fisier << invitatie;
        fisier.close();
       // cout << "Invitatie generata cu success pentru clientul " << client.nume << " " << client.prenume << " si salvata in fisierul " << fileName << endl;
    //} else {
      //  cerr << "Nu s-a putut genera invitatie pentru clientul " << client.nume << " " << client.prenume << endl;
    }
}

bool esteVip(const Client& client) {
    return client.sumaTotala > 1000;
}

bool confirmaCumparearea() {
    char confirmare;
    cout << "Doriti sa confirmati cumpararea? (y/n)" << endl;
    cin >> confirmare;
    return confirmare == 'y'  || confirmare == 'Y';
}

vector<ProdusCumparat> produseCumparate;
const int NumarPiese = 20;
Piese_Auto piese[NumarPiese];
float pretTotal = 0.0;

unordered_map<string, double> coduriPromotie = {
    {"PROMO10", 0.1},
    {"PROMO20", 0.2},
    {"PROMO30", 0.3},
    {"PROMO40", 0.4},
    {"PROMO50", 0.5}
};

vector<PachetPromotional> pachetePromotii;

void afisarePiese(Piese_Auto piese[], int nr_piese) {
    chrono::steady_clock::time_point now = chrono::steady_clock::now();
    chrono::duration<double> elapsed_seconds = chrono::duration_cast<chrono::duration<double>>(now - startReducere);

    for (int i = 0; i < nr_piese; i++) {
        cout << "ID: " << i + 1 << ", Piesa: " << piese[i].nume << ", Pret: " << piese[i].pret << " lei, Cantitate: " << piese[i].cantitate;
        if(produseReducere.find(i) != produseReducere.end() && elapsed_seconds.count() < durataReducere) {
            double reducere = produseReducere[i];
            double pretRedus = piese[i].pret * (1 - reducere);
            cout << " (Reducere: " << reducere * 100 << "%, Pret redus: " << pretRedus << " lei)";
        }
        cout << endl;
    }
}

void aplicaReducereTemp() {
    startReducere = chrono::steady_clock::now();
    produseReducere.clear();
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, NumarPiese - 1);
    uniform_real_distribution<> reducere_dis(0.1, 0.3);

    for (int i = 0; i < 5; ++i) {
        int id = dis(gen);
        double reducere = reducere_dis(gen);
        produseReducere[id] = reducere;
    }
}

void aplicaPachetPromotional(int nr_piese) {
    for (size_t i = 0; i < pachetePromotii.size(); ++i) {
        const auto& pachet = pachetePromotii[i];
        bool pachetDisponibil = true;
        for(int id : pachet.id_uri) {
            if(id >= nr_piese || piese[id].cantitate == 0) {
                pachetDisponibil = false;
                break;
            }
        }
        if(pachetDisponibil) {
            cout << "ID Pachet Promo: " << i + 1 << ", Pachetul disponibil: ";
            for(size_t j = 0; j < pachet.id_uri.size(); ++j) {
                cout << piese[pachet.id_uri[j]].nume;
                if(j < pachet.id_uri.size() -1) {
                    cout << " + ";
                }
            }
            cout << " la pretul de " << pachet.pretPachet << " lei" << endl;
        }
    }
}

void cumparaPachetPromotional(int nr_piese) {
    int pachetId;
    cout << "Introduceti ID-ul pachetului promotional: " << endl;
    cin >> pachetId;
    if(cin.fail() || pachetId < 1 || pachetId > pachetePromotii.size()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "ID-ul introdus este invalid" << endl;
        return;
    }
    pachetId -= 1;

    const auto& pachet = pachetePromotii[pachetId];
    bool pachetDisponibil = true;
    for(int id : pachet.id_uri) {
        if(id >= nr_piese || piese[id].cantitate == 0) {
            pachetDisponibil = false;
            break;
        }
    }
    if (pachetDisponibil) {
        for (int id : pachet.id_uri) {
            piese[id].cantitate -= 1;
        }
        produseCumparate.push_back({"Pachet Promotional", pachet.pretPachet, 1});
        pretTotal += pachet.pretPachet;
        cout << "Pachetul a fost achizionat cu success!" << endl;
    } else {
        cout << "Pachetul nu este disponibil" << endl;
    }
}

void genereazaPachetPromotional(int nr_piese) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, nr_piese -1);
    uniform_int_distribution<> pret_dis(120.59, 442.7);

    for(int i = 0; i < 5; ++i) {
        int id1 = dis(gen);
        int id2;
        do {
            id2 = dis(gen);
        } while (id2 == id1);
        double pretPachet = pret_dis(gen);
        pachetePromotii.push_back({{id1, id2}, pretPachet});
    }
}


void cumparaPiesa(int nr_piese) {
    int id;
    string cantitateStr;
    cout << "Introduceti ID-ul piesei: " << endl;
    cin >> id;
    if (cin.fail() || cin.peek() != '\n') {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Tasta invalida." << endl;
        return;
    }
    id -= 1;
    if(id >= 0 && id < nr_piese) {
        if(piese[id].cantitate == 0) {
            cout << "Stoc epuizat pentru: " << piese[id].nume << endl;
            return;
        }
        cout << "Introduceti cantitatea dorita: " << endl;
        cin >> cantitateStr;

        if (!all_of(cantitateStr.begin(), cantitateStr.end(), ::isdigit)) {
            cout << "Cantitatea invalida. Introduceti un numar intreg exact." << endl;
            return;
        }

        int cantitate;
        try {
            cantitate = stoi(cantitateStr);
        } catch (const out_of_range& e) {
            cout << "Cantitatea introdusa este prea mare." << endl;
            return;
        }
        if(cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Cantitate invalida. Introduceti un numar de la 1 pana la cantitatea disponibila" << endl;
            return;
        }
        if (cantitate < 0) {
            cout << "Cantitatea introduse este invalida" << endl;
            return;
        }

        chrono::steady_clock::time_point now = chrono::steady_clock::now();
        chrono::duration<double> elapsed_seconds = chrono::duration_cast<chrono::duration<double>>(now - startReducere);
        if(elapsed_seconds.count() >= durataReducere) {
            produseReducere.clear();
        }

        if (piese[id].cantitate >= cantitate) {
            double pretFinal = piese[id].pret;
            if (produseReducere.find(id) != produseReducere.end()) {
                double reducere = produseReducere[id];
                pretFinal = piese[id].pret * (1 - reducere);
            }
            piese[id].cantitate -= cantitate;
            pretTotal += pretFinal * cantitate;

            auto it = find_if(produseCumparate.begin(), produseCumparate.end(), [id](const ProdusCumparat& produs) {
                return produs.nume == piese[id].nume;
            });

            if( it != produseCumparate.end()) {
                it -> cantitate += cantitate;
                it -> pret += pretFinal * cantitate;
            } else {
                produseCumparate.push_back({piese[id].nume, pretFinal * cantitate, cantitate});
            }

            if(cantitate > 4 && piese[id].cantitate > 0) { // BOGO offer
                cout << "Felicitari! Ai primit un produs gratuit!" << endl;
                auto it = find_if(produseCumparate.begin(), produseCumparate.end(), [id](const ProdusCumparat& produs) {
                    return produs.nume == piese[id].nume;
                });

                if(it != produseCumparate.end()) {
                    it -> cantitate += 1;
                } else {
                    produseCumparate.push_back({piese[id].nume, 0.0, 1});
                }
                piese[id].cantitate -=1;
            }
        } else {
            cout << "Cantitatea ceruta este mai mare decat cea disponibila" << endl;
        }
    } else {
        cout << "ID-ul introdus nu exista" << endl;
    }
}

const double pretTransport = 1200.0;

void afisarePretTotal() {
    cout << "Pretul total al pieselor alese este: " << pretTotal << " lei" << endl;
    cout << "Produsele alese: " << endl;
    for (const auto& produs : produseCumparate) {
        cout << "Nume: " << produs.nume << ", Pret: " << produs.pret << " lei, Cantitate: " << produs.cantitate << endl;
    }
    if(pretTotal > pretTransport) {
        cout << "Transportul va fi gratuit" << endl;
    } else {
        cout << "Transportul nu va fi gratuit" << endl;
    }
}

bool codPromotionalAplicat = false;
void aplicaCodPromotional() {
    if(codPromotionalAplicat) {
        cout << "Un cod promotional a fost deja aplicat" << endl;
        return;
    }

    string cod;
    cout << "Introduceti codul promotional: " << endl;
    cin >> cod;
    if(coduriPromotie.find(cod) != coduriPromotie.end()) {
        double reducere = coduriPromotie[cod];
        pretTotal *= (1 - reducere);
        cout << "Codul a fost aplicat. Reducere este de " << reducere * 100 << "%" << endl;
        codPromotionalAplicat = true;
    } else {
        cout << "Codul introdus este invalid" << endl;
    }
}

bool verificaIDNP(const vector<Client>& clienti, const string& idnp, const string& nume, const string& prenume) {
    for (const auto& client : clienti) {
        if (client.idnp == idnp) {
            if (client.nume != nume || client.prenume != prenume) {
                cout << "Eroare: Exista deja un client cu IDNP-ul " << idnp << " dar cu un nume diferit." << endl;
                return false;
            }
            return true;
        }
    }
    return true;
}


void afisareMeniu() {
    cout << "--- Meniu Principal ---" << endl;
    cout << "--------------------" << endl;
    cout << "1. Afisare piese" << endl;
    cout << "--------------------" << endl;
    cout << "2. Cumpara piesa" << endl;
    cout << "--------------------" << endl;
    cout << "3. Afiseaza ce este in cos si pretul total" << endl;
    cout << "--------------------" << endl;
    cout << "4. Aplica cod promotional" << endl;
    cout << "--------------------" << endl;
    cout << "5. Afiseaza pachetele promotionale" << endl;
    cout << "--------------------" << endl;
    cout << "6. Cumpara pachet promotional" << endl;
    cout << "--------------------" << endl;
    cout << "7. Iesire(Confirmati cumparatura)" << endl;
    cout << "--------------------" << endl;
    cout << "Introduceti optiunea: " << endl;
}

void proceseazaOptiune(int optiuneInt, int nr_piese, bool& cumparaturaConfirmata) {
    switch (optiuneInt) {
        case 1:
            afisarePiese(piese, nr_piese);
        break;
        case 2:
            cumparaPiesa(nr_piese);
        break;
        case 3:
            afisarePretTotal();
        break;
        case 4:
            aplicaCodPromotional();
        break;
        case 5:
            aplicaPachetPromotional(nr_piese);
        break;
        case 6:
            cumparaPachetPromotional(nr_piese);
        break;
        case 7:
            if (confirmaCumparearea()) {
                cout << "Cumparatura a fost confirmata. La revedere" << endl;
                afisarePretTotal();
                cumparaturaConfirmata = true;
                return;
            } else {
                cout << "Cumparatura nu a fost confirmata. Continuati cumparaturile" << endl;
            }
        break;
        default:
            cout << "Optiune invalida" << endl;
        break;
    }
}

void meniu(int nr_piese) {
    string optiune;
    bool cumparaturaConfirmata = false;

    do {
        afisareMeniu();
        cin >> optiune;

        if (optiune.size() == 1 && isdigit(optiune[0])) {
            int optiuneInt = optiune[0] - '0';
            proceseazaOptiune(optiuneInt, nr_piese, cumparaturaConfirmata);
        } else {
            cout << "Optiune invalida. Introduceti un numar de la 1 la 7" << endl;
        }
    } while (!cumparaturaConfirmata);
}

void initializePiese() {
    piese[0] = Piese_Auto("Ulei pentru motor 5w-30 2.4L", 183.0, 10);
    piese[1] = Piese_Auto("Antigel SWAG (verde) 1.5L", 153.90, 30);
    piese[2] = Piese_Auto("Lichid parbriz de vara", 342.67, 15);
    piese[3] = Piese_Auto("Parasolar Lateral Negru", 44.0, 50);
    piese[4] = Piese_Auto("Suport Auto Telefon", 255.54, 50);
    piese[5] = Piese_Auto("Kit Instrumente", 209.19, 20);
    piese[6] = Piese_Auto("Cabluri Tractare", 127.64, 25);
    piese[7] = Piese_Auto("Aparatoare Noroi", 224.28, 45);
    piese[8] = Piese_Auto("AD Blue 10L", 284.32, 12);
    piese[9] = Piese_Auto("Lichid de frana 1L", 132.54, 27);
    piese[10] = Piese_Auto("Filtru de combustibil(Pentru motoare Diesel)", 164.31, 30);
    piese[11] = Piese_Auto("Filtru de aer", 114.23, 55);
    piese[12] = Piese_Auto("Placute de frana", 304, 22);
    piese[13] = Piese_Auto("Amortizor hidraulic", 570.46, 40);
    piese[14] = Piese_Auto("Kit ambreiaj", 1900, 15);
    piese[15] = Piese_Auto("Curea de distributie", 450.35, 20);
    piese[16] = Piese_Auto("Radiator motor", 1140.87, 33);
    piese[17] = Piese_Auto("Catalizator", 2700.67, 46);
    piese[18] = Piese_Auto("Injector combustibil", 1215.24, 38);
    piese[19] = Piese_Auto("Senzor de parcare", 410.54, 25);
}

Client gestioneazaClient(vector<Client>& clienti) {
    string idnp, nume, prenume;
    bool validData = false;

    while (!validData) {
        cout << "Introduceti IDNP-ul: ";
        cin >> idnp;
        cout << "Introduceti numele: ";
        cin >> nume;
        cout << "Introduceti prenumele: ";
        cin >> prenume;

        validData = verificaIDNP(clienti, idnp, nume, prenume);
    }

    auto it = find_if(clienti.begin(), clienti.end(), [&](const Client& client) {
        return client.idnp == idnp && client.nume == nume && client.prenume == prenume;
    });

    Client client(idnp, nume, prenume, 0, 0.0);
    if (it != clienti.end()) {
        client = *it;
        client.numarVizite++;
    } else {
        client.numarVizite = 1;
    }
    return client;
}

void finalizeazaCumparatura(Client& client) {
    if (client.esteEligibilPentruCardReducere(client)) {
        client.cardReducere = true;
        cout << "Felicitari! Ati obtinut un card de reducere de 10%!" << endl;
    }

    if (client.cardReducere && client.numarVizite > 5) {
        pretTotal *= 0.9;
    }

    client.sumaTotala += pretTotal;

    if (esteVip(client)) {
        genereazaInvitatieVIP(client);
    }

    client.actualizareFisier("dateclient.txt");
}

int main() {
    initializePiese();
    ClientProdus clientProdus("Ulei pentru motor 5w-30 2.4L", 183.0, 0);
    clientProdus.descrieProdus();
    cout << "Pretul total: " << clientProdus.calculeazaPret() << " lei" << endl;
    cout << "Disponibilitate: " << (clientProdus.afiseazaDisponibilitate() ? "Da" : "Nu") << endl;
    aplicaReducereTemp();
    genereazaPachetPromotional(NumarPiese);

    //cout << "Current working directory: " << filesystem::current_path() << endl;
    vector<Client> clienti = citesteClientDinFisier("dateclient.txt");
    Client client = gestioneazaClient(clienti);
    meniu(NumarPiese);
    if (confirmaCumparearea()) {
        finalizeazaCumparatura(client);
    }
    return 0;
 }