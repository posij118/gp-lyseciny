#include <vector>
#include <math.h>
#include <algorithm>
#include <iostream>
#include <set>
#include <map>
#include <queue>
#include <random>
#include <iomanip>
#include <chrono>
#include <thread>
#include <pthread.h>
#include <signal.h>
using namespace std;

// Debug musí být false.
bool debug = true;

// Playtest musí být false.
bool playtest = true;

// Sem patří obsah souboru input_body.txt.
vector<vector<double> > points = {{50.71275919675827, 15.83528995513916}, {50.71362890303135, 15.834453105926514}, {50.71556478738785, 15.834753513336182}, {50.714654847979546, 15.83778977394104}, {50.71163132786751, 15.839388370513916}, {50.71519799530506, 15.840343236923218}, {50.717955976724625, 15.8352792263031}, {50.72011582553387, 15.835343599319458}, {50.71275249123573, 15.84302544593811}, {50.710062235593796, 15.846115350723267}, {50.709464102983475, 15.841695070266724}, {50.708458945155144, 15.848819017410278}, {50.707819908857346, 15.841180086135864}, {50.70869900286198, 15.833396315574646}, {50.70626690983772, 15.830671191215515}, {50.708671510219574, 15.836893916130066}, {50.70684157311916, 15.838622599840164}, {50.70456974208355, 15.837634205818176}, {50.70372752845287, 15.829394459724426}, {50.70880964398384, 15.824115872383118}, {50.71133695542812, 15.826948285102844}, {50.71190759539604, 15.823858380317688}, {50.71087494492531, 15.821455121040344}, {50.70886395871639, 15.821755528450012}, {50.71071200072765, 15.82930862903595}, {50.704787001013756, 15.825231671333313}, {50.70628233253956, 15.826776623725891}, {50.70397160947323, 15.821455121040344}, {50.706798657774925, 15.819781422615051}, {50.71024999022484, 15.830896496772766}, {50.703537091612816, 15.825360417366028}, {50.70212356746197, 15.827506184577942}, {50.70543944835663, 15.848663449287415}, {50.70484131574631, 15.846303105354309}, {50.713021382689476, 15.840466618537903}, {50.71005955338478, 15.820210576057434}, {50.705086067318916, 15.818536877632141}, {50.70622734725475, 15.841067433357239}, {50.70660822093487, 15.836346745491028}, {50.7031562179327, 15.837677121162415}, {50.70182450115681, 15.829866528511047}, {50.70889078080654, 15.8430415391922}, {50.70902690291405, 15.818365216255188}, {50.717939883470535, 15.838707089424133}, {50.711065381765366, 15.843170285224915}, {50.70707023143768, 15.823429226875305}, {50.70943459868431, 15.828450322151184}, {50.700872987508774, 15.831969380378723}, {50.70019371807575, 15.828106999397278}, {50.697910487651825, 15.830338597297668}};

// Sem patří řešení a místa šifer:
vector<pair<int, string> > lvl1_ciphers = {{1 ,"Prilis"}, {3, "Zlutoucky"}, {14, "Kun"}, {17, "Upel"}};
vector<pair<int, string> > lvl2_ciphers = {{24, "Dabelske"}, {41, "Ody"}};
vector<pair<int, string> > lvl3_ciphers = {{8, "Danilova"}, {26, "Mama"}};

// Sem patří hinty k šifrám:
map<int, string> hints = {{1, "Je to jasný!"},
 {3, "Je to jasný!"},
 {14, "Je to jasný!"},
 {17, "Je to jasný!"},
 {24, "Je to jasný!"},
 {41, "Je to jasný!"},
 {8, "Je to jasný!"},
 {26, "Je to jasný!"}};

class Deposit{
    public:
        char type;    // Jedno z 'i', 'c', 'o'
        string code;    // Náhodný třípísmenný kencr, který zadávají.

        double capacity;
        double speed;
        int level;

        int i;

        Deposit(char type, string code, double capacity, double speed, int level, int i){
            this -> type = type;
            this -> code = code;
            this -> capacity = capacity;
            this -> speed = speed;
            this -> level = level;
            this -> i = i;
        }
};

vector<pair<int, double> > zero_vector = {{}};

template <typename T> class Resources{
    public:
        T iron;
        T copper;
        T oil;

        T machines;
        T circuits;
        T plastic;

        T rocket;
        T boosters;
        T rocket_fuel;

        int time;

        Resources(vector<pair<int, T> > nonzero){
            this -> iron = 0;
            this -> copper = 0;
            this -> oil = 0;
            this -> machines = 0;
            this -> circuits = 0;
            this -> plastic = 0;
            this -> rocket = 0;
            this -> boosters = 0;
            this -> rocket_fuel = 0;
            this -> time = -1;

            for(auto resource: nonzero){
                switch(resource.first){
                    case 0:
                        this -> iron = resource.second;
                        break;
                    case 1:
                        this -> copper = resource.second;
                        break;
                    case 2:
                        this -> oil = resource.second;
                        break;
                    case 3:
                        this -> machines = resource.second;
                        break;
                    case 4:
                        this -> circuits = resource.second;
                        break;
                    case 5:
                        this -> plastic = resource.second;
                        break;
                    case 6:
                        this -> rocket = resource.second;
                        break;
                    case 7:
                        this -> boosters = resource.second;
                        break;
                    case 8:
                        this -> rocket_fuel = resource.second;
                        break;
                    case 9:
                        this -> time = resource.second;
                        break;
                }
            }
        }

        void print(int sp){
            cout << std::fixed << std::setprecision(sp) << "Železo: " << this -> iron << endl;
            cout << std::fixed << std::setprecision(sp) << "Měď: " << this -> copper << endl;
            cout << std::fixed << std::setprecision(sp) << "Ropa: " << this -> oil << endl;
            cout << std::fixed << std::setprecision(sp) << "Stroje: " << this -> machines << endl;
            cout << std::fixed << std::setprecision(sp) << "Obvody: " << this -> circuits << endl;
            cout << std::fixed << std::setprecision(sp) << "Plast: " << this -> plastic << endl;
            cout << std::fixed << std::setprecision(sp) << "Raketa: " << this -> rocket << endl;
            cout << std::fixed << std::setprecision(sp) << "Boostery: " << this -> boosters << endl;
            cout << std::fixed << std::setprecision(sp) << "Raketové palivo: " << this -> rocket_fuel << endl;
        }

        void add(Resources a){
            this -> iron += a.iron;
            this -> copper += a.copper;
            this -> oil += a.oil;
            this -> machines += a.machines;
            this -> circuits += a.circuits;
            this -> plastic += a.plastic;
            this -> rocket += a.rocket;
            this -> boosters += a.boosters;
            this -> rocket_fuel += a.rocket_fuel;
        }

        Resources<T> constant_multiply(double c){
            Resources<T> ret = Resources({
            {0, this -> iron * c},
            {1, this -> copper * c},
            {2, this -> oil * c},
            {3, this -> machines * c},
            {4, this -> circuits * c},
            {5, this -> plastic * c},
            {6, this -> rocket * c},
            {7, this -> boosters * c},
            {8, this -> rocket_fuel * c}});

            return ret;
        }

        void subtract(Resources a){
            add(a.constant_multiply(-1));
        }

        bool less_than(Resources a){
            if(this -> iron < a.iron) return 1;
            if(this -> copper < a.copper) return 1;
            if(this -> oil < a.oil) return 1;
            if(this -> machines < a.machines) return 1;
            if(this -> circuits < a.circuits) return 1;
            if(this -> plastic < a.plastic) return 1;
            if(this -> rocket < a.rocket) return 1;
            if(this -> boosters < a.boosters) return 1;
            if(this -> rocket_fuel < a.rocket_fuel) return 1;

            return 0;
        }

        Resources floor(){
            Resources ret = Resources({
            {0, std::floor(this -> iron)},
            {1, std::floor(this -> copper)},
            {2, std::floor(this -> oil)},
            {3, std::floor(this -> machines)},
            {4, std::floor(this -> circuits)},
            {5, std::floor(this -> plastic)},
            {6, std::floor(this -> rocket)},
            {7, std::floor(this -> boosters)},
            {8, std::floor(this -> rocket_fuel)},
            });
            return ret;
        }
};

class CraftingStation{
    public:
        Resources<double> in = Resources<double>(zero_vector);
        Resources<double> out = Resources<double>(zero_vector);
        vector<pair<int, double> > oil_used;
        bool active = 0;
        int tick_when_starts_producing = -1;
        bool producing = 0;
        string type;
};

// Parametry
int INIT_IRON_STORED = 5000;
int INIT_COPPER_STORED = 5000;
int INIT_OIL_STORED = 0;
int COPPER_PRODUCTION = 1;
int IRON_PRODUCTION = 1;
int OIL_PRODUCTION = 0;

double GLOBAL_TRANSPORT_COST_COEFFICIENT = 80000;

double BELT_IRON_COST = 3;
double BELT_COPPER_COST = 1;

double TRAIN_IRON_COST = 2;
double TRAIN_COPPER_COST = 1;
double TRAIN_MACHINES_COST = 1;
double TRAIN_CIRCUITS_COST = 0.25;
double TRAIN_PLASTIC_COST = 0.25;

