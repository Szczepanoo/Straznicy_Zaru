#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <random>

using namespace std;

void sleep(int time_ms){
    this_thread::sleep_for(chrono::milliseconds(time_ms));
}

int getChoice(){
    string choiceStr;
    int choice;
    cin >> choiceStr;
    try {
        choice = stoi(choiceStr);
    } catch (invalid_argument){
        choice = -999;
    }
    return choice;
}

// Klasa reprezentująca smoka
class Dragon {
public:
    string name;
    int health;
    int damage;

    Dragon(string n, int h, int d) : name(n), health(h), damage(d) {}

    void print_info() {
        cout << "[" << name << "]" << endl;
        if (name == "NIKCZEMNIUCH") {
            cout << "Sila: brak danych" << endl;
        } else {
            cout << "Sila: " << damage << endl;
        }
        cout << "Zdrowie: " << health << endl << endl;
    }
};

// Klasa reprezentująca gracza-strażaka
class Firefighter {
public:
    string name;
    int health;
    int max_health;
    int water;
    int experience;
    int extinguisher_lvl;
    int waterBomb_lvl;
    int waterBomb_amt;
    int medkits;


    Firefighter() : name(""), health(120), max_health(120), water(100), experience(0), extinguisher_lvl(0), waterBomb_lvl(0), waterBomb_amt(0), medkits(0){}

    void useExtinguisher() {
        if (water > 0) {
            cout << "Uzywasz wody do gaszenia pozaru!" << endl;
            water -= 10;
        } else {
            cout << "Brak wody!" << endl;
        }
    }

    void rescueCivilian() {
        cout << "Ratujesz cywila!" << endl;
        experience += 10;
    }
};

void displayStatus(Firefighter &ff, Dragon &dragon) {
    cout << "Status Gracza: Zdrowie = " << ff.health << ", Woda = " << ff.water << ", Doswiadczenie = " << ff.experience << endl;
    cout << "Status Smoka: Zdrowie = " << dragon.health << endl;
}

int fight(Firefighter &player, Dragon &dragon) {
    int max_dragon_health = dragon.health;
    dragon.print_info();
    while (player.health >= 0 && dragon.health > 0) {
        int base_player_damage;
        bool choice_accepted = false;
        bool usemedkit = false;
        while (!choice_accepted) {
            cout << "Wybierz akcje:" << endl;
            if (player.extinguisher_lvl > 0) {
                cout << "1. Atak gasnica" << endl;
            }

            if (player.waterBomb_lvl > 0 && player.waterBomb_amt > 0) {
                cout << "2. Bomba wodna" << endl;
            }
            if (player.medkits > 0){
                cout << "9. Apteczka" << endl;
            }


            int choice = getChoice();
            switch (choice) {
                case 1:
                    base_player_damage = player.extinguisher_lvl * 10;
                    choice_accepted = true;
                    break;
                case 2:
                    if (player.waterBomb_lvl > 0 && player.waterBomb_amt > 0){
                        base_player_damage = -10;
                        choice_accepted = true;
                    } else {
                        cout << "Nieprawidlowy wybor." << endl;
                    }
                    break;
                case 3:
                    base_player_damage = -10;
                    choice_accepted = true;
                    break;
                case 9:
                    if (player.medkits > 0){
                        base_player_damage = 0;
                        usemedkit = true;
                        choice_accepted = true;
                    } else {
                        cout << "Nieprawidlowy wybor." << endl;
                    }
                    break;
                default:
                    cout << "Nieprawidlowy wybor." << endl;
                    break;
            }
        }

        //Generator liczb losowych z losowym ziarniem
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(static_cast<int>(base_player_damage * 0.75), static_cast<int>(base_player_damage * 1.25));
        int real_player_damage = dis(gen);

        uniform_int_distribution<> dis2(static_cast<int>(dragon.damage * 0.75), static_cast<int>(dragon.damage * 1.25));
        int real_dragon_damage = dis2(gen);

        // Uzycie apteczki lub atak smoka
        if (usemedkit){
            player.health = player.max_health;
            cout << "Uzywasz apteczki." << endl;
            player.medkits--;
            cout << "Pozostalo " << player.medkits << "szt." << endl;
        } else {
            dragon.health -= real_player_damage;
            cout << "Zadajesz " << real_player_damage << " obrazen smokowi!" << endl;
        }


        // Sprawdzenie czy gracz przegrywa w walce z pierwszym smokiem
        if (player.health - real_dragon_damage <= 0 && dragon.name == "NIKCZEMNIUCH"){
            cout << "Starszy Strazak Franciszek zadaje 35 obrazen smokowi!" << endl;
            cout << "Poziom zdrowia przeciwnika: " << dragon.health - 35 << "/" << max_dragon_health  << endl;
            cout << "Zwyciestwo! Zdobywasz: 10xp" << endl;
            cout << "Nacisnij ENTER, aby kontynuowac..." << endl;
            cin.get();
            return -1;

        }

        player.health -= real_dragon_damage;
        cout << dragon.name << " atakuje zadajac " << real_dragon_damage << " obrazen!" << endl << endl;
        cout << "Poziom twojego zdrowia: " << player.health << "/" << player.max_health << endl;
        cout << "Poziom zdrowia przeciwnika: " << dragon.health << "/" << max_dragon_health << endl;
        if (player.health <= 0) {
            cout << "Porazka" << endl;
            player.experience += 1;
            cout << "Nacisnij ENTER, aby kontynuowac..." << endl;
            cin.get();
            return -1;
        } else if (dragon.health <= 0) {
            cout << "Zwyciestwo! Zdobywasz: "<< max_dragon_health << "xp." << endl;
            player.experience+= max_dragon_health;
            cout << "Nacisnij ENTER, aby kontynuowac..." << endl;
            cin.get();
            return 1;
        }
    }
}

