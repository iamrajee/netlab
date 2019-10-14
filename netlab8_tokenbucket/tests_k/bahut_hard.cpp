#include <bits/stdc++.h>

using namespace std;

struct packet{
    float arr;
    int id;
    float len;
    packet(float _X=0, int _Y=0, float _Z=0) : arr{_X}, id{_Y}, len{_Z} {}
};


struct fifo_buffer {
    queue<packet> q;
    int max_size;
    int curr_size;

    fifo_buffer(int s) : max_size{s}, curr_size{0} {}

    bool push(packet p) {
        if (p.len + curr_size <= max_size) {
            q.push(p);
            curr_size += p.len;
            return true;
        } 
        return false;
    }

    packet pop() {
        if(not q.empty()){
            packet p = q.front();
            q.pop();
            curr_size -= p.len;
            return p;
        } return packet();
    }

    bool empty() {return q.empty();}

    packet front() {
        return q.front();
    }
};


bool is_packet_dropped(float x, vector<packet> &for_shape){
    int full_bucket_size = 500;
    int n = 0;
    float t = 0;
    int curr_bucket_size = full_bucket_size;
    float curr_time = 0;

    vector<packet> for_fifo;

    for (auto &p : for_shape) {
        float dt;
        if (p.arr >= curr_time) {
            dt = p.arr - curr_time;
            curr_bucket_size = min(int(curr_bucket_size + dt * x), full_bucket_size);
            curr_time = p.arr;
        }

        float L = p.len;
        if (L <= curr_bucket_size) {
            curr_bucket_size -= L;
        } else if (curr_bucket_size < L) {
            dt = ((L-curr_bucket_size)/x);
            curr_time += dt;
            curr_bucket_size = 0;
        } 
        
        for_fifo.push_back(packet(curr_time, p.id, p.len));
    }

    float next_tran_time = 0;
    int buffer_size = 1000;
    float rate = 10.0;

    fifo_buffer buff(buffer_size);

    bool packet_dropped = false;

    for (auto &p : for_fifo) {
        if (p.len <= buffer_size) {
            while(next_tran_time <= p.arr){
                packet t = buff.pop();
                // if (t.id != 0) printf("%.2f %d %d\n", next_tran_time, t.id, (int)t.len);

                if (not buff.empty()) {
                    next_tran_time += buff.front().len / rate;
                } else {
                    next_tran_time = p.arr + p.len / rate ;
                }
            }
            packet_dropped = not buff.push(p);
            if (packet_dropped) {
                break;
            }
        }
    }
    return packet_dropped;
}


int main(int argc, char *argv[]) {
    float x; // to find

    float X;    // arrival time
    int Y;      // packet ID
    int Z;      // packet length

    float U;    // transmission time
    int V;      // packet ID
    float W;      // length of packet

    
    vector<packet> for_shape;

    while(cin >> X >> Y >> Z){
        for_shape.push_back(packet(X,Y,Z));
    }

    float hi = 1000.0;      // adjust so that packet drop must be true
    float lo = 1.0;         // adjust so that packet drop must be false
    float mid;

    cout << "Is Packet drop at " << hi << " : " << is_packet_dropped(hi, for_shape) << endl;    // must be true
    cout << "Is Packet drop at " << lo << " : " << is_packet_dropped(lo, for_shape) << endl;    // must be false

    cout << "Binary Search .. " << endl;
    cout << setprecision(7);

    while (abs(hi - lo) > 1e-6) {
        
        cout << "lo : " << lo <<" hi : " << hi << " mid : " << mid << endl;
        mid = (hi + lo) / 2;

        if (is_packet_dropped(mid, for_shape)) {
            hi = mid;
        } else {
            lo = mid;
        }        
    }

    cout << "Is Packet drop at " << mid << " : " << is_packet_dropped(mid, for_shape) << endl;    // must be false

    cout << "Value of x is " << mid << endl;

    return 0;
}