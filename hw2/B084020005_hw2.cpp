#include <bits/stdc++.h>
#define endl "\n"

using namespace std;

void greedy();

int backpack_capacity;
vector<int> w_list, p_list;
int ans_price;
string ans_record;
string dir_name;

class MyObject{
    public:
        MyObject();
        MyObject(int index, int price, int weight);
        bool operator <(MyObject &right);
        int index;
        int price;
        int weight;
};

MyObject::MyObject(): index(-1), price(-1), weight(-1){}

MyObject::MyObject(int index, int price, int weight): index(index), price(price), weight(weight){}

bool MyObject::operator <(MyObject &right){
    if((double)price / (double)weight < (double)right.price / (double)right.weight) return true;
    else return false;
}

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

    greedy();

    return 0;
}

void greedy(){

    ans_price = -1;
    ans_record = "";

    vector<MyObject> item_list;
    for(int i = 0; i < (int)w_list.size(); i++){
        item_list.push_back(MyObject(i, p_list[i], w_list[i]));
    }
    sort(item_list.rbegin(), item_list.rend());
    int current_weight = 0, temp_price = 0;
    bool record[w_list.size()] = {};
    for(int i = 0; i < (int)w_list.size(); i++){
        if(backpack_capacity - current_weight >= item_list[i].weight){
            current_weight += item_list[i].weight;
            temp_price += item_list[i].price;
            record[item_list[i].index] = true;
        }
    }

    ans_price = temp_price;

    for(int i = 0; i < (int)w_list.size(); i++){
        if(record[i] == true) ans_record += "1";
        else ans_record += "0";
    }

    ofstream output_stream;
    output_stream.open("ans_" + dir_name + ".txt");
    output_stream << ans_price << endl;
    for(int i = 0; i < (int)ans_record.length(); i++) output_stream << ans_record[i] << endl;
    output_stream.close();

}