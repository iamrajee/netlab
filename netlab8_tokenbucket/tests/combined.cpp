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
// float inr;//incoming rate
float highr;//high rate
float lowr;//low rate
float dt;//delta t
float t;//time
int cc; //current capacity
// bool dropped;

//not required
typedef tuple<double,int,int> tp;

vector <tp> shape_input;

bool check_if_dropped(int inr){
    
    //===========start fresh=====
    vector <tp> fifo_input;
    queue <tp> q; //initialise for each call
    // q.clear();//error
    fifo_input.clear();

    //======================================== shape ===========================
    t = 0;//time
    c=500;//fixed

    cc = c; //current capacity=full capacity
    // inr= as per function argument

    for(auto ele:shape_input){
        tt=get<0>(ele);
        id=get<1>(ele);
        l=get<2>(ele);
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
        // printf("%.2f %d %d\n", t, id, (int)l); //dont print here inside push in vector for fifo input
        fifo_input.push_back({t,id,l});
    }

    //======================================== fifo ===========================
    t = 0;//time
    cc = 0;
    
    //!! fix them
    int cfifo = 1000;
    float inrfifo = 10;
    
    bool dropped = 0;
    
    for(auto ele:fifo_input){
        tt=get<0>(ele);
        id=get<1>(ele);
        l=get<2>(ele);
        if (l <= cfifo) { //if len < max q size else discard
            while(t <= tt){
                if(!q.empty()){
                    auto it = q.front(); q.pop();
                    tt_=get<0>(it);
                    id_=get<1>(it);
                    l_ =get<2>(it);
                    cc-=l_;
                    // printf("%.2f %d %d\n", t, id_, (int)l_);
                }

                if (!q.empty()) {
                    l_ = get<2>(q.front());
                    t += l_/inrfifo;
                } else {
                    t = tt + l / inrfifo ; //skip to time taken by curr packet
                }
            } 
            //push when current time arrives
            if(cc+l<=cfifo){
                q.push({tt,id,l});
                cc+=l;
                dropped = 0;
            }else{//dropped
                dropped = 1;
                break;
            }
        }
    }
    //===================================================================

    return dropped;
}

float bs(float hr, float lr){
    float mid;
    while(abs(hr-lr)>1e-6){
        mid = (hr+lr)/2;
        cout<<"hr:"<<hr<<", mid:"<<mid<<", lr:"<<lr<<endl;
        if(check_if_dropped(mid)){ //dropped
            hr=mid;
            // mid = bs(hr,lr);
        }else{
            lr=mid;
            // mid = bs(hr,lr);
        }
    }
    return lr;
}

int main(int argc, char* argv[]){
    
    while(cin>>tt>>id>>l){
        // cout<<tt<<" "<<id<<" "<<l<<endl; //print before
        shape_input.push_back({tt,id,l});
    }

    highr=4.0;
    lowr=1.0;

    cout<<"lowr:"<<lowr<<", highr:"<<highr<<endl;

    cout<<"dropped(lowr) : "<<check_if_dropped(lowr)<<endl;
    cout<<"dropped(highr) : "<<check_if_dropped(highr)<<endl;
    cout<<"dropped(2) : "<<check_if_dropped(2)<<endl;
    cout<<"dropped(1.95) : "<<check_if_dropped(1.95)<<endl;
    cout<<"dropped(1.99) : "<<check_if_dropped(1.99)<<endl;

    float safe_rate = bs(highr,lowr);

    cout<<"safe_rate = "<<safe_rate<<endl;
    printf("safe_rate := %.2f\n", safe_rate);

    cout<<"dropped(safe_rate) : "<<check_if_dropped(safe_rate)<<endl;

    return 0;
}