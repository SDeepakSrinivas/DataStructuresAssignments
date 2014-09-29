#include <iostream>
#include<cstdlib>
#include<cstring>
#include<ctime>
#include<stack>
using namespace std;

class treapnode
{
public:
treapnode *parent,*left,*right;    //parent to check for priorities condition
int n;    //no. in node
float p;    //priority in node
int leftcnt;    //no of nodes in left subtree

treapnode();    //constructor
treapnode* search(int k);    //search in a subtree from this node
void insert(int k,float pr);
void remove(int k);
void findmsm(int m);    //mth smallest
};

class treap
{
public:
treapnode *root;
treap();
treapnode* search(int k);    //to search in this tree
void insert(int k,float pr);    //to insert in this tree
void remove(int k);    //to delete from this tree
void findmthsmallest(int m);    //to find the mth smallest element
};

treap::treap()
{
root=NULL;
}
treapnode::treapnode()
{
parent=NULL;
left=NULL;
right=NULL;
n=0;
p=0.0;
leftcnt=0;
}

treapnode* treapnode::search(int k)
{
if(this==NULL)
return NULL;
if(n==k)
return this;
else if(n>k)
return (this->left)->search(k);
else if(n<k)
return (this->right)->search(k);
}


treapnode* treap::search(int k)
{
return root->search(k);
//returns the node with key or null if key is not present
}



//A function to update leftcnt for all parents
void updateleftcnt(treapnode* node,int i)
{
while(node!=NULL)
{
if(node->parent!=NULL)
if(node->parent->left==node)
node->parent->leftcnt+=i;
node=node->parent;
}

}

//Function to rotate left in bubbleup and shiftdown
void rotateleft(treapnode* p1,treapnode* p2)
{

p2->parent=p1->parent;
if(p1->parent!=NULL)
{
if(p1->parent->left==p1)
p1->parent->left=p2;
else if(p1->parent->right==p1)
p1->parent->right=p2;
}
p1->parent=p2;
if(p2->left!=NULL)
p2->left->parent=p1;
p1->right=p2->left;
p2->left=p1;
p2->leftcnt+=(1+p1->leftcnt);

}


//Function to rotate right in bubbleup and shiftdown
void rotateright(treapnode* p1,treapnode* p2)
{
p2->parent=p1->parent;
if(p1->parent!=NULL)
{
if(p1->parent->left==p1)
p1->parent->left=p2;
else if(p1->parent->right==p1)
p1->parent->right=p2;
}
p1->parent=p2;
if(p2->right!=NULL)
p2->right->parent=p1;
p1->left=p2->right;
p2->right=p1;
p1->leftcnt-=(1+p2->leftcnt);
}


//A function to balance treap after insertion
void balanceforins(treapnode* par,treapnode* chi)
{
if(par==NULL)
return;
if(par->p>chi->p)
{
if(par->right==chi)
rotateleft(par,chi);
else if(par->left==chi)
rotateright(par,chi);

balanceforins(chi->parent,chi);
}


}


//Search position for insert
treapnode* searchforins(treapnode* node,int k)
{
if(node==NULL)
return NULL;
if((node->left==NULL)&&(node->right==NULL))
return node;
else if(node->n>=k)
{
if(node->left!=NULL)
return searchforins(node->left,k);
else
return node;
}
else if(node->n<k)
{
if(node->right!=NULL)
return searchforins(node->right,k);    
else
return node;

}
}

//Function to insert
void treapnode::insert(int k,float pr)
{

treapnode* temp=new treapnode;
temp->n=k;
temp->p=pr;

treapnode* temp1=new treapnode;

temp1=searchforins(this,k);

temp->parent=temp1;
if(k<=temp1->n)
temp1->left=temp;
else if(k>temp1->n)
temp1->right=temp;
updateleftcnt(temp,1);
balanceforins(temp1,temp);


}

void treap::insert(int k,float pr)
{
if(root==NULL)
{
treapnode* temp=new treapnode;
temp->n=k;
temp->p=pr;
root=temp;
}
else
root->insert(k,pr);

//This is to ensure the change of root (a method instead of changing root at each rotation)
treapnode* t=new treapnode;
t=root;
while(t->parent!=NULL)
t=t->parent;
root=t;

}



//A function to balance for delete
void balancefordel(treapnode* node)
{
if((node->left==NULL)&&(node->right==NULL))
return;

if(node->left!=NULL)
if(node->p>node->left->p)
if(node->right!=NULL)
{
if(node->left->p<node->right->p)
{rotateright(node,node->left);
balancefordel(node);
}
}
else if(node->right==NULL)
{rotateright(node,node->left);
balancefordel(node);
}

if(node->right!=NULL)
if(node->p>node->right->p)
if(node->left!=NULL)
{
if(node->right->p<node->left->p)
{rotateleft(node,node->right);
balancefordel(node);
}
}
else if(node->left==NULL)
{rotateleft(node,node->right);
balancefordel(node);
}


}



void removenode(treapnode* temp)
{


if((temp->left==NULL)&&(temp->right==NULL))
{
updateleftcnt(temp,-1);
if(temp->parent!=NULL)
{
if(temp->parent->left==temp)
temp->parent->left=NULL;
else if(temp->parent->right==temp)
temp->parent->right=NULL;
}
temp->parent=NULL;
delete temp;
}


else if((temp->left!=NULL)&&(temp->right==NULL))
{
updateleftcnt(temp,-1);
temp->left->parent=temp->parent;
if(temp->parent!=NULL)
{
if(temp->parent->left==temp)
temp->parent->left=temp->left;
else if(temp->parent->right==temp)
temp->parent->right=temp->left;
}
temp->parent=NULL;
temp->left=NULL;
delete temp;

}

else if((temp->left==NULL)&&(temp->right!=NULL))
{
updateleftcnt(temp,-1);
temp->right->parent=temp->parent;
if(temp->parent!=NULL)
{
if(temp->parent->left==temp)
temp->parent->left=temp->right;
else if(temp->parent->right==temp)
temp->parent->right=temp->right;
}
temp->parent=NULL;
temp->right=NULL;
delete temp;


}

else if((temp->left!=NULL)&&(temp->right!=NULL))
{
treapnode* suc=new treapnode;
suc=temp->left;
while(suc->right!=NULL)
suc=suc->right;
temp->n=suc->n;
temp->p=suc->p;
removenode(suc);
balancefordel(temp);
}
}


void treapnode::remove(int k)
{
treapnode* temp=this->search(k);
removenode(temp);
}


void treap::remove(int k)
{
//If key not present
if(root->search(k)==NULL)
{
cout<<"Key not present\n";
return;
}

//If treap has only single element 
if((root->n==k)&&((root->left==NULL)&&(root->right==NULL)))
{
root=NULL;
return;
}

//If root has the element
if(root->n==k)
{
if((root->left!=NULL)&&(root->right==NULL))
{
root->left->parent=NULL;
root=root->left;
return;
}
}
if(root->n==k)
{
if((root->left==NULL)&&(root->right!=NULL))
{
root->right->parent=NULL;
root=root->right;
return;
}
}

if((root->n==k)&&(root->left!=NULL)&&(root->right!=NULL))
{
treapnode* suc=new treapnode;
suc=root->left;
while(suc->right!=NULL)
suc=suc->right;
root->n=suc->n;
root->p=suc->p;
removenode(suc);
balancefordel(root);
treapnode* t=new treapnode;
t=root;
while(t->parent!=NULL)
t=t->parent;
root=t;
return;
}
//Otherwise
root->remove(k);
}

//Find mth smallest node in the subtree rooted at this node
void treapnode::findmsm(int m)
{
if(m<=leftcnt)
left->findmsm(m);
else if(m==leftcnt+1)
cout<<n<<"\n";
else if(m>leftcnt)
right->findmsm(m-(1+leftcnt));
}

//To find mth smallest node in this treap
void treap::findmthsmallest(int m)
{
root->findmsm(m);
}

//Function to generate random number
int random_number(int min_num, int max_num)
        {
            int result=0,low_num=0,hi_num=0;
            if(min_num<max_num)
            {
                low_num=min_num;
                hi_num=max_num+1; // this is done to include max_num in output.
            }else{
                low_num=max_num+1;// this is done to include max_num in output.
                hi_num=min_num;
            }
            srand(time(NULL));
            result = (rand()%(hi_num-low_num))+low_num;
            return result;
        }

//A function to insert all elements of 1 treap into another
void inorderjoin( treap &t,treapnode* roo ){
    if( roo == NULL ) return;
    inorderjoin( t,roo->left );
    t.insert(roo->n,roo->p);
    inorderjoin( t,roo->right );
}

//Three way join
treap threewayjoin(treap t1,int k,treap t2)
{
int prior=random_number(1,32000);
t1.insert(k,prior);
inorderjoin(t1,t2.root);
return t1;
}

void splittree(treap t1,int k,treap &t2,treap &t3)
{
t1.insert(k,-32000);
t2.root=t1.root->left;
t2.root->parent->left=NULL;
t2.root->parent=NULL;
t3.root=t1.root->right;
t3.root->parent->right=NULL;
t3.root->parent=NULL;
}




int main() {

int tno,noofchoice,noofele;
cin>>noofchoice;
treap tr[10001];
char str[10];
int i,ele,m;

while(noofchoice--)
{
cin>>str;
if(strcmp(str,"insert")==0)
{

cin>>tno>>noofele;
for(i=0;i<noofele;i++)
{
cin>>ele;
int prior=random_number(1,32000);
tr[tno].insert(ele,prior);
}

}

if(strcmp(str,"delete")==0)
{
cin>>tno>>noofele;
for(i=0;i<noofele;i++)
{
cin>>ele;
tr[tno].remove(ele);
}
}

if(strcmp(str,"select")==0)
{
cin>>tno>>m;
tr[tno].findmthsmallest(m);
}


if(strcmp(str,"join")==0)
{
int tr1,tr2,tr3,k;
cin>>tr1>>k>>tr2>>tr3;
tr[tr3]=threewayjoin(tr[tr1],k,tr[tr2]);

}

if(strcmp(str,"split")==0)
{
int tr1,tr2,tr3,k;
cin>>tr1>>k>>tr2>>tr3;
splittree(tr[tr1],k,tr[tr2],tr[tr3]);

}

}
return 0;
}
