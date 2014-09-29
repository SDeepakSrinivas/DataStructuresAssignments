#include<iostream>
#include<cstring>
#include<cstdlib>
#include<cmath>
using namespace std;

//Global variables for maximum time, total no. of queues, no. of input queues

float maxtime;
int totalq;
int inputq;

//To generate a pseudo random number for the given rate

float genexp(float rate)
{
float temp = ((1.0*rand())/RAND_MAX);
return (1/rate) * log(1/(1-temp));
}

//Class definition of minheap winth entry time and exit time

class minheap
{
public:
float time;
float itime;
int q;
};

//Function definition of swap() which is used in 'bubble up' in insert

void swap( minheap *x , minheap *y)
{
minheap temp;
temp = *x;
*x = *y;
*y = temp;
}

//Function definition of insert() used to insert elements in a heap

void insert( minheap customer[], minheap p , int *size)
{
int s = *size;
customer[++s] = p;
int i = s;
//bubble up
while( i != 1)
{
if( customer[i].time < customer[i/2].time)
{
swap( &customer[i], &customer[i/2]);
i = i/2;
}
else
break;
}
*size = s;
}

//Function definition of delete() for deleting an element from heap

minheap delete_min( minheap customer[], int *size)
{
int s = *size;
swap( &customer[1], &customer[s]) ;
s--;
*size = s;
int i = 1;
//shift down
while((2*i <= s) && ((2*i + 1) <= s ))
{
if( customer[2*i].time < customer[(2*i)+1].time )
{
if( customer[2*i].time < customer[i].time)
{
swap( &customer[2*i], &customer[i] );
i = 2*i;
}
else
return customer[s+1];
}
else
{
if( customer[(2*i)+1].time < customer[i].time)
{
swap( &customer[(2*i)+1], &customer[i] );
i = 2*i +1;
}
else
return customer[s+1];
}
}
if((2*i <= s) && ((2*i + 1) > s ))
{
if( customer[2*i].time < customer[i].time)
swap( &customer[2*i], &customer[i] );
else
return customer[s+1];
}
return customer[s+1];
}


int main()
{
//Declaration and inputing all the desired variables
int i = 0, j = 0, a = 0, size = 0, totalsize;
float t = 0 , tp, totaltime = 0, probab , pr = 0;
minheap cuss;
cin >> maxtime;
cin >> totalq;
cin >> inputq;
float lambda[totalq-1];    //rate of service at all queues
int noofout[totalq-1];    //Contains no. of queues to which a given queue is linked
int nextqu[totalq-1][totalq-1];    //Contains the queue numbers of the queues to which a given queue is linked
float tperq[totalq-1];    //Contains time for each queue
float nextqprob[totalq-1][totalq-1];    //Contains the probab of going to each queue
//Inputing rates and setting tperq=0
for( i = 0; i < totalq-1 ; i++)
{
cin >> lambda[i];
tperq[i] = 0;
}
//Inputing the probabilities
for( i = 0; i < totalq-1 ; i++)
{
cin >> noofout[i];
for (j = 0; j < noofout[i]; j++)
cin >> nextqu[i][j] >> nextqprob[i][j];
}

//Finding an approx. vaue for no. of minheap objects to be declared
for( i = 0 ; i < inputq ; i++)
a = a + lambda[i];
a = 1.3*a*maxtime;
minheap *customer=new minheap[a];

//Inserting the events (input queues) in a  minheap
for ( i = 0 ; i < inputq ; i++)
{

//While loop shall run till the maxtime is reached
while( t < maxtime)
{
tp = genexp(lambda[i]);
t += tp;
cuss.time = t;
cuss.itime = t;
cuss.q = i;
insert(customer , cuss , &size);

}
t = 0;
}

//Total size is the total no of customers gone into counters
totalsize = size;

//Implementing the whole heap, by checking probabilities etc.
while( size >= 1)
{
cuss = delete_min(customer, &size);

//Checking which queue to go to
probab = 1.0*rand()/RAND_MAX;
for( i = 0; i < noofout[cuss.q] ; i++)
{
if( i == noofout[cuss.q]-1)
pr = 1.0;
else
pr += nextqprob[cuss.q][i];
if( probab <= pr)
{
cuss.q = nextqu[cuss.q][i];
break;
}
}
pr = 0;

//If output queue
if( cuss.q == totalq-1)
{
totaltime = totaltime + cuss.time - cuss.itime ;
continue;
}

//Generating random time
tp = genexp(lambda[cuss.q]);

//Checking which time is greater
if( cuss.time < tperq[cuss.q])
cuss.time = tp + tperq[cuss.q];
else
cuss.time += tp ;

//Updating time for each queue to the present time
tperq[cuss.q] = cuss.time;
insert(customer , cuss , &size);
}

//Calculating average time for each customer by dividing totaltime by no. of customers
float avgtime;
avgtime = totaltime/totalsize;

//Output the average time
cout << avgtime <<"\n";

}
