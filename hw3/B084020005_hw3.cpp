#include <bits/stdc++.h>
#define endl "\n"

using namespace std;

int backpack_capacity;
vector<int> w_list, p_list;
int ans_price;
string dir_name;

const int N = 1000;
bool answer_record[N] = {};

const int W = 538350;
int dp[W + 1] = {};
bool put_record[N][W + 1] = {};

void dynamic_programming();

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

    dynamic_programming();

    cout << "Finish!" << endl;

    return 0;
}

void dynamic_programming(){

    int item_num = p_list.size();

    for(int i = 0; i < item_num; i++){
        for(int j = backpack_capacity; j >= w_list[i]; j--){
            if(dp[j - w_list[i]] + p_list[i] > dp[j]){
                dp[j] = dp[j - w_list[i]] + p_list[i];
                put_record[i][j] = true;
            }
        }
    }

    ans_price = dp[backpack_capacity];

    for(int i = item_num - 1, j = backpack_capacity; i >= 0; i--){
        if(put_record[i][j]){
            answer_record[i] = true;
            j -= w_list[i];
        }
    }

    cout << "ans_price: " << ans_price << endl;

    ofstream output_stream;

    output_stream.open("ans_" + dir_name + ".txt");
    output_stream << ans_price << endl;
    for(int i = 0; i < item_num; i++){
        output_stream << answer_record[i] << endl;
    }
    output_stream.close();

}