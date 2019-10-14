#include <bits/stdc++.h>

using namespace std;

struct packet{
    float arr;
    int id;
    float len;
    packet(float _X=0, int _Y=0, float _Z=0) : arr{_X}, id{_Y}, len{_Z} {}
};

int main(int argc, char *argv[]) {
    int full_bucket_size = atoi(argv[1]);
    float rate = atof(argv[2]);

    float full_bucket_time = full_bucket_size / rate;
    // cout << "full bucket time : " << full_bucket_time << endl;

    float X;    // arrival time
    int Y;      // packet ID
    int Z;      // packet length

    float U;    // transmission time
    int V;      // packet ID
    float W;      // length of packet

    int n = 0;
    float t = 0;
    int curr_bucket_size = full_bucket_size;
    float curr_time = 0;

    vector<packet> q;

    while(cin >> X >> Y >> Z){
        q.push_back(packet(X,Y,Z));
    }

    for (auto &p : q) {
        float dt;
        if (p.arr >= curr_time) {
            dt = p.arr - curr_time;
            curr_bucket_size = min(int(curr_bucket_size + dt * rate), full_bucket_size);
            curr_time = p.arr;
        }

        // cout << "Curr time : " << curr_time << "\t curr_size: " << curr_bucket_size << endl;

        float L = p.len;
        if (L <= curr_bucket_size) {
            curr_bucket_size -= L;
        } else if (curr_bucket_size < L) {
            // send when capacity is equal to L
            dt = ((L-curr_bucket_size)/rate);
            curr_time += dt;
            curr_bucket_size = 0;
        } 
        printf("%.2f %d %d\n", curr_time, p.id, (int)p.len);
    }

    return 0;
}