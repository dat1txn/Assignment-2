// Assignment2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main()
{
	string filename, filename1, filename2, iline, oline, str;
	string insize, outsize;
	size_t found, found1, found2, found3, found4, found5, found6;
	size_t found7, found8, found9, found10, found11, foundname1, foundname2;
	int bittemp, temp, compare = 0, bitsize = 0;

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
			oline = "module " + filename + "#(parameter DATAWIDTH = " + outsize + ")(a, b, c, z, x); \n";
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
					oline = oline + "INC #(.DATAWIDTH(XX)) INC_1(.a(a), .b(b), .sum(d)); \n";
				}
				else
				{
					oline = oline + "ADD #(.DATAWIDTH(XX)) ADD_1(.a(a), .b(b), .sum(d)); \n";
				}
			}
			found2 = iline.find("-");
			if (found2 != string::npos)
			{
				temp = 2;
				found2 = iline.find("- 1");
				if (found2 != string::npos)
				{
					oline = oline + "DEC #(.DATAWIDTH(XX)) DEC_1(.a(f), .b({8'b00000000, d}), .diff(xwire)); \n";
				}
				else
				{
					oline = oline + "SUB #(.DATAWIDTH(XX)) SUB_1(.a(f), .b({8'b00000000, d}), .diff(xwire)); \n";
				}
			}
			found3 = iline.find("*");
			if (found3 != string::npos)
			{
				oline = oline + "MUL #(.DATAWIDTH(XX)) MUL_1(.a(a), .b(c), .prod(f)); \n";
				temp = 3;
			}
			found4 = iline.find("/");
			if (found4 != string::npos)
			{
				oline = oline + "DIV #(.DATAWIDTH(XX)) DIV_1(.a(a), .b(b), .quot(e)); \n";
				temp = 4;
			}
			found5 = iline.find("%");
			if (found5 != string::npos)
			{
				oline = oline + "MOD #(.DATAWIDTH(XX)) MOD_1(.a(a), .b(c), .rem(g)); \n";
				temp = 5;
			}
			found6 = iline.find("<<");
			if (found6 != string::npos)
			{
				oline = oline + "SHL #(.DATAWIDTH(XX)) SHL_1(.a(g), .sh_amt({31'b0,dLTe}), .d(xwire)); \n";
				temp = 6;
			}
			found7 = iline.find(">>");
			if (found7 != string::npos)
			{
				oline = oline + "SHR #(.DATAWIDTH(XX)) SHR_1(.a(l2), .sh_amt({8'b00000000, sa}), .d(l2div2)); \n";
				temp = 7;
			}
			found8 = iline.find("?");
			if (found8 != string::npos)
			{
				oline = oline + "MUX2x1 #(.DATAWIDTH(XX)) MUX2x1(.a(d), .b(e), .sel(g), .d(z)); \n";
				temp = 8;
			}
			found9 = iline.find("==");
			if (found9 != string::npos)
			{
				oline = oline + "COMP #(.DATAWIDTH(XX)) COMP_1(.a(d), .b(e), .gt(g), .lt(l), .eq(e)); \n";
				temp = 9;
			}
			found10 = iline.find("<");
			if ((found10 != string::npos) && (temp != 6))
			{
				oline = oline + "COMP #(.DATAWIDTH(XX)) COMP_1(.a(d), .b(e), .gt(g), .lt(l), .eq(e)); \n";
				temp = 10;
			}
			found11 = iline.find(">");
			if ((found11 != string::npos) && (temp != 7))
			{
				oline = oline + "COMP #(.DATAWIDTH(XX)) COMP_1(.a(d), .b(e), .gt(g), .lt(l), .eq(e)); \n";
				temp = 11;
			}
			if ((found != string::npos) && (temp == 0))
			{
				oline = oline + "REG #(.DATAWIDTH(XX)) REG_1(.d(xwire), .Clk(1), .Rst(0), .q(x)); \n";
			}
		}
	}
	myfile2 << oline << '\n';
	myfile2 << "endmodule" << '\n';
	myfile1.close();
	myfile2.close();

	return 0;
}

