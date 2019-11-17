#include<bits/stdc++.h>
using namespace std;

int i,j,k;
int V,E,u,v;
typedef pair<int,int> pii;
typedef vector <long long int > vll;
vector < vector <pii>  > adj;
int src,d;
long long int w;
vector <vll> dist;

void bellmanford(){
    // i = src;
    for(int i=0; i<V; i++){
        dist[i][i]=0;
        for(int r=0; r<V-1; r++){//V-1 round or untill there is no changes
            for(int u=0; u<V; u++){//all vertex
                for(auto it:adj[u]){
                    v = it.first;
                    w = it.second;
                    dist[i][v] = min(dist[i][v], dist[i][u]+w);
                }
            }
        }
    }
}

int main(){
    cin>>V>>E;
    adj.resize(V);
    dist.resize(V, vll (V, INT32_MAX) );
    while(E--){
        cin>>u>>v>>w;
        adj[u].push_back({v,w});
	    adj[v].push_back({u,w});
    }
    cin>>src;
	bellmanford();
    for(auto it:dist){
        for(auto it2:it){
            cout<<it2<<" ";
        }
        cout<<endl;
    }
    return 0;
}