double TRAIN_CAPACITY = 10;

double PIPE_COPPER_COST = 3;
double PIPE_IRON_COST = 1;

double IRON_DEPOSIT_1_IRON_COST = 200;
double IRON_DEPOSIT_1_COPPER_COST = 400;
double COPPER_DEPOSIT_1_IRON_COST = 400;
double COPPER_DEPOSIT_1_COPPER_COST = 200;
double OIL_DEPOSIT_1_IRON_COST = 800;
double OIL_DEPOSIT_1_COPPER_COST = 600;

double IRON_DEPOSIT_2_IRON_COST = 2000;
double IRON_DEPOSIT_2_COPPER_COST = 4000;
double COPPER_DEPOSIT_2_IRON_COST = 4000;
double COPPER_DEPOSIT_2_COPPER_COST = 2000;
double OIL_DEPOSIT_2_IRON_COST = 8000;
double OIL_DEPOSIT_2_COPPER_COST = 6000;

double MACHINES_CRAFT_IRON_COST = 5;
double MACHINES_CRAFT_COPPER_COST = 1;
double MACHINES_CRAFT_OIL_COST = 0;
double MACHINES_CRAFT_TIME = 2;

double CIRCUITS_CRAFT_IRON_COST = 1;
double CIRCUITS_CRAFT_COPPER_COST = 5;
double CIRCUITS_CRAFT_OIL_COST = 0;
double CIRCUITS_CRAFT_TIME = 3;

double PLASTIC_CRAFT_IRON_COST = 2;
double PLASTIC_CRAFT_COPPER_COST = 2;
double PLASTIC_CRAFT_OIL_COST = 15;
double PLASTIC_CRAFT_TIME = 5;

double ROCKET_CRAFT_IRON_COST = 80000;
double ROCKET_CRAFT_COPPER_COST = 60000;
double ROCKET_CRAFT_OIL_COST = 0;
double ROCKET_CRAFT_MACHINES_COST = 15000;
double ROCKET_CRAFT_CIRCUITS_COST = 25000;
double ROCKET_CRAFT_PLASTIC_COST = 10000;
double ROCKET_CRAFT_ROCKET_FUEL_COST = 600;
double ROCKET_CRAFT_TIME = 600;

double BOOSTER_CRAFT_IRON_COST = 30000;
double BOOSTER_CRAFT_COPPER_COST = 20000;
double BOOSTER_CRAFT_OIL_COST = 0;
double BOOSTER_CRAFT_MACHINES_COST = 2000;
double BOOSTER_CRAFT_CIRCUITS_COST = 1000;
double BOOSTER_CRAFT_PLASTIC_COST = 100;
double BOOSTER_CRAFT_ROCKET_FUEL_COST = 200;
double BOOSTER_CRAFT_TIME = 1800;

double ROCKET_FUEL_CRAFT_OIL_COST = 1000;
double ROCKET_FUEL_CRAFT_TIME = 60;

double MACHINES_STATION_IRON_COST = 1500;
double MACHINES_STATION_COPPER_COST = 3000;

double CIRCUITS_STATION_IRON_COST = 3000;
double CIRCUITS_STATION_COPPER_COST = 1500;

double PLASTIC_STATION_IRON_COST = 4000;
double PLASTIC_STATION_COPPER_COST = 5000;
double PLASTIC_STATION_MACHINES_COST = 500;

double ROCKET_STATION_IRON_COST = 20000;

double BOOSTER_STATION_IRON_COST = 5000;
double BOOSTER_STATION_COPPER_COST = 5000;
double BOOSTER_STATION_OIL_COST = 0;
double BOOSTER_STATION_MACHINES_COST = 2000;
double BOOSTER_STATION_CIRCUTS_COST = 2000;
double BOOSTER_STATION_PLASTIC_COST = 1000;

double ROCKET_FUEL_STATION_IRON_COST = 3000;
double ROCKET_FUEL_STATION_COPPER_COST = 6000;
double ROCKET_FUEL_STATION_PLASTIC_COST = 1500;

double CRAFTING_UNLOCK_IRON_COST = 5000;
double CRAFTING_UNLOCK_COPPER_COST = 5000;

double FROM_BASE_UNLOCK_IRON_COST = 50000;
double FROM_BASE_UNLOCK_COPPER_COST = 50000;
double LVL2_UNLOCK_IRON_COST = 50000;
double LVL2_UNLOCK_COPPER_COST = 50000;
double LVL2_UNLOCK_MACHINES_COST = 5000;
double LVL2_UNLOCK_CIRCUITS_COST = 5000;
double LVL2_UNLOCK_PLASTIC_COST = 2000;

int SHORT_TIME_WARP = 2400;
int LONG_TIME_WARP = 7200;

vector<Deposit> IRON_DEPOSITS = {
    Deposit('i', "bjc", 60000, 2, 1, 2),
    Deposit('i', "qeb", 35000, 4, 1, 6),
    Deposit('i', "tyz", 40000, 5, 1, 39),
    Deposit('i', "bhk", 60000, 3, 1, 45),
    Deposit('i', "vnw", 60000, 4, 1, 19),
    Deposit('i', "vjc", 50000, 4, 1, 21),
    Deposit('i', "kby", 55000, 6, 1, 46),
    Deposit('i', "sbl", 60000, 30, 2, 7),
    Deposit('i', "mel", 150000, 15, 2, 13),
    Deposit('i', "ucp", 70000, 22, 2, 47),
    Deposit('i', "hrw", 100000, 17, 2, 23),
    Deposit('i', "umj", 180000, 15, 2, 31)
};

vector<Deposit> COPPER_DEPOSITS = {
    Deposit('c', "prt", 50000, 2, 1, 5),
    Deposit('c', "vhn", 30000, 4, 1, 4),
    Deposit('c', "vyx", 30000, 5, 1, 18),
    Deposit('c', "pif", 65000, 3, 1, 15),
    Deposit('c', "urg", 20000, 5, 1, 30),
    Deposit('c', "nqg", 65000, 3, 1, 20),
    Deposit('c', "aql", 30000, 6, 1, 29),
    Deposit('c', "ufi", 130000, 8, 2, 35),
    Deposit('c', "qdl", 100000, 15, 2, 10),
    Deposit('c', "cwm", 120000, 18, 2, 48),
    Deposit('c', "hpl", 250000, 15, 2, 22),
    Deposit('c', "ywq", 200000, 30, 2, 28)
};

vector<Deposit> OIL_DEPOSITS = {
    Deposit('o', "kne", 200000, 6, 1, 16),
    Deposit('o', "ipf", 160000, 10, 1, 44),
    Deposit('o', "bzs", 120000, 12, 1, 38),
    Deposit('o', "def", 120000, 14, 1, 27),
    Deposit('o', "tml", 240000, 8, 1, 25),
    Deposit('o', "vvu", 750000, 50, 2, 9),
    Deposit('o', "lxy", 1000000, 60, 2, 12),
    Deposit('o', "lxn", 1000000, 70, 2, 40),
    Deposit('o', "lqa", 750000, 120, 2, 49),
    Deposit('o', "lae", 1000000, 90, 2, 37)
};

set<string> commands = {"mine", "belt", "train", "pipe", "make", "build", "stored", "skip", "prod", "ratio", "all", "hint", "unlock", "time"};
vector<string> station_names = {"mach", "circ", "plas", "rock", "boos", "fuel"};

// Inicializace objektů surovin
vector<pair<int, double> > st_i = {make_pair(0, INIT_IRON_STORED), make_pair(1, INIT_COPPER_STORED), make_pair(2, INIT_OIL_STORED)};
vector<pair<int, double> > basic_prod_i = {make_pair(0, (double)IRON_PRODUCTION), make_pair(1, (double)COPPER_PRODUCTION), make_pair(2, (double)OIL_PRODUCTION)};
Resources<double> team_basic_production = Resources(basic_prod_i);
vector<pair<int, double> > belt_i = {{0, BELT_IRON_COST}, {1, BELT_COPPER_COST}};
Resources<double> belt_cost = Resources(belt_i);
vector<pair<int, double> > train_i = {{0, TRAIN_IRON_COST}, {1, TRAIN_COPPER_COST}, {3, TRAIN_MACHINES_COST}, {4, TRAIN_CIRCUITS_COST}, {5, TRAIN_PLASTIC_COST}};
Resources<double> train_cost = Resources(train_i);
vector<pair<int, double> > pipe_i = {{0, PIPE_IRON_COST}, {1, PIPE_COPPER_COST}};
Resources<double> pipe_cost = Resources(pipe_i);
vector<pair<int, double> > iron_deposit_1_i = {{0, IRON_DEPOSIT_1_IRON_COST}, {1, IRON_DEPOSIT_1_COPPER_COST}};
Resources<double> iron_deposit_1_cost = Resources(iron_deposit_1_i);
vector<pair<int, double> > copper_deposit_1_i = {{0, COPPER_DEPOSIT_1_IRON_COST}, {1, COPPER_DEPOSIT_1_COPPER_COST}};
Resources<double> copper_deposit_1_cost = Resources(copper_deposit_1_i);
vector<pair<int, double> > oil_deposit_1_i = {{0, OIL_DEPOSIT_1_IRON_COST}, {1, OIL_DEPOSIT_1_COPPER_COST}};
Resources<double> oil_deposit_1_cost = Resources(oil_deposit_1_i);
vector<pair<int, double> > iron_deposit_2_i = {{0, IRON_DEPOSIT_2_IRON_COST}, {1, IRON_DEPOSIT_2_COPPER_COST}};
Resources<double> iron_deposit_2_cost = Resources(iron_deposit_2_i);
vector<pair<int, double> > copper_deposit_2_i = {{0, COPPER_DEPOSIT_2_IRON_COST}, {1, COPPER_DEPOSIT_2_COPPER_COST}};
Resources<double> copper_deposit_2_cost = Resources(copper_deposit_2_i);
vector<pair<int, double> > oil_deposit_2_i = {{0, OIL_DEPOSIT_2_IRON_COST}, {1, OIL_DEPOSIT_2_COPPER_COST}};
Resources<double> oil_deposit_2_cost = Resources(oil_deposit_2_i);
vector<pair<int, double> > to_win_i = {{6, 1}, {7, 2}};
Resources<double> to_win = Resources(to_win_i);

