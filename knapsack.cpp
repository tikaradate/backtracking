#include <vector>
#include <algorithm>
#include <set>
#include <iostream>




/* NAIVE IMPLEMENTATION */
// no pruning

std::vector<int> x, opt_x, p, w;
int xp, n, m;

void knapsack1(int l){
    if (l == n){
        int s = 0;
        for(int i = 0; i < n; i++){
            s += w[i]*x[i];
        }
        if(s < m){
            int curp = 0;
            for(int i = 0; i < n; i++){
                curp += p[i]*x[i];
            }
            if(curp > xp){
                xp = curp;
                opt_x = x;
            }
        }
    } else {
        x[l] = 0;
        knapsack1(l+1);
        x[l] = 1;
        knapsack1(l+1);
    }
}

// pruning

std::vector<int> x, opt_x, p, w;
int xp, n, m;
std::vector<std::set<int>> choice;

void knapsack2(int l, int curW){
    if (l == n){
        int s = 0;
        for(int i = 0; i < n; i++){
            s += w[i]*x[i];
        }
        if(s < m){
            int curp = 0;
            for(int i = 0; i < n; i++){
                curp += p[i]*x[i];
            }
            if(curp > xp){
                xp = curp;
                opt_x = x;
            }
        }
        
    } else {
        // builds the choice set
        if(curW + w[l] <= m){
            choice[l].insert(0);
            choice[l].insert(1);
        } else {
            choice[l].insert(0);
        }
    }
        // runs through the choice set and recursively calls the function
    for(auto member: choice[l]){
        x[l] = member;
        knapsack2(l+1, curW + w[l]*x[l]);
    }
    
    
}

// bounding

// auxiliary bounding function for the knapsack problem

std::vector<double> x, opt_x, p, w;
int xp, n, m;

double rational_knapsack(std::vector<int> w, std::vector<int> p, int m){
    // getting the weights and profits in reverse order
    std::sort(w.begin(), w.end());
    std::reverse(w.begin(), w.end());
    std::sort(p.begin(), p.end());
    std::reverse(p.begin(), p.end());

    for(int j = 0; j < n; j++){
        x[j] = 0;
    }
    double weight = 0;
    double profit = 0;
    int i = 0;
    while(weight < m && i < n){
        if(weight + w[i] <= m){
            x[i] = 1;
            weight += w[i];
            profit += p[i];
            i++;
        } else {
            x[i] = (m - weight)/w[i];
            weight = m;
            profit += x[i]*p[i];
            i++;
        }
    }
    return profit;
}

void knapsack3(int l, int curW){
    if (l == n){
        int s = 0;
        for(int i = 0; i < n; i++){
            s += w[i]*x[i];
        }
        if(s < m){
            int curp = 0;
            for(int i = 0; i < n; i++){
                curp += p[i]*x[i];
            }
            if(curp > xp){
                xp = curp;
                opt_x = x;
            }
        }
    } else {
        // builds the choice set
        if(curW + w[l] <= m){
            choice[l].insert(0);
            choice[l].insert(1);
        } else {
            choice[l].insert(0);
        }
    }
    
    double b = 0;
    for(int i = 0; i < l; i++){
        b += p[i]*x[i] + rational_knapsack(p, w, m - curW);
    }

    // runs through the choice set and recursively calls the function
    for(auto member: choice[l]){
        if(b <= xp) return;
        x[l] = member;
        knapsack3(l+1, curW + w[l]*x[l]);
    }
}


// branch & bound

void knapsack4(int l, int curW){
    if (l == n){
        int s = 0;
        for(int i = 0; i < n; i++){
            s += w[i]*x[i];
        }
        if(s < m){
            int curp = 0;
            for(int i = 0; i < n; i++){
                curp += p[i]*x[i];
            }
            if(curp > xp){
                xp = curp;
                opt_x = x;
            }
        }
    } else {
        // builds the choice set
        if(curW + w[l] <= m){
            choice[l].insert(0);
            choice[l].insert(1);
        } else {
            choice[l].insert(0);
        }
    }
    
    double b = 0;
    for(int i = 0; i < l; i++){
        b += p[i]*x[i];
    }
    b += + rational_knapsack(p, w, m - curW);

    std::vector<std::pair<int, int>> choice_bound;
    int count = 0;
    for(auto member: choice[l]){
        if(b <= xp) return;
        x[l] = member;
        choice_bound.push_back(std::make_pair(x[l], b+(p[l]*x[l])));
        count++;
    }
    std::sort(choice_bound.begin(), choice_bound.end(), [](auto &left, auto &right) {
        return left.second < right.second;
    });
    for(int i = 0; i < count; i++){
            if(choice_bound[i].second <= xp) return;
            x[l] = choice_bound[i].first;
            knapsack4(l + 1, curW + w[l]*x[l]);
    }
}


int main(){
    
    return 0;
}