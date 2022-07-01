#include <bits/stdc++.h>
#define endl "\n"

using namespace std;

void exhaustive_search(int depth, string record, int len_limit);
bool is_capacity_ok(string &record);
int cal_price(string &record);

clock_t start, finish;
int backpack_capacity;
vector<int> w_list, p_list;
int ans_price;
string ans_record;
string dir_name;

int main(){
    
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

    exhaustive_search(0, "", w_list.size());

    return 0;
}

void exhaustive_search(int depth, string record, int len_limit){

    if(depth == 0){
        ans_price = -1;
        ans_record = "";
        start = clock();
    }

    if(depth == len_limit){
        int temp_price;
        if(is_capacity_ok(record)){
            temp_price = cal_price(record);
            if(temp_price > ans_price){
                ans_price = temp_price;
                ans_record = record;
            }
        }
        return;
    }

    finish = clock();
    if(finish - start >= 60000){
        ofstream output_stream;
        output_stream.open("ans_" + dir_name + ".txt");
        output_stream << ans_price << endl;
        for(int i = 0; i < (int)ans_record.length(); i++) output_stream << ans_record[i] << endl;
        output_stream.close();
        exit(1);
    }

    // if(!is_capacity_ok(record)) return;

    // left is chosen the current one, and right is not.    
    exhaustive_search(depth + 1, record + "1", len_limit);
    exhaustive_search(depth + 1, record + "0", len_limit);

    if(depth == 0){
        ofstream output_stream;
        output_stream.open("ans_" + dir_name + ".txt");
        output_stream << ans_price << endl;
        for(int i = 0; i < (int)ans_record.length(); i++) output_stream << ans_record[i] << endl;
        output_stream.close();
    }
}

bool is_capacity_ok(string &record){
    int sum = 0;
    for(int i = 0; i < (int)record.length(); i++){
        if(record[i] == '1'){
            sum += w_list[i];
        }
    }
    if(sum > backpack_capacity) return false;
    else return true;
}

int cal_price(string &record){
    int price = 0;
    for(int i = 0; i < (int)record.length(); i++){
        if(record[i] == '1') price += p_list[i];
    }
    return price;
}