vector<pair<int, double> > machines_craft_in_i = {{0, MACHINES_CRAFT_IRON_COST}, {1, MACHINES_CRAFT_COPPER_COST}, {2, MACHINES_CRAFT_OIL_COST}, {9, MACHINES_CRAFT_TIME}};
vector<pair<int, double> > circuits_craft_in_i = {{0, CIRCUITS_CRAFT_IRON_COST}, {1, CIRCUITS_CRAFT_COPPER_COST}, {2, CIRCUITS_CRAFT_OIL_COST}, {9, CIRCUITS_CRAFT_TIME}};
vector<pair<int, double> > plastic_craft_in_i = {{0, PLASTIC_CRAFT_IRON_COST}, {1, PLASTIC_CRAFT_COPPER_COST}, {2, PLASTIC_CRAFT_OIL_COST}, {9, PLASTIC_CRAFT_TIME}};
vector<pair<int, double> > rocket_craft_in_i = {{0, ROCKET_CRAFT_IRON_COST}, {1, ROCKET_CRAFT_COPPER_COST}, {2, ROCKET_CRAFT_OIL_COST}, {3, ROCKET_CRAFT_MACHINES_COST}, {4, ROCKET_CRAFT_CIRCUITS_COST}, {5, ROCKET_CRAFT_PLASTIC_COST}, {8, ROCKET_CRAFT_ROCKET_FUEL_COST}, {9, ROCKET_CRAFT_TIME}};
vector<pair<int, double> > booster_craft_in_i = {{0, BOOSTER_CRAFT_IRON_COST}, {1, BOOSTER_CRAFT_COPPER_COST}, {2, BOOSTER_CRAFT_OIL_COST}, {3, BOOSTER_CRAFT_MACHINES_COST}, {4, BOOSTER_CRAFT_CIRCUITS_COST}, {5, BOOSTER_CRAFT_PLASTIC_COST}, {8, BOOSTER_CRAFT_ROCKET_FUEL_COST}, {9, BOOSTER_CRAFT_TIME}};
vector<pair<int, double> > rocket_fuel_craft_in_i = {{2, ROCKET_FUEL_CRAFT_OIL_COST}, {9, ROCKET_FUEL_CRAFT_TIME}};

vector<pair<int, double> > craft_unlock_i = {{0, CRAFTING_UNLOCK_IRON_COST}, {1, CRAFTING_UNLOCK_COPPER_COST}};
Resources<double> craft_unlock_cost = Resources(craft_unlock_i);
vector<pair<int, double> > from_base_unlock_i = {{0, FROM_BASE_UNLOCK_IRON_COST}, {1, FROM_BASE_UNLOCK_COPPER_COST}};
Resources<double> from_base_unlock_cost = Resources(from_base_unlock_i);
vector<pair<int, double> > lvl2_unlock_i = {{0, LVL2_UNLOCK_IRON_COST}, {1, LVL2_UNLOCK_COPPER_COST}, {3, LVL2_UNLOCK_MACHINES_COST}, {4, LVL2_UNLOCK_CIRCUITS_COST}, {5, LVL2_UNLOCK_PLASTIC_COST}};
Resources<double> lvl2_unlock_cost = Resources(lvl2_unlock_i);

// Pomocné proměnné

vector<Deposit> all_deposits(IRON_DEPOSITS);  // Neukládají se změny během jednoho tiku
Resources<double> team_production = Resources(zero_vector);
Resources<double> team_average_production(zero_vector);
Resources<double> prod_from_belts = Resources(zero_vector);
Resources<double> prod_from_trains = Resources(zero_vector);
pair<Resources<double>, Resources<double> > prod_from_craft = {Resources(zero_vector), Resources(zero_vector)};

CraftingStation crafting_station_blueprints[6];

// Proměnné zachycující současný stav

Resources<double> team_stored = Resources(st_i);

vector<vector<pair<int, int> > > belt_graph(points.size());  // vrchol, id hrany
vector<vector<int> > train_graph(points.size());
vector<vector<int> > pipe_graph(points.size());

vector<Deposit> iron_deposits_activated; // Neukládají se změny během jednoho tiku
vector<Deposit> copper_deposits_activated;
vector<Deposit> oil_deposits_activated;
vector<Deposit> all_deposits_activated;
vector<Deposit> all_deposits_activated_with_changes;
map<int, int> indices_in_all_deposits_by_index; // Ukládají se změny během jednoho tiku

int ticks_passed = 0;
int belt_edge_id = 0;
int total_deposits_activated = 0;

vector<CraftingStation> all_crafting_stations;

double iron_copper_constant = 1;
string oil_priority = "mine"; // "craft" nebo "mine"

bool lvl2_deposits_unlocked = 0;
bool lvl1_deposit_build_from_base = 0;
bool crafting_unlocked = 0;
bool lvl2_ciphers_unlocked = 0;
bool lvl3_ciphers_unlocked = 0;
vector<pair<pair<int, string>, pair<int, bool> > > active_ciphers; // index, sol, lvl, hint_shown
int left_level = 0;
int right_level = 0;

bool first_step = 0;
bool second_step = 0;

// Pomocné funkce

string tolower(string s){
    for(int i = 0; i<s.size(); i++){
        if(s[i] <= 'Z' && s[i] >= 'A') s[i] += 'a' - 'A';
    }

    return s;
}

bool equals(string s, string t){
    if(s.length() != t.length()) return 0;
    if(tolower(s) == tolower(t)) return 1;
    return 0;
}

bool areDistinctPoints(int i, int j){
    if(1 <= i && i <= points.size() && 1 <= j && j <= points.size() && i != j) return 1;
    else return 0;
}

double distance(int i, int j){
    return sqrt(((points[i][0] - points[j][0])*(points[i][0] - points[j][0])
      + (0.63338087263 *(points[i][1] - points[j][1]))*(0.63338087263 *(points[i][1] - points[j][1]))));

    // = cos(50.7)
}

void buildCraftingStation(string type){
    Resources<double> cost = Resources(zero_vector);
    int index_in_blueprints;

    if(type == "mach"){
        cost = Resources<double>({{0, MACHINES_STATION_IRON_COST}, {1, MACHINES_STATION_COPPER_COST}});
        index_in_blueprints = 0;
    }
    else if(type == "circ"){
        cost = Resources<double>({{0, CIRCUITS_STATION_IRON_COST}, {1, CIRCUITS_STATION_COPPER_COST}});
        index_in_blueprints = 1;
    }
    else if(type == "plas"){
        cost = Resources<double>({{0, PLASTIC_STATION_IRON_COST}, {1, PLASTIC_STATION_COPPER_COST}, {3, PLASTIC_STATION_MACHINES_COST}});
        index_in_blueprints = 2;
    }
    else if(type == "rock"){
        cost = Resources<double>({{0, ROCKET_STATION_IRON_COST}});
        index_in_blueprints = 3;
    }
    else if(type == "boos"){
        cost = Resources<double>({{0, BOOSTER_STATION_IRON_COST}, {1, BOOSTER_STATION_COPPER_COST}, {2, BOOSTER_STATION_OIL_COST}, {3, BOOSTER_STATION_MACHINES_COST}, {4, BOOSTER_STATION_CIRCUTS_COST}, {5, BOOSTER_STATION_PLASTIC_COST}});
        index_in_blueprints = 4;
    }
    else if(type == "fuel"){
        cost = Resources<double>({{0, ROCKET_FUEL_STATION_IRON_COST}, {1, ROCKET_FUEL_STATION_COPPER_COST}, {5, ROCKET_FUEL_STATION_PLASTIC_COST}});
        index_in_blueprints = 5;
    }
    else{
        cout << "BUG 2" << endl;
        return;
    }

    if(debug || !team_stored.less_than(cost)){
        crafting_station_blueprints[index_in_blueprints].type = type;
        all_crafting_stations.push_back(crafting_station_blueprints[index_in_blueprints]);
        team_stored.subtract(cost);
        cout << "Craftící stanice úspěšně postavena" << endl;
    }

    else{
        cout << "Nemáte dostatek surovin na stavbu." << endl;
        return;
    }
}

