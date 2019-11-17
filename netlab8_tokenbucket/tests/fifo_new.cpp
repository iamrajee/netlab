#include<bits/stdc++.h>
using namespace std;

int i,j,k;//helper variable

int id; //id::y
int id_;
int l;  //packet length::z
int l_;
float tt; //transmission time ::x
float tt_;
int c;//capacity
float inr;//incoming rate
float dt;//delta t
float t;//time
int cc; //current capacity

typedef tuple<double,int,int> tp;
queue <tp> q;

int main(int argc, char* argv[]){

    c= atoi(argv[1]);
    inr= atof(argv[2]);
    // cout<<"capacity :"<<c<<", in rate:"<<inr<<"\n\n\n";

    t = 0;//time
    cc = c; //current capacity=0
    float oldTS = -1;
    
    while(cin>>tt>>id>>l){
        // cout<<tt<<" "<<id<<" "<<l<<endl; //print before
        if (l > c) continue; //if len > max q size then discard

        t = max(tt,oldTS);

        while(!q.empty() && get<0>(q.front())<=t){ //packet of future
            auto it = q.front(); q.pop();
            l_ =get<2>(it);
            cc+=l_;
        }
        if(l>cc) continue;
        oldTS = t + l/inr;
        printf("%.2f %d %d\n", oldTS, id, l);
        cc-=l;
        q.push({oldTS,id,l});
    }

    while(!q.empty()){
        auto it = q.front(); q.pop();
        l_ =get<2>(it);
        cc+=l_;
    }

    return 0;
}