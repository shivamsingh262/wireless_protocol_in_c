# include <iostream.h>
# include <string.h>
# include <math.h>
using namespace std;
int nodes;
int threshold;
struct packettype
{
    string value;
    string node_list;
};
struct nodetype
{
    char id;
    string cache[100];
    int x;
    int y;
    int z;
    int cache_size;
    int clock;
    char key;
};
struct nodetype node[100];
struct packet_rrep
{
    char initiator;
    char target;
    string key_list;
    int time_interval;
    string target_MAC;
    string node_list;
    string MAC_list[100];
    string MAC1;
    string MAC2;
};
struct packet_rreq
{
    char initiator;
    char target;
    int id;
    int time_interval;
    string hash_chain;
    string node_list;
    string MAC_list[100];
};
struct packet
{
    int header[25];
    string data;
};
void sort_cache(char);
int find_route(nodetype,nodetype);
string reverse(string);
void intialize(int,char,int,int,int,char,int);
long int to_decimal(string);
int check_range(nodetype,nodetype);
int check_route_cache(nodetype,nodetype);
void route_discovery(nodetype,nodetype);
string MAC(char,char,int,int,string,string,string [],char,char);
string hash(char,string);
int random();
void broadcast(nodetype,nodetype,packet_rreq);
void reply(nodetype,nodetype,packet_rrep);
void send_packet(packettype);
int main()
{
    int a,b;
    nodes = 5;
    threshold = 60;
    //intialize node(id,x,y,z,key,clock)
    intialize(1,'A',5,5,5,'A',5);
    intialize(2,'B',6,6,6,'B',5);
    intialize(3,'C',7,7,7,'C',5);
    intialize(4,'D',8,8,8,'D',5);
    intialize(5,'E',9,9,9,'E',5);
    int test=0;
    cout<<"\n Input source and destination route nos ";
    cin>>a>>b;
    //cout<<"\n Output saved to output.txt";
    //freopen( "output.txt", "w", stdout );
    test = find_route(node[a],node[b]);
    cout<<"\n Route cache of "<<node[a].id<<":";
    cout<<"\n Cache size: "<<node[a].cache_size;
    for(int i=1;i<=node[a].cache_size;i++)
    {
        cout<<"\n "<<node[a].cache[i]<<" Length:  "<<node[a].cache[i].length();
    }
    cout<<"\n Route id is "<<test;
    cout<<"\n Route to be used: "<<reverse(node[a].cache[test]);
    cout<<"\n Sending Packet";
    packettype pp;
    pp.value = "hello";
    pp.node_list = node[a].id + reverse(node[a].cache[test]);
    send_packet(pp);
    //freopen("CON", "w", stdout);
    return 1;
}
int find_route(nodetype source,nodetype destination)
{
    cout<<"\n Finding route between "<<source.id<<" and "<<destination.id;
    int route_id = check_route_cache(source,destination); // to find the required index of source.cache
    if (route_id == -1)
    {
        cout<<"\n No route in cache of "<<source.id;
        cout<<"\n Route Discovery:";
        route_discovery(source,destination);   // no route exists, intiate route discovery
        sort_cache(source.id);
        route_id = check_route_cache(source,destination);
    }
    return route_id;
}
int check_route_cache(nodetype source,nodetype destination)
{
    int i=0,len=0;
     int ns;
    for(ns=1;ns<=nodes;ns++)
    {
        if(node[ns].id==source.id)
        break;
    }
    int n=node[ns].cache_size;
    while(n>=0)
    {
        if(node[ns].cache[i][0]==destination.id)               // first index to match will correspond to shortest path as cache array is sorted
            return i;
        i++;
        n--;
    }
    return -1;                                           // loop exits, no route found
}
void sort_cache(char k)
{
    int n;
    for(n=1;n<=nodes;n++)
    {
        if(node[n].id==k)
        break;
    }
    string tmp;
    int i,j;
    for(i=1;i<node[n].cache_size;i++)
    {
        for(j=1;j<node[n].cache_size;j++)
        {
            if((node[n].cache[j]).length()>(node[n].cache[j+1]).length())
            {
                tmp=node[n].cache[j];
                node[n].cache[j]=node[n].cache[j+1];
                node[n].cache[j+1]=tmp;
            }
        }
    }
}
int random()
{
    return 125;
}
void route_discovery(nodetype source,nodetype destination)
{
    packet_rreq P;
    string n="";
    string n1[2]="";
    P.initiator = source.id;
    P.target = destination.id;
    P.id = random();
    P.time_interval = source.clock;
    P.hash_chain = MAC(P.initiator,P.target,P.id,P.time_interval,n,n,n1,source.key,destination.key);
    P.node_list = "";
    P.MAC_list[0]="";
    for(int i = 1;i<=nodes;i++)
    {
        if(check_range(node[i],source)==1&&(node[i].id!=source.id))
        broadcast(source,node[i], P);
    }
}
int check_range(nodetype A,nodetype B)
{
    int d1,d2,s;
    d1=(A.x*A.x)+(A.y*A.y)+(A.z*A.z);
    d2=(B.x*B.x)+(B.y*B.y)+(B.z*B.z);
    if(d1>d2)
    s=d1-d2;
    else
    s=d2-d1;
    if(s<=threshold)
    return 1;
    else
    return 0;
}
string MAC(char s,char d,int id,int t,string h,string n,string m[],char k1,char k2)
{
    char result[10];
    int k,dd;
    long int temp;
    int s1,d1,m1=0,n1=0,h1=0;
    if(h!="")
    h1=to_decimal(h);
    if(m[0]!="")
    {
        int i=0;
        while(m[i]!="")
        {
            m1=m1+to_decimal(m[i]);
            i++;
        }
    }
    if(n!="")
    {
        int i=0;
        while(i<n.length())
        {
            n1=n1+n[i] - 55;
            i++;
        }
    }
    s1 = s - 55;
    d1 = d- 55;
    temp= s1+d1+id+t+m1+n1+h1;
    temp= temp%1000;
    result[0] = k1;
    for(k=3;k>=1;k--)
    {
        dd = temp%10;
        result[k]=((char)dd)+48;
        temp = temp/10;
    }
    result[4] = k2;
    result[5] = '\0';
    return string(result);
}
long int to_decimal(string st)
{
    int k=st.length()-1;
    int d,t=0;
    long int s=0;
    while(k>=0)
    {
        if(((st[k]-48)<=9)&&((st[k]-48)>=0))
        d=st[k]-48;
        else
        d=st[k]-55;
        s=s+d*(pow(16,t));
        k--;
        t++;
    }
    return s;
}
void broadcast(nodetype ns,nodetype nd,packet_rreq P)
{
    cout<<"\n Packet broadcast from "<<ns.id<<" received at "<<nd.id;
    cout<<"\n Packet Initiator: "<<P.initiator<<" Target: "<<P.target<<" Id: "<<P.id<<" Time interval: "<<P.time_interval<<" Hash chain "<<P.hash_chain<<" Node list "<<P.node_list<<" MAC list ";
    int n=0;
    while(P.MAC_list[n][0]!='\0')
    {
        cout<<P.MAC_list[n]<<" ";
        n++;
    }
    if(nd.id!= P.target)           // node is not the destination required
    {
        P.node_list = P.node_list + nd.id;
        P.hash_chain = hash(nd.id,P.hash_chain);
        char t = P.time_interval + 48;
        string mac = MAC(P.initiator,P.target,P.id,P.time_interval,P.hash_chain,P.node_list,P.MAC_list,nd.key,t);
        int k=0;
        while(1)
        {
            if(P.MAC_list[k][0]=='\0')
            {
                P.MAC_list[k]=mac;
                break;
            }
            k++;
        }
        for(int i = 1;i<=nodes;i++)
       {
           int k=0,flag=0;
           while(k<P.node_list.length()-1)
            {
                if(P.node_list[k]==node[i].id)
                {
                    flag=1;
                    break;
                }
                k++;
             }
            if((check_range(node[i],nd)==1)&&(node[i].id!=ns.id)&&(flag!=1)&&node[i].id!=nd.id)
            broadcast(nd,node[i], P);
        }
    }
    else
    {
        cout<<"\n"<<nd.id<<" is destination";
        string mac = MAC(P.initiator,P.target,0,P.time_interval,"",P.node_list,P.MAC_list,P.target,P.initiator);
        packet_rrep Q;
        Q.initiator = P.target;
        Q.target = P.initiator;
        Q.time_interval = P.time_interval;
        Q.node_list = P.node_list;
        int k=0;
        cout<<"\n \n Route Reply: ";
        while(1)
        {
             if(P.MAC_list[k][0]=='\0')
            break;
                Q.MAC_list[k]=P.MAC_list[k];
            k++;
        }
        Q.target_MAC = mac;
        Q.key_list = "";
        for(int i = 1;i<=nodes;i++)
    {
        if(node[i].id==Q.node_list[(Q.node_list.length()-1)])
        {
            reply(nd,node[i],Q);
            break;
        }
    }
    }
}
void reply(nodetype ns,nodetype nd,packet_rrep Q)
{
    cout<<"\n Reply from "<<ns.id<<" to "<<nd.id;
    cout<<"\n Packet Initiator: "<<Q.initiator<<" Target: "<<Q.target<<" Time interval: "<<Q.time_interval<<" Node list: "<<Q.node_list<<" MAC list ";
    int n=0;
    while(Q.MAC_list[n][0]!='\0')
    {
        cout<<Q.MAC_list[n]<<" ";
        n++;
    }
    cout<<"Target MAC "<<Q.target_MAC;
    char k1,k2='A';
    if((ns.id==Q.initiator)||(ns.id==Q.node_list[Q.node_list.length()-1]))
    k1=Q.initiator;
    else
    {
        for(int i=0;i<Q.node_list.length();i++)
        {
            if(Q.node_list[i]==ns.id)
            {
                k1=Q.node_list[i+1];
                break;
            }
        }
    }
    k2=nd.id;
    string n1[]="";
    Q.MAC1 = MAC(Q.initiator,Q.target,0,Q.time_interval,Q.target_MAC,Q.node_list,n1,k1,k2);
    cout <<"\n MAC1 "<<Q.MAC1;
    k1=ns.id;
    if((nd.id==Q.target)||(nd.id==Q.node_list[0]))
    k2=Q.target;
    else
    {
    for(int i=0;i<Q.node_list.length();i++)
    {
        if(Q.node_list[i]==nd.id)
        {
            k2=Q.node_list[i-1];
        }
    }
    }
    Q.MAC2 = MAC(Q.initiator,Q.target,0,Q.time_interval,Q.target_MAC,Q.node_list,n1,k1,k2);
    cout <<" MAC2 "<<Q.MAC2;
    if(nd.id!=Q.target)
    {
         int i,k;
    for(i=Q.node_list.length()-1;i>=0;i--)
        {
            if(Q.node_list[i]==nd.id)
            break;
        }
        for(k = 1;k<=nodes;k++)
    {
        if((node[k].id==Q.target)&&(i==0))
        {
            reply(nd,node[k],Q);
            break;
        }
        if(node[k].id==Q.node_list[i-1])
        {
           reply(nd,node[k],Q);
            break;
        }
    }

    }
    else
    {
        cout<<"\n Received at source \n";
        int i;
        for(i =1;i<=nodes;i++)
        {
            if(node[i].id==nd.id)
            break;
        }
        node[i].cache[node[i].cache_size+1]=Q.initiator + reverse(Q.node_list);
        node[i].cache_size++;
    }
}
string reverse(string st)
{
    char m[15];
    int k=0;
    int i = st.length()-1;
    while(i>=0)
    {
        m[k]=st[i];
        k++;
        i--;
    }
    m[k]='\0';
    return (string)m;
}
string hash(char c,string d)
{
    int i=0;
    while(i<d.length())
    {
        d[i]=d[i+1];
        i++;
    }
    d[i-1]=c;
    d[i]='\0';
    return d;
}
void intialize(int i,char id,int x,int y,int z,char key,int clock)
{
    node[i].id=id;
    node[i].x=x;
    node[i].y=y;
    node[i].z=z;
    node[i].key=key;
    node[i].clock=clock;
    node[i].cache_size=0;
}
void send_packet(packettype pp)
{
    cout<<"\n\n Packet sent along "<<pp.node_list;
    cout<<"\n Value "<<pp.value;
}