void buildTrain(int v1, int v2){
    Resources<double> cost = train_cost.constant_multiply(GLOBAL_TRANSPORT_COST_COEFFICIENT * distance(v1 - 1, v2 - 1)).floor();

    cout << "Postavení takového vlaku vás bude stát:" << endl;
    cost.print(0);

    if(!debug && team_stored.less_than(cost)){
        cout << "Nemáte dostatek surovin na stavbu." << endl;
        return;
    }

    cout << "Chcete pokračovat? (Y|n)?" << endl;
    string answer;
    getline(cin, answer);

    if(equals(answer, "Y")){
        team_stored.subtract(cost);

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

        for(pair<int, int> vert_to: belt_graph[x]){
            if(vert_to.first >= 0){
                if(!vis[vert_to.first]){
                    vis[vert_to.first] = 1;
                    dfs.push_back(vert_to.first);
                }
            }
        }
    }

    if(vis[from - 1] == 1){
        cout << "Postavením tohoto pásu byste vytvořili cyklus. Zkuste jiné vrcholy." << endl;
        return;
    }
    // Konec DFS

    Resources<double> cost = belt_cost.constant_multiply(GLOBAL_TRANSPORT_COST_COEFFICIENT * distance(from - 1, to - 1));

    cout << "Postavení takového pásu vás bude stát:" << endl;
    cost.print(0);

    if(!debug && team_stored.less_than(cost)){
        cout << "Nemáte dostatek surovin na stavbu." << endl;
        return;
    }

    cout << "Chcete pokračovat? (Y|n)?" << endl;
    string answer;
    getline(cin, answer);

    if(equals(answer, "Y")){
        team_stored.subtract(cost);
        belt_edge_id++;

        belt_graph[from - 1].push_back({to - 1, belt_edge_id});
        belt_graph[to - 1].push_back({from - 1 - points.size(), belt_edge_id});
        cout << "Pás úspěšně postaven." << endl;
    }
}

void buildPipe(int v1, int v2){
    Resources<double> cost = pipe_cost.constant_multiply(GLOBAL_TRANSPORT_COST_COEFFICIENT * distance(v1 - 1, v2 - 1));

    cout << "Postavení takové trubky vás bude stát:" << endl;
    cost.print(0);

    if(!debug && team_stored.less_than(cost)){
        cout << "Nemáte dostatek surovin na stavbu." << endl;
        return;
    }

    cout << "Chcete pokračovat? (Y|n)?" << endl;
    string answer;
    getline(cin, answer);

    if(equals(answer, "Y")){
        team_stored.subtract(cost);

        pipe_graph[v1 - 1].push_back(v2 - 1);
        pipe_graph[v2 - 1].push_back(v1 - 1);
        cout << "Trubka úspěšně postavena." << endl;
    }
}

bool activateDeposit(Deposit deposit){
    Resources<double> cost = iron_deposit_1_cost;
    if(deposit.type == 'i' && deposit.level == 2) cost = iron_deposit_2_cost;
    if(deposit.type == 'c' && deposit.level == 1) cost = copper_deposit_1_cost;
    if(deposit.type == 'c' && deposit.level == 2) cost = copper_deposit_2_cost;
    if(deposit.type == 'o' && deposit.level == 1) cost = oil_deposit_1_cost;
    if(deposit.type == 'o' && deposit.level == 2) cost = oil_deposit_2_cost;

    if(!debug && team_stored.less_than(cost)){
        cout << "Nemáte dostatek surovin na stavbu." << endl;
        return 0;
    }

    team_stored.subtract(cost);
    if(deposit.type == 'i') iron_deposits_activated.push_back(deposit);
    if(deposit.type == 'c') copper_deposits_activated.push_back(deposit);
    if(deposit.type == 'o') oil_deposits_activated.push_back(deposit);
    all_deposits_activated.push_back(deposit);
    all_deposits_activated_with_changes.push_back(deposit);
    indices_in_all_deposits_by_index[deposit.i] = total_deposits_activated;
    total_deposits_activated++;
    return 1;
}

Resources<double> produceFromBelts(){
    Resources<double> belt_prod = Resources(zero_vector);

    map<int, queue<int> > edges;
    for(int i = 0; i<points.size(); i++){
        for(pair<int, int> to: belt_graph[i]){
            edges[to.second] = {};
        }
    }

    int NUM_ITER = 1000;
    int cnt_at_base[points.size()];
    fill(cnt_at_base, cnt_at_base + points.size(), 0);

    for(int i = 0; i<NUM_ITER; i++){
        for(int j = 0; j<points.size(); j++){
            vector<pair<int, int> > ingoing_res;
            vector<int> outgoing_edg;

            for(pair<int, int> to: belt_graph[j]){
                if(to.first >= 0 && edges[to.second].size() < 50) outgoing_edg.push_back(to.second);
                if(to.first < 0 && !edges[to.second].empty())
                 ingoing_res.push_back(make_pair(to.second, edges[to.second].front()));
            }

            if(indices_in_all_deposits_by_index.count(j + 1)){
                for(int k = 0; k<floor(min(all_deposits_activated_with_changes[indices_in_all_deposits_by_index[j + 1]].speed,
                 all_deposits_activated_with_changes[indices_in_all_deposits_by_index[j + 1]].capacity)); k++){
                    ingoing_res.push_back(make_pair(-1, j));
                }
            }

            while(ingoing_res.size() && (outgoing_edg.size() || j == 0)){
                int rand_res = rand() % ingoing_res.size();
                int rand_edg;
                int edg;

                if(j != 0) rand_edg = rand() % outgoing_edg.size();

                pair<int, int> res = ingoing_res[rand_res];
                int sz = ingoing_res.size();
                if(j == 0 && ingoing_res.size() == 2){
                    //cout << "os " << res.second << " " << outgoing_edg.size() << endl;
                }
                //cout << j << " " << res.second << endl;
                if(j != 0) edg = outgoing_edg[rand_edg];
                if(j == 0) cnt_at_base[res.second]++;

                ingoing_res.erase(ingoing_res.begin() + rand_res);
                if(j != 0) outgoing_edg.erase(outgoing_edg.begin() + rand_edg);

                if(sz == 2 && j == 0){
                    //cout << "sizes " << ingoing_res.size() << " " << outgoing_edg.size() << endl;
                }

                if(j != 0) edges[edg].push(res.second);
                if(res.first != -1){
                    edges[res.first].pop();
                }
            }

            ingoing_res.clear();
            outgoing_edg.clear();
        }
    }

    edges.clear();

    for(int i = 0; i<points.size(); i++){
        if(cnt_at_base[i] != 0){
            all_deposits_activated_with_changes[indices_in_all_deposits_by_index[i + 1]].capacity -= (double)cnt_at_base[i] / NUM_ITER;
            all_deposits_activated_with_changes[indices_in_all_deposits_by_index[i + 1]].speed -= (double)cnt_at_base[i] / NUM_ITER;
            if(all_deposits_activated_with_changes[indices_in_all_deposits_by_index[i + 1]].type == 'i') belt_prod.add(Resources<double>({{0, (double)cnt_at_base[i] / NUM_ITER}}));
            if(all_deposits_activated_with_changes[indices_in_all_deposits_by_index[i + 1]].type == 'c') belt_prod.add(Resources<double>({{1, (double)cnt_at_base[i] / NUM_ITER}}));
        }
    }

    return belt_prod;
}

void craftAmount(int num_to_be_crafted, string type){
    int num_crafted = 0;
    Resources<double> cost_required = Resources<double>(zero_vector);

    for(CraftingStation &crafting_station: all_crafting_stations){
        if(crafting_station.type == type && num_crafted < num_to_be_crafted){
            if(!crafting_station.active){
                crafting_station.active = 1;
                num_crafted++;
            }
        }

        else if(crafting_station.type == type){
            if(crafting_station.producing){
                cost_required.subtract(crafting_station.in);
                cost_required.add(Resources<double>({{2, crafting_station.in.oil}}));
            }

            crafting_station.active = 0;
            crafting_station.producing = 0;
        }
    }

    team_stored.subtract(cost_required);
    cout << "Craftění úspěšně zahájeno / ukončeno" << endl;
}

// min cost max flow

const long long INF = 1000*1000*1000;

struct rib {
	int b;
	long long u, c, f; // Hrana u a, b - to, u - kapacita, c - cost, f - tok
	size_t back;
};

void add_rib (vector < vector<rib> > & g, int a, int b, double u, double c) {
	rib r1 = { b, (long long) floor(u*INF), (long long) floor(c*INF), 0, g[b].size() };
	rib r2 = { a, 0, (long long) floor(-c*INF), 0, g[a].size() };
	g[a].push_back (r1);
	g[b].push_back (r2);
}

