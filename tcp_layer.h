

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
using namespace std;

#define SRC_PORT_ROW 0
#define DES_PORT_ROW 0
#define SEQ_ROW 1
#define ACK_ROW 2
#define WIN_SIZE_ROW 3
#define HEAD_RES_ROW 3
#define CHKSUM_ROW 4
#define URGENT_ROW 4
#define OPTIONS_ROW 5
#define PADDINIG_ROW 5
#define COMPLETE 1
#define SYN_FRAME 3
#define ACK_FRAME 4
#define FIN_FRAME 5
#define SYN_ACK_FRAME 6
#define DATA_FRAME 7
#define SERVER 1
#define CLIENT 2

class tcp_Layer{

 const int SENDING= 1,RECEVING =2,TOTAL_DATA_PACKS = 1;
 const char source[4]= {'0','0','1','6'};
 const char dest[4]={'0','0','1','6'};
 const char header_length = '5';
 const char window_size[4] = {'0','8','0','0'};
 int mSeqNumber=0,mAckNumber=10;

 /////-------/////
 int mCharInFile=0;


 private :
 int mMachineType=-1,mSentDataPacks=0;
 bool mHandShakeSucess = false,mTransaction = false,mStart=true,finish=false;
 char tcp_header[5+500][8];


 public : void initialize(){

    int i,a;
    string strSeqNumber,strAckNumber;
    stringstream seqStream,ackStream;
    seqStream<<hex<<(int)mSeqNumber;
    ackStream<<hex<<(int)mAckNumber;
    strSeqNumber = seqStream.str();
    strAckNumber = ackStream.str();
    cout<<"*********************"<<mAckNumber<<"\n";
    while(strSeqNumber.length()<8){
        strSeqNumber.insert(0,1,'0');
    }
    while(strAckNumber.length()<8){
        strAckNumber.insert(0,1,'0');
    }

        for(i=0;i<4;i++){
            tcp_header[SRC_PORT_ROW][i]= source[i];    //src port
            tcp_header[CHKSUM_ROW][i]='0';             //checksum
            tcp_header[SEQ_ROW][i]=strSeqNumber[i];
            tcp_header[ACK_ROW][i]=strAckNumber[i];
        }
        for(a=0;i<8;i++,a++){
            tcp_header[DES_PORT_ROW][i]= dest[a];     //dest port
            tcp_header[HEAD_RES_ROW][i]= window_size[a]; //window size
            tcp_header[URGENT_ROW][i]='0';
            tcp_header[SEQ_ROW][i]=strSeqNumber[i];
            tcp_header[ACK_ROW][i]=strAckNumber[i];
         }
        tcp_header[HEAD_RES_ROW][0]= header_length;   //header length
        tcp_header[HEAD_RES_ROW][1] = '0';            //reserved


 }

/*
    Below two are the file io functions
    only during start or finish we read or write to above layer
    from tcp_input.txt rest all time we read from ip_input and
    write to it only
    @Param : file name str
    @return: 0 success

*/


 int setDataToFile(){

    string str;
    int i,j;

    if(finish == true){
       initialize();
       cout<<"in  "<< mMachineType<<"\n";
       for(i=5;i<5+mCharInFile/8;i++){
            for(j=0;j<8;j++){
                str.push_back(tcp_header[i][j]);
                cout<<tcp_header[i][j]<<" ";
            }cout<<"\n";
        }
        ofstream file ("input_tcp.txt");
        if(file.is_open()){
            file << str;
        }
        file.close();
    }
    else{
        initialize();
        cout<<"in  "<< mMachineType<<"\n";

        for(i=0;i<5+mCharInFile/8;i++){
            for(j=0;j<8;j++){
                str.push_back(tcp_header[i][j]);
                cout<<tcp_header[i][j]<<" ";
            }cout<<"\n";
        }
//        mCharInSendFile=0;
       ofstream file ("input_ip.txt");
        if(file.is_open()){
            file << str;
        }
        file.close();
    }
 }

 int getDataFromFile(){
    FILE *file;
    if(mStart && mMachineType==CLIENT){

        file = fopen("input_tcp.txt", "r");
        int i=5,b=0;
        char c;
        while((c=getc(file))!=EOF){
            tcp_header[i][b]=c;
            //cout<<" "<<send_tcp_header[i][b];
            if(b==7){
                b=0;i++;
            }else{
               b++;
            }

            mCharInFile++;
        }
        mStart=false;
    }else {

      //  initialize();
        file = fopen("input_ip.txt", "r");
        mCharInFile=0;
            int i=0,b=0;
            char c;
            while((c=getc(file))!=EOF){

                tcp_header[i][b]=c;
                if(b==7){
                    b=0;i++;
                }else{
                    b++;
                }

                if(i>=5)
                    mCharInFile++;
        }
    }
        fclose(file);

   // cout<<mCharInSendFile;
 }

 /*
    This takes message from below layer i.e network layer
    and it 'standalone manages handshake and finish'.
    no ERROR detection and control
    @Param : frame array
    @return : 0 == sucess
 */

