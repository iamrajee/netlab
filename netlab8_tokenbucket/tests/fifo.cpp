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
    cc = 0; //current capacity=0

    while(cin>>tt>>id>>l){
        // cout<<tt<<" "<<id<<" "<<l<<endl; //print before
        if (l <= c) { //if len < max q size else discard
            while(tt>=t){ //packet of future
                if(!q.empty()){ //one packet sent
                    auto it = q.front(); q.pop();
                    // tt_=get<0>(it);
                    tt_ = t;
                    id_=get<1>(it);
                    l_ =get<2>(it);
                    cc-=l_;
                    // cout<<t<<" "<<id_<<" "<<l_<<endl;
                    printf("%.2f %d %d\n", tt_, id_, (int)l_);
                }

                if (!q.empty()) {
                    l_ = get<2>(q.front());
                    t += l_/inr;
                }else{
                    t = tt + l / inr ; //skip to time taken by curr packet
                }
            }
            if(cc+l<=c){
                q.push({tt,id,l});
                cc+=l;
            }
        }
    }

    // empty rest of q
    while(!q.empty()) {

        //remove one
        auto it = q.front(); q.pop();
        // tt_=get<0>(it);
        tt_=t;
        id_=get<1>(it);
        l_ =get<2>(it);
        cc-=l_;
        printf("%.2f %d %d\n", tt_, id_, (int)l_);
        
        if(!q.empty()){
            l_ = get<2>(q.front());
            t += l_/inr;
        }
    }

    return 0;
}