#include "includes.h"
#include "globals.h"

/* extracted from https://www.geeksforgeeks.org/haversine-formula-to-find-distance-between-two-points-on-a-sphere/
modified to use earth radius 4182.44949
cout << haversineDistance(51.5007,  0.1246, 40.6892, 74.0445 ) << endl;
*/
double haversineDistance(double lat1, double lon1, double lat2, double lon2){
    // distance between latitudes 
    // and longitudes 
    double dLat = (lat2 - lat1) * PI / 180.0; 
    double dLon = (lon2 - lon1) *  PI / 180.0; 

    // convert to radians 
    lat1 = (lat1) * PI / 180.0; 
    lat2 = (lat2) * PI / 180.0; 

    // apply formulae 
    double a = pow(sin(dLat / 2), 2) +  
                pow(sin(dLon / 2), 2) *  
                cos(lat1) * cos(lat2); 
    return 8364.89898 * asin(sqrt(a)); 
}

void _exit(void){
    //escribir_en_archivo_resultados();

    delete[] vertex; 
    for (int i = 0; i < sizeInstance; i++){
        delete distances[i];
    }
    delete[] distances;

    //salida sin error
    exit(0);
    return;
}


int read_params(int argc, char **argv){

    
    //archivo con la instancia del problema
    configuration_file=(char *)(argv[1]);
    
    //archivo donde escribir los resultados de la ejecucion
    results_file=(char *)(argv[2]);
    
    //SEMILLA
    seed=atoi(argv[3]);

    //BUDGET
    Mr=atoi(argv[4]);
    if(debug)
        printf("Mr: %d\n", Mr);

    debug=atoi(argv[5]);

    return 1;
}

/* Split string into a string array */
template <size_t N>
void splitString(string (&arr)[N], string str){
    int n = 0;
    istringstream iss(str);
    for (auto it = istream_iterator<string>(iss); it != istream_iterator<string>() && n < N; ++it, ++n)
        arr[n] = *it;
}

void read_config_file(void){
    
    ifstream config(configuration_file);
    string str; 

    getline(config, str);
    
    string arr[8];
    splitString(arr, str);

    nCustomers = stoi(arr[1]);
    nStations = stoi(arr[2]);
    maxTime = stoi(arr[3]);
    maxDistance = stoi(arr[4]);
    serviceTime = stoi(arr[6]);
    refuelTime = stoi(arr[7]);
    speed = stof(arr[5]);

    if (debug){
        cout << "nCustomers " << nCustomers << endl;
        cout << "nStations " << nStations << endl;
        cout << "maxTime " << maxTime << endl;
        cout << "maxDistance "  << maxDistance << endl;
        cout << "speed " << speed << endl;
        cout << "serviceTime " << serviceTime << endl;
        cout << "refuelTime " << refuelTime << endl;
    }

    sizeInstance = nCustomers + nStations + 1; // the 1 is the depot
    //Reserva de espacio para el arreglo de coordenadas y la matriz de distancia
    vertex = new node[sizeInstance];
    distances = new double*[sizeInstance]; // matrix of distances 
    for( int i = 0 ; i < sizeInstance ; i++ ) distances[i] = new double [sizeInstance];

    int id;
    double x,y;
    int cont = 0;
    char type;
    string arr2[4], temp;    

    max_X = infinity();
    min_X = -infinity();
    max_Y = infinity();
    min_Y = -infinity();

    if (0 < min_X)cout << "yes" << endl;

    while (getline(config, str) && cont < sizeInstance) {
        temp = str;
        splitString(arr2, temp);
        
        if (debug){
            cout << arr2[0]  <<  " ";
            cout << arr2[1]  <<  " ";
            cout << arr2[2]  <<  " ";
            cout << arr2[3]  <<  endl;
        }

        id = stoi(arr2[0]);
        type = arr2[1][0];
        x = stof(arr2[2]);
        y = stof(arr2[3]);

        if (x < max_X){
            max_X = x;
        }
        if (x > min_X){
            min_X = x;
        }
        if (y < max_Y){
            max_Y = y;
        }
        if (y > min_Y){
            min_Y = y;
        }
        
        
        vertex[cont].id = id;
        vertex[cont].type = type;
        vertex[cont].x = x;
        vertex[cont].y = y;
        vertex[cont].served = 0;
        vertex[cont].pos = cont;

        cont++;

    }
    config.close();

    //cout << "x range: " << min_X << " " << max_X << endl;
    //cout << "y range: " << min_Y << " " << max_Y << endl;

    // distances matrix 
    for(int i = 0; i < sizeInstance; i++){
        for (int j = 0; j < sizeInstance; j++){            
            distances[i][j] =  haversineDistance(vertex[i].x, vertex[i].y, vertex[j].x, vertex[j].y);
        }
    }

    if (debug) {
        for(int i=0; i < sizeInstance; i++)
            for(int j = 0; j < sizeInstance; j++)
            cout << distances[i][j] << " ";
        cout << endl;  
    }

}

