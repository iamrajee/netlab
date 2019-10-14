#include<bits/stdc++.h>
using namespace std;

int i,j,k;//helper variable

int id; //id::y
int l;  //packet length::z
float tt; //transmission time ::x
int c;//capacity
float inr;//incoming rate
float fullT; //time taken to fill full bucket
float dt;//delta t
float t;//time
int cc; //current capacity

//not required
// typedef tuple<double,int,int> tp;
// queue <tp> q;

int main(int argc, char* argv[]){

    c= atoi(argv[1]);
    inr= atof(argv[2]);
    fullT = c/inr;
    // cout<<"capacity :"<<c<<", in rate:"<<inr<<", fullT:"<<fullT<<"\n\n\n";

    t = 0;//time
    cc = c; //current capacity=full capacity

    while(cin>>tt>>id>>l){
        // cout<<tt<<" "<<id<<" "<<l<<endl; //print before
        // q.push({tt,id,l}); //not required
        if(tt>=t){//if ariving packet is in future then skip to there
            dt = tt - t;
            cc = min(int(cc + dt*inr), c);
            t = tt;//skip
        }
        if(l <= cc){ //send whole if posible
            cc -= l;
        }else if(cc < l){ //send whatever possible, for rest reverse time
            dt = ((l-cc)/inr);//t required for rest
            t += dt;//reserve
            cc = 0;
        }
        // cout<<t<<" "<<id<<" "<<l<<endl;
        printf("%.2f %d %d\n", t, id, (int)l);
    }

    //inside of using queue directly doing in above loop.
    // while(!q.empty()){
    //     auto it = q.front(); q.pop();
    //     // tt,id,l=get<0>(it),get<1>(it),get<2>(it); //gives wrong answer
    //     tt=get<0>(it);
    //     id=get<1>(it);
    //     l =get<2>(it);
    //     cout<<tt<<" "<<id<<" "<<l<<endl;
    // }

    return 0;
}