pair<double, double> minCostMaxFlow(long long k, int s, int t, vector< vector<rib> > &g){
	int n = g.size();
	long long flow = 0;
	long long cost = 0;

	while (flow < k) {
		vector<long long> id (n, 0);
		vector<long long> d (n, INF*INF);
		vector<int> q (n);
		vector<int> p (n);
		vector<size_t> p_rib (n);
		int qh=0, qt=0;
		q[qt++] = s;
		d[s] = 0;
		while (qh != qt) {
			int v = q[qh++];
			id[v] = 2;
			if (qh == n)  qh = 0;
			for (size_t i=0; i<g[v].size(); ++i) {
				rib & r = g[v][i];
				if (r.f < r.u && d[v] + r.c < d[r.b]) {
					d[r.b] = d[v] + r.c;
					if (id[r.b] == 0) {
						q[qt++] = r.b;
						if (qt == n)  qt = 0;
					}
					else if (id[r.b] == 2) {
						if (--qh == -1)  qh = n-1;
						q[qh] = r.b;
					}
					id[r.b] = 1;
					p[r.b] = v;
					p_rib[r.b] = i;
				}
			}
		}
		if (d[t] == INF*INF)  break;
		long long addflow = k - flow;
		for (int v=t; v!=s; v=p[v]) {
			int pv = p[v];  size_t pr = p_rib[v];
			addflow = min (addflow, g[pv][pr].u - g[pv][pr].f);
		}
		for (int v=t; v!=s; v=p[v]) {
			int pv = p[v];  size_t pr = p_rib[v],  r = g[pv][pr].back;
			g[pv][pr].f += addflow;
			g[v][r].f -= addflow;
			cost += g[pv][pr].c * addflow;
		}
		flow += addflow;
    }
    /*
    for(int i = 0; i<g.size(); i++){
        for(rib j: g[i]){
            if(j.f > 0) cout << i << " " << j.b << " " << j.u << " " << j.c << " " << j.f << endl;
        }
	}
	*/

    return make_pair((double)flow / INF, (double)cost / INF);
}

vector<vector<int> > findPipeComponents(){
    vector<vector<int> > comps;
    vector<int> dfs;
    int vis[points.size()];
    fill(vis, vis + points.size(), 0);

    for(int i = 0; i<points.size(); i++){
        if(!vis[i]){
            vector<int> comp;
            vis[i] = 1;
            dfs.push_back(i);
            comp.push_back(i);

            while(!dfs.empty()){
                int x = dfs.back();
                dfs.pop_back();
                for(int to: pipe_graph[x]){
                    if(!vis[to]){
                        vis[to] = 1;
                        dfs.push_back(to);
                        comp.push_back(to);
                    }
                }
            }

            comps.push_back(comp);
        }
    }

    return comps;
}

Resources<double> produceFromTrains(){
    vector<vector<int> > pipe_comps = findPipeComponents();
    vector<vector<pair<pair<pair<double, double>, pair<double, double> >, int> > > pipe_comps_detailed;

    vector<vector<rib> > g(points.size() + 1 + pipe_comps.size());
    for(int i = 0; i<points.size(); i++){
        for(int to: train_graph[i]){
            add_rib(g, i, to, TRAIN_CAPACITY, 0);
        }
    }

    for(int i = 0; i<pipe_comps.size(); i++){
        vector<int> pipe_comp = pipe_comps[i];

        vector<pair<pair<pair<double, double>, pair<double, double> >, int> > pipe_comp_detailed;
        double total_oil_speed = 0;

        // oil cap, oil speed, other cap, other speed, index

        for(int index: pipe_comp){
            if(indices_in_all_deposits_by_index.count(index + 1)){
                if(all_deposits_activated_with_changes[indices_in_all_deposits_by_index[index + 1]].type == 'o'){
                     pipe_comp_detailed.push_back({{{
                     all_deposits_activated_with_changes[indices_in_all_deposits_by_index[index + 1]].capacity,
                     min(all_deposits_activated_with_changes[indices_in_all_deposits_by_index[index + 1]].capacity,
                     all_deposits_activated_with_changes[indices_in_all_deposits_by_index[index + 1]].speed)}, {0, 0}}, index});

                     total_oil_speed += min(all_deposits_activated_with_changes[indices_in_all_deposits_by_index[index + 1]].capacity,
                     all_deposits_activated_with_changes[indices_in_all_deposits_by_index[index + 1]].speed);
                }

                else{

                     pipe_comp_detailed.push_back({{{0, 0}, {
                     all_deposits_activated_with_changes[indices_in_all_deposits_by_index[index + 1]].capacity,
                     min(all_deposits_activated_with_changes[indices_in_all_deposits_by_index[index + 1]].capacity,
                     all_deposits_activated_with_changes[indices_in_all_deposits_by_index[index + 1]].speed)}}, index});

                     if(all_deposits_activated_with_changes[indices_in_all_deposits_by_index[index + 1]].type == 'i'){
                         add_rib(g, points.size() + i + 1, index, min(all_deposits_activated_with_changes[indices_in_all_deposits_by_index[index + 1]].capacity,
                         all_deposits_activated_with_changes[indices_in_all_deposits_by_index[index + 1]].speed),
                         iron_copper_constant / all_deposits_activated_with_changes[indices_in_all_deposits_by_index[index + 1]].capacity);
                     }

                     else{
                         add_rib(g, points.size() + i + 1, index, min(all_deposits_activated_with_changes[indices_in_all_deposits_by_index[index + 1]].capacity,
                         all_deposits_activated_with_changes[indices_in_all_deposits_by_index[index + 1]].speed),
                         1/ (iron_copper_constant * all_deposits_activated_with_changes[indices_in_all_deposits_by_index[index + 1]].capacity));
                     }
                }
            }

            else pipe_comp_detailed.push_back({{{0, 0}, {0, 0}}, index});
        }

        sort(pipe_comp_detailed.begin(), pipe_comp_detailed.end());
        reverse(pipe_comp_detailed.begin(), pipe_comp_detailed.end());

        pipe_comps_detailed.push_back(pipe_comp_detailed);

        add_rib(g, points.size(), points.size() + i + 1, total_oil_speed, 0);
    }

    pair<double, double> max_flow_result;
    max_flow_result = minCostMaxFlow(INF*INF, points.size(), 0, g);

    double flow, cost;
    flow = max_flow_result.first;
    cost = max_flow_result.second;

    double train_iron_production = 0;
    double train_copper_production = 0;

    for(int j = 0; j<pipe_comps.size(); j++){
        vector<int> pipe_comp = pipe_comps[j];
        vector<pair<pair<pair<double, double>, pair<double, double> >, int> > pipe_comp_detailed = pipe_comps_detailed[j];

        double iron_production = 0;
        double copper_production = 0;

        for(int i: pipe_comp){
            for(rib edge: g[i]){
                if(edge.b > points.size()){
                    all_deposits_activated_with_changes[indices_in_all_deposits_by_index[i + 1]].capacity -= (double)-edge.f / INF;
                    all_deposits_activated_with_changes[indices_in_all_deposits_by_index[i + 1]].speed -= (double)-edge.f / INF;

                    if(all_deposits_activated_with_changes[indices_in_all_deposits_by_index[i + 1]].type == 'i'){
                        iron_production += (double)-edge.f / INF;
                    }

                    else{
                        copper_production += (double)-edge.f / INF;
                    }
                }
            }
        }

        double total_needed_oil = iron_production + copper_production;

        for(auto vert: pipe_comp_detailed){
            double cur_used_oil;
            if(indices_in_all_deposits_by_index.count(vert.second + 1)){
                cur_used_oil = min(min(vert.first.first.first, vert.first.first.second), total_needed_oil);
            }

            else cur_used_oil = 0;

            if(cur_used_oil > 0){
                all_deposits_activated_with_changes[indices_in_all_deposits_by_index[vert.second + 1]].capacity -= cur_used_oil;
                all_deposits_activated_with_changes[indices_in_all_deposits_by_index[vert.second + 1]].speed -= cur_used_oil;
                total_needed_oil -= cur_used_oil;
            }
        }

        if(total_needed_oil > 0) cout << "BUG" << endl;

        train_iron_production += iron_production;
        train_copper_production += copper_production;
    }

    //cout << train_iron_production << " " << train_copper_production << endl;
    // if(flow > 0) cout << std::fixed << std::setprecision(10) << flow << " " << cost << endl;
    Resources<double> train_prod = Resources<double>({{0, train_iron_production}, {1, train_copper_production}});
    return train_prod;
}

