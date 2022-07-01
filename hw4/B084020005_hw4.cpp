#include <bits/stdc++.h>
#define endl "\n"

using namespace std;

int backpack_capacity;
vector<int> w_list, p_list;
string dir_name;

void simulated_annealing();
void balance();
void produce_random_initial_answser();
void calculate_backpack_weight_and_price(bool *way);
void remove_one(bool *backpack);
void put_one(bool *backpack);

const int N = 1000;
const int W = 538350;
int best_answer = 0;
int T = 200.0;
float af = 0.95;
int iterate_time = 100, balance_time = 10;
bool best_way[N], current_way[N];
int price_sum, weight_sum;

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

    simulated_annealing();

    cout << "best_answer: " << best_answer << endl;

    ofstream output_stream;

    int item_num = p_list.size();
    output_stream.open("ans_" + dir_name + ".txt");
    output_stream << best_answer << endl;
    for(int i = 0; i < item_num; i++){
        output_stream << best_way[i] << endl;
    }
    output_stream.close();

    cout << "Finish!" << endl;

    return 0;
}

void simulated_annealing(){

    produce_random_initial_answser();

    for(int i = 0; i < iterate_time; i++){
        balance();
        T *= af;
    }

}

void balance(){

    int item_num = p_list.size();
    bool test_answer[N] = {};

    for(int i = 0; i < balance_time; i++){
        calculate_backpack_weight_and_price(current_way);
        int now_price = price_sum;
        memcpy(test_answer, current_way, sizeof(bool) * item_num);
        int index = (rand() % item_num);
        if(test_answer[index] == true){
            put_one(test_answer);
            test_answer[index] = false;
        }else{
            if(((double)rand() / (RAND_MAX + 1.0)) < 0.5){
                test_answer[index] = 1;
            }else{
                remove_one(test_answer);
                test_answer[index] = 1;
            }
        }

        calculate_backpack_weight_and_price(test_answer);

        int test_price = price_sum;

        if(weight_sum > backpack_capacity){
            continue;
        }
        if(test_price > best_answer){
            best_answer = test_price;
            memcpy(best_way, test_answer, sizeof(bool) * item_num);
        }

        if(test_price > now_price){
            memcpy(current_way, test_answer, sizeof(bool) * item_num);
        }else{
            double g = (double)(test_price - now_price) / T;
            if(((double)rand() / (RAND_MAX + 1.0)) < exp(g)){
                memcpy(current_way, test_answer, sizeof(bool) * item_num);
            }
        }

    }
}

void produce_random_initial_answser(){
    int item_num = p_list.size();
    while(true){
        for(int i = 0; i < item_num; i++){
            if(((double)rand() / (RAND_MAX + 1.0)) < 0.5){
                current_way[i] = true;
            }else{
                current_way[i] = false;
            }
        }
        calculate_backpack_weight_and_price(current_way);
        if(weight_sum <= backpack_capacity) break;
    }

    best_answer = price_sum;
    memcpy(best_way, current_way, sizeof(bool) * item_num);
}

void calculate_backpack_weight_and_price(bool *way){
    int item_num = p_list.size();
    price_sum = 0;
    weight_sum = 0;

    for(int i = 0; i < item_num; i++){
        if(way[i] == true){
            price_sum += p_list[i];
            weight_sum += w_list[i];
        }
    }
}

void remove_one(bool *backpack){
    int item_num = p_list.size();
    while(true){
        int index = rand() % item_num;
        if(backpack[index] == true){
            backpack[index] = false;
            break;
        }
    }
}

void put_one(bool *backpack){
    int item_num = p_list.size();
    while(true){
        int index = rand() % item_num;
        if(backpack[index] == false){
            backpack[index] = true;
            break;
        }
    }
}