 public : int recieveFrame(){


    getDataFromFile();

    switch(recognizeFrame()){

        case SYN_FRAME:

                        if(mMachineType==SERVER){
                            sendFrame(SYN_ACK_FRAME);
                        }

                        break;

        case ACK_FRAME:
                        if(mMachineType==SERVER){
                            if(!mHandShakeSucess){
                                mHandShakeSucess=true;
                            }

                            if(mTransaction!=COMPLETE)
                                sendFrame(ACK_FRAME);
                            else{
                                finish=true;
                                sendFrame(ACK_FRAME);///Frame type here is not imp
                            }
                        }
                        else if(mMachineType==CLIENT){
                            if(!mHandShakeSucess){
                                mHandShakeSucess=true;
                            }
                            if(mSentDataPacks!=TOTAL_DATA_PACKS){
                                sendFrame(DATA_FRAME);
                                mSentDataPacks++;
                            }else{
                                sendFrame(FIN_FRAME);
                            }
                        }
                        break;

        case FIN_FRAME:
                        if(mMachineType==SERVER){
                            sendFrame(ACK_FRAME);
                            mTransaction=COMPLETE;
                       //     wait(1);
                            sendFrame(FIN_FRAME);

                        }else if(mMachineType==CLIENT){
                            mTransaction=COMPLETE;
                            sendFrame(ACK_FRAME);
                            finish=true;
                            sendFrame(DATA_FRAME); /// FRAME TYPE DOESNT MATTER;
                            // SEND TO APPLICATION LAYER
                        }
                        break;

        case SYN_ACK_FRAME:

                        if(mMachineType==CLIENT){
                            sendFrame(ACK_FRAME);
                        }
                        break;

        case DATA_FRAME:
                        if(mMachineType==SERVER){
                            sendFrame(ACK_FRAME);
                        }
                        break;


    }
 }

 /*
    This gives message to below layer i.e network layer
    This is slave of recieveFrame. i.e it just does what
    recive frame asks it to and nothing more like it doesnt
    care the machine type or the error or any thing it just
    sends the message
    @return : 0 == sucess
 */

 private : int sendFrame(int mFrameType){

    switch(mFrameType){


        case SYN_FRAME:
                        setFrameType(SYN_FRAME);
                        // FILE IO
                        break;

        case ACK_FRAME:
                        setFrameType(ACK_FRAME);
                        // FILE IO
                        break;

        case FIN_FRAME:
                        setFrameType(FIN_FRAME);
                        // FILE IO
                        break;

        case SYN_ACK_FRAME:
                        setFrameType(SYN_ACK_FRAME);
                        // FILE IO
                        break;

        case DATA_FRAME:
                        setFrameType(DATA_FRAME);
//                        for(i=0;i<data;i++){
//                            send_tcp_header[]=[];
//                        }
                        // FILE IO
                        break;
    }

    setDataToFile();
 }

 /*
    Set the type of frame for sending eg: syn frame or ack frame
    @Param : type hex value in char
    @return : 0 == sucess
 */

 private : int setFrameType(char mFrameType){

    switch(mFrameType){

        case SYN_FRAME:

                       tcp_header[HEAD_RES_ROW][2] ='0';
                        tcp_header[HEAD_RES_ROW][3] ='2';

                        break;
        case ACK_FRAME:

                        tcp_header[HEAD_RES_ROW][2] ='1';
                        tcp_header[HEAD_RES_ROW][3] ='0';

                        break;
        case FIN_FRAME:
                       tcp_header[HEAD_RES_ROW][2] ='0';
                        tcp_header[HEAD_RES_ROW][3] ='1';

                        break;
        case SYN_ACK_FRAME:

                        tcp_header[HEAD_RES_ROW][2] ='1';
                        tcp_header[HEAD_RES_ROW][3] ='2';

                        cout<<"Entered synack";
                        break;
        case DATA_FRAME:

                        tcp_header[HEAD_RES_ROW][2] ='0';
                        tcp_header[HEAD_RES_ROW][3] ='0';

                        break;
    }
 }

/*
  Used identify the frame
*/

 private : int recognizeFrame(){

    int type1 = (tcp_header[HEAD_RES_ROW][2] - '0')*10+
                (tcp_header[HEAD_RES_ROW][3] - '0');



    switch(type1){
        case 2:   setAckNumber();
                return SYN_FRAME;
            break;
        case 10:mAckNumber++;
                return ACK_FRAME;
            break;
        case 1: return FIN_FRAME;
            break;
        case 12: return SYN_ACK_FRAME;
            break;
        case 0: return DATA_FRAME;
            break;

    }
 }

 private : void setAckNumber(){
    string strReceived;
    int i;
    for(i=0;i<8;i++)
      strReceived.push_back(tcp_header[ACK_ROW][i]);
    stringstream ss (strReceived);
    ss>>hex>>mAckNumber;
    mAckNumber++;
    cout<<"**********setacknumber****"<<strReceived<<"\n";
 }

 public : void setMachineType(int type){
    mMachineType = type;
 }

 public : bool isFinished(){
    return mTransaction;
  }

 public: int start(){
    if(mMachineType== CLIENT){
        getDataFromFile();
        sendFrame(SYN_FRAME);
    }else if(mMachineType==SERVER){
    }
 }

};

//int main(){
//
//    tcp_Layer mytcp;
//    tcp_Layer myServer;
////
//    mytcp.setMachineType(CLIENT);
//    myServer.setMachineType(SERVER);
//
//    mytcp.start();
//
//    myServer.start();
//
//    while(!mytcp.isFinished()){
//    myServer.recieveFrame();  //receives frame
//    mytcp.recieveFrame(); // sends frame
//    }
//
//    myServer.recieveFrame();
//
//    return 0;
//
//}
