#include<iostream>
#include<cstdlib>
#include<cstdio>
using namespace std;

//Class to represent an edge of the graph
class edge
{
public:
int beg,end,flag;    //beginning, end and color of edge
edge(){beg=0;end=0;flag=-1;}
};

//Class to represent a graph
class graph
{
public:
int ver,E,red,blue;    //NO of vertices,NO of edges, red edges and blue edges
edge *ed;        //Array of edges
graph(){ver=0;red=0;blue=0;}
};

//Function to initialize graph
graph* creategraph(int V)
{
    graph* gra=new graph;
    gra->ver = V;
    //Assuming a maximum of 200000 edges
    gra->ed = (edge*) malloc( 200000 * sizeof(edge ) );
    return gra;
}

//Function to update red edges and blue edges
void updateredbluecnt(graph* &G)
{
for(int i=0;i<G->E;i++)
if(G->ed[i].flag==0)G->red++;
else G->blue++;
}
 
//As we have used kruskals algorithm we define union-find classes

// A class to represent a subset for union-find
class subset
{
public:
    int parent;
    int rank;
};
 
//A function to find set of an element (path compression used)
int find(subset subsets[], int i)
{
    // find root and make root as parent of i (path compression)
    if (subsets[i].parent != i)
        subsets[i].parent = find(subsets, subsets[i].parent);
 
    return subsets[i].parent;
}
 
// A function that does union of two sets of x and y (uses union by rank)
void Union(subset subsets[], int x, int y)
{
    int xset = find(subsets, x);
    int yset = find(subsets, y);
 
    //Union by Rank
    if (subsets[xset].rank < subsets[yset].rank)
        subsets[xset].parent = yset;
    else if (subsets[xset].rank > subsets[yset].rank)
        subsets[yset].parent = xset;
    else
    {
        subsets[yset].parent = xset;
        subsets[xset].rank++;
    }

}
 
//Compare edges wrt to color required for qsort
int compare(const void* a, const void* b)
{
   edge* a1 = (edge*)a;
   edge* b1 = (edge*)b;
    return a1->flag > b1->flag;
}

//Compare edges wrt to color required for qsort
int compare1(const void* a, const void* b)
{
   edge* a1 = (edge*)a;
   edge* b1 = (edge*)b;
    return a1->flag < b1->flag;
}

// The main function to construct MST using Kruskal's algorithm with min red edges
graph* gramin(graph* gra)
{
    int V = gra->ver;
    graph *res=creategraph(V);      // This will store the resultant MST

int e =0;  // An index variable, used for result[]
    int i = 0;  // An index variable, used for sorted edges
 
    //Sort all edges by color
    qsort(gra->ed, gra->E, sizeof(gra->ed[0]), compare1);

    // Allocate memory for creating V ssubsets
    subset *subsets =(subset*) malloc( V * sizeof(subset) );
 
    // Create V subsets with single elements
    for (int v = 0; v < V; ++v)
    {
        subsets[v].parent = v;
        subsets[v].rank = 0;
    }
 
    // Number of edges to be taken is equal to V-1
    while (e < V - 1)
    {
        // Step 2: Pick the smallest edge. And increment the index
        // for next iteration
        edge next_edge = gra->ed[i++];
 
        int x = find(subsets, next_edge.beg);
        int y = find(subsets, next_edge.end);
 
        // If including this edge does't cause cycle, include it
        // in result and increment the index of result for next edge
        if (x != y)
        {
            res->ed[e++] = next_edge;
            Union(subsets, x, y);
        }
        // Else discard the next_edge
    }
 res->E=e;

updateredbluecnt(res);



    return res;
}
 
// The main function to construct MST using Kruskal's algorithm with max red edges
graph* gramax(graph* gra)
{
    int V = gra->ver;
    graph *res=creategraph(V);      // This will store the resultant MST

    int e =0;  // An index variable, used for result[]
    int i = 0;  // An index variable, used for sorted edges
 
    //Sort all edges by color
    qsort(gra->ed, gra->E, sizeof(gra->ed[0]), compare);


 
    // Allocate memory for creating V ssubsets
    subset *subsets =(subset*) malloc( V * sizeof(subset) );
 
    // Create V subsets with single elements
    for (int v = 0; v < V; ++v)
    {

        subsets[v].parent = v;
        subsets[v].rank = 0;
    }
 
    // Number of edges to be taken is equal to V-1
    while (e < V - 1)
    {
        // Step 2: Pick the smallest edge. And increment the index
        // for next iteration
        edge next_edge = gra->ed[i++];
  
        int x = find(subsets, next_edge.beg);
        int y = find(subsets, next_edge.end);
 
        // If including this edge does't cause cycle, include it
        // in result and increment the index of result for next edge
        if (x != y)
        {
            res->ed[e++] = next_edge;
            Union(subsets, x, y);
        }
        // Else discard the next_edge
    }
 res->E=e;
updateredbluecnt(res);


    return res;
}

void deletecommon(edge* rededges,graph* gramin,int &redon)
{
for(int i=0;i<gramin->E;i++)
{
for(int j=0;j<redon;j++)
{
if((rededges[j].beg==gramin->ed[i].beg)&&(rededges[j].end==gramin->ed[i].end))
{
for(int l=j;l<redon;l++)
{
rededges[l]=rededges[l+1];
}
redon--;
}
}
}

}


graph* newgraph(graph* gramin,edge *rededges,int redon,int k)
{

deletecommon(rededges,gramin,redon);

int last;
int rr=k-gramin->red;
for(int i=0;i<rr;i++)
{
last=gramin->E;
gramin->ed[last++]=rededges[i];
gramin->E++;
gramin->red++;
graph *gg=gramax(gramin);
gramin=gg;

}
return gramin;
}

void outputgraph(graph* res)
{

for (int i = 0; i < res->E; ++i)
        printf("%d %d\n", res->ed[i].beg+1, res->ed[i].end+1);


}


// Driver program to test above functions
int main()
{
    int V,k;  // Number of vertices in graph
cin>>k>>V;
int u,v,f,i=0;
    graph* gra = creategraph(V);
 
for(u=1;u<=V;u++)
{
while(1)
{
ip:
cin>>v;
if(v==-1)break;
cin>>f;
for(int j=0;j<i;j++)
if((gra->ed[j].beg==v-1)&&(gra->ed[j].end==u-1))
goto ip;
gra->ed[i].beg=u-1;
gra->ed[i].end=v-1;
gra->ed[i].flag=f;
gra->E=i+1;
i+=1;
}
}
 
graph* redmax=gramax(gra);
graph* redmin=gramin(gra); 

if((k<redmin->red)||(k>redmax->red))
{
cout<<"NO"<<"\n";
return 0;
}
else if(redmax->red==k)
{
cout<<"YES"<<"\n";
outputgraph(redmax);
return 0;
}
else if(redmin->red==k)
{
cout<<"YES"<<"\n";
outputgraph(redmin);
return 0;
}

edge *rededges=(edge*) malloc( 200000 * sizeof(edge ) );
int l=0;
for(int p=0;p<redmax->E;p++)
if(redmax->ed[p].flag==0)
rededges[l++]=redmax->ed[p];



cout<<"YES"<<"\n";
outputgraph(newgraph(redmin,rededges,l,k));



return 0;
}
