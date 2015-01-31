#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<sys/types.h>
#define SERV_PORT 5576

#define START_FRAME 1
#define END_FRAME 2

class mysocket{

const int CLIENT = 1, SERVER =2;
int i,j,mMachineType,mFrameType;
char s[80],f[80];
int temp=0;
FILE *fp;
ssize_t n;
int listenfd,connfd,sockfd;
struct sockaddr_in servaddr,cliaddr;
socklen_t clilen;

/*
  Initializes the socket establishes connection
  @Params Machine type
  @return void
*/

    public: void initialize(int machineType){

        setMachineType(machineType);

        if(machineType == CLIENT){
            sockfd=socket(AF_INET,SOCK_STREAM,0);
            bzero(&servaddr,sizeof(servaddr));

            servaddr.sin_family=AF_INET;
            servaddr.sin_port=ntohs(SERV_PORT);

            connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
        }else{
            listenfd=socket(AF_INET,SOCK_STREAM,0);

            bzero(&servaddr,sizeof(servaddr));

            servaddr.sin_family=AF_INET;
            servaddr.sin_port=htons(SERV_PORT);

            bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr));

            listen(listenfd,5);
            clilen=sizeof(cliaddr);

//using sockfd here to prevent if else in sending and receiving
            sockfd=accept(listenfd,(struct sockaddr*)&cliaddr,&clilen);
            cout<<"Sockets Initialized ---- \n";

        }
    }

/*
 The communicate cycle will end after every
 receive . So if we wish to send we will have
 call communicate again

// every time the upper layer finishes its job it asks mac
// to communicate i.e this function is called when communication
// is needed ...
*/

    public : void setMachineType(int type){
        mMachineType = type;
    }

    public : void readFromSocket(){
        FILE *fp ("mac_input.txt");
        while((sockfd,recvline,80)!=0){
            fputs(recvline,fp);
        }
    }

    public : void writeToSocket(){
        FILE *fp ("mac_input.txt");
        while(fgets(s,80,fp)!=NULL){
            write(connfd,s,sizeof(s));
        }
    }

};





