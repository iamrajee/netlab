#include<bits/stdc++.h>
using namespace std;

int i,j,t,k;
int y,z;
double x;
int c,outr,inr;
typedef tuple<double,int,int> tp;
// priority_queue <tp, vector<tp>, greater<> > pq;

queue <tp> q;

int main(int argc, char* argv[]){

    c= stoi(argv[1]);
    inr= stoi(argv[1]);

    cout<<"c:"<<c<<", inr:"<<inr<<"\n\n\n";

    while(cin>>x>>y>>z){
        // cout<<x<<" "<<y<<" "<<z<<endl;
        q.push({x,y,z});
    }

    while(!q.empty()){
        auto it = pq.top();
        q.pop();
        // auto it = pq.top();
        // pq.pop();
        // cout<<get<0>(it)<<" "<<get<1>(it)<<" "<<get<2>(it)<<endl;
    }

    return 0;
}