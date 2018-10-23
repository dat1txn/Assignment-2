// Assignment2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

string iovalues(string &str, string &x, string &y, string &z)
{
	z = str.substr(0, str.find(" "));
	str = str.substr((str.find(" ") + 3));
	x = str.substr(0, str.find(" "));
	str = str.substr((str.find(" ") + 3));
	y = str.substr(0, str.find(" "));
	return (x, y, z);
}

string iovaluesmux(string &str, string &w, string &x, string &y, string &z)
{
	z = str.substr(0, str.find(" "));
	str = str.substr((str.find(" ") + 3));
	w = str.substr(0, str.find(" "));
	str = str.substr((str.find(" ") + 3));
	x = str.substr(0, str.find(" "));
	str = str.substr((str.find(" ") + 3));
	y = str.substr(0, str.find(" "));
	return (x, y, z);
}

string iovaluesreg(string &str, string &x, string &z)
{
	z = str.substr(0, str.find(" "));
	str = str.substr((str.find(" ") + 3));
	x = str.substr(0, str.find(" "));
	return (x, z);
}

string iovaluesshift(string &str, string &x, string &y, string &z)
{
	z = str.substr(0, str.find(" "));
	str = str.substr((str.find(" ") + 3));
	x = str.substr(0, str.find(" "));
	str = str.substr((str.find(" ") + 4));
	y = str.substr(0, str.find(" "));
	return (x, y, z);
}

string iovaluescomp(string &str, string &x, string &y, string &z)
{
	int tempinc;
	size_t yes;
	z = str.substr(0, str.find(" "));
	str = str.substr((str.find(" ") + 3));
	x = str.substr(0, str.find(" "));
	yes = (str.find("=="));
	if (yes != string::npos)
	{
		tempinc = 4;
	}
	else
	{
		tempinc = 3;
	}
	str = str.substr((str.find(" ") + tempinc));
	y = str.substr(0, str.find(" "));
	return (x, y, z);
}


