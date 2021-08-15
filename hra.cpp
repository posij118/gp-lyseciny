#include <vector>
#include <math.h>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <thread>
#include <pthread.h>
#include <signal.h>
using namespace std;

// Debug musí být false.
bool debug = true;

// Sem patří obsah souboru input_body.txt.
vector<vector<double> > points = {{50.71275919675827, 15.83528995513916}, {50.71362890303135, 15.834453105926514}, {50.71556478738785, 15.834753513336182}, {50.714654847979546, 15.83778977394104}, {50.71163132786751, 15.839388370513916}, {50.71519799530506, 15.840343236923218}, {50.717955976724625, 15.8352792263031}, {50.72011582553387, 15.835343599319458}, {50.71275249123573, 15.84302544593811}, {50.710062235593796, 15.846115350723267}, {50.709464102983475, 15.841695070266724}, {50.708458945155144, 15.848819017410278}, {50.707819908857346, 15.841180086135864}, {50.70869900286198, 15.833396315574646}, {50.70626690983772, 15.830671191215515}, {50.708671510219574, 15.836893916130066}, {50.70684157311916, 15.838622599840164}, {50.70456974208355, 15.837634205818176}, {50.70372752845287, 15.829394459724426}, {50.70880964398384, 15.824115872383118}, {50.71133695542812, 15.826948285102844}, {50.71190759539604, 15.823858380317688}, {50.71087494492531, 15.821455121040344}, {50.70886395871639, 15.821755528450012}, {50.71071200072765, 15.82930862903595}, {50.704787001013756, 15.825231671333313}, {50.70628233253956, 15.826776623725891}, {50.70397160947323, 15.821455121040344}, {50.706798657774925, 15.819781422615051}, {50.71024999022484, 15.830896496772766}, {50.703537091612816, 15.825360417366028}, {50.70212356746197, 15.827506184577942}, {50.70543944835663, 15.848663449287415}, {50.70484131574631, 15.846303105354309}, {50.713021382689476, 15.840466618537903}, {50.71005955338478, 15.820210576057434}, {50.705086067318916, 15.818536877632141}, {50.70622734725475, 15.841067433357239}, {50.70660822093487, 15.836346745491028}, {50.7031562179327, 15.837677121162415}, {50.70182450115681, 15.829866528511047}, {50.70889078080654, 15.8430415391922}, {50.70902690291405, 15.818365216255188}, {50.717939883470535, 15.838707089424133}, {50.711065381765366, 15.843170285224915}, {50.70707023143768, 15.823429226875305}, {50.70943459868431, 15.828450322151184}, {50.700872987508774, 15.831969380378723}, {50.70019371807575, 15.828106999397278}, {50.697910487651825, 15.830338597297668}};

class Deposit{
    string type;    // Jedno z 'i', 'c', 'o'
    string code;    // Náhodný třípísmenný kencr, který zadávají.

    int capacity;
    int speed;
    int level;

    int i;
};

template <typename T> class Resources{
    public:
        T iron = 0;
        T copper = 0;
        T oil = 0;

        T machines = 0;
        T circuits = 0;
        T plastic = 0;

        T rocket = 0;
        T boosters = 0;
        T rocket_fuel = 0;

        int time = -1;

        Resources(vector<pair<int, T> > nonzero){
            for(auto resource: nonzero){
                switch(resource.first){
                    case 0:
                        this -> iron = resource.second;
                    case 1:
                        this -> copper = resource.second;
                    case 2:
                        this -> oil = resource.second;
                    case 3:
                        this -> machines = resource.second;
                    case 4:
                        this -> circuits = resource.second;
                    case 5:
                        this -> plastic = resource.second;
                    case 6:
                        this -> rocket = resource.second;
                    case 7:
                        this -> boosters = resource.second;
                    case 8:
                        this -> rocket_fuel = resource.second;

                }
            }
        }

        void print(){
            cout << "Železo: " << this -> iron << endl;
            cout << "Měď: " << this -> copper << endl;
            cout << "Ropa: " << this -> oil << endl;
            cout << "Stroje: " << this -> machines << endl;
            cout << "Obvody: " << this -> circuits << endl;
            cout << "Plast: " << this -> plastic << endl;
            cout << "Raketa: " << this -> rocket << endl;
            cout << "Boostery: " << this -> boosters << endl;
            cout << "Raketové palivo: " << this -> rocket_fuel << endl;
        }

        void add(Resources a){
            this -> iron += a.iron;
            this -> copper += a.copper;
            this -> oil += a.oil;
            this -> machines = a.machines;
            this -> circuits = a.circuits;
            this -> plastic = a.plastic;
            this -> rocket = a.rocket;
            this -> boosters = a.boosters;
            this -> rocket_fuel = a.rocket_fuel;
        }
};

