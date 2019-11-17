#include<bits/stdc++.h>
using namespace std;

int i,j,k;//helper variable

int id; //id::y
int l;  //packet length::z
float tt; //transmission time ::x
int c;//capacity
float inr;//incoming rate
float dt;//delta t
float t;//time
int cc; //current capacity

int main(int argc, char* argv[]){

    c= atoi(argv[1]);
    inr= atof(argv[2]);
    // cout<<"capacity :"<<c<<", in rate:"<<inr<<"\n\n\n";

    t = 0;//time
    cc = c; //current capacity=full capacity //initially bucket is full

    while(cin>>tt>>id>>l){
        if(tt>=t){//if ariving packet is in future then skip to there
            t = tt;//skip
            cc = min(int(cc + (tt - t)*inr), c);//update cc
        }
        if(l <= cc){ //send whole if posible
            cc -= l;
        }else if(l > cc){ //send whatever possible, for rest reserve time
            dt = ((l-cc)/inr);//t required for rest
            t += dt;//reserve
            cc = 0;
        }
        // cout<<t<<" "<<id<<" "<<l<<endl;
        printf("%.2f %d %d\n", t, id, (int)l);
    }

    return 0;
}