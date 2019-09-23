/**
 * Author: Rajendra Singh 
 * Computer Netoworks Lab 6
 */

#include <bits/stdc++.h>
#include <sys/types.h>
#include <endian.h>
#include <dirent.h>
#include <fstream>
using namespace std;

#define PAYLOAD_SIZE 64

struct ip_header {
    string file_name;
    unsigned int version:4;
    unsigned int ihl:4;
    uint8_t tos;
    uint16_t tot_len;
    uint16_t id;
    uint8_t flags:3;
    uint16_t frag_off:13;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t check;
    uint32_t saddr;
    uint32_t daddr;
    char payload[PAYLOAD_SIZE];// remaining options
};

auto ip_header_cmp = [](ip_header a, ip_header b) {
    return a.frag_off < b.frag_off;
};

typedef set<ip_header, decltype(ip_header_cmp)> ip_set;

void list_files(const char *path, vector<string>& v) {
    struct dirent *dp;
    DIR *dir = opendir(path);

    // Unable to open directory stream
    if (!dir) return; 

    while ((dp = readdir(dir)) != NULL) {
        //printf("%s\n", dp->d_name);
        if (string(dp->d_name) != "." && string(dp->d_name) != "..")
			v.push_back(string(dp->d_name));
    }

    // Close directory stream
    closedir(dir);
}

bool check_integrity(const char *filename){
    // reading a file byte-byte-byte
    ifstream input(filename, ios::binary);
    vector<uint8_t> buffer(istreambuf_iterator<char>(input), {});

    int hlen = (buffer[0] & 0xF); // number of 32-bit words

    vector <uint16_t> header_bytes;
    for (int i= 0; i < 4 * hlen ; i+=2){
        uint16_t temp = ((buffer[i] << 8) | buffer[i+1]);
        header_bytes.push_back(temp);
    }

    unsigned long sum = 0;
    for (int i= 0 ; i < header_bytes.size() ; i++){
        sum += header_bytes[i];
        if (sum >> 16){
            sum = (sum >> 16) + (sum & 0xFFFF);
        }
    }
    while (sum >> 16){
        sum = (sum >> 16) + (sum & 0xFFFF);
    }
    if (sum == 0xFFFF){
        return true;
    }
    else return false;
}

ostream& operator << (ostream &out, const ip_header &ih) {
    out << "Version: " << ih.version << "\n";
    out << "Header Length: " << ih.ihl << "\n";
    out << "Total Length: " << ih.tot_len << "\n";
    out << "Frament Id: " << ih.id << "\n";
    out << "Flags: " << ih.flags << "\n";
    out << "Offset: " << ih.frag_off << "\n";
    out << "Source: " << ((ih.saddr >> 8*3) & 0xFF) << "."
                    << ((ih.saddr >> 8*2) & 0xFF) << "."
                    << ((ih.saddr >> 8*1) & 0xFF) << "."
                    << ((ih.saddr) & 0xF) << "\n";
    out << "Destination: " << ((ih.daddr >> 8*3) & 0xFF) << "."
                    << ((ih.daddr >> 8*2) & 0xFF) << "."
                    << ((ih.daddr >> 8*1) & 0xFF) << "."
                    << ((ih.daddr) & 0xFF) << "\n";

    out << "Payload : " << ih.payload << "\n" ;
    return out;
}

ip_header read_ip_header(const char *filename) {
    // reading a file byte-byte-byte
    ifstream input(filename, ios::binary);
    vector<unsigned char> b(istreambuf_iterator<char>(input), {});
    vector<uint> buffer;
    int hlen = (b[0] & 0xF); // number of 32-bit words : multiples of 4 bytes

    for (auto d : b) {
        buffer.push_back(uint(d));
    }

    ip_header ih = {
        .file_name = string(filename),
        .version = uint((buffer[0] & 0xF0)>>4),
        .ihl = uint(buffer[0] & 0xF),
        .tos = uint8_t(buffer[1]),
        .tot_len = uint16_t(((buffer[2] << 8) | buffer[3])),
        .id = uint16_t((buffer[4] << 8) | buffer[5]),
        .flags = uint8_t(buffer[6]>>5),
        .frag_off = uint16_t((((buffer[6] << 8) | buffer[7]) & 0x1FFF) / 8),  // in bits
        .ttl =  uint8_t(buffer[8]),
        .protocol = uint8_t(buffer[9]),
        .check = uint16_t((buffer[10] << 8) | buffer[11]),
        .saddr = uint32_t((buffer[12] << 8*3) | (buffer[13] << 8*2) | (buffer[14] << 8) | (buffer[15])),
        .daddr = uint32_t((buffer[16] << 8*3) | (buffer[17] << 8*2) | (buffer[18] << 8) | (buffer[19]))
    };

    cout << "flags on fire : " << (buffer[6]&7) << "\n";

    int j = 0;
    for (int i = hlen * 4; i < buffer.size(); ++i, ++j) {
        ih.payload[j] = char(buffer[i]);
    } ih.payload[j] = '\0';

    // cout << "ih.saddr : " <<ih.saddr << endl;

    // cout << "Src : " << buffer[12] << "." << buffer[13] << "." << buffer[14] << "." << buffer[15] << endl;
    // cout << "Dest : " << buffer[16] << "." << buffer[17] << "." << buffer[18] << "." << buffer[19] << endl;

    return ih;
}

int main(int argc, char *argv[]) {
    vector<string> ip_frags;
    vector<string> good_ip_frags;
    vector<ip_header> hdr_vec;
    map<uint16_t, ip_set> m;

    string path(argv[1]);

    list_files(path.c_str(), ip_frags);
    
	for(auto &s : ip_frags) {
        if (check_integrity(string(path + "/" + s).c_str())) {
            good_ip_frags.push_back(string(path + "/" + s));
        }
    }

    cout << "Number of files good: " << good_ip_frags.size() << "\n";


    for(auto &s : good_ip_frags) {
        ip_header ih = read_ip_header(s.c_str());
        hdr_vec.push_back(ih);
        cout << "File : " << s << "\n";
        cout << ih << "\n";

        auto srch = m.find(ih.id);
        if (srch != m.end()) {
            srch->second.insert(ih);
        } else {
            ip_set pack(ip_header_cmp);
            pack.insert(ih);
            m.insert(pair<uint16_t, ip_set>(ih.id, pack));
        }
    }

    for(auto &a : m) {
        string s = "";
        cout << "-------------------------------\nId: " << a.first << "\n";
        for (auto &b : a.second) {
            cout << "File : " << b.file_name << "\t|\t" << b.payload  << "\n";
            s.append(b.payload);
        } cout << "\n" << "Reassembled Message: " << s << "\n";
    }

    cout << endl;

    return 0;
}