// Parametry
int INIT_IRON_STORED = 3000;
int INIT_COPPER_STORED = 3000;
int INIT_OIL_STORED = 0;
int COPPER_PRODUCTION = 1;
int IRON_PRODUCTION = 1;
int OIL_PRODUCTION = 0;

double GLOBAL_TRANSPORT_COST_COEFFICIENT = 50000;

double BELT_IRON_COST = 3;
double BELT_COPPER_COST = 1;
double BELT_CAPACITY = 0.5;

double TRAIN_IRON_COST = 2;
double TRAIN_COPPER_COST = 1;
double TRAIN_MACHINES_COST = 1;
double TRAIN_CIRCUITS_COST = 0.25;
double TRAIN_PLASTIC_COST = 0.25;

double TRAIN_CAPACITY = 5;

double PIPE_COPPER_COST = 3;
double PIPE_IRON_COST = 1;

vector<Deposit> IRON_DEPOSITS = {};
vector<Deposit> COPPER_DEPOSITS = {};
vector<Deposit> OIL_DEPOSITS = {};


// Proměnné zachycující současný stav
vector<pair<int, double> > st = {make_pair(0, INIT_IRON_STORED), make_pair(1, INIT_COPPER_STORED), make_pair(2, INIT_OIL_STORED)};
Resources<double> team_stored = Resources(st);
vector<pair<int, double> > basic_prod = {make_pair(0, (double)IRON_PRODUCTION), make_pair(1, (double)COPPER_PRODUCTION), make_pair(2, (double)OIL_PRODUCTION)};
Resources<double> team_basic_production = Resources(basic_prod);
vector<pair<int, double> > prod = {};
Resources<double> team_production = Resources(prod);

vector<vector<int> > belt_graph(points.size());
vector<vector<int> > train_graph(points.size());
vector<vector<int> > pipe_graph(points.size());

vector<int> iron_deposits_activated;
vector<int> copper_deposits_activated;
vector<int> oil_deposits_activated;
int ticks_passed = 0;

void initValues(){

}

// Pomocné funkce

bool equals(string s, string t){
    if(s.length() != t.length()) return 0;

    for(int i = 0; i<s.length(); i++){
        if(tolower(s[i]) != tolower(t[i])) return 0;
    }

    return 1;
}

bool are_distinct_points(int i, int j){
    if(1 <= i && i <= points.size() && 1 <= j && j <= points.size() && i != j) return 1;
    else return 0;
}

double distance(int i, int j){
    return sqrt(((points[i][0] - points[j][0])*(points[i][0] - points[j][0])
      + (0.63338087263 *(points[i][1] - points[j][1]))*(0.63338087263 *(points[i][1] - points[j][1]))));

    // = cos(50.7)
}

