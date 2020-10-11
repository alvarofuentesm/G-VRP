struct node{
    int pos; // position in vertex array
    int id, served; // 0 not served, 1 served
    double x, y, angle;
    char type;
};


char* configuration_file;
char* results_file;
int seed;
int debug;
int Mr;
int sizeInstance;
node* vertex;
double **distances;

int trucks, best_trucks, n_served_customer;


int nCustomers, nStations, maxTime, maxDistance, serviceTime, refuelTime;
float speed;

double min_X, max_X, min_Y, max_Y;

class solucion {
  public:
    double eval_function;
    vector<node> tour;
    int real_size, customer_served, vehicles;

    vector<node>* tourList;
    int* tourListExceded;

    solucion() {   // Constructor
        eval_function = 0;
        real_size = 0;
        tour= vector<node>(sizeInstance);
        customer_served = 0;
         
        vehicles = 0;
        tourList = new vector<node>[sizeInstance];
        tourListExceded = new int[sizeInstance];
    }

    void reset (){
        eval_function = 0;
        real_size = 0;
        tour= vector<node>(sizeInstance);
        customer_served = 0;
    }

    void print_debug_solucion(){
        cout << "[";
        for (int i = 0; i < tour.size(); i++) {
            cout << "(" << tour[i].id << ", " << tour[i].type << "), " ; 
        }
        cout << "]" << endl;
    }

    void print_debug_solucion_real(){
        cout << "[";
        for (int i = 0; i < real_size - 1; i++) {
            cout << "(" << tour[i].id << ", " << tour[i].type << "), " ; 
        }
        cout << "]" << endl;
    }

    void add_to_eval_function(double value){
        eval_function+= value; 
    }

    double get_eval_function(){
        return eval_function;
    }

    node get_node(int pos){
        return tour[pos];
    }

    void update_node(int pos, node new_node){
        tour[pos] = new_node;
    }

    void recalculate_eval_function(){
        double eval = 0;
        //cout << eval << endl;
        for (int i = 0; i < real_size - 2; i++){
            eval+=distances[tour[i].pos][tour[i+1].pos] ;
            //cout << eval << endl;
        }
        eval_function = eval;
    }

};

solucion best_solution;