void print_letter_by_letter(string string){
    for (int i = 0; i < string.length(); i ++){
        cout << string[i] << flush;
//        sleep(50);
    }
    cout << endl;
}

void showEquiment(Firefighter &player){
    cout << endl << "[" << player.name << "]" << endl;
    cout << "Stan zdrowia: "<< player.health << "/" << player.max_health << endl;
    switch (player.extinguisher_lvl) {
        case 1:
            cout << "Gasnica: ZWYKLA GASNICA" << endl;
            break;
        case 2:
            cout << "Gasnica: NIEPOSPOLITA GASNICA" << endl;
            break;
        case 3:
            cout << "Gasnica: GASNICA DOWODCY" << endl;
            break;
        case 4:
            cout << "Gasnica: LEGENDARNA GASNICA" << endl;
            break;
        case 5:
            cout << "Gasnica: GASNICA ZE SMOCZEJ LUSKI" << endl;
            break;
        default:
            cout << "Gasnica: BRAK" << endl;
            break;
    }
    cout << "Bomby wodne (szt): " << player.waterBomb_amt << endl;
    cout << "Apteczki (szt):" << player.medkits << endl;
}

void RescueCivilianMission(){

}

void HuntForDragonMission(){

}

void SaveCityMission(){

}

int main() {
    Dragon Nikczemniuch("NIKCZEMNIUCH",60,35);
    Dragon Burzogniew("BURZOGNIEW",1000,70);
    Dragon Pyros("PYROS",5000,90);
    Dragon Zguba_Miast("ZGUBA MIAST",7000,100);
    Dragon Wladca_Zaru("WLADCA ZARU", 10000, 250);
    Firefighter player;
    bool showMenu = true;
    cout << "Straznicy Zaru: Ognisty Konflikt" << endl;
    while (showMenu) {
        cout << "1. Nowa gra\n2. Wczytaj zapis\n0. Wyjscie z gry\n";
        int choice = getChoice();

        switch (choice) {
            case 1:
                player = Firefighter();
                showMenu = false;
                break;
            case 2:
                cout << "Coming soon..." << endl;
                break;
            case 0:
                cout << "Do zobaczenia!" << endl;
                return 0;
            default:
                cout << "Nieprawidlowy wybor." << endl;
                break;
        }

    }
    // PROLOGUE
        if (player.experience == 0){
            system("cls");
            print_letter_by_letter("Witaj w miescie Pyroklas!");
            sleep(500);
            print_letter_by_letter("Codzienne zycie splata sie tu z cieniem niebezpieczenstwa unoszacego sie w powietrzu. "
                                   "W miasteczku, gdzie smoki nie sa legenda, ale rzeczywistoscia. "
                                   "Ostatnio jednak atmosfera napiecia narasta, a grozba zlowrogich pozarow staje sie coraz bardziej realna. "
                                   "Jako mlody adept pozarnictwa wstepujesz w szeregi lokalnej jednostki strazy pozarnej, aby wesprzec ich w dzialaniach.");
            sleep(2000);
            cout << "[---GLOWNA SIEDZIBA STRAZNIKOW ZARU---]" << endl;
            cout << "Dowodca Strazakow Samuel: ";
            print_letter_by_letter("Witaj swiezaku, jestem Sam i dowodze tymi oszolomami.");
            sleep(3000);
            print_letter_by_letter("To jest Franek, poznajcie sie, wdrozy Cie w nasze szeregi i oprowadzi po jednostce.");
            cout << "Starszy Strazak Franciszek: ";
            print_letter_by_letter("Czesc mlody, na poczatek, trzymaj podstawowy przedmiot kazdego strazaka");
            sleep(500);
            player.extinguisher_lvl++;
            cout << "[+] DODANO PRZEDMIOT: ZWYKLA GASNICA" << endl;
            sleep(500);
            cout << "Starszy Strazak Franciszek: ";
            print_letter_by_letter("W tych czasach kazdy z nas nosi pod reka swoja gasnice. "
                                   "To narzedzie ktore najlepiej sprawdza sie w ratowaniu cywili, ale od biedy moze tez "
                                   "sluzyc w walce ze smokami.");
            sleep(1000);
            cout << "Starszy Strazak Franciszek: ";
            print_letter_by_letter("Pozwol, ze oprowadze Cie teraz po naszej centrali...");
            cout << "[TRACHHH!]" << endl;
            sleep(2000);
            cout << "Dowodca Strazakow Samuel: ";
            print_letter_by_letter("SLYSZELISCIE TO!? TO NIKCZEMNIUCH! Myslalem ze ostatnim razem udalo nam sie go pokonac, "
                                   "ale wyglada na to za powrocil! Franek, bierz nowego i zajmijcie sie nim. Ja zbiore chlopakow i "
                                   "zaraz do was dolaczymy!");
            cout << "Starszy Strazak Franciszek: ";
            print_letter_by_letter("Za mna mlody, wydaje mi sie, ze wyladowal na dachu. Pamietaj o gasnicy! ");
            sleep(3000);
            cout << "[---GLOWNA SIEDZIBA STRAZNIKOW ZARU - DACH---]" << endl;
            cout << "Starszy Strazak Franciszek: ";
            print_letter_by_letter("Acha! Jest tutaj! Do dziela mlody!");
            fight(player,Nikczemniuch);
            sleep(2000);
            cout << "Dowodca Strazakow Samuel: ";
            print_letter_by_letter("Psia kostka! Cale szczescie zyjecie! I.. to... Nikczemniuch!? Pokonany?! "
                                   "Musze przyznac niezle wam poszlo!");
            sleep(1000);
            cout << "Starszy Strazak Franciszek: ";
            print_letter_by_letter("Z calym szacunkiem kapitanie, ale to zasluga mlodego. Moj udzial w tej walce byl znikomy");
            sleep(1000);
            cout << "Dowodca Strazakow Samuel: ";
            print_letter_by_letter("Ach tak? To w takim razie moze powiedz jak Ci na imie swiezy?");
            cout << "Twoja odpowiedz: (NIE WPISUJ SPACJI)";
            cin >> player.name;
            //getline(cin, player.name);
            // cout << player.name;
            cout << "Dowodca Strazakow Samuel: ";
            print_letter_by_letter("Milo Cie poznac " + player.name + ". Franek dokonczyl twoje szkolenie? "
                                                                      "Jeden problem rozwiazany, ale nie mozemy teraz spoczac na laurach. "
                                                                      "Bierzmy sie do roboty.");
            sleep(1000);
            cout << "Starszy Strazak Franciszek: ";
            print_letter_by_letter("Najpierw wez ten zestaw apteczek. Ten gad niezle cie poturbowal.");
            player.medkits += 5;
            cout << "[+] DODANO APTECZKI: 5" << endl;
            sleep(1000);
            cout << "Starszy Strazak Franciszek: ";
            print_letter_by_letter("To maly pakunek, ktory zawsze warto miec przy sobie. Szczegolnie jesli wybierasz sie "
                                   "na polowanie na smoki. Przywroci Cie do pelni sil, ale jej uzycie moze sprawic, ze "
                                   "latwiej bedzie Cie trafic. Uzyj jednej od razu.");
            cout << "[Nacisnij 9 by uzyc apteczki]"<< endl;
            int useMedkit = getChoice();
            if (useMedkit == 9){
                player.health = player.max_health;
                cout << "[PRZYWROCONO ZDROWIE]" << endl;
                cout << "Starszy Strazak Franciszek: ";
                print_letter_by_letter("No, od razu lepiej.");
            }else {
                cout << "Starszy Strazak Franciszek: ";
                print_letter_by_letter("Nie upieram sie. Mozesz zostawic je sobie na pozniej. "
                                       "Pamietaj, ze stawanie do walki oslabionym moze sie zle skonczyc.");
            }
            sleep(1000);
            cout << "Starszy Strazak Franciszek: ";
            print_letter_by_letter("Przejdzmy teraz do omowienia naszych codziennych aktywnosci. "
                                   "Standardowo wykonujemy 3 rodzaje misji: ratowanie cywili, gaszenie pozarow i polowanie na smoki. "
                                   "Kazda z misji polega na czyms innym i jest niemniej wazna od pozostalych. "
                                   "Misje ratunkowe zwiekszaja poziom bezpieczenstwa w miescie oraz nasz poziom respektu wsrod ludzi. "
                                   "Gaszenie pozarow odblokowuje dostep do zniszczonych czesci miasta, "
                                   "w ktorych istnieje szansa na odnalezienie lepszej broni lub nowego ekwipunku. "
                                   "Polowanie na smoki to najbardziej wymagajaca misja. "
                                   "Z naszych ustalen wynika, ze w miescie grasuja 4 smoki, no teraz to juz 3 i Wladca Zaru - prawdopodobnie"
                                   "rowniez smok - najpotezniejszy i najsilniejszy ze wszystkich. "
                                   "Mysle, ze polowanie na smoki to aktualnie zly wybor, ale decyzja nalezy do Ciebie. "
                                   "Czym chesz sie teraz zajac?");


        }
        // MAIN GAME LOOP
    bool gameOver = false;
    while (!gameOver) {
        cout << "\n[Wybierz akcje]\n1. Misja ratunkowa\n2. Gaszenie pozaru\n3. Polowanie na smoki\n4. Zapisz gre\n5. Wyswietl ekwipunek\n0. Wyjscie z gry\n";

        int choice = getChoice();

        switch (choice) {
            case 1:
                RescueCivilianMission();
                break;
            case 2:
                SaveCityMission();
                break;
            case 3:
                HuntForDragonMission();
                break;
            case 4:
                cout << "Coming soon..." << endl;
                break;
            case 5:
                showEquiment(player);
                break;
            case 0:
                gameOver = true;
                break;
            default:
                cout << "Nieprawidlowy wybor. " << endl;
                break;
        }
    }

    cout << "Dziekujemy za gre!" << endl;
    return 0;
}