void buildTrain(int v1, int v2){
    int iron_cost = (int) ceil(GLOBAL_TRANSPORT_COST_COEFFICIENT * distance(v1 - 1, v2 - 1) * TRAIN_IRON_COST);
    int copper_cost = (int) ceil(GLOBAL_TRANSPORT_COST_COEFFICIENT * distance(v1 - 1, v2 - 1) * TRAIN_COPPER_COST);
    int machines_cost = (int) ceil(GLOBAL_TRANSPORT_COST_COEFFICIENT * distance(v1 - 1, v2 - 1) * TRAIN_MACHINES_COST);
    int circuits_cost = (int) ceil(GLOBAL_TRANSPORT_COST_COEFFICIENT * distance(v1 - 1, v2 - 1) * TRAIN_CIRCUITS_COST);
    int plastic_cost = (int) ceil(GLOBAL_TRANSPORT_COST_COEFFICIENT * distance(v1 - 1, v2 - 1) * TRAIN_PLASTIC_COST);

    cout << "Postavení takového vlaku vás bude stát " + to_string(iron_cost) +
    " železa, " + to_string(copper_cost) + " měďi, " + to_string(machines_cost) + " strojů, "
     + to_string(circuits_cost) + " obvodů a " + to_string(plastic_cost) + " plastu." << endl;
    if(!debug && (team_stored.iron < iron_cost || team_stored.copper < copper_cost ||
    team_stored.machines < machines_cost || team_stored.circuits < circuits_cost || team_stored.plastic < plastic_cost)){
        cout << "Nemáte dostatek surovin na stavbu." << endl;
        return;
    }

    cout << "Chcete pokračovat? (Y|n)?" << endl;
    string answer;
    cin >> answer;

    if(equals(answer, "Y")){
        team_stored.iron -= iron_cost;
        team_stored.copper -= copper_cost;
        team_stored.machines -= machines_cost;
        team_stored.circuits -= circuits_cost;
        team_stored.plastic -= plastic_cost;

        train_graph[v1 - 1].push_back(v2 - 1);
        train_graph[v2 - 1].push_back(v1 - 1);

        cout << "Vlak úspěšně postaven." << endl;
    }
}

void buildBelt(int from, int to){
    // DFS
    bool vis[points.size()];
    vector<int> dfs;
    dfs.push_back(to - 1);
    fill(vis, vis + points.size(), 0);
    vis[to - 1] = 1;
    while(!dfs.empty()){
        int x = dfs.back();
        dfs.pop_back();

        for(int vert_to: belt_graph[x]){
            if(!vis[vert_to]){
                vis[vert_to] = 1;
                dfs.push_back(vert_to);
            }
        }
    }

    if(vis[from - 1] == 1){
        cout << "Postavením tohoto pásu byste vytvořili cyklus. Zkuste jiné vrcholy." << endl;
        return;
    }
    // Konec DFS

    int iron_cost = (int) ceil(GLOBAL_TRANSPORT_COST_COEFFICIENT * distance(from - 1, to - 1) * BELT_IRON_COST);
    int copper_cost = (int) ceil(GLOBAL_TRANSPORT_COST_COEFFICIENT * distance(from - 1 , to - 1) * BELT_COPPER_COST);

    cout << "Postavení takového pásu vás bude stát " + to_string(iron_cost) +
    " železa a " + to_string(copper_cost) + " měďi." << endl;
    if(!debug && (team_stored.iron < iron_cost || team_stored.copper < copper_cost)){
        cout << "Nemáte dostatek surovin na stavbu." << endl;
        return;
    }

    cout << "Chcete pokračovat? (Y|n)?" << endl;
    string answer;
    cin >> answer;

    if(equals(answer, "Y")){
        team_stored.iron -= iron_cost;
        team_stored.copper -= copper_cost;

        belt_graph[from - 1].push_back(to - 1);
        cout << "Pás úspěšně postaven." << endl;
    }
}

void buildPipe(int v1, int v2){
    int iron_cost = (int) ceil(GLOBAL_TRANSPORT_COST_COEFFICIENT * distance(v1 - 1, v2 - 1) * PIPE_IRON_COST);
    int copper_cost = (int) ceil(GLOBAL_TRANSPORT_COST_COEFFICIENT * distance(v1 - 1 , v2 - 1) * PIPE_COPPER_COST);

    cout << "Postavení takové trubky vás bude stát " + to_string(iron_cost) +
    " železa a " + to_string(copper_cost) + " měďi." << endl;
    if(!debug && (team_stored.iron < iron_cost || team_stored.copper < copper_cost)){
        cout << "Nemáte dostatek surovin na stavbu." << endl;
        return;
    }

    cout << "Chcete pokračovat? (Y|n)?" << endl;
    string answer;
    cin >> answer;

    if(equals(answer, "Y")){
        team_stored.iron -= iron_cost;
        team_stored.copper -= copper_cost;

        pipe_graph[v1 - 1].push_back(v2 - 1);
        pipe_graph[v2 - 1].push_back(v1 - 1);
        cout << "Trubka úspěšně postavena." << endl;
    }
}

