#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <string.h>
using namespace std;

class app_layer{
public : void application_send()
{
	string strfile;
	string strout;
  	stringstream ss;
  	int i=0,nbytes;
  	char c;
   	FILE *file;

   	//store input.txt into a string

 	file = fopen("input.txt", "r");
  	if (file)
  	{
      		while ((c = getc(file)) != EOF)
      		{
          		putchar(c);
          		strfile.push_back(c);
      		}
      		fclose(file);

  	}

  	//convert strfile to hex and store it in strout as a string

  	for(i=0;i<strfile.size()-1;i++)
  	{
    	stringstream ss;
   	 	ss<<hex<<(int)strfile[i];
    	strout.append(ss.str());
  	}

  	//attach application header, no.of bytes and EOF

  	nbytes=strout.size()/2;
  	ss<<hex<<nbytes;
  	strout = "40" + ss.str() + strout + "04";

  	cout<<"\nhexadecimal frame:\n"<<strout<<"\n";

  	//write the hex string to input_tcp.txt

  	ofstream myfile ("input_tcp.txt");
	if (myfile.is_open())
  	{
    	myfile << strout;
   		myfile.close();
  	}
}

public : void application_receive()
{
	string strfile,outfile;
	FILE *file;
	stringstream ss;
	char c;
	int temp1,temp2,i=0;
	//store input.txt into a string

 	file = fopen("input_tcp.txt", "r");
  	if (file)
  	{
      		while ((c = getc(file)) != EOF)
      		{
          		//putchar(c);
          		strfile.push_back(c);
      		}
      		fclose(file);
  	}

   	while(i<strfile.size()-3)
	{
		string s;
		s.push_back(strfile[i]);
		s.push_back(strfile[i+1]);
		unsigned long int value;
		istringstream iss(s);
		iss>>hex>>value;
		c=value;
		//putchar(c);
		if(i>3)
			outfile.push_back(c);
		i=i+2;
	}
  	cout<<"\nfinal message:\n"<<outfile<<endl;

}

};

//int main ()
//{
//	application_send();
////	application_receive();
//	return 0;
//}