pair<Resources<double>, Resources<double> > produceFromCrafting(){
    Resources<double> craft_prod_average = Resources(zero_vector);
    Resources<double> craft_prod_cur = Resources(zero_vector);

    vector<int> random_indices(all_crafting_stations.size());
    iota(random_indices.begin(), random_indices.end(), 0);

    for(CraftingStation &crafting_station: all_crafting_stations){
        if(crafting_station.active){
            craft_prod_average.add(crafting_station.out.constant_multiply((double)1 / crafting_station.in.time));
            craft_prod_average.subtract(crafting_station.in.constant_multiply((double)1 / crafting_station.in.time));

            if(crafting_station.producing && ((crafting_station.tick_when_starts_producing + crafting_station.in.time) <= ticks_passed)){
                crafting_station.producing = 0;
                craft_prod_cur.add(crafting_station.out);

                for(auto oil_deposit: crafting_station.oil_used){
                    all_deposits_activated[indices_in_all_deposits_by_index[oil_deposit.first + 1]].speed += oil_deposit.second;
                }
            }
        }
    }

    for(int i: random_indices){
        if(all_crafting_stations[i].active && !all_crafting_stations[i].producing){
            craft_prod_cur.subtract(all_crafting_stations[i].in);
            craft_prod_cur.add(Resources<double>({{2, all_crafting_stations[i].in.oil}}));

            if(!debug && team_stored.less_than(craft_prod_cur.constant_multiply(-1))){
                craft_prod_cur.add(all_crafting_stations[i].in);
                craft_prod_cur.subtract(Resources<double>({{2, all_crafting_stations[i].in.oil}}));
                continue;
            }

            double oil_required = all_crafting_stations[i].in.oil / all_crafting_stations[i].in.time;
            double oil_have_total = 0;
            double oil_have_cur = 0;
            int comp_with_base;

            vector<vector<int> > pipe_components = findPipeComponents();
            for(int comp = 0; comp<pipe_components.size(); comp++){
                for(int j: pipe_components[comp]){
                    if(j == 0) comp_with_base = comp;
                }
            }

            vector<pair<double, pair<double, int> > > oil_have; // Capacity, speed, index

            for(int j: pipe_components[comp_with_base]){
                if(indices_in_all_deposits_by_index.count(j+1)){
                    if(all_deposits_activated[indices_in_all_deposits_by_index[j + 1]].type == 'o'){
                        oil_have.push_back({all_deposits_activated_with_changes[indices_in_all_deposits_by_index[j +1]].capacity,
                        {min(all_deposits_activated_with_changes[indices_in_all_deposits_by_index[j +1]].capacity /
                        all_crafting_stations[i].in.time,
                        all_deposits_activated_with_changes[indices_in_all_deposits_by_index[j +1]].speed), j}});

                        oil_have_total += min(all_deposits_activated_with_changes[indices_in_all_deposits_by_index[j +1]].capacity /
                        all_crafting_stations[i].in.time,
                        all_deposits_activated_with_changes[indices_in_all_deposits_by_index[j +1]].speed);
                    }
                }
            }

            sort(oil_have.begin(), oil_have.end());
            reverse(oil_have.begin(), oil_have.end());

            vector<pair<int, double> > oil_used;
            for(auto oil_deposit: oil_have){
                oil_used.push_back({oil_deposit.second.second, min(oil_required - oil_have_cur, oil_deposit.second.first)});
                oil_have_cur = min(oil_required, oil_have_cur + oil_deposit.second.first);
            }

            if(oil_have_total >= oil_required){
                all_crafting_stations[i].producing = 1;
                all_crafting_stations[i].oil_used = oil_used;
                all_crafting_stations[i].tick_when_starts_producing = ticks_passed;

                for(auto oil_deposit: oil_used){
                    all_deposits_activated_with_changes[indices_in_all_deposits_by_index[oil_deposit.first + 1]].speed
                     -= oil_deposit.second;
                    all_deposits_activated[indices_in_all_deposits_by_index[oil_deposit.first + 1]].speed
                     -= oil_deposit.second;

                    all_deposits_activated_with_changes[indices_in_all_deposits_by_index[oil_deposit.first + 1]].capacity
                     -= oil_deposit.second * all_crafting_stations[i].in.time;
                    all_deposits_activated[indices_in_all_deposits_by_index[oil_deposit.first + 1]].capacity
                     -= oil_deposit.second * all_crafting_stations[i].in.time;
                }
            }

            else{
                craft_prod_cur.add(all_crafting_stations[i].in);
                craft_prod_cur.subtract(Resources<double>({{2, all_crafting_stations[i].in.oil}}));
            }
        }
    }

    return {craft_prod_average, craft_prod_cur};
}

