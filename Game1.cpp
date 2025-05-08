#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>

using namespace std;

// Codici colore ANSI
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"

// Pulizia schermo
void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void delay(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

int InterfacciaMenu(){
    cout << CYAN;
    cout << "+---------------------------------+\n";
    cout << "|       BENVENUTO IN 007         |\n";
    cout << "|         THE GAME! 🎮           |\n";
    cout << "+---------------------------------+\n";
    cout << RESET;
    cout << "PREGO, SELEZIONARE LA MODALITA' DI GIOCO:\n";
    cout << YELLOW << "1" << RESET << ") Gioco Base\n";
    cout << YELLOW << "x" << RESET << ") Exit\n";
    char mod;
    cin >> mod;
    switch (mod)
    {
    case '1':
        cout << GREEN << "Hai scelto Gioco Base, INIZIAMO...\n" << RESET;
        return 1;
    case 'x':
        cout << RED << "Oh, va bene, buon proseguimento X|\n" << RESET;
        return -1;
    default:
        cout << RED << "Scelta non valida, ARRESTO IMMEDIATO\n" << RESET;
        return -1;
    }
}

void stampo_scelta(int player, int& choice){
    switch(choice){
        case 0:
            cout << BLUE << "🛡️ Player " << player << " si è protetto\t" << RESET;
            break;
        case 1:
            cout << YELLOW << "🔄 Player " << player << " ha ricaricato (+1 colpo)\t" << RESET;
            break;
        case 2:
            cout << RED << "💥 PLAYER " << player << " HA FATTO FUOCO, BOOOM\t" << RESET;
            break;
        default:
            break;
    }
}

void resoconto(int& LifeP1, int& LifeP2, int& shotsP1, int& shotsP2, unsigned int counterRound){
    cout << CYAN << "\n===== ROUND " << counterRound << " =====\n" << RESET;
    cout << "❤️ Vite P1: ";
    for(int i = 0; i < LifeP1; i++) cout << "❤️ ";
    cout << "\t❤️ Vite P2: ";
    for(int i = 0; i < LifeP2; i++) cout << "❤️ ";
    cout << "\n🔫 Colpi P1: ";
    for(int i = 0; i < shotsP1; i++) cout << "💥 ";
    cout << "\t🔫 Colpi P2: ";
    for(int i = 0; i < shotsP2; i++) cout << "💥 ";
    cout << "\n";
}

int ROUND(int& LifeP1, int& lifeP2, int shotsP1, int shotsP2){
    unsigned int count = 1;
    while(LifeP1 > 0 && lifeP2 > 0){
        clear_screen();
        resoconto(LifeP1, lifeP2, shotsP1, shotsP2, count);
        cout << GREEN << "\nScegli la tua prossima mossa:\n"
             << "0 = 🛡️ Shield\t1 = 🔄 Reload\t2 = 💥 Fire\n" << RESET;
        int choice1;
        cin >> choice1;
        if(shotsP1 == 0){
            while(choice1 < 0 || choice1 > 1){
                cout << RED << "Non hai colpi! Scegli tra 0 (scudo) o 1 (ricarica):\n" << RESET;
                cin >> choice1;
            }
        } else {
            while(choice1 < 0 || choice1 > 2){
                cout << RED << "Mossa non valida, riprovare:\n" << RESET;
                cin >> choice1;
            }
        }

        srand(time(nullptr));
        int choice2;
        if(shotsP2 == 0) choice2 = rand() % 2;
        else choice2 = rand() % 3;

        delay(500);
        stampo_scelta(1, choice1);
        delay(500);
        stampo_scelta(2, choice2);
        cout << endl;

        if(choice1 == 1) shotsP1++;
        if(choice2 == 1) shotsP2++;

        if(choice1 == 2){
            if(choice2 == 0){
                cout << BLUE << "🛡️ P2 si è difeso, nessun danno!\n" << RESET;
                shotsP1--;
            } else if(choice2 == 1){
                cout << GREEN << "P1 coglie di sorpresa P2! 💥\n" << RESET;
                lifeP2--;
                shotsP1--;
            } else if(choice2 == 2){
                if(shotsP1 > shotsP2){
                    shotsP1 -= shotsP2;
                    shotsP2 = 0;
                    lifeP2--;
                    cout << GREEN << "P1 ha prevalso nello scontro a fuoco! 💥\n" << RESET;
                } else if(shotsP1 < shotsP2){
                    shotsP2 -= shotsP1;
                    shotsP1 = 0;
                    LifeP1--;
                    cout << RED << "P2 ha vinto lo scambio di fuoco! 💥\n" << RESET;
                } else {
                    cout << YELLOW << "Stallo! Entrambi perdono 1 colpo 💥💥\n" << RESET;
                    shotsP1--;
                    shotsP2--;
                }
            }
        }

        if(choice2 == 2 && choice1 == 0){
            cout << BLUE << "🛡️ P1 si difende dal colpo di P2!\n" << RESET;
            shotsP2--;
        }
        if(choice1 == 1 && choice2 == 2){
            cout << RED << "P2 colpisce P1 mentre ricaricava! 💥\n" << RESET;
            LifeP1--;
            shotsP2--;
        }

        delay(1500);
        count++;
    }

    return (LifeP1 == 0) ? 2 : 1;
}

int main(){
    clear_screen();
    int modalità = InterfacciaMenu();
    if(modalità == -1) return 0;

    int LifeP1 = 3, LifeP2 = 3;
    int Result = ROUND(LifeP1, LifeP2, 0, 0);

    cout << CYAN << "\n==== RISULTATO ====\n" << RESET;
    if(Result == 1)
        cout << GREEN << "🎉 Player 1 ha VINTO!\n" << RESET;
    else
        cout << RED << "💀 Player 2 ha VINTO!\n" << RESET;

    cout << YELLOW << "Grazie per aver giocato a 007 THE GAME! 🕵️‍♂️\n" << RESET;
    return 0;
}
