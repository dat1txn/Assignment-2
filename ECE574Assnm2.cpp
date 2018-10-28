// Assignment2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

//find the datawidth
string outdatawidth(string outstr, string wirestr, string regstr, string z, string &z_dw)
{
	size_t foundstr;
	if ((foundstr = (outstr.find(z))) && (foundstr < 50))
		z_dw = "1";
	if ((foundstr = (wirestr.find(z))) && (foundstr < 50))
		z_dw = "2";
	if ((foundstr = (regstr.find(z))) && (foundstr < 50))
		z_dw = "3";
	return(z_dw);
}

//genaric routine to find input and output variables for datapath components
string iovalues(string &str, string &x, string &y, string &z) 
{
	z = str.substr(0, str.find(" "));	
	str = str.substr((str.find(" ") + 3));
	x = str.substr(0, str.find(" "));
	str = str.substr((str.find(" ") + 3));
	y = str.substr(0, str.find(" "));
	return (x, y, z);
}

//MUX  find the input and output variables for MUX
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

//REG  find the input and output variables for REG
string iovaluesreg(string &str, string &x, string &z)
{
	z = str.substr(0, str.find(" "));
	str = str.substr((str.find(" ") + 3));
	x = str.substr(0, str.find(" "));
	return (x, z);
}

//SHIFT  find the input and output variables for SHR and SHL
string iovaluesshift(string &str, string &x, string &y, string &z) 
{
	z = str.substr(0, str.find(" "));
	str = str.substr((str.find(" ") + 3));
	x = str.substr(0, str.find(" "));
	str = str.substr((str.find(" ") + 4));
	y = str.substr(0, str.find(" "));
	return (x, y, z);
}

