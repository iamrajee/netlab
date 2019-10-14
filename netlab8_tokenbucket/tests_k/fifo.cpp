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

int main(int argc, char *argv[]) {
    int buffer_size = atoi(argv[1]);
    float rate = atof(argv[2]);

    float X;    // arrival time
    int Y;      // packet ID
    int Z;      // packet length

    float U;    // transmission time
    int V;      // packet ID
    float W;      // length of packet

    int n = 0;

    float curr_time = 0;
    float next_tran_time = 0;

    vector<packet> v;

    while(cin >> X >> Y >> Z){
        v.push_back(packet(X,Y,Z));
    }


    fifo_buffer buff(buffer_size);

    for (auto &p : v) {
        if (p.len <= buffer_size) {     // else discard
            while(next_tran_time <= p.arr){
                packet t = buff.pop();
                if (t.id != 0) printf("%.2f %d %d\n", next_tran_time, t.id, (int)t.len);

                if (not buff.empty()) {
                    next_tran_time += buff.front().len / rate;
                } else {
                    next_tran_time = p.arr + p.len / rate ;
                }
            }

            buff.push(p);
        }
    }

    // clear the buffer
    while(not buff.empty()) {
        packet t = buff.pop();
        printf("%.2f %d %d\n", next_tran_time, t.id, (int)t.len);
        if (not buff.empty()) {
            next_tran_time += buff.front().len / rate;
        }
    }

    return 0;
}