double double_rand(double a, double b){
  //generacion de numeros aleatorios entre a y b-1
  double retorno=0;
  if (a < b){ 
    retorno=(b-a)*drand48();
    retorno=retorno+a;
  }
  else{
    retorno=(a-b)*drand48();
    retorno=retorno+b;
  }
  return retorno;
}

bool acompare(node lhs, node rhs) { return lhs.angle < rhs.angle; }


void angle_method(double xa, double ya, node* customers_angle_array){
    double x0, y0, x1, y1;
    x0 = vertex[0].x;
    y0 = vertex[0].y;

    //cout << "(" << x0 << ", " << y0 << ")" << endl;

    //cout << "nStations: " << nStations << endl;
    double a, b, c, d, angle, e ,f;
    for (int i = 0; i < nCustomers; i++){

        if (vertex[nStations + i + 1].served){
            continue;
        }
        
        //cout << vertex[nStations + i + 1].id << "|" << vertex[nStations + i + 1].type << endl;  
        x1 = vertex[nStations + i + 1].x;
        y1 = vertex[nStations + i + 1].y;
        //cout << "(" << x1 << ", " << y1 << ")" << endl;

        a = (xa - x0);
        b = (x1 - x0);
        c = (ya - y0);
        d = (y1 - y0);
        e = (a*b + c*d);
        f = sqrt(pow(a, 2)+pow(c, 2)) * sqrt(pow(b, 2)+pow(d, 2)) ;
        /*
        cout << a << endl;
        cout << b << endl;
        cout << c << endl;
        cout << d << endl;
        //cout << e << endl;
        //cout << f << endl;
        */
        angle = acos( e/f );

        vertex[nStations + i + 1].angle = angle;
        //cout << angle << endl;
        customers_angle_array[i] = vertex[nStations + i + 1];
    }

    
    sort(customers_angle_array, customers_angle_array+nCustomers, acompare); // sort by ascending order
}

/* build initial solution using with the Cordeau, Laporte, and Mercier (2001) 
approach by using the angle between the dopot and a random point in the map*/