//COMP  find the input and output variables for COMP
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
	string filename, filename1, filename2, iline, oline, newline, str;
	//string iline  
	//oline
	string instr[10] = {}, outstr[10] = {}, wirestr[10] = {}, regstr[10] = {};
	string insize, outsize, w, x, y, z, x_dw, y_dw, z_dw;
	size_t found, found1, found2, found3, found4, found5, found6;
	size_t found7, found8, found9, found10, found11, foundname1, foundname2;
	int bittemp, temp = 0, bitsize = 0, start = 0, i = 0, m = 0, DW[10] = {0,0,0,0,0,0,0,0,0,0};
	//array to count instances of each datapath component
	int count_DPC[12]={0,0,0,0,0,0,0,0,0,0,0,0};		

	cout << "Please enter filename: "; // generate output file 
	cin >> filename;
	filename1 = filename + ".txt";
	filename2 = filename + ".v";
	cout << filename1 << endl;
	cout << "\n";
	cout << filename2 << endl;
	cout << "\n";

	ifstream in_file1(filename1); 			// open input file
	ofstream out_file2(filename2); 			//open output file

	//if (myfile1.is_open()) // open input file check and write to output file check
	//if (in_file1.is_open()) // open input file check and write to output file check
		//{
		oline = "`timescale 1ns / 1ns \n";			//add clock to verilog file
			if (out_file2.is_open())
			{
				out_file2 << oline << '\n';
			}
			else cout << "Unable to open out_file";
	//}
	//else cout << "Unable to open in_file";

	/***************
	This while loop block separates the input file into 4 types of strings:
	inputs, outputs, wires and register strings.
	it determines the DATAWIDTH for the module 
	and writes the first line of the module's code that identifies the module, DATAWIDTH, and variables
	******************/
	//while (getline(myfile1, iline)) //parse the input variables
	while (getline(in_file1, iline)) //put elements of in_file1 (input file) into string iline
		{
		foundname1 = iline.find("input");				//look for input variable
		if (foundname1 != string::npos)
		{
			iline = iline.substr(foundname1 + 6);
			foundname1 = iline.find("Int");
			iline = iline.substr(foundname1 + 3);
			//instr += iline + "#";
			foundname2 = iline.find(" ");			
			if (foundname2 != string::npos)
			{
				insize = iline.substr(0, foundname2);
				bitsize = stoi(insize);					//width of datatype
				DW[i] = bitsize;
				i = i++;								//i counts the variables
			}
			instr[m] = iline.substr(foundname2);		//inputs are put in instr
			m++;
		}
		foundname1 = iline.find("output"); 				// look for output variables
		if (foundname1 != string::npos)
		{
			iline = iline.substr(foundname1 + 6);
			foundname1 = iline.find("Int");
			iline = iline.substr(foundname1 + 3);
			//outstr += iline + "#";
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
			outstr[m] = iline.substr(foundname2);		//outputs are put in outstr
			m++;
			DW[i] = bitsize;
			i = i++;
			oline = "module " + filename + " #(parameter DATAWIDTH = " + outsize + ")(a, b, c, z, x); \n";
		}

		foundname1 = iline.find("wire");					//look for wires
		if ((foundname1 != string::npos) && (foundname1 /*start*/ == 0))
		{
			iline = iline.substr(foundname1 + 4);
			foundname1 = iline.find("Int");
			iline = iline.substr(foundname1 + 3);
			//wirestr += iline + "#";
			foundname2 = iline.find(" ");
			if (foundname2 != string::npos)
			{
				insize = iline.substr(0, foundname2);
				bitsize = stoi(insize);
			}
			wirestr[m] = iline.substr(foundname2);
			m++;
			DW[i] = bitsize;
			i = i++;
		}

		foundname1 = iline.find("register");				//look for registers
		if (foundname1 != string::npos)
		{
			iline = iline.substr(foundname1 + 8);
			foundname1 = iline.find("Int");
			iline = iline.substr(foundname1 + 3);
			//regstr += iline + "#";
			foundname2 = iline.find(" ");
			if (foundname2 != string::npos)
			{
				insize = iline.substr(0, foundname2);
				bitsize = stoi(insize);
			}
			regstr[m] = iline.substr(foundname2);
			m++;
			DW[i] = bitsize;
			i = i++;
		}
	}  //while (getline(in_file1, iline))

	/**************
	This for loop block  creates declartions of inputs, outputs, wires and registers
	***************/
	out_file2 << oline;
	for (int p = 0; p < i; p++)			
	{
		if (instr[p] != "")			//if instr[p
		{
			str = instr[p];
			string str2 = std::to_string((stoi(outsize)) / (DW[p]));
			newline = newline + "input[DATAWIDTH/" + str2 + "-1:0]" + str + "; \n";
		}
		if (outstr[p] != "")
		{
			str = outstr[p];
			string str2 = std::to_string((stoi(outsize)) / (DW[p]));
			newline = newline + "output wire[DATAWIDTH/" + str2 + "-1:0]" + str + "; \n";
		}
		if (wirestr[p] != "")
		{
			str = wirestr[p];
			string str2 = std::to_string((stoi(outsize)) / (DW[p]));
			newline = newline + "wire[DATAWIDTH/" + str2 + "-1:0]" + str + "; \n";
		}
		if (regstr[p] != "")
		{
			str = regstr[p];
			string str2 = std::to_string((stoi(outsize)) / (DW[p]));
			newline = newline + "register[DATAWIDTH/" + str2 + "-1:0]" + str + "; \n";
		}
	}
	oline = "";
	in_file1.close();
	out_file2 << newline;
	ifstream myfile3(filename1);

	if (myfile3.is_open())				// open input file
	{
		out_file2 << '\n';
	}
	else cout << "Unable to open file";

	/**********************************
	this while loop bloc works through the input file looking for datapath components
	and writes a line to the output file for each datapath component
	***********************************/
	while (getline(myfile3, iline)) 
	{
		found = iline.find("="); // detemine which operation is being performed and generate structure line to output
		if ((found != string::npos) && ( found < 50))							
		{
			start = 1;
			found1 = iline.find("+");					//select ADD or INC
			if (found1 != string::npos)
			{
				temp = 1;
				found1 = iline.find("+ 1");
				if (found1 != string::npos)
				{
					str = iline;
					(x, y, z) = iovalues(str, x, y, z);
					count_DPC[10]++;							//count instances of INC
					//oline = oline + "INC #(.DATAWIDTH(XX)) INC_1(.a(" + x + "), .d(" + z + ")); \n";
					oline = oline + "INC #(.DATAWIDTH(XX)) INC_" + std::to_string(count_DPC[10]) + "(.a(" + x + "), .d(" + z + ")); \n";
				}
				else
				{
					str = iline;
					(x, y, z) = iovalues(str, x, y, z);
					count_DPC[1]++;							//count instances of ADD
					//z_dw = outdatawidth(outstr, wirestr, regstr, z, z_dw);
					oline = oline + "ADD #(.DATAWIDTH(" + z_dw + ")) ADD_" + std::to_string(count_DPC[1]) + "(.a(" + x + "), .b(" + y + "), .sum(" + z + ")); \n";
				}
			}
			found2 = iline.find("-");					//select SUB or DEC
			if (found2 != string::npos)
			{
				temp = 2;
				found2 = iline.find("- 1");
				if (found2 != string::npos)
				{
					str = iline;
					(x, y, z) = iovalues(str, x, y, z); 
					count_DPC[11]++;							//count instances of DEC
					oline = oline + "DEC #(.DATAWIDTH(XX)) DEC_" + std::to_string(count_DPC[11]) + "(.a(" + x + "), .d(" + z + ")); \n";
				}
				else
				{
					str = iline;
					(x, y, z) = iovalues(str, x, y, z); 
					count_DPC[2]++;							//count instances of SUB
					oline = oline + "SUB #(.DATAWIDTH(XX)) SUB_" + std::to_string(count_DPC[2]) + "(.a(" + x + "), .b(" + y + "), .diff(" + z + ")); \n";
				}
			}
			found3 = iline.find("*");					//select MUL
			if (found3 != string::npos)
			{
				str = iline;
				(x, y, z) = iovalues(str, x, y, z); 
				count_DPC[3]++;							//count instances of MUL
				oline = oline + "MUL #(.DATAWIDTH(XX)) MUL_" + std::to_string(count_DPC[3]) + "(.a(" + x + "), .b(" + y + "), .prod(" + z + ")); \n";
				temp = 3;
			}
			found4 = iline.find("/");					//select DIV
			if (found4 != string::npos)
			{
				str = iline;
				(x, y, z) = iovalues(str, x, y, z); 
				count_DPC[8]++;							//count instances of DIV
				oline = oline + "DIV #(.DATAWIDTH(XX)) DIV_" + std::to_string(count_DPC[8]) + "(.a(" + x + "), .b(" + y + "), .quot(" + z + ")); \n";
				temp = 4;
			}
			found5 = iline.find("%");					//select MOD
			if (found5 != string::npos)
			{
				str = iline;
				(x, y, z) = iovalues(str, x, y, z); 
				count_DPC[9]++;							//count instances of MOD
				oline = oline + "MOD #(.DATAWIDTH(XX)) MOD_" + std::to_string(count_DPC[9]) + "(.a(" + x + "), .b(" + y + "), .rem(" + z + ")); \n";
				temp = 5;
			}
			found6 = iline.find("<<");					//select shift left, SHL
			if (found6 != string::npos)
			{
				str = iline;
				(x, y, z) = iovaluesshift(str, x, y, z); 
				count_DPC[7]++;							//count instances of SHL
				oline = oline + "SHL #(.DATAWIDTH(XX)) SHL_" + std::to_string(count_DPC[7]) + "(.a(" + x + "), .sh_amt(" + y + "), .d(" + z + ")); \n";
				temp = 6;
			}
			found7 = iline.find(">>");					//select shift right, SHR
			if (found7 != string::npos)
			{
				str = iline;
				(x, y, z) = iovaluesshift(str, x, y, z); 
				count_DPC[6]++;							//count instances of SHR
				oline = oline + "SHR #(.DATAWIDTH(XX)) SHR_" + std::to_string(count_DPC[6]) + "(.a(" + x + "), .sh_amt(" + y + "), .d(" + z + ")); \n";
				temp = 7;
			}
			found8 = iline.find("?");					//select MUX
			if (found8 != string::npos)
			{
				str = iline;
				(w, x, y, z) = iovaluesmux(str, w, x, y, z); 
				count_DPC[5]++;							//count instances of MUX
				oline = oline + "MUX2x1 #(.DATAWIDTH(XX)) MUX2x1_" + std::to_string(count_DPC[6]) + "(.a(" + x + "), .b(" + y + "), .sel(" + w + "), .d(" + z + ")); \n";
				temp = 8;
			}
			found9 = iline.find("==");					//select COMP, eq output
			if (found9 != string::npos)
			{
				str = iline;
				(x, y, z) = iovaluescomp(str, x, y, z); 
				count_DPC[4]++;							//count instances of COMP
				oline = oline + "COMP #(.DATAWIDTH(XX)) COMP_" + std::to_string(count_DPC[4]) + "(.a(" + x + "), .b(" + y + "), .eq(" + z + ")); \n";
				temp = 9;
			}
			found10 = iline.find("<");					//select COMP, lt output 
			if ((found10 != string::npos) && (temp != 6))
			{
				str = iline;
				(x, y, z) = iovaluescomp(str, x, y, z); 
				count_DPC[4]++;							//count instances of COMP
				oline = oline + "COMP #(.DATAWIDTH(XX)) COMP_" + std::to_string(count_DPC[4]) + "(.a(" + x + "), .b(" + y + "), .lt(" + z + ")); \n";
				temp = 10;
			}
			found11 = iline.find(">");					//select COMP, gt output
			if ((found11 != string::npos) && (temp != 7))
			{
				str = iline;
				(x, y, z) = iovaluescomp(str, x, y, z); 
				count_DPC[4]++;							//count instances of COMP
				oline = oline + "COMP #(.DATAWIDTH(XX)) COMP_" + std::to_string(count_DPC[4]) + "(.a(" + x + "), .b(" + y + "), .gt(" + z + ")); \n";
				temp = 11;
			}
			if ((found != string::npos) && (temp == 0))		//select REG
			{
				str = iline;
				(x, z) = iovaluesreg(str, x, z); 
				count_DPC[0]++;							//count instances of REG
				oline = oline + "REG #(.DATAWIDTH(XX)) REG_" + std::to_string(count_DPC[0]) + "(.d(" + x + "), .Clk(1), .Rst(0), .q(" + z + ")); \n";
			}
		}
	}
	out_file2 << oline << '\n';
	out_file2 << "endmodule" << '\n';
	myfile3.close();
	out_file2.close();

	return 0;
}