void updateCapacities(){
    for(auto &index_deposit: indices_in_all_deposits_by_index){
        for(Deposit &deposit: iron_deposits_activated){
            if(deposit.i == index_deposit.first) deposit.capacity = all_deposits_activated_with_changes[index_deposit.second].capacity;
        }

        for(Deposit &deposit: copper_deposits_activated){
            if(deposit.i == index_deposit.first) deposit.capacity = all_deposits_activated_with_changes[index_deposit.second].capacity;
        }

        for(Deposit &deposit: oil_deposits_activated){
            if(deposit.i == index_deposit.first) deposit.capacity = all_deposits_activated_with_changes[index_deposit.second].capacity;
        }

        for(Deposit &deposit: all_deposits_activated){
            if(deposit.i == index_deposit.first){
                deposit.capacity = all_deposits_activated_with_changes[index_deposit.second].capacity;
                all_deposits_activated_with_changes[index_deposit.second].speed = deposit.speed;
            }
        }
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

// main funkce .................................//

void doMainStuff(){
    ticks_passed++;
    if(debug){
        //cout << "ONE_TICK" << endl;
    }

    Resources<double> team_cur_production = Resources(zero_vector);

    team_cur_production.add(team_basic_production);

    prod_from_belts = produceFromBelts();
    team_cur_production.add(prod_from_belts);

    Resources<double> team_cur_average_production = team_cur_production;
    Resources<double> cur_prod_from_craft = prod_from_craft.second;
    Resources<double> average_prod_from_craft = prod_from_craft.first;

    if(oil_priority == "mine"){
        prod_from_trains = produceFromTrains();
        prod_from_craft = produceFromCrafting();
    }

    if(oil_priority == "craft"){
        prod_from_trains = produceFromTrains();
        prod_from_craft = produceFromCrafting();
    }

    //prod_from_belts.print(1);
    //prod_from_trains.print(1);

    team_cur_production.add(prod_from_trains);
    team_cur_production.add(cur_prod_from_craft);
    team_cur_average_production.add(prod_from_trains);
    team_cur_average_production.add(average_prod_from_craft);

    team_production = team_cur_production;
    team_average_production = team_cur_average_production;

    team_stored.add(team_production);
    if(!team_stored.less_than(to_win)){
        cout << "Gratulujeme, postavili jste raketu a vyhráli jste tuto hru." << endl;
        cout << "Trvalo vám to přesně " << ticks_passed << " sekund." << endl;
        cout << "Politbyro vám tímto nařizuje jít spát." << endl;
    }

    updateCapacities();
}

void* doStuffForSomeTime(void* t){
    for(int i = 0; i<*static_cast<int*>(t); i++){
        doMainStuff();
        if(i % 600 == 0){
            cout << "Skipnuto deset minut" << endl;
        }
    }
    pthread_exit(NULL);
}

// Nesahat na mulithreding, pokud nevíte, co děláte, jinak se to rozbije.
void *readInput(void* input){
    while(true){
        getline(cin, *static_cast<string*>(input));

        string user_input = *static_cast<string*>(input);

        if(user_input.length() >=  50){
            cout << "Příliš dlouhý vstup" << endl;
            continue;
        }

        string command = user_input.substr(0, user_input.find(' '));
        if(!commands.count(command)){
            cout << "Zadali jste neplatný příkaz." << endl;
            continue;
        }
        //cout << command << endl;
        if(user_input == command) user_input.clear();
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

            if(areDistinctPoints(from, to)) buildBelt(from, to);
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

            if(areDistinctPoints(v1, v2)) buildTrain(v1, v2);
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

            if(areDistinctPoints(v1, v2)) buildPipe(v1, v2);
            else{
                cout << "Špatně zadaná čísla bodů." << endl;
                continue;
            }
        }

        if(equals(command, "stored")){
            cout << "Suroviny k dispozici na základně:" << endl;
            team_stored.print(0);
        }

        if(equals(command, "time")){
            cout << "Sekundy uplynulé od začátku hry: " << ticks_passed << endl;
        }

        if(equals(command, "prod")){
            vector<string> tokens = tokenise(user_input);

            if(tokens.size() > 1){
                cout << "Neplatný vstup" << endl;
                continue;
            }

            if(tokens.size() == 0){
                cout << "Produkce přicházející na základnu:" << endl;
                team_average_production.print(3);
                continue;
            }

            string token = tokens[0];
            if(equals(token, "belt")){
                cout << "Produkce přicházející na základnu z pásů:" << endl;
                prod_from_belts.print(3);
                continue;
            }

            else if(equals(token, "train")){
                cout << "Produkce přicházející na základnu z vlaků:" << endl;
                prod_from_trains.print(3);
                continue;
            }

            else if(equals(token, "craft")){
                cout << "Produkce přicházející na základnu z craftění:" << endl;
                prod_from_craft.first.print(3);
                continue;
            }

            else{
                cout << "Neplatný vstup" << endl;
                continue;
            }
        }

        if(equals(command, "all")){
            vector<string> tokens = tokenise(user_input);
            if(tokens.size() >= 3 || tokens.size() == 0){
                cout << "Neplatný vstup" << endl;
                continue;
            }

            string token = tokens[0];

            if(equals(token, "depo")){
                cout << "Aktivovaná těžící ložiska železa: ";
                for(Deposit deposit: iron_deposits_activated){
                     if(deposit.capacity > 0) cout << deposit.i << " ";
                }
                cout << endl;

                cout << "Aktivovaná těžící ložiska měďi: ";
                for(Deposit deposit: copper_deposits_activated){
                    if(deposit.capacity > 0) cout << deposit.i << " ";
                }
                cout << endl;

                cout << "Aktivovaná těžící ložiska ropy: ";
                for(Deposit deposit: oil_deposits_activated){
                    if(deposit.capacity > 0) cout << deposit.i << " ";
                }
                cout << endl;

                cout << "Všechna těžící aktivovaná ložiska: ";
                for(Deposit deposit: all_deposits_activated){
                    if(deposit.capacity > 0) cout << deposit.i << " ";
                }
                cout << endl;
            }

            else if(equals(token, "craft")){
                for(int i = 0; i<6; i++){
                    cout << "Počet craftících stanic typu " << station_names[i] << ": ";

                    if(tokens.size() == 2){
                        if(equals(tokens[1], "active")) cout << "(aktivních) ";
                    }

                    int cnt = 0;
                    for(CraftingStation crafting_station: all_crafting_stations){
                        if(tokens.size() == 2){
                            if(equals(tokens[1], "active") && crafting_station.type == station_names[i] && crafting_station.active) cnt++;
                            else if(!equals(tokens[1], "active") && crafting_station.type == station_names[i]) cnt++;
                        }

                        else if(crafting_station.type == station_names[i]) cnt++;
                    }

                    cout << cnt << endl;
                }
            }

            else if(equals(token, "cipher")){
                cout << "Aktivní šifry jsou na stanovištích:" << endl;
                for(auto ciph: active_ciphers){
                    cout << ciph.first.first << " ";
                }
                cout << endl;
            }

            else if(equals(token, "belt")){
                cout << "Máte postaveny tyto pásy (od-do):" << endl;
                for(int i = 0; i<points.size(); i++){
                    for(auto j: belt_graph[i]){
                        if(j.first >= 0) cout << i + 1 << " " << j.first + 1 << endl;
                    }
                }
            }

            else if(equals(token, "train")){
                cout << "Máte postaveny tyto vlaky (mezi body):" << endl;
                for(int i = 0; i<points.size(); i++){
                    for(int j: train_graph[i]){
                        if(i < j) cout << i + 1 << " " << j + 1 << endl;
                    }
                }
            }

            else if(equals(token, "pipe")){
                cout << "Máte postaveny tyto trubky (mezi body):" << endl;
                for(int i = 0; i<points.size(); i++){
                    for(int j: pipe_graph[i]){
                        if(i < j) cout << i + 1 << " " << j + 1 << endl;
                    }
                }
            }

            else{
                cout << "Neplatný vstup" << endl;
            }
        }

        if(equals(command, "make")){
            vector<string> tokens = tokenise(user_input);
            if(tokens.size() != 2){
                cout << "Neplatný vstup" << endl;
                continue;
            }

            int num_to_be_active;

            try{
                num_to_be_active = stoi(tokens[0]);
            }

            catch(...){
                cout << "Neplatný vstup" << endl;
                continue;
            }

            if(find(station_names.begin(), station_names.end(), tolower(tokens[1])) == station_names.end()){
                cout << "Špatný kód suroviny" << endl;
                continue;
            }

            int num_crafting_stations = 0;
            for(CraftingStation crafting_station: all_crafting_stations){
                if(crafting_station.type == tolower(tokens[1])) num_crafting_stations++;
            }

            if(num_crafting_stations < num_to_be_active || num_to_be_active < 0){
                cout << "Chcete vyrobit víc, než máte postaveno, nebo záporně." << endl;
                continue;
            }

            craftAmount(num_to_be_active, tolower(tokens[1]));
        }

        if((debug || playtest) && equals(command, "skip")){
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

            int rc;
            pthread_t skip_thread;
            rc = pthread_create(&skip_thread, NULL, doStuffForSomeTime, &t);
        }

        //cout << command << "   " << user_input << endl;

        if(equals(command, "mine")){
            vector<string> tokens = tokenise(user_input);
            if(tokens.size() != 1){
                cout << "Neplatný vstup" << endl;
                continue;
            }

            set<int> activated_indices;
            for(Deposit deposit: all_deposits_activated){
                activated_indices.insert(deposit.i);
            }

            string token = tokens[0];
            for(int i = 0; i<token.size(); i++){
                if('A' <= token[i] && token[i] <= 'Z'){
                    token[i] += 'a' - 'A';
                }
            }

            bool deposit_found = 0;

            if('a' <= token[0] && token[0] <= 'z'){
                for(Deposit deposit: all_deposits){
                    //cout << "KENCER" << endl;
                    if(deposit.code == token && (deposit.level == 1 || lvl2_deposits_unlocked)){
                        if(activated_indices.count(deposit.i)){
                            cout << "Ložisko už bylo dříve aktivováno." << endl;
                            deposit_found = 1;
                        }
                        else{
                            bool activated = activateDeposit(deposit);

                            if(activated) cout << "Ložisko surovin úspěšně aktivováno" << endl;
                            deposit_found = 1;
                            break;
                        }
                    }
                }
            }

            else{
                int i;
                try{
                    i = stoi(token);
                }

                catch(...){
                    cout << "Neplatný vstup." << endl;
                    continue;
                }

                for(Deposit deposit: all_deposits){
                    if(deposit.i == i && deposit.level == 1 && lvl1_deposit_build_from_base){
                        if(activated_indices.count(deposit.i)){
                            cout << "Ložisko už bylo dříve aktivováno." << endl;
                            deposit_found = 1;
                        }
                        else{
                            bool activated = activateDeposit(deposit);

                            if(activated) cout << "Ložisko surovin úspěšně aktivováno" << endl;
                            deposit_found = 1;
                            break;
                        }
                    }
                }
            }

            if(!deposit_found){
                cout << "Zadali jste špatný kód nebo ložisko nemáte odemčené." << endl;
                continue;
            }
        }

        if(equals(command, "ratio")){
            vector<string> tokens = tokenise(user_input);
            if(tokens.size() != 1){
                cout << "Neplatný vstup" << endl;
                continue;
            }

            string token = tokens[0];

            double iron_copper_new_constant;
            try{
                iron_copper_new_constant = stod(token);
            }

            catch(...){
                cout << "Neplatný vstup." << endl;
                continue;
            }

            if(iron_copper_new_constant < 1e6 && iron_copper_new_constant > 0.000001){
                iron_copper_constant = iron_copper_new_constant;
                cout << "Konstanta úspěšně změněna." << endl;
            }

            else{
                cout << "Konstanta musí být mezi 0.000001 a 1000000" << endl;
                continue;
            }
        }

        if(equals(command, "oil")){
            vector<string> tokens = tokenise(user_input);
            if(tokens.size() != 1){
                cout << "Neplatný vstup" << endl;
                continue;
            }

            string token = tokens[0];
            if(token == "mine" || token == "craft"){
                oil_priority = token;
                cout << "Prioritní využití ropy změněno." << endl;
            }

            else cout << "Neplatný vstup" << endl;
        }

        if((debug || crafting_unlocked) && equals(command, "build")){
            vector<string> tokens = tokenise(user_input);
            if(tokens.size() != 1){
                cout << "Neplatný vstup" << endl;
                continue;
            }

            string token = tokens[0];
            if(find(station_names.begin(), station_names.end(), tolower(token)) == station_names.end()){
                cout << "Neplatný vstup" << endl;
                continue;
            }

            buildCraftingStation(token);
        }

        if(equals(command, "hint")){
            vector<string> tokens = tokenise(user_input);
            if(tokens.size() != 1){
                cout << "Neplatný vstup" << endl;
                continue;
            }

            int index;
            try{
                index = stoi(tokens[0]);
            }

            catch(...){
                cout << "Neplatný vstup" << endl;
                continue;
            }

            bool hinted = 0;
            for(auto &cipher: active_ciphers){
                if(cipher.first.first == index){
                    cipher.second.second = 1;
                    hinted = 1;

                    cout << hints[index] << endl;
                }
            }

            if(!hinted) cout << "Špatně zadané číslo šifry, může být ještě vám skrytá, nebo je už vyřešená" << endl;
        }

        if(equals(command, "unlock")){
            vector<string> tokens = tokenise(user_input);
            if(tokens.size() != 1){
                cout << "Neplatný vstup" << endl;
                continue;
            }

            string token = tokens[0];

            if(equals(token, "craft")){
                if(crafting_unlocked){
                    cout << "Craftění už máte odemčeno." << endl;
                    continue;
                }

                if(!debug && team_stored.less_than(craft_unlock_cost)){
                    cout << "Na odemknutí nemáte dostatek surovin." << endl;
                    continue;
                }

                team_stored.subtract(craft_unlock_cost);
                crafting_unlocked = 1;
                cout << "Craftění odemčeno." << endl;
            }

            else if(equals(token, "moskva")){
                if(lvl1_deposit_build_from_base){
                    cout << "Těžbu ze základny už máte odemčenou." << endl;
                    continue;
                }

                if(!debug && team_stored.less_than(from_base_unlock_cost)){
                    cout << "Na odemknutí nemáte dostatek surovin." << endl;
                    continue;
                }

                team_stored.subtract(from_base_unlock_cost);
                left_level = 3;
                lvl1_deposit_build_from_base = 1;
                cout << "Těžba ze základny odemčena." << endl;
            }

            else if(equals(token, "lvl2")){
                if(lvl2_deposits_unlocked){
                    cout << "Pokročilá ložiska už máte odemčena." << endl;
                    continue;
                }

                if(!lvl1_deposit_build_from_base){
                    cout << "Ještě nemáte odemčenou těžbu ze základny" << endl;
                    continue;
                }

                if(!debug && team_stored.less_than(lvl2_unlock_cost)){
                    cout << "Na odemknutí nemáte dostatek surovin." << endl;
                    continue;
                }

                team_stored.subtract(lvl2_unlock_cost);
                left_level = 4;
                lvl2_deposits_unlocked = 1;
                cout << "Těžba ze základny odemčena." << endl;
            }

            else{
                cout << "Neplatný vstup" << endl;
            }
        }

        for(int i = active_ciphers.size() - 1; i>=0; i--){
            if(equals(command, active_ciphers[i].first.second) && !active_ciphers[i].second.second){
                string improvement_command;

                while(1){
                    cout << "Gratulace. Úspěšně jste vyřešili šifru. Nyní si vyberte bonus." << endl;
                    cout << "Na výběr máte:" << endl;
                    if(left_level < 4 || right_level < 2) cout << "Plus jeden level na obou stromech - kód lvl." << endl;
                    if(right_level < 2) cout << "Maximální level na stromu napravo a kratší timewarp - kód ciph." << endl;
                    cout << "Delší timewarp - kód tw." << endl;
                    cout << "Nechat si bonus na jindy - kód back." << endl;

                    getline(cin, improvement_command);
                    if(equals(improvement_command, "lvl") && (left_level < 4 || right_level < 2)){
                        active_ciphers.erase(active_ciphers.begin() + i);
                        commands.erase(tolower(command));
                        left_level++;
                        right_level++;

                        if(left_level == 3) lvl1_deposit_build_from_base = 1;
                        if(left_level == 4) lvl2_deposits_unlocked = 1;
                        if(right_level == 1){
                            cout << "Další šifry jsou na lokacích: ";
                            for(auto cipher: lvl2_ciphers){
                                active_ciphers.push_back({cipher, {2, 0}});
                                commands.insert(tolower(cipher.second));
                                cout << cipher.first << " ";
                            }
                            cout << endl;
                        }
                        if(right_level == 2){
                            cout << "Další šifry jsou na lokacích: ";
                            for(auto cipher: lvl3_ciphers){
                                active_ciphers.push_back({cipher, {3, 0}});
                                commands.insert(tolower(cipher.second));
                                cout << cipher.first << " ";
                            }
                            cout << endl;
                        }

                        cout << "Vylepšení stromu provedeno." << endl;
                        break;
                    }

                    else if(equals(improvement_command, "ciph") && right_level < 2){
                        active_ciphers.erase(active_ciphers.begin() + i);
                        commands.erase(tolower(command));

                        if(right_level == 0){
                            cout << "Další šifry jsou na lokacích: ";
                            for(auto cipher: lvl2_ciphers){
                                active_ciphers.push_back({cipher, {2, 0}});
                                commands.insert(tolower(cipher.second));
                                cout << cipher.first << " ";
                            }
                            cout << endl;

                            cout << "Další šifry jsou na lokacích: ";
                            for(auto cipher: lvl3_ciphers){
                                active_ciphers.push_back({cipher, {3, 0}});
                                commands.insert(tolower(cipher.second));
                                cout << cipher.first << " ";
                            }
                            cout << endl;
                        }

                        if(right_level == 1){
                            cout << "Další šifry jsou na lokacích: ";
                            for(auto cipher: lvl3_ciphers){
                                active_ciphers.push_back({cipher, {3, 0}});
                                commands.insert(tolower(cipher.second));
                                cout << cipher.first << " ";
                            }
                            cout << endl;
                        }

                        cout << "Vylepšení pravého stromu provedeno." << endl;

                        right_level = 2;
                        for(int i = 0; i<SHORT_TIME_WARP; i++){
                            doMainStuff();
                        }

                        cout << "Time warp úspěšně dokončen." << endl;
                        break;
                    }

                    else if(equals(improvement_command, "tw")){
                        active_ciphers.erase(active_ciphers.begin() + i);
                        commands.erase(tolower(command));
                        for(int i = 0; i<LONG_TIME_WARP; i++){
                            doMainStuff();
                        }

                        cout << "Time warp úspěšně dokončen." << endl;
                        break;
                    }

                    else if(equals(improvement_command, "back")){
                        break;
                    }

                    else{
                        cout << "Špatně zadaný příkaz. Přece proboha chcete tu odměnu, ne?" << endl;
                    }
                }
            }

            else if(equals(command, active_ciphers[i].first.second)){
                string improvement_command;

                while(1){
                    cout << "Gratulace. Úspěšně jste vyřešili šifru s hintem. Nyní si vyberte bonus." << endl;
                    cout << "Na výběr máte:" << endl;
                    if(!lvl3_ciphers_unlocked) cout << "Plus jeden level na stromu napravo - kód ciph." << endl;
                    if(!crafting_unlocked) cout << "Odemknutí craftění - kód craft." << endl;
                    cout << "Kratší timewarp - kód tw." << endl;
                    cout << "Nechat si bonus na jindy - kód back." << endl;

                    getline(cin, improvement_command);

                    if(equals(improvement_command, "ciph") && !lvl3_ciphers_unlocked){
                        active_ciphers.erase(active_ciphers.begin() + i);
                        commands.erase(tolower(command));
                        if(right_level == 0){
                            cout << "Další šifry jsou na lokacích: ";
                            for(auto cipher: lvl2_ciphers){
                                active_ciphers.push_back({cipher, {2, 0}});
                                commands.insert(tolower(cipher.second));
                                cout << cipher.first << " ";
                            }
                            cout << endl;
                        }

                        if(right_level == 1){
                            cout << "Další šifry jsou na lokacích: ";
                            for(auto cipher: lvl3_ciphers){
                                active_ciphers.push_back({cipher, {3, 0}});
                                commands.insert(tolower(cipher.second));
                                cout << cipher.first << " ";
                            }
                            cout << endl;
                        }

                        cout << "Vylepšení pravého stromu provedeno." << endl;

                        right_level++;
                        break;
                    }

                    else if(equals(improvement_command, "tw")){
                        active_ciphers.erase(active_ciphers.begin() + i);
                        commands.erase(tolower(command));
                        for(int i = 0; i<SHORT_TIME_WARP; i++){
                            doMainStuff();
                        }

                        cout << "Time warp úspěšně dokončen." << endl;
                        break;
                    }

                    else if(equals(improvement_command, "craft") && !crafting_unlocked){
                        active_ciphers.erase(active_ciphers.begin() + i);
                        commands.erase(tolower(command));
                        crafting_unlocked = 1;
                        cout << "Craftění odemčeno" << endl;
                        break;
                    }

                    else if(equals(improvement_command, "back")){
                        break;
                    }

                    else{
                        cout << "Špatně zadaný příkaz. Přece proboha chcete tu odměnu, ne?" << endl;
                    }
                }
            }
        }

    }
    pthread_exit(NULL);
}

void initValues(){
    all_deposits.insert(all_deposits.end(), COPPER_DEPOSITS.begin(), COPPER_DEPOSITS.end());
    all_deposits.insert(all_deposits.end(), OIL_DEPOSITS.begin(), OIL_DEPOSITS.end());

    all_deposits_activated.reserve(200);
    iron_deposits_activated.reserve(50);
    copper_deposits_activated.reserve(50);
    oil_deposits_activated.reserve(50);
    all_deposits_activated_with_changes.reserve(200);

    if(debug) lvl1_deposit_build_from_base = true;
    if(debug) lvl2_deposits_unlocked = true;

    crafting_station_blueprints[0].in = machines_craft_in_i;
    crafting_station_blueprints[0].out = Resources<double>({{3, (double)1}});

    crafting_station_blueprints[1].in = circuits_craft_in_i;
    crafting_station_blueprints[1].out = Resources<double>({{4, (double)1}});

    crafting_station_blueprints[2].in = plastic_craft_in_i;
    crafting_station_blueprints[2].out = Resources<double>({{5, (double)1}});

    crafting_station_blueprints[3].in = rocket_craft_in_i;
    crafting_station_blueprints[3].out = Resources<double>({{6, (double)1}});

    crafting_station_blueprints[4].in = booster_craft_in_i;
    crafting_station_blueprints[4].out = Resources<double>({{3, (double)1}});

    crafting_station_blueprints[5].in = rocket_fuel_craft_in_i;
    crafting_station_blueprints[5].out = Resources<double>({{3, (double)1}});

    for(int i = 0; i<lvl1_ciphers.size(); i++){
        commands.insert(tolower(lvl1_ciphers[i].second));
        active_ciphers.push_back({lvl1_ciphers[i], {1, 0}});
    }
}

int main(){
    cout << "Hra začíná. Za celé politbyro vám přeji hodně štěstí." << endl;
    srand(time(NULL));

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
        doMainStuff();
    }
}