void build_initial_solution(solucion *i_temp) {

    
    i_temp->tour[0] = vertex[0]; // depot
    i_temp-> real_size++; 


    node* customers_angle_array = new node[nCustomers];

    double x0, y0; // depot coordinates 
    double xa, ya; // random physical point
    double x1, y1;
    xa = double_rand(min_X, max_X);
    ya = double_rand(min_Y, max_Y);

    angle_method(xa, ya,  customers_angle_array);
    
    int n_served_customer = 0;
    int t = 0; // time
    int cont_angle_array = 0, cont_tour = 0; 
    double dist_comb = 0, dist_rec = 0, exceded = 0;

    int cont_tour_list = 0;

    node prev, next;

    while (n_served_customer < nCustomers){
        
        prev = i_temp->tour[cont_tour];

        next = customers_angle_array[cont_angle_array];

       /*
        cout << "prev.id: " << prev.id << " " <<  prev.type << " pos: "<< prev.pos << endl;
        cout << "cont_tour " << cont_tour << endl;
        cout << "next.id: " << next.id << " " <<  next.type << " pos: "<< next.pos << endl;
        cout << "distance:" << distances[prev.pos][next.pos] << endl;
        cout << "dist_comb: " << dist_comb << " maxDistance: " << maxDistance << endl;
        */

        if (t + distances[prev.pos][next.pos] > maxTime){ // return to depot
            cout << "RETURN TO DEPOT" << endl;
            exceded = distances[prev.pos][0]; // exceded distance
            cont_tour++;

            i_temp->tour[cont_tour] = vertex[0]; // back to depot
            i_temp-> real_size++; 
            i_temp->add_to_eval_function(distances[prev.pos][0]);
            i_temp->tourList[cont_tour_list] = i_temp->tour;
            i_temp->tourListExceded[cont_tour_list] = exceded;
            i_temp->vehicles++;

            t = 0; 
            cont_angle_array = 0; 
            dist_comb = 0;
            dist_rec = 0; 
            exceded = 0;
            continue;
        }


        else if (dist_comb > maxDistance){ // running out of fuel, go to AFS

            // search closet AFS
            double minimum_distance = infinity();
            node afs;
            int pos_afs = 0;
            for (int z = 0; z < nStations;  z++){ // depot can be afs. 
                //cout << distances[prev.pos][z] << " " << minimum_distance << endl;
                if (distances[prev.pos][z] < minimum_distance) {
                    minimum_distance = distances[prev.pos][z];
                    pos_afs = z;
                }
            }

            afs = vertex[pos_afs];
            angle_method(afs.x, afs.y,  customers_angle_array);
 
            dist_rec += distances[prev.pos][afs.pos];
            
            // if cannot go to the nearest afs, will go back to depot 
            if (t + distances[prev.pos][afs.pos] > maxTime){ // return to depot
                cout << "RETURN TO DEPOT" << endl;
                exceded = distances[prev.pos][0]; // exceded distance
                cont_tour++;

                i_temp->tour[cont_tour] = vertex[0]; // back to depot
                i_temp-> real_size++; 
                i_temp->add_to_eval_function(distances[prev.pos][0]);
                i_temp->tourList[cont_tour_list] = i_temp->tour;
                i_temp->tourListExceded[cont_tour_list] = exceded;
                i_temp->vehicles++;

                t = 0; 
                cont_angle_array = 0; 
                dist_comb = 0;
                dist_rec = 0; 
                exceded = 0;
                continue;
            }
            else{ // recharge alternative fuel
                //cout << "GO TO AFS" << endl;
                t += refuelTime;
                dist_comb = 0;    
                cont_tour++;
                i_temp->tour[cont_tour] = vertex[afs.pos];
                i_temp->add_to_eval_function(distances[prev.pos][next.pos]); 
                i_temp-> real_size++; 
            }   

        }

        else{ // serve client
            //cout << "CUSTOMER SERVED" << endl;
            t+= distances[prev.pos][next.pos]/speed;
            t+= serviceTime;
            dist_comb+= distances[prev.pos][next.pos];
            cont_angle_array++;
            cont_tour++;
            
            vertex[next.pos].served = 1; // customer served
            i_temp->tour[cont_tour] = vertex[next.pos];
            i_temp->add_to_eval_function(distances[prev.pos][next.pos]); 

            i_temp-> real_size++; 
            n_served_customer++;            

        }
    }

    
    i_temp->customer_served = n_served_customer;


    /*
    cout << "-----------------" << endl;
    cout << "exceded milles: " << exceded << endl;
    cout << "customer served: " << n_served_customer << endl;
    cout << "eval: " << i_temp->get_eval_function() << endl;
    i_temp->print_debug_solucion();
    cout << "-----------------" << endl;
    */


    delete[] customers_angle_array;

}