int main()
{
	string filename, filename1, filename2, iline, oline, str;
	string insize, outsize, w, x, y, z;
	size_t found, found1, found2, found3, found4, found5, found6;
	size_t found7, found8, found9, found10, found11, foundname1, foundname2;
	int bittemp, temp, bitsize = 0;
	

	cout << "Please enter filename: ";
	cin >> filename;
	filename1 = filename + ".txt";
	filename2 = filename + ".v";

	ifstream myfile1(filename1);
	ofstream myfile2(filename2);


	if (myfile1.is_open())
	{
		oline = "`timescale 1ns / 1ns \n";
			if (myfile2.is_open())
			{
				myfile2 << oline << '\n';
			}
			else cout << "Unable to open file";
	}
	else cout << "Unable to open file";

	while (getline(myfile1, iline))
	{
		foundname1 = iline.find("input");
		if (foundname1 != string::npos)
		{
			iline = iline.substr(foundname1 + 6);
			foundname1 = iline.find("Int");
			iline = iline.substr(foundname1 + 3);
			foundname2 = iline.find(" ");
			if (foundname2 != string::npos)
			{
				insize = iline.substr(0, foundname2);
				bitsize = stoi(insize);
			}
			
		}
		foundname1 = iline.find("output");
		if (foundname1 != string::npos)
		{
			iline = iline.substr(foundname1 + 6);
			foundname1 = iline.find("Int");
			iline = iline.substr(foundname1 + 3);
			foundname2 = iline.find(" ");
			if (foundname2 != string::npos)
			{
				outsize = iline.substr(0, foundname2);
				bittemp = stoi(outsize);
				if (bittemp < bitsize)
					bitsize = bitsize;
				else
					bitsize = bittemp;
			}
			oline = "module " + filename + " #(parameter DATAWIDTH = " + outsize + ")(a, b, c, z, x); \n";
		}

		found = iline.find("=");
		if (found != string::npos)
			temp = 0;
		{
			found1 = iline.find("+");
			if (found1 != string::npos)
			{
				temp = 1;
				found1 = iline.find("+ 1");
				if (found1 != string::npos)
				{
					str = iline;
					(x, y, z) = iovalues(str, x, y, z);
					oline = oline + "INC #(.DATAWIDTH(XX)) INC_1(.a(" + x + "), .d(" + z + ")); \n";
				}
				else
				{
					str = iline;
					(x, y, z) = iovalues(str, x, y, z);
					oline = oline + "ADD #(.DATAWIDTH(XX)) ADD_1(.a(" + x + "), .b(" + y + "), .sum(" + z + ")); \n";
				}
			}
			found2 = iline.find("-");
			if (found2 != string::npos)
			{
				temp = 2;
				found2 = iline.find("- 1");
				if (found2 != string::npos)
				{
					str = iline;
					(x, y, z) = iovalues(str, x, y, z); 
					oline = oline + "DEC #(.DATAWIDTH(XX)) DEC_1(.a(" + x + "), .d(" + z + ")); \n";
				}
				else
				{
					str = iline;
					(x, y, z) = iovalues(str, x, y, z); 
					oline = oline + "SUB #(.DATAWIDTH(XX)) SUB_1(.a(" + x + "), .b(" + y + "), .diff(" + z + ")); \n";
				}
			}
			found3 = iline.find("*");
			if (found3 != string::npos)
			{
				str = iline;
				(x, y, z) = iovalues(str, x, y, z); 
				oline = oline + "MUL #(.DATAWIDTH(XX)) MUL_1(.a(" + x + "), .b(" + y + "), .prod(" + z + ")); \n";
				temp = 3;
			}
			found4 = iline.find("/");
			if (found4 != string::npos)
			{
				str = iline;
				(x, y, z) = iovalues(str, x, y, z); 
				oline = oline + "DIV #(.DATAWIDTH(XX)) DIV_1(.a(" + x + "), .b(" + y + "), .quot(" + z + ")); \n";
				temp = 4;
			}
			found5 = iline.find("%");
			if (found5 != string::npos)
			{
				str = iline;
				(x, y, z) = iovalues(str, x, y, z); 
				oline = oline + "MOD #(.DATAWIDTH(XX)) MOD_1(.a(" + x + "), .b(" + y + "), .rem(" + z + ")); \n";
				temp = 5;
			}
			found6 = iline.find("<<");
			if (found6 != string::npos)
			{
				str = iline;
				(x, y, z) = iovaluesshift(str, x, y, z); 
				oline = oline + "SHL #(.DATAWIDTH(XX)) SHL_1(.a(" + x + "), .sh_amt(" + y + "), .d(" + z + ")); \n";
				temp = 6;
			}
			found7 = iline.find(">>");
			if (found7 != string::npos)
			{
				str = iline;
				(x, y, z) = iovaluesshift(str, x, y, z); 
				oline = oline + "SHR #(.DATAWIDTH(XX)) SHR_1(.a(" + x + "), .sh_amt(" + y + "), .d(" + z + ")); \n";
				temp = 7;
			}
			found8 = iline.find("?");
			if (found8 != string::npos)
			{
				str = iline;
				(w, x, y, z) = iovaluesmux(str, w, x, y, z); 
				oline = oline + "MUX2x1 #(.DATAWIDTH(XX)) MUX2x1(.a(" + x + "), .b(" + y + "), .sel(" + w + "), .d(" + z + ")); \n";
				temp = 8;
			}
			found9 = iline.find("==");
			if (found9 != string::npos)
			{
				str = iline;
				(x, y, z) = iovaluescomp(str, x, y, z); 
				oline = oline + "COMP #(.DATAWIDTH(XX)) COMP_1(.a(" + x + "), .b(" + y + "), .gt(0), .lt(0), .eq(" + z + ")); \n";
				temp = 9;
			}
			found10 = iline.find("<");
			if ((found10 != string::npos) && (temp != 6))
			{
				str = iline;
				(x, y, z) = iovaluescomp(str, x, y, z); 
				oline = oline + "COMP #(.DATAWIDTH(XX)) COMP_1(.a(" + x + "), .b(" + y + "), .gt(0), .lt(" + z + "), .eq(0)); \n";
				temp = 10;
			}
			found11 = iline.find(">");
			if ((found11 != string::npos) && (temp != 7))
			{
				str = iline;
				(x, y, z) = iovaluescomp(str, x, y, z); 
				oline = oline + "COMP #(.DATAWIDTH(XX)) COMP_1(.a(" + x + "), .b(" + y + "), .gt(" + z + "), .lt(0), .eq(0)); \n";
				temp = 11;
			}
			if ((found != string::npos) && (temp == 0))
			{
				str = iline;
				(x, z) = iovaluesreg(str, x, z); 
				oline = oline + "REG #(.DATAWIDTH(XX)) REG_1(.d(" + x + "), .Clk(1), .Rst(0), .q(" + z + ")); \n";
			}
		}
	}
	myfile2 << oline << '\n';
	myfile2 << "endmodule" << '\n';
	myfile1.close();
	myfile2.close();

	return 0;
}




