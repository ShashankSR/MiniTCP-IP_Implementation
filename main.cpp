#include<stdio.h>
#include "tcp_layer.h"
#include "ip.h"
#include "app_layer.h"
//#include "mac_layer.h"
// dummy code ...... this will be more clean
int mType;
int main(){

    tcp_Layer myTcp;
    app_layer myApp;
    ip_layer myIp;
   // BitCRC myMac;

    myTcp.setMachineType(mType);
    //myMac.initalizeSockets(mType);

   // mtApp.setType(mType);
    //myIp.setType(mType);
   // myMac.setType(mType);
 // if(mType==CLIENT){
 //if(mType==CLIENT){
    myApp.application_send();
    myTcp.start();
    myIp.ip_send();
//    myMac.sendData();
 //}

//   // myMac.start();
//
//    while(!myTcp.isFinished()){
//
//        myMac.getData();
//        myIp.ip_receive();
//        myTcp.recieveFrame();
//
//       if(!myTcp.isFinished()){
//            myIp.ip_send();
//            myMac.sendData();
//
//        }
//    }
//
//    myApp.application_receive();

 return 0;
}