bool SWaP(solucion* actual_solution, solucion* candidate_solution, int pos1, int pos2){
    *candidate_solution = *actual_solution;
    //cout << actual_solution->real_size << endl;
    //actual_solution->print_debug_solucion_real();
    //cout << "SWap" << endl;

    node node_1, node_2; 
    node_1 = actual_solution->get_node(pos1);
    node_2 = actual_solution->get_node(pos2);
    
    candidate_solution->update_node(pos1, node_2);
    candidate_solution->update_node(pos2, node_1);
    
    candidate_solution->recalculate_eval_function();

    //actual_solution->print_debug_solucion_real();
    //candidate_solution->print_debug_solucion_real();
    //cout << actual_solution->get_eval_function() << " "; 
    //cout << candidate_solution->get_eval_function() << endl;


    // simulate tour
    int t; // time
    int cont_angle_array = 0, cont_tour = 0; 
    double dist_comb = 0, dist_rec = 0, exceded = 0;

    node prev, next;
    while (cont_tour < candidate_solution->real_size - 2){
        
        prev = candidate_solution->tour[cont_tour];

        next = candidate_solution->tour[cont_tour + 1];

        /*
        cout << "----------------------------" << endl;
        cout << "cont_tour " << cont_tour << endl;
        cout << "prev.id: " << prev.id << " " <<  prev.type << " pos: "<< prev.pos << endl;
        cout << "next.id: " << next.id << " " <<  next.type << " pos: "<< next.pos << endl;
        cout << "distance:" << distances[prev.pos][next.pos] << endl;
        cout << "dist_comb: " << dist_comb << " maxDistance: " << maxDistance << endl;
        */

        if (t + distances[prev.pos][next.pos] > maxTime){ // return to depot
            //cout << "FALSE" << endl;
            return false;
        }

        if (dist_comb > maxDistance){ 
            //cout << "FALSE" << endl;
            return false;
        }

        else{
            t+= distances[prev.pos][next.pos]/speed;
            dist_comb+= distances[prev.pos][next.pos];

            if (next.type == 'f'){
                t+= refuelTime;
                dist_comb = 0;
            }

            else{
                t+= serviceTime;
            }             
            cont_tour++;
        }

    }
    return true; // if no contraist is broken
    
}


int main (int argc, char *argv[]){

    //lectura de parametros
    if(!read_params(argc,argv)){
        cout<<"Problemas en la lectura de los parametros";
        exit(1);
    }


    read_config_file();

    //semilla aleatoria
    srand48 (seed);
    solucion actual_solution, candidate_solution, best_candidate;
    int restart = -1, iteration;

    build_initial_solution(&actual_solution);

    best_solution = actual_solution;
    bool optimo_local;

    cout << Mr << endl;
    Mr = 1;
    for(restart = 0; restart < Mr; restart++) {
        cout << "RESTART: " <<  restart << endl;

        if (restart > 0)build_initial_solution(&actual_solution);

        optimo_local = false;
        iteration = 0;

        do{
            if(debug)cout << "iteration: " << iteration << endl;
            best_candidate = actual_solution;
            
            for(int current = 0 ; current < actual_solution.real_size - 3; current++){
                
                // movement
                if (SWaP(&actual_solution, &candidate_solution, current, current +1 ) == false){
                    continue;
                }
                else{
                    cout << "true" << endl; 
                    if(candidate_solution.get_eval_function() < best_candidate.get_eval_function())
                        best_candidate = candidate_solution;
                }
            }
            iteration++;

            if(best_candidate.get_eval_function() < actual_solution.get_eval_function()){ //Minimize
                actual_solution = best_candidate;
            }
            else
                optimo_local=true;
        } while(!optimo_local);

        if(actual_solution.get_eval_function() < best_solution.get_eval_function()){
            best_solution = actual_solution;
        }
        actual_solution.reset();

        for (int k=0; k < sizeInstance; k++){ // reset vertex
            vertex[k].served = 0;
        }

    }

    cout << "eval: " << best_solution.get_eval_function() << endl;
    best_solution.print_debug_solucion_real();



    _exit();
}