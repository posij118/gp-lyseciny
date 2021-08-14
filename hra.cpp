#include<bits/stdc++.h>
#include <chrono>
#include <pthread.h>
#define MAX_LENGTH     50
using namespace std;

// Debug musí být false.
bool debug = true;

// Sem patří obsah souboru input_body.txt.
vector<vector<double> > points = {{50.71275919675827, 15.83528995513916}, {50.71362890303135, 15.834453105926514}, {50.71556478738785, 15.834753513336182}, {50.714654847979546, 15.83778977394104}, {50.71163132786751, 15.839388370513916}, {50.71519799530506, 15.840343236923218}, {50.717955976724625, 15.8352792263031}, {50.72011582553387, 15.835343599319458}, {50.71275249123573, 15.84302544593811}, {50.710062235593796, 15.846115350723267}, {50.709464102983475, 15.841695070266724}, {50.708458945155144, 15.848819017410278}, {50.707819908857346, 15.841180086135864}, {50.70869900286198, 15.833396315574646}, {50.70626690983772, 15.830671191215515}, {50.708671510219574, 15.836893916130066}, {50.70684157311916, 15.838622599840164}, {50.70456974208355, 15.837634205818176}, {50.70372752845287, 15.829394459724426}, {50.70880964398384, 15.824115872383118}, {50.71133695542812, 15.826948285102844}, {50.71190759539604, 15.823858380317688}, {50.71087494492531, 15.821455121040344}, {50.70886395871639, 15.821755528450012}, {50.71071200072765, 15.82930862903595}, {50.704787001013756, 15.825231671333313}, {50.70628233253956, 15.826776623725891}, {50.70397160947323, 15.821455121040344}, {50.706798657774925, 15.819781422615051}, {50.71024999022484, 15.830896496772766}, {50.703537091612816, 15.825360417366028}, {50.70212356746197, 15.827506184577942}, {50.70543944835663, 15.848663449287415}, {50.70484131574631, 15.846303105354309}, {50.713021382689476, 15.840466618537903}, {50.71005955338478, 15.820210576057434}, {50.705086067318916, 15.818536877632141}, {50.70622734725475, 15.841067433357239}, {50.70660822093487, 15.836346745491028}, {50.7031562179327, 15.837677121162415}, {50.70182450115681, 15.829866528511047}, {50.70889078080654, 15.8430415391922}, {50.70902690291405, 15.818365216255188}, {50.717939883470535, 15.838707089424133}, {50.711065381765366, 15.843170285224915}, {50.70707023143768, 15.823429226875305}, {50.70943459868431, 15.828450322151184}, {50.700872987508774, 15.831969380378723}, {50.70019371807575, 15.828106999397278}, {50.697910487651825, 15.830338597297668}};

struct Deposit{
    string type;        // Jedno z 'i', 'c', 'o'
    int capacity;
    int speed;
    int level;

    int i;
};

// Parametry
int INIT_IRON_STORED = 2000;
int INIT_COPPER_STORED = 2000;
int INIT_OIL_STORED = 0;
int COPPER_PRODUCTION = 1;
int IRON_PRODUCTION = 1;
int OIL_PRODUCTION = 0;

vector<Deposit> IRON_DEPOSITS = {};
vector<Deposit> COPPER_DEPOSITS = {};
vector<Deposit> OIL_DEPOSITS = {};


// Proměnné zachycující současný stav

int iron_stored = INIT_IRON_STORED;
int copper_stored = INIT_COPPER_STORED;
int oil_stored = INIT_OIL_STORED;

int iron_production = IRON_PRODUCTION;
int copper_production = COPPER_PRODUCTION;
int oil_production = OIL_PRODUCTION;

vector<vector<int> > belt_graph;
vector<vector<int> > train_graph;
vector<vector<int> > pipe_graph;

vector<int> iron_deposits_activated;
vector<int> copper_deposits_activated;
vector<int> oil_deposits_activated;

bool stop_input = true;

void *readInput(void* input){
    getline(cin, *static_cast<string*>(input));
    pthread_exit(NULL);
}

int main(){
    cout << "Hra začíná. Za celé politbyro vám přeji hodně štěstí." << endl;

    chrono::milliseconds timespan(1000);
    string user_input;
    int rc;


    while(1){
        // Jeden tik je jedna vteřina.
        user_input = "";
        pthread_t input_thread;
        rc = pthread_create(&input_thread, NULL, readInput, &user_input);
        this_thread::sleep_for(timespan);
        pthread_kill(input_thread, 3);

        if(debug){
            cout << "ONE_TICK" << endl;
        }

        cout << user_input << endl;
    }
}
