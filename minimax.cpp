#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>

using namespace std;

int findArea(int i, int j){
    int area;
    if(i<3){
        if(j<3) area = 0;
        else if(j<6) area = 1;
        else area = 2;
    }
    else if(i<6){
        if(j<3) area = 3;
        else if(j<6) area = 4;
        else area = 5;
    }
    else{
        if(j<3) area = 6;
        else if(j<6) area = 7;
        else area = 8;
    }
    return area;
}
int alfa;
struct node{
    int i1;
    int j1;
    int value1;
    int minimax;
    bool visited;
    int remain;
    int player;
};

class Graph {
    int V;
    vector <vector <int> > adj;
    vector <vector <int> > rev;
    vector <node> elements;
public:
    vector <vector <int> > grid;
    vector <vector <int> > rows;
    vector <vector <int> > cols;
    vector <vector <int> > boxes;
    Graph(int V) {
        this->V = V;
        adj.resize(V);
        rev.resize(V);
    }

    void optimize(int V) {
        this->V = V;
        adj.resize(V);
        rev.resize(V);
    }

    void addEdge(int v, int f) {
        adj[v].push_back(f);
        rev[f].push_back(v);
    }

    void add_node(node m) {
        m.minimax = -1;
        elements.push_back(m);
    }

    int last_remain(int last_count){
        return elements[last_count].remain;
    }

    void mini_max(int counter, int last_count, int mm){
        elements[counter].minimax = mm;
        if(elements[counter].player){
            if(elements[last_count].minimax != 1) elements[last_count].minimax = mm;
        }
        else {
            if(elements[last_count].minimax != 0) elements[last_count].minimax = mm;
        }
    }

    void findResult(){
        if(!elements[0].minimax) cout<<2;
        else cout << elements[0].minimax;
    }

    void set_grid(vector <vector <int>> &temp_grid, vector <vector <int>> &temp_rows, vector <vector <int>> &temp_cols, vector <vector <int>> &temp_boxes, int n) {
        while(n != 0){
            int i = elements[n].i1;
            int j = elements[n].j1;
            int value = elements[n].value1;
            temp_grid[i][j] = value;
            temp_rows[i].erase(remove(temp_rows[i].begin(), temp_rows[i].end(), value), temp_rows[i].end());
            temp_cols[j].erase(remove(temp_cols[j].begin(), temp_cols[j].end(), value), temp_cols[j].end());
            int area = findArea(i, j);
            temp_boxes[area].erase(remove(temp_boxes[area].begin(), temp_boxes[area].end(), value), temp_boxes[area].end());
            n = rev[n][0];
        }
    }


    int allPossibleMoves(vector <vector <int>> &temp_rows, vector <vector <int>> &temp_cols, vector <vector <int>> &temp_boxes,
                          vector <vector <int>> &temp_grid, int last_count, int &counter, int row, int col, int area){
        int previous = counter;
        for(int i=0; i<(int)temp_rows[row].size(); i++){
            int is_found = 0;
            for(int j=0; j<(int)temp_cols[col].size(); j++){
                if(temp_rows[row][i] == temp_cols[col][j]){
                    for(int k=0; k<(int)temp_boxes[area].size(); k++){
                        if(temp_rows[row][i] == temp_boxes[area][k]){
                            node *m = new node;
                            m->i1 = row;
                            m->j1 = col;
                            int value = temp_rows[row][i];
                            m->value1 = value;
                            m->remain = last_remain(last_count) - 1;
                            if(elements[last_count].player) m->player = 0;
                            else m->player = 1;
                            optimize(counter+1);
                            add_node(*m);
                            addEdge(last_count, counter);
                            counter++;
                            //cout << "last: " << last_count << " " << " " << row << " " << col << " " << value << endl;
                            if(m->remain == 0) {
                                mini_max(counter-1, last_count, m->player);
                                if(alfa) return 1;
                            }
                            is_found = true;
                            break;
                        }
                    }
                }
                if(is_found) break;
            }
        }
        if(previous == counter) {
            int mm;
            if(elements[last_count].player) mm = 0;
            else mm = 1;
            elements[last_count].minimax = mm;
            if(alfa) return 1;
        }
        return 0;
    }

    void DFS_recur(int now, int last_count, int &counter){

        elements[now].visited = true;

        vector <vector <int>> temp_grid = grid;
        vector <vector <int>> temp_rows = rows;
        vector <vector <int>> temp_cols = cols;
        vector <vector <int>> temp_boxes = boxes;
        set_grid(temp_grid, temp_rows, temp_cols, temp_boxes, now);

        for(int i=0; i<9; i++){
            for(int j=0; j<9; j++){
                int area = findArea(i, j);
                if(temp_grid[i][j] == 0){
                    if(allPossibleMoves(temp_rows, temp_cols, temp_boxes, temp_grid, now, counter, i, j, area)) return;
                }
            }
        }
        for (int i = 0; i < (int)adj[now].size(); ++i){
            int x = adj[now][i];
            if (!elements[x].visited){
                DFS_recur(x, now,counter);
            }
            if(elements[x].minimax != -1){
            	if(elements[x].player){
                    if(elements[now].minimax != 1) elements[now].minimax = elements[x].minimax;
                    if(alfa) if(elements[x].minimax == 1) return;
                }
                else {
                    if(elements[now].minimax != 0) elements[now].minimax = elements[x].minimax;
                    if(alfa) if(elements[x].minimax == 0) return;
                }
			}
        }
    }

};



int main(int argv, char* argc[]){
    if (argc[1] == NULL) {
        cout << "No parameter!" << endl;
        return 1;
    }
    ifstream file(argc[1]);
    if (argc[2] != NULL){
        alfa = 1;
    }
    else alfa = 0;
    int value, remain=0;
    vector <vector <int>> rows(9);
    vector <vector <int>> cols(9);
    vector <vector <int>> boxes(9);
    for(int i=0; i<9; i++){
        for(int j=0; j<9; j++){
            rows[i].push_back(j+1);
            cols[i].push_back(j+1);
            boxes[i].push_back(j+1);
        }
    }
    int area = 0;
    vector <vector <int>> grid(9);
    for(int i=0; i<9; i++){
        for(int j=0; j<9; j++){
            file>>value;
            area = findArea(i, j);
            grid[i].push_back(value);
            if(value){
                rows[i].erase(remove(rows[i].begin(), rows[i].end(), value), rows[i].end());
                cols[j].erase(remove(cols[j].begin(), cols[j].end(), value), cols[j].end());
                boxes[area].erase(remove(boxes[area].begin(), boxes[area].end(), value), boxes[area].end());
            }
            else remain++;
        }
    }

    node m;
    m.value1 = -1;
    m.remain = remain;
    m.player = 0;
    Graph gr(1);
    gr.add_node(m);
    gr.grid = grid;
    gr.rows = rows;
    gr.cols = cols;
    gr.boxes = boxes;
    int counter = 1;
    gr.DFS_recur(0, 0, counter);
    //cout << counter << endl; // number of nodes
    gr.findResult();
    return 0;
}
