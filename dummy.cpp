#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <string.h>

using namespace std;

int main(){
 char myarray[8];
 int seq =200,i,j;
 for(i=200;i<220;i++){
        stringstream ss;
   	 	ss<<hex<<(int)seq;
    	string strout (ss.str());
        while(strout.length()<8){
            strout.insert(0,1,'0');
        }
        for(j=0;j<8;j++){
            myarray[j]=strout[j];
            cout<<myarray[j];
        }
       // cout<<strout<<"\n";
        seq+=1;
 }
 return 0;

}