vector<string> tokenise(string user_input){
    vector<string> tokens;
    int last_length;

    while(!user_input.empty()){
        last_length = user_input.length();
        tokens.push_back(user_input.substr(0, user_input.find(' ')));
        user_input.erase(0, user_input.find(' ') + 1);
        if(user_input.length() == last_length){
            break;
        }
    }

    return tokens;
}

// Nesahat na mulithreding, pokud nevíte, co děláte, jinak se to rozbije.
string first_char_user_input_read = "";

void *readInput(void* input){
    while(true){
        getline(cin, *static_cast<string*>(input));

        string user_input = *static_cast<string*>(input);

        if(user_input.length() >=  50){
            cout << "Příliš dlouhý vstup" << endl;
            continue;
        }

        string command = user_input.substr(0, user_input.find(' '));
        //cout << command << endl;
        user_input.erase(0, user_input.find(' ') + 1);

        //cout << "KENCR" << endl;
        if(equals(command, "belt")){
            vector<string> tokens = tokenise(user_input);
            if(tokens.size() != 2){
                cout << "Neplatný vstup" << endl;
                continue;
            }

            int from;
            int to;

            try{
                from = stoi(tokens[0]);
                to = stoi(tokens[1]);
            }

            catch(...){
                cout << "Neplatný vstup" << endl;
                continue;
            }

            if(are_distinct_points(from, to)) buildBelt(from, to);
            else{
                cout << "Špatně zadaná čísla bodů." << endl;
                continue;
            }
        }

        if(equals(command, "train")){
            vector<string> tokens = tokenise(user_input);
            if(tokens.size() != 2){
                cout << "Neplatný vstup" << endl;
                continue;
            }

            int v1;
            int v2;

            try{
                v1 = stoi(tokens[0]);
                v2 = stoi(tokens[1]);
            }

            catch(...){
                cout << "Neplatný vstup" << endl;
                continue;
            }

            if(are_distinct_points(v1, v2)) buildTrain(v1, v2);
            else{
                cout << "Špatně zadaná čísla bodů." << endl;
                continue;
            }
        }

        if(equals(command, "pipe")){
            vector<string> tokens = tokenise(user_input);
            if(tokens.size() != 2){
                cout << "Neplatný vstup" << endl;
                continue;
            }

            int v1;
            int v2;

            try{
                v1 = stoi(tokens[0]);
                v2 = stoi(tokens[1]);
            }

            catch(...){
                cout << "Neplatný vstup" << endl;
                continue;
            }

            if(are_distinct_points(v1, v2)) buildPipe(v1, v2);
            else{
                cout << "Špatně zadaná čísla bodů." << endl;
                continue;
            }
        }

        if(equals(command, "stored")){
            cout << "Suroviny k dispozici na základně:" << endl;
            team_stored.print();
        }

        if(equals(command, "prod")){
            cout << "Produkce přicházející na základnu" << endl;
            team_production.print();
        }

        if(equals(command, "skip")){
            vector<string> tokens = tokenise(user_input);
            if(tokens.size() != 1){
                cout << "Neplatný vstup" << endl;
                continue;
            }

            int t;
            try{
                t = stoi(tokens[0]);
            }

            catch(...){
                cout << "Neplatný vstup" << endl;
                continue;
            }

            if(t >= 50000 || t < 0){
                cout << "Skipujete příliš daleko nebo do minulosti." << endl;
                continue;
            }

            for(int i = 0; i<t; i++){
                team_stored.add(team_production);
            }
        }

    }
    //pthread_exit(NULL);
}

int main(){
    cout << "Hra začíná. Za celé politbyro vám přeji hodně štěstí." << endl;

    // Nesahat, jinak se to rozbije.
    chrono::milliseconds timespan(1000);
    string user_input_read = "";
    int rc;
    pthread_t input_thread;
    rc = pthread_create(&input_thread, NULL, readInput, &user_input_read);

    initValues();
    while(1){
        // Jeden tik je jedna vteřina.
        this_thread::sleep_for(timespan);
        ticks_passed++;

        if(debug){
            //cout << "ONE_TICK" << endl;
        }

        team_production = Resources(prod);
        team_production.add(team_basic_production);


        team_stored.add(team_production);
    }
}
