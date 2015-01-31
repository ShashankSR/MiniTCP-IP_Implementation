
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
//#include "mysockets.h"
using namespace std;

#define CRC_LENGTH 32
#define CHAR_COUNT 500


 class mac_layer{

    int mCrc[32];
    int i,charcount=0;
    int mCrcError[32];
    int mBinaryData[CHAR_COUNT*8];
    int mFrame[22*8+CHAR_COUNT*8+CRC_LENGTH];
    string myhex;
//
    public:BitCRC(){
        for(i=0;i<32;i++){
            mCrc[i]=0;
            mCrcError[i]=0;
        }
    }

//---------------------------- conversion of data to binary ----------------------//

   private:void binary(int x){         // we convert each char into bin and add the bin
                                // into the main BinaryDataArray . so last statement
        int binary[8],i;        // is a call to data(binary) which does the job

        for(i=7;i>=0;i--){
            binary[i]=x%2;
            x=x/2;
     //       cout<<binary[i]<<" ";

        }//cout<<"\n";

        data(binary);
    }

   private: void toHex(){

          int myint,i;
          for(i=0;i< 26*8 + charcount*8;){
               myint = mFrame[i++]*2*2*2;
               myint+= mFrame[i++]*2*2;
               myint+= mFrame[i++]*2;
               myint+= mFrame[i++];
               stringstream ss;
               ss <<hex<< (int)myint;
               myhex.append(ss.str());
             //  cout<<"myint    "<<myint;
              // cout<<ss.str()<<"***\n";

          }

    }

    private:void data(int binary[]){
        static int index=0;
        static int data[CHAR_COUNT*8],i;

        for(i=index;i<index+8;i++){
             mBinaryData[i]=binary[i-index];
        //     printf("%d%d     " ,binary[i-index],data[i]);
        }

        index+=8;
//        printf("    value of index %d   ",index);
  //      printf("\n");
       // cout<<index<<" ";
//        if(index == (CHAR_COUNT)*8){
//            cout<<"Calculating crc\n";
//      //      isDataBinary = true;
//            // Calculating crc when we convert whole data into binary
//            calculateCrc(mBinaryData,CHAR_COUNT);
        //}
    //    printf("\n");

    }
//---------------------------- conversion of data to binary ----------------------//


//-----------------------------making crc ---------------------------------------//

    //crc is the remainder of the divsion of the data with some crc function ..

    private: void calculateCrc(int mInput[],int mSize){

        //store last value == outvalue in temp
        // visual setup of array crc below
        // + [0] + [1] + [2] [3] [4] ...
        initCrc();
        int  mCrcXorIndices[] = {26,23,20,16,12,11,10,8,7,3,2,0};
        int  mOutValue =0 ,q=0,j;

        for(i=0;i<mSize;i++){                           // for each bit
            for(j=CRC_LENGTH-1;j>0;j--){                   // cal crc for each bit NOTE : J is greater than 0 as 0 is taken care out the loop
             //    cout<<"in crc  "<<i<< "\n";
                if(j==mCrcXorIndices[q]){                   //NOTE: we are going in backward direction i.e from last cell of hardware crc
                    mCrc[j] = (mCrc[j-1]==mOutValue) ? 0:1; // Xor and shift to next
                    q++;
                }
                else{
                    mCrc[j]=mCrc[j-1];                      // shift
                }
            }

            mCrc[0] = (mInput[i] == mOutValue) ? 0:1;
            mOutValue = mCrc[31];


        }
             /*
                31 shift to temp; 30 shift ; 29 shift; 28 shift ; 27 shift
                26 shift ; 25 xorshift ;24 shift ; 23 shift ; 22 xorshift
                21 xorshift; 20 shift; 19 shift; 18 shift; 17 shift
                16 shift;15 xorshift;14 shift; 13 shift ;12 shift
                11 xorshift; 10 xorshift; 9 xorshift; 8 shift
                7 xorshift; 6 xorshift; 5 shift; 4 xorshift
                3 xorshift; 2 shift; 1 xorshift; 0 xorshift
                input xorshift
            */
//        cout<<"finalcrc\n";
//        for(i=0;i<32;i++)
//            cout<<mCrc[i]<<" ";
//        cout<<"\n";
        {
        makeFrame();
//        int i;
//        for(i=0;i<26*8+CHAR_COUNT*8;i++)
//            cout<<mFrame[i]<<" ";
        }
    }

//---------------------------- making crc ----------------------//


//------------------------------making frame --------------------//

    private : void makeFrame(){
        // here we add all
        int i ;

        for(i=0;i<7*8 + 6;i++){
            mFrame[i] = (i)%2 == 0 ? 1 : 0 ;
            cout<<mFrame[i]<<" ";
        }
        mFrame[i++] = 1;
        cout<<mFrame[i-1]<<" ";
        mFrame[i++]=1;
        cout<<mFrame[i-1]<<" ";
        cout<<"\n";
        // Finished preamble .... doing destination and source addresss
        for(;i<14*8;i++){
            mFrame[i]=3;
            cout<<mFrame[i]<<" ";
        }
        cout<<"\n";

        for(;i<20*8;i++){
            mFrame[i]=2;
            cout<<mFrame[i]<<" ";
        }
        cout<<"\n";

        for(;i<22*8;i++){
            mFrame[i]=3;
            cout<<mFrame[i]<<" ";
        }
        cout<<"\n";
        //finally keeping data
        int a;
        for(a=0;i<22*8 + charcount*8;i++,a++){
            mFrame[i]=mBinaryData[a];
            cout<<mFrame[i]<<" ";
        }
        cout<<"\n";

        //and crc ..
        for(a=0;i<26*8 + charcount*8;i++,a++){
            mFrame[i]=mCrc[a];
            cout<<mFrame[i]<<" ";
        }
        cout<<"\n";
    }

//--------------------------making frame ---------------------------//


//--------------------------error checking --------------------------//

    //Note : -> for error check we need the whole data + the crc ...
    // we again find the crc and if its "non zero" we "have error".

    public: bool isError(){

        int  mCrcXorIndices[] = {26,23,20,16,12,11,10,8,7,3,2,0};
        int  mOutValue =0 ,q=0,j,mSize;

        cout<<"Enter the total data bit count == charcount*8 + 32 crc bit length\n";
        cin>>mSize;     //size of crc+data

        int mInput[mSize];
        cout<<"Enter data and crc \n";
        for(i=0;i<mSize;i++){
            cin>>mInput[i];
        }


        initErrorCrc();
        for(i=0;i<mSize;i++){
            for(j=CRC_LENGTH-1;j>0;j--){                   // cal crc for each bit
                if(j==mCrcXorIndices[q]){                   //NOTE: we are going in backward direction i.e from last cell of hardware crc
                   mCrcError[j] = (mCrcError[j-1]==mOutValue) ? 0:1; // Xor and shift to next
                   q++;
                }
                else{
                   mCrcError[j]=mCrcError[j-1];                      // shift
                }
            }

            mCrcError[0] = (mInput[i] == mOutValue) ? 0:1;
            mOutValue = mCrcError[31];
        }

        for(i=0;i<31;i++)
            if(mCrcError[i]!=0)
                return true;

        return false;
    }

//-------------------------error check ----------------------------//

//-------------------------other lilliput functions --------------//
    private: void initErrorCrc(){
        for(i=0;i<32;i++)
            mCrcError[i]=0;
    }

    private :void initCrc(){
        for(i=0;i<32;i++)
            mCrc[i]=0;
    }

    public :int* getCrc(){
        return mCrc;
    }





private : mysocket mSocket;

 void sendData(){

    FILE *file;
    file = fopen("input_mac.txt","r");
    char c;
    while((c=getc(file))!=EOF){
        string str;
        str.push_back(c);
        str.push_back(getc(file));

        istringstream ss (str);
        unsigned long int value;
        ss>>hex>>value;
        cout<< str <<"   " << value <<"\n";
        binary(value);
        charcount++;
    }
    calculateCrc(mBinaryData,CHAR_COUNT);
 //   makeFrame();

    //convertframe to hex
    string finalstr;
    toHex();
    fclose(file);
    ofstream out ("input_mac.txt");
    out<<myhex;
    cout<<myhex;
//    fputs(finalstr,file);
    out.close();
//    fclose(file);

    mSocket.writeToSocket();  //writes whatever is in mac.txt to sockets
    // set to socket input
    // set hex str to file

}

public : void getData(){
    mSocket.readFromSocket();
  ///Dont have proper crc calculation .....
  /// need one last check up for the following
  /// crc
  /// tcp checsum and ack,seq numbers
  /// final test with sockets...
//    while(hexArray[i]!=0){
        // write to socket
    }

public : void intializeSockets(int mType){
    mSocket.initialize(mType);
 }

};
int main(){

mac_layer myMac;
myMac.sendData();

// int mCharCount=5,i;
// char data[]="hello world";
// BitCRC myCrc;

// for(i=0;i<CHAR_COUNT;i++){
//    cout<<data[i];
//    cout<<"Entered primary loop"<<i<<"\n";
//    myCrc.binary(data[i]);
//
// }

// if(myCrc.isError())
//    cout<<"Error ";
// else
//    cout<<"no error";
}
