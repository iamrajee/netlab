#include<bits/stdc++.h>
using namespace std;

int i,j,k;
long long int INF = INT32_MAX;
int V,E,u,v;
typedef pair<long long int,long long int> pii;
vector < vector <pii>  > adj;
priority_queue <pii, vector<pii>, greater<> > pq;

int src,w,d;
vector <vector <long long int> > dist;

void floyd(){
    for(k=0;k<V;k++){
        for(i=0;i<V;i++){
            for(j=0;j<V;j++){
                if(dist[i][j] > dist[i][k]+dist[k][j]){
                    dist[i][j] = dist[i][k]+dist[k][j];
                }
            }
        }
    }
}

int main(){
    cin>>V>>E;
    adj.resize(V);
    dist.resize(V);
    for(i=0;i<V;i++) dist[i].resize(V,INF);
    for(i=0;i<V;i++) dist[i][i]=0;//i,i = 0
    while(E--){
        cin>>u>>v>>w;
        adj[u].push_back({v,w});
	    dist[u][v]=w;

        adj[v].push_back({u,w}); //since here undirected graph, although this algo. applicable for both graphs
        dist[v][u]=w;
    }

	floyd();
    for(i=0;i<V;i++){
        for(j=0;j<V;j++)
            cout<<dist[i][j]<<" ";
        cout<<endl;
    }
    return 0;
}