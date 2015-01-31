#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string.h>
#include <vector>
using namespace std;

class ip_layer{
string hex_frame;

void ip_convert_to_hex(string binary_frame)
{
  hex_frame="";
  int i=0;
  char temp[10];

  while (i<binary_frame.size()-1)
  {
    temp[0]=binary_frame[i];
    temp[1]=binary_frame[i+1];
    temp[2]=binary_frame[i+2];
    temp[3]=binary_frame[i+3];
    temp[4]='\0';

    if(strcmp(temp,"0000")==0)
      hex_frame.push_back('0');
    else if(strcmp(temp,"0001")==0)
      hex_frame.push_back('1');
    else if(strcmp(temp,"0010")==0)
      hex_frame.push_back('2');
    else if(strcmp(temp,"0011")==0)
      hex_frame.push_back('3');
    else if(strcmp(temp,"0100")==0)
      hex_frame.push_back('4');
    else if(strcmp(temp,"0101")==0)
      hex_frame.push_back('5');
    else if(strcmp(temp,"0110")==0)
      hex_frame.push_back('6');
    else if(strcmp(temp,"0111")==0)
      hex_frame.push_back('7');
    else if(strcmp(temp,"1000")==0)
      hex_frame.push_back('8');
    else if(strcmp(temp,"1001")==0)
      hex_frame.push_back('9');
    else if(strcmp(temp,"1010")==0)
      hex_frame.push_back('a');
    else if(strcmp(temp,"1011")==0)
      hex_frame.push_back('b');
    else if(strcmp(temp,"1100")==0)
      hex_frame.push_back('c');
    else if(strcmp(temp,"1101")==0)
      hex_frame.push_back('d');
    else if(strcmp(temp,"1110")==0)
      hex_frame.push_back('e');
    else if(strcmp(temp,"1111")==0)
      hex_frame.push_back('f');
    else
          ;
    i=i+4;
  }
}
public : void ip_send()
{
  string ip;
  string version="0100",header_length="0101",service_type="00011110",total_length,identification="0000000000000000",flags="010";
  string fragment_offset="0000000000000",ttl="11111111",protocol="00000110",header_checksum ="0000000000000000";
  string source_ip_address="01010000101000010000010100010101",destination_ip_address="01011000101001010000010100000101",temp1;
  string instr,outstr;
  char c;
  int nchar=0,tot_len,i=0,j;

  //calculation of total length = header + data

  FILE *file;
  file = fopen("input_ip.txt", "r");
    if (file)
    {
        while ((c = getc(file)) != EOF)
        {
         // putchar(c);
          instr.push_back(c);
          nchar++;
        }
          fclose(file);
    }
  tot_len=5+nchar/2;
  while(i<16)
  {
    if(tot_len%2==1)
      total_length.push_back('1');
    else
      total_length.push_back('0');
    tot_len=tot_len/2;
    i++;
  }
  //cout<<total_length<<endl;
  reverse(total_length.begin(),total_length.end());
  //cout<<total_length<<endl;

  ip = version + header_length + service_type + total_length + identification + flags + fragment_offset +  ttl + protocol + header_checksum + source_ip_address+ destination_ip_address;
///*****************
//  cout<<"before checksum calculation"<<endl;
  ip_convert_to_hex(ip);
//  cout<<"sender side ip header :"<<endl;
//  cout<<"binary : "<<ip<<endl;
//  cout<<"hex : "<<hex_frame<<endl;


  //checksum calculation

  for(i=0;i<ip.size()-1;i=i+16)
  {
    temp1="";
    temp1.push_back(ip[i]);
    temp1.push_back(ip[i+1]);
    temp1.push_back(ip[i+2]);
    temp1.push_back(ip[i+3]);
    temp1.push_back(ip[i+4]);
    temp1.push_back(ip[i+5]);
    temp1.push_back(ip[i+6]);
    temp1.push_back(ip[i+7]);
    temp1.push_back(ip[i+8]);
    temp1.push_back(ip[i+9]);
    temp1.push_back(ip[i+10]);
    temp1.push_back(ip[i+11]);
    temp1.push_back(ip[i+12]);
    temp1.push_back(ip[i+13]);
    temp1.push_back(ip[i+14]);
    temp1.push_back(ip[i+15]);
    for(j=0;j<16;j++)
    {
      if(temp1[j]==header_checksum[j])
        header_checksum[j]='0';
      else
        header_checksum[j]='1';
    }
  }

  ip = version + header_length + service_type + total_length + identification + flags + fragment_offset +  ttl + protocol + header_checksum + source_ip_address+ destination_ip_address;
///**********
//  cout<<"after checksum calculation"<<endl;
  ip_convert_to_hex(ip);
  cout<<"sender side ip header :"<<endl;
//  cout<<"binary : "<<ip<<endl;
 // cout<<"hex : "<<hex_frame<<endl;

  //ip = version + header_length + service_type + total_length + identification + flags + fragment_offset +  ttl + protocol + header_checksum + source_ip_address+ destination_ip_address;

  outstr = hex_frame + instr;

  ofstream myfile ("input_mac.txt");
  if (myfile.is_open())
  {
    myfile << outstr;
    myfile.close();
  }

  cout<<outstr;
 // cout<<"\nChecksum : "<<header_checksum<<"\n";

}

public : void ip_receive()
{
  string str,strout,strheader;
  char c;
  FILE *file;
  file = fopen("input_mac.txt", "r");
  if (file)
  {
    while ((c = getc(file)) != EOF)
    {
     // putchar(c);
      if(str.size()<24)
        str.push_back(c);

      if(strheader.size()<40)
        strheader.push_back(c);
      else
        strout.push_back(c);
    }
    fclose(file);
  }

//  cout<<"\nreceiver side :\n";
//  cout<<"str : "<<str<<endl;
  //cout<<"strout : "<<strout<<endl;

    string s;
    s.push_back(str[20]);
    s.push_back(str[21]);
    s.push_back(str[22]);
    s.push_back(str[23]);
    string check="0000000000000000";


  //cout<<"fucking checksum : "<<s<<endl;

  int i=0,j=0,flag=0;
  while(i<strheader.size())
  {
    string s="",strbin="";
    s.push_back(strheader[i]);
    s.push_back(strheader[i+1]);
    s.push_back(strheader[i+2]);
    s.push_back(strheader[i+3]);
 //   cout<<"16-bit word no. "<<i<<" (hex): "<<s<<endl;
    unsigned long int value;
    istringstream iss(s);
    iss>>hex>>value;
    int k=0;
    while(k<16)
    {
      if(value%2==1)
        strbin.push_back('1');
      else
        strbin.push_back('0');
      value=value/2;
      k++;
    }
    reverse(strbin.begin(),strbin.end());

 //   cout<<"16-bit word no. "<<i<<" (bin): "<<strbin<<endl;
    for(j=0;j<16;j++)
    {
      if(check[j]==strbin[j])
        check[j]='0';
      else
        check[j]='1';
    }

    i=i+4;
  }

 // cout<<"check : "<<check<<endl;
  for(j=0;j<check.size();j++)
    {
      if(check[j]!='0')
      {
        //cout<<check[j]<<endl;
        flag=1;
        //cout<<"j = "<<j<<"  flag = "<<flag<<endl;
        break;
      }
    }

  if(flag==0)
  {
    ofstream myfile ("input_tcp_receiver.txt");
    if (myfile.is_open())
    {
   //   cout<<"No error!"<<endl;
      myfile << strout;
      myfile.close();
    }
  }
  else
    cout<<"error!"<<endl;

  cout<<strout;
}

};

//int main ()
//{
// ip_layer ip;
//	ip.ip_send();
////  ip_receive();
//	return 0;
//}
