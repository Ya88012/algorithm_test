#include <bits/stdc++.h>
#define endl "\n"

#define my_random ((double)rand() / (RAND_MAX + 1))

using namespace std;

int backpack_capacity;
vector<int> w_list, p_list;
string dir_name;

const int max_DNA_size = 1000;
double cross_rate = 0.5;
double mutate_rate = 0.01;
const int pop_size = 400;
int generation_num = 500;

void genetic_algorithm(int DNA_size);
void get_fitness(int DNA_size);
void select(int DNA_size);
void mutate(bool *DNA, int DNA_size);
void crossover(int DNA_size);
void show_population(int DNA_size);

bool population[pop_size][max_DNA_size];
bool pool[pop_size][max_DNA_size];
double fitness_list[pop_size] = {};

int main(){

    srand(time(NULL));

    int temp_w, temp_p;

    ifstream input_stream;
    cout << "Input the directory of test data: ";
    cin >> dir_name;

    input_stream.open(dir_name + "\\c.txt");
    input_stream >> backpack_capacity;
    input_stream.close();

    input_stream.open(dir_name + "\\w.txt");
    while(input_stream >> temp_w){
        w_list.push_back(temp_w);
    }
    input_stream.close();
    
    input_stream.open(dir_name + "\\p.txt");
    while(input_stream >> temp_p){
        p_list.push_back(temp_p);
    }
    input_stream.close();

    genetic_algorithm(p_list.size());

    return 0;
}

void genetic_algorithm(int DNA_size){

    bool all_generation_best_gene[DNA_size] = {};
    int all_generation_best_fitness = -1;

    for(int i = 0; i < pop_size; i++){
        for(int j = 0; j < max_DNA_size; j++){
            if(my_random < 0.5) population[i][j] = false;
            else population[i][j] = true;
        }
    }

    for(int i = 0; i < pop_size; i++){
        for(int j = 0; j < DNA_size; j++){
            if(my_random < 0.5) population[i][j] = false;
            else population[i][j] = true;
        }
    }

    // cout << "The initial environment:" << endl;
    // show_population();
    // cout << endl;

    for(int i = 0; i < generation_num; i++){

        get_fitness(DNA_size);

        // show_population(DNA_size);

        int best_index = -1;
        double best_fitness = 0;

        for(int j = 0; j < pop_size; j++){
            if(best_fitness < fitness_list[j]){
                best_index = j;
                best_fitness = fitness_list[j];
            }
        }

        if(fitness_list[best_index] >= all_generation_best_fitness){
            memcpy(all_generation_best_gene, population[best_index], sizeof(bool) * DNA_size);
            all_generation_best_fitness = best_fitness;
        }

        cout << "Gen: " << i << endl;
        cout << "best_index: " << best_index << endl;
        cout << "best_fit: " << fitness_list[best_index] << endl;
        // cout << "Gene's sequence:";
        // for(int j = 0; j < DNA_size; j++){
        //     cout << " " << population[best_index][j];
        // } 
        // cout << endl;

        cout << endl;

        select(DNA_size);
        crossover(DNA_size);

    }

    cout << "The best gene in all generation is:" << endl;
    for(int i = 0; i < DNA_size; i++){
        cout << all_generation_best_gene[i] << " ";
    }
    cout << endl;
    cout << "Its fitness score: " << all_generation_best_fitness << endl;

    ofstream output_stream;

    int item_num = p_list.size();
    output_stream.open("ans_" + dir_name + ".txt");
    output_stream << all_generation_best_fitness << endl;
    for(int i = 0; i < item_num; i++){
        output_stream << all_generation_best_gene[i] << endl;
    }
    output_stream.close();

    cout << "Finish!" << endl;

}

void get_fitness(int DNA_size){

    for(int i = 0; i < pop_size; i++){
        int weight_sum = 0;
        int price_sum = 0;
        for(int j = 0; j < DNA_size; j++){
            if(population[i][j]){
                weight_sum += w_list[j];
                price_sum += p_list[j];
            }

            // cout << "weight_sum: " << weight_sum << endl;
            // cout << "price_sum: " << price_sum << endl;

        }
        fitness_list[i] = weight_sum > backpack_capacity ? 0: price_sum;
        // cout << "fitness_list[" << i << "]: " << fitness_list[i] << endl;
    }

}

void select(int DNA_size){

    double fitness_sum = 0.0;
    for(int i = 0; i < pop_size; i++){
        fitness_sum += fitness_list[i];
    }

    int slack = pop_size, counter = -1, has_copy = 0;

    for(int i = 0; i < pop_size && slack != 0; i++){
        counter = (int)round(fitness_list[i] / fitness_sum * pop_size);
    
        if(counter > slack) counter = slack;

        for(int j = 0; j < counter; j++, has_copy++){
            memcpy(pool[has_copy], population[i], sizeof(bool) * DNA_size);
        }
    
        slack -= counter;

    }

    int g_1, g_2;

    int temp_index = -1;
    while(has_copy < pop_size){
        g_1 = rand() % pop_size;
        do{
            g_2 = rand() % pop_size;
        }while(g_1 == g_2);

        if(fitness_list[g_1] >= fitness_list[g_2]) temp_index = g_1;
        else temp_index = g_2;
        memcpy(pool[has_copy++], population[temp_index], sizeof(bool) * DNA_size);
    }

    for(int i = 0; i < pop_size; i++){
        memcpy(population[i], pool[i], sizeof(bool) * DNA_size);
    }

}

void mutate(bool *DNA, int DNA_size){
    for(int i = 0; i < DNA_size; i++){
        if(my_random <= mutate_rate){
            if(DNA[i]) DNA[i] = false;
            else DNA[i] = true;
        }
    }
}

void crossover(int DNA_size){

    for(int i = 0; i < pop_size; i++){
        if(my_random < cross_rate){
            int another_gene = rand() % pop_size;

            bool cross_points[DNA_size] = {};
            for(int j = 0; j < DNA_size; j++){
                if(my_random < 0.5) cross_points[j] = true;
            }

            // cout << "Before crossover:" << endl;
            // cout << "A:";
            // for(int j = 0; j < DNA_size; j++){
            //     cout << " " << population[i][j];
            // }
            // cout << endl;
            // cout << "B:";
            // for(int j = 0; j < DNA_size; j++){
            //     cout << " " << population[another_gene][j];
            // }
            // cout << endl;

            for(int j = 0; j < DNA_size; j++){
                if(cross_points[j]) swap(population[i][j], population[another_gene][j]);
            }
            
            // cout << "cross points:";
            // for(int j = 0; j < DNA_size; j++){
            //     cout << " " << cross_points[j];
            // }
            // cout << endl;

            // cout << "After crossover:" << endl;
            // cout << "A:";
            // for(int j = 0; j < DNA_size; j++){
            //     cout << " " << population[i][j];
            // }
            // cout << endl;
            // cout << "B:";
            // for(int j = 0; j < DNA_size; j++){
            //     cout << " " << population[another_gene][j];
            // }
            // cout << endl;

        }

        mutate(population[i], DNA_size);

    }

}

void show_population(int DNA_size){
cout << "show_population:" << endl;
    for(int i = 0; i < pop_size; i++){
        for(int j = 0; j < DNA_size; j++){
            cout << population[i][j] << " ";
        }
        cout << endl;
    }
}