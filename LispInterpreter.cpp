#include<iostream>
#include<cstdlib>
#include<cstring>
#include<stack>
#include<string>
#include<sstream>
#include<cmath>

using namespace std;

// CLASS DEFINITIONS

class GenListNode
{
public:
int tag;
string s;
GenListNode *down, *next;

GenListNode()
{
    tag=0;
    down=NULL;
    next=NULL;
}

};


class Value
{
public:
int tag;
float val;
GenListNode* link;

Value()
{
    tag=0;
    link=NULL;
}

};

class Env
{
public:
Value* table[26];
Env* parent;

Env()
{
for(int i=0; i<26; i++)
table[i]=NULL;
parent=NULL;
}

};

//FUNCTION TO ADD/CHANGE VALUE OF A VARIABLE IN ENVIRONMENT

void add(char x, Value val_new, Env &e)
{
Value *p = new Value;
p->tag=val_new.tag;
p->link=val_new.link;
p->val=val_new.val;
e.table[x-'a']=p;
}

//FUNCTION TO FIND VALUE STORED IN A VARIABLE IN ENVIRONMENT 

Value find(char x, Env& e)
{
if (e.table[x-'a'] != NULL)
{
return *(e.table[x-'a']);
}
else if (e.parent == NULL)
{
exit(0);
}
else
{
return find(x, *(e.parent));
}
}

//EVALLIST DECLARED SO THAT WE CAN CALL IT FROM EVALNODE
Value evalList(GenListNode *p, Env& e);

//EVALNODE FUNCTION TO EVALUATE THE VALUE OF A LIST

Value evalNode(GenListNode *p, Env& e)
{

 if((p->tag)==1)
 return evalList(p->down,e);
 else
 {
     char ch,c1[20];
     float n;
 ch=p->s[0];
if((ch>='a')&&(ch<='z'))
     return find(ch,e);
    else
    {
stringstream c(p->s);
	 c>>n;
        Value v;
        v.tag=0;
        v.val=n;
        v.link=NULL;
        return v;
    }
 }
}

//EVALLIST FUNCTION TO EVALUATE THE RETURN VALUE OF A LIST

