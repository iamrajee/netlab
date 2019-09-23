#include<bits/stdc++.h>
using namespace std;
struct IP_HEADER
{
    unsigned char  iph_header_len    :4;
    unsigned char  iph_version       :4;
    unsigned char  iph_tos;				 
    unsigned short iph_total_length;	
    unsigned short iph_id;				
    unsigned char  iph_frag_offset   :5;
    unsigned char  iph_more_fragment :1;
    unsigned char  iph_dont_fragment :1;
    unsigned char  iph_reserved_zero :1;
    unsigned char  iph_frag_offset1;	
    unsigned char  iph_ttl;				
    unsigned char  iph_protocol;		
    unsigned short iph_checksum;		
    //IN_ADDR        iph_source;			
    //IN_ADDR        iph_destination;		
};

int main(){
	ifstream f;
	f.open("ipfrags/ip_0jeTTE", ios::binary);
	//f.open("ipfrags/ip_0vTnYn", ios::binary);
	char * memblock;
	streampos size;
	vector <bool> v;
	if(f.is_open()){
		//f.seekg (0, ios::end);
		//size = f.tellg();
		//memblock = new char[size];
		//vector	<char> memblock(size);
		
		//f.seekg(0, ios::beg);
		//f.read(memblock, size);
		

		vector<unsigned char> buf(istreambuf_iterator<char>(f), {});

		vector <bool> bits(8);		
		unsigned char mask = 1;

		for(auto it:buf){
			// cout<<int(it)<<",";

			for (int i = 0; i < 8; i++) {
				bits[i] = int(it) & (1 << i);
			}
			for (int i = 7; i > -1; i--) {
				// cout<<bits[i];
				v.push_back(bits[i]);
			}
			// cout<<" | ";
		}
		
		cout<<endl;

	}
		//cout<<"\nhi"<<buf.size()<<endl;
	
	for(auto it:v){
		cout<<it;
	}

	int i=0;
	//while(i<size){
	//	cout<<memblock[i];
	//	i++;
	//}
	//for(auto it:buf){
	//	cout<<it;
	//}
	cout<<endl;
	f.close();
	return 0;
}
