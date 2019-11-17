#include<bits/stdc++.h>
#include <sys/types.h>
#include <endian.h>
#include <dirent.h>
#include <fstream>
using namespace std;

int i,j,k,t,r,sum,ctr;
int ihl;
string filedir = "ipfrags";
vector <string> vs;
typedef vector <int> vi;


struct ip_header {
    string filename;
    unsigned int version:4;
    unsigned int ihl:4;//header length
    uint8_t tos;//service
    uint16_t tl;//total len
    uint16_t id;//identiifcation
    uint8_t flags:3;//flag
    uint16_t frag_off:13;//fragmentation offset
    uint8_t ttl;//time of live
    uint8_t protocol;
    uint16_t check;//checksum
    uint32_t saddr;
    uint32_t daddr;
    char payload[64];// remaining options
};
vector <struct ip_header> viph;


bool ipshort(struct ip_header a, struct ip_header b){
	return a.frag_off < b.frag_off;
}

int main(){

	//============================= read also file name ====================//
	DIR *dir = opendir(filedir.c_str());
	if (!dir) return 0; //invalid file
	struct dirent *dp;
    while ((dp = readdir(dir)) != NULL) { 
        if (string(dp->d_name) != "." && string(dp->d_name) != "..")//also return "." and ".."
			vs.push_back(string(dp->d_name));
    }
    closedir(dir);
	//======================================================================

	//for all files
	ctr=0;
	for(auto s:vs){
		ifstream f;
		
		f.open(("ipfrags/"+s).c_str(), ios::binary);
		vi bufint;
		if(f.is_open()){
			vector<unsigned char> buf(istreambuf_iterator<char>(f), {}); //reading byte by btye
			for(auto it:buf){
				bufint.push_back(int(it));
			}
		}



		//================= check if good or bad(checksum) ================//
		ihl=bufint[0] & 0xF;
		sum=0;
		for(i=0; i<4*ihl; i+=2){//for ihl bit
			sum+=((bufint[i]<<8) | bufint[i+1]);
			while(sum>>16){
				sum = (sum & 0xFFFF) + (sum>>16);
			}
		}
		while(sum>>16){
			sum = (sum & 0xFFFF) + (sum>>16);
		}
		if(sum!=0xFFFF) continue; //continue if bad
		ctr++;
		// cout<<("ipfrags/"+s).c_str()<<endl;
		// cout<<ihl<<"\n"<<endl;
		//========================================================


		//========================= find ip header =======================//
		ip_header ih = {
			.filename = s,
        	.version = uint((bufint[0] & 0xF0)>>4),
        	.ihl = uint(bufint[0] & 0xF),
        	.tos = uint8_t(bufint[1]),
        	.tl = uint16_t(((bufint[2] << 8) | bufint[3])),
        	.id = uint16_t((bufint[4] << 8) | bufint[5]),
        	.flags = uint8_t(bufint[6]>>5),
        	.frag_off = uint16_t((((bufint[6] << 8) | bufint[7]) & 0x1FFF) / 8),  // given bits, we want in bytes
        	.ttl =  uint8_t(bufint[8]),
        	.protocol = uint8_t(bufint[9]),
        	.check = uint16_t((bufint[10] << 8) | bufint[11]),
        	.saddr = uint32_t((bufint[12] << 8*3) | (bufint[13] << 8*2) | (bufint[14] << 8) | (bufint[15])),
        	.daddr = uint32_t((bufint[16] << 8*3) | (bufint[17] << 8*2) | (bufint[18] << 8) | (bufint[19]))
		};
		j=0;
		for(i=4*ihl; i<bufint.size(); i++,j++){
			ih.payload[j] = char(bufint[i]);
    	}
		ih.payload[j] = '\0';
		//=================================================================//
		// cout<<"id:"<<ih.id<< "  flag:" <<ih.flags<<"  offset:"<< ih.frag_off <<endl;

		viph.push_back(ih);
		f.close();
	}

	sort(viph.begin(), viph.end(), ipshort);

	for(auto ih:viph){
		cout<<"id:"<<ih.id<< "  flag:" <<ih.flags<<"  offset:"<< ih.frag_off <<endl;
	}

	cout<<"\nctr=>"<<ctr<<endl;
	return 0;
}