Value evalList(GenListNode *p, Env& e)
{

 if(p->tag==0)
 {

 if(p->s=="define")
 {
     char ch;
     ch=p->next->s[0];
    Value v=evalNode(p->next->next,e);
    add(ch,v,e);
    return v;
 }

 else if(p->s=="set")
 {
     char ch;
     ch=p->next->s[0];
    Value v=evalNode(p->next->next,e);
    add(ch,v,e);
    return v;
 }

else if(p->s=="begin")
{
    p=p->next;
    while(p->next!=NULL)
    {
        evalNode(p,e);
        p=p->next;
    }
    return evalNode(p,e);
}

else if(p->s=="if")
{
    p=p->next;
    Value v=evalNode(p,e);
    if(v.val!=0)
    return evalNode(p->next,e);
    else
    return evalNode(p->next->next,e);
}

else if(p->s=="while")
{
    p=p->next;
    Value v;
    while(evalNode(p,e).val!=0)
      v=evalNode(p->next,e);
    return v;
}

else if(p->s=="+")
{
   Value v1,v2,v3;
    v1=evalNode(p->next,e);
    v2=evalNode(p->next->next,e);
    v3.tag=0;
    v3.val=v1.val+v2.val;
    return v3;
}

else if(p->s=="-")
{
    Value v1,v2,v3;
    v1=evalNode(p->next,e);
    v2=evalNode(p->next->next,e);
    v3.tag=0;
    v3.val=v1.val-v2.val;
    return v3;
}

else if(p->s=="*")
{
    Value v1,v2,v3;
    v1=evalNode(p->next,e);
    v2=evalNode(p->next->next,e);
    v3.tag=0;
    v3.val=v1.val*v2.val;
    return v3;
}

else if(p->s=="/")
{
    Value v1,v2,v3;
    v1=evalNode(p->next,e);
    v2=evalNode(p->next->next,e);
    v3.tag=0;
    v3.val=v1.val/v2.val;
    return v3;
}

else if(p->s=="%")
{
    Value v1,v2,v3;
    v1=evalNode(p->next,e);
    v2=evalNode(p->next->next,e);
    v3.tag=0;
    v3.val=fmod(v1.val,v2.val);
    return v3;
}

else if(p->s==">")
{
    Value v1,v2,v3;
    v1=evalNode(p->next,e);
    v2=evalNode(p->next->next,e);
    v3.tag=0;
    if(v1.val>v2.val)
    v3.val=1;
    else
    v3.val=0;
    return v3;
}
 
else if(p->s=="<")
{
    Value v1,v2,v3;
    v1=evalNode(p->next,e);
    v2=evalNode(p->next->next,e);
    v3.tag=0;
    if(v1.val<v2.val)
    v3.val=1;
    else
    v3.val=0;
    return v3;
}
 
 
else if(p->s==">=")
{
    Value v1,v2,v3;
    v1=evalNode(p->next,e);
    v2=evalNode(p->next->next,e);
    v3.tag=0;
    if(v1.val>=v2.val)
    v3.val=1;
    else
    v3.val=0;
    return v3;
}
 
else if(p->s=="<=")
{
    Value v1,v2,v3;
    v1=evalNode(p->next,e);
    v2=evalNode(p->next->next,e);
    v3.tag=0;
    if(v1.val<=v2.val)
    v3.val=1;
    else
    v3.val=0;
    return v3;
}
 
 
else if(p->s=="==")
{
    Value v1,v2,v3;
    v1=evalNode(p->next,e);
    v2=evalNode(p->next->next,e);
    v3.tag=0;
    if(v1.val==v2.val)
	v3.val=1;
	else
	v3.val=0;
    return v3;
}

 else if(p->s=="lambda")
 {
     Value new_val;
    new_val.tag=1;
    new_val.link=p;
    return new_val;
 }

 else if((p->s[0]>='a')&&(p->s[0]<='z'))
    {
        Value v=evalNode(p,e);
if(v.tag==0) return v;
       Env e1;
        e1.parent=&e;
	char ch=p->s[0];	
 
        GenListNode *arglist;
        GenListNode *body;
        GenListNode *counter;
      
        arglist=v.link->next->down;
        body=v.link->next->next;
        counter=p->next;
              
Value vvvv;
        while(arglist!=NULL)
        {
vvvv=evalNode(counter,e);
        add(arglist->s[0],vvvv,e1);
        arglist=arglist->next;
        counter=counter->next;
        }
        
Value vvv;
        vvv= evalNode(body,e1);
return vvv;

 }
return evalNode(p,e);

 }

else if(p->tag==1)
{

GenListNode *q=new GenListNode;
q=p->down;
Env e1;
        e1.parent=&e;
        
        
 
        GenListNode *arglist=new GenListNode;
        GenListNode *body=new GenListNode;
        GenListNode *counter=new GenListNode;
        
        arglist=q->next->down;
        body=q->next->next;
       counter=p->next;
                
 
        while(arglist!=NULL)
        {
        add(arglist->s[0],evalNode(counter,e),e1);
        arglist=arglist->next;
        counter=counter->next;
        }
        
        return evalNode(body,e1);
 
}

 
}

 

 

stack<GenListNode*> Stack;

GenListNode *makeList()
{
 
    int flag;
    string t;
GenListNode* head=new GenListNode;
 
    for(flag=0;t!=")";)
   {
        cin>>t;
 
    if((t=="("))
        {
        GenListNode *g=new GenListNode();
        g->tag=1;
        head->next=g;
        head=g;
        if(flag==0)
       {
            Stack.push(head);
            flag=1;
        }
        head->down=makeList();
        }
    else if((t==")"))
    {
        head->next=NULL;
        head=Stack.top();
        Stack.pop();
    }
    else
    {
        GenListNode *g=new GenListNode;
        g->tag=0;
        g->s=t;
        head->next=g;
        head=g;
        if(flag==0)
        {
            Stack.push(head);
            flag=1;
        }
    }
 
}
 return head;

 }

int main()
{
    GenListNode *a=new GenListNode;
    string s;
    cin>>s;
 
    
   a=makeList();
Env e;
Value v = evalList(a,e);
cout<<v.val<<"\n";
return 0;
}
