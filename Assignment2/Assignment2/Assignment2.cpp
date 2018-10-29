// Assignment2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

//genaric routine to find input and output variables for datapath components

string iovalues(string &str, string &x, string &y, string &z) //find input and output variables for structure
{
	z = str.substr(0, str.find(" "));	
	str = str.substr((str.find(" ") + 3));
	x = str.substr(0, str.find(" "));
	str = str.substr((str.find(" ") + 3));
	y = str.substr(0, str.find(" "));
	return (x, y, z);
}

//MUX  find the input and output variables for MUX

string iovaluesmux(string &str, string &w, string &x, string &y, string &z) //find input and output variables for structure
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

string iovaluesreg(string &str, string &x, string &z) //find input and output variables for structure
{
	z = str.substr(0, str.find(" "));
	str = str.substr((str.find(" ") + 3));
	x = str.substr(0, str.find(" "));
	return (x, z);
}

//SHIFT  find the input and output variables for SHR and SHL

string iovaluesshift(string &str, string &x, string &y, string &z) //find input and output variables for structure
{
	z = str.substr(0, str.find(" "));
	str = str.substr((str.find(" ") + 3));
	x = str.substr(0, str.find(" "));
	str = str.substr((str.find(" ") + 4));
	y = str.substr(0, str.find(" "));
	return (x, y, z);
}

//COMP  find the input and output variables for COMP

string iovaluescomp(string &str, string &x, string &y, string &z) //find input and output variables for structure
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


int opcheck(string newline, string &x, string &z) //operation check
{
	int e;
	size_t h;
	string op;
	string ops[11] = { "+", "-", "*", "/", "%", "<", ">", "==", "<<", ">>", "?"};
	h = newline.find(" ");
	newline = newline.substr(h + 1);
	h = newline.find(" ");
	newline = newline.substr(h + 1);	
	op = newline.substr(0,(newline.find(" ")));
	for (int b = 0; b < 12; b++)
	{
		if ((op != ops[b]) && (op != x) && (op != " "))
		{
			e = 1;
		}
		else
		{
			e = 0;
			break;
		}					
	}
	return (e);
}

int varcheck(int &nx, int &ny, int &nz, string x, string y, string z, string str2, string str3, string str4, string str5) //variable check
{
	size_t v, q;
	string x1 = x + ",", x2 = x + " ";
	string y1 = y + ",", y2 = y + " ";
	string z1 = z + ",", z2 = z + " ";
	string s2 = str2 + ",", s3 = str3 + ",", s4 = str4 + ",", s5 = str5 + ",";
	v = s2.find(x1);
	q = s2.find(x2);
	if ((v != string::npos) || (q != string::npos))
	{
		nx++;
	}
	v = s3.find(x1);
	q = s3.find(x2);
	if ((v != string::npos) || (q != string::npos))
	{
		nx++;
	}
	v = s4.find(x1);
	q = s4.find(x2);
	if ((v != string::npos) || (q != string::npos))
	{
		nx++;
	}
	v = s5.find(x1);
	q = s5.find(x2);
	if ((v != string::npos) || (q != string::npos))
	{
		nx++;
	}
	v = s2.find(y1);
	q = s2.find(y2);
	if ((v != string::npos) || (q != string::npos))
	{
		ny++;
	}
	v = s3.find(y1);
	q = s3.find(y2);
	if ((v != string::npos) || (q != string::npos))
	{
		ny++;
	}
	v = s4.find(y1);
	q = s4.find(y2);
	if ((v != string::npos) || (q != string::npos))
	{
		ny++;
	}
	v = s5.find(y1);
	q = s5.find(y2);
	if ((v != string::npos) || (q != string::npos))
	{
		ny++;
	}
	v = s2.find(z1);
	q = s2.find(z2);
	if ((v != string::npos) || (q != string::npos))
	{
		nz++;
	}
	v = s3.find(z1);
	q = s3.find(z2);
	if ((v != string::npos) || (q != string::npos))
	{
		nz++;
	}
	v = s4.find(z1);
	q = s4.find(z2);
	if ((v != string::npos) || (q != string::npos))
	{
		nz++;
	}
	v = s5.find(z1);
	q = s5.find(z2);
	if ((v != string::npos) || (q != string::npos))
	{
		nz++;
	}	
	return (0);
}

int varcheck2(int &nw, int &nx, int &ny, int &nz, string w, string x, string y, string z, string str2, string str3, string str4, string str5) //variable check
{
	
	string w1 = w + ",", w2 = w + " ";
	string x1 = x + ",", x2 = w + " ";
	string y1 = y + ",", y2 = w + " ";
	string z1 = z + ",", z2 = w + " ";
	string s2 = str2 + ",", s3 = str3 + ",", s4 = str4 + ",", s5 = str5 + ",";
	size_t v, q;
	v = s2.find(w1);
	q = s2.find(w2);
	if ((v != string::npos) || (q != string::npos))
	{
		nw++;
	}
	v = s3.find(w1);
	q = s3.find(w2);
	if ((v != string::npos) || (q != string::npos))
	{
		nw++;
	}
	v = s4.find(w1);
	q = s4.find(w2);
	if ((v != string::npos) || (q != string::npos))
	{
		nw++;
	}
	v = s5.find(w1);
	q = s5.find(w2);
	if ((v != string::npos) || (q != string::npos))
	{
		nw++;
	}
	v = s2.find(x1);
	q = s2.find(x2);
	if ((v != string::npos) || (q != string::npos))
	{
		nx++;
	}
	v = s3.find(x1);
	q = s3.find(x2);
	if ((v != string::npos) || (q != string::npos))
	{
		nx++;
	}
	v = s4.find(x1);
	q = s4.find(x2);
	if ((v != string::npos) || (q != string::npos))
	{
		nx++;
	}
	v = s5.find(x1);
	q = s5.find(x2);
	if ((v != string::npos) || (q != string::npos))
	{
		nx++;
	}
	v = s2.find(y1);
	q = s2.find(y2);
	if ((v != string::npos) || (q != string::npos))
	{
		ny++;
	}
	v = s3.find(y1);
	q = s3.find(y2);
	if ((v != string::npos) || (q != string::npos))
	{
		ny++;
	}
	v = s4.find(y1);
	q = s4.find(y2);
	if ((v != string::npos) || (q != string::npos))
	{
		ny++;
	}
	v = s5.find(y1);
	q = s5.find(y2);
	if ((v != string::npos) || (q != string::npos))
	{
		ny++;
	}
	v = s2.find(z1);
	q = s2.find(z2);
	if ((v != string::npos) || (q != string::npos))
	{
		nz++;
	}
	v = s3.find(z1);
	q = s3.find(z2);
	if ((v != string::npos) || (q != string::npos))
	{
		nz++;
	}
	v = s4.find(z1);
	q = s4.find(z2);
	if ((v != string::npos) || (q != string::npos))
	{
		nz++;
	}
	v = s5.find(z1);
	q = s5.find(z2);
	if ((v != string::npos) || (q != string::npos))
	{
		 nz++;
	}
	return (0);
}
int varcheck3(int &nx, int &nz, string x, string z, string str2, string str3, string str4, string str5) //variable check
{
	
	size_t v, q;
	string x1 = x + ",", x2 = x + " ";
	string z1 = z + ",", z2 = z + " ";
	string s2 = str2 + ",", s3 = str3 + ",", s4 = str4 + ",", s5 = str5 + ",";
	v = s2.find(x1);
	q = s2.find(x2);
	if ((v != string::npos) || (q != string::npos))
	{
		nx++;
	}
	v = s3.find(x1);
	q = s3.find(x2);
	if ((v != string::npos) || (q != string::npos))
	{
		nx++;
	}
	v = s4.find(x1);
	q = s4.find(x2);
	if ((v != string::npos) || (q != string::npos))
	{
		nx++;
	}
	v = s5.find(x1);
	q = s5.find(x2);
	if ((v != string::npos) || (q != string::npos))
	{
		nx++;
	}
	v = s2.find(z1);
	q = s2.find(z2);
	if ((v != string::npos) || (q != string::npos))
	{
		nz++;
	}
	v = s3.find(z1);
	q = s3.find(z2);
	if ((v != string::npos) || (q != string::npos))
	{
		nz++;
	}
	v = s4.find(z1);
	q = s4.find(z2);
	if ((v != string::npos) || (q != string::npos))
	{
		nz++;
	}
	v = s5.find(z1);
	q = s5.find(z2);
	if ((v != string::npos) || (q != string::npos))
	{
		nz++;
	}
	return (0);
}

int main()
{
	string filename, filename1, filename2, iline, oline, newline, str, str1, strc, strv;
	string str2, str3, str4, str5;
	string instr[10] = {}, outstr[10] = {}, wirestr[10] = {}, regstr[12] = {};	
	string insize, outsize, w, x, y, z, x_dw, y_dw, z_dw[13] = {};
	size_t found, found1, found2, found3, found4, found5, found6;
	size_t found7, found8, found9, found10, found11, foundname1, foundname2;
	int bittemp, temp = 0, bitsize = 0, start = 0, i = 0, m = 0, DW[15] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	//array to count instances of each datapath component
	int s = 0, error = 0, here = 0, count_DPC[12] = { 0,0,0,0,0,0,0,0,0,0,0,0 };
	int nw = 0, nx = 0, ny = 0, nz = 0;


	cout << "Please enter filename: "; // generate output file 
	cin >> filename;
	filename1 = filename + ".txt";
	filename2 = filename + ".v";

	ifstream myfile1(filename1); // open input file
	ofstream myfile2(filename2); //open output file


	if (myfile1.is_open()) // open input file check and write to output file check
	{
		oline = "`timescale 1ns / 1ns \n";
			if (myfile2.is_open())
			{
				myfile2 << oline << '\n';
			}
			else cout << "Unable to open file";
	}
	else cout << "Unable to open file";

	/***************
	This while loop block separates the input file into 4 types of strings:
	inputs, outputs, wires and register strings.
	it determines the DATAWIDTH for the module
	and writes the first line of the module's code that identifies the module, DATAWIDTH, and variables
	******************/

	while (getline(myfile1, iline)) //parse the input variables
	{
		foundname1 = iline.find("input");
		if (foundname1 != string::npos)
		{
			if ((foundname1 = iline.find("UInt")) != string::npos)
				iline = iline.substr(foundname1 + 4);
			if ((foundname1 = iline.find("Int")) != string::npos)
				iline = iline.substr(foundname1 + 3);
			//iline = iline.substr(foundname1 + 3);
			foundname2 = iline.find(" ");
			if (foundname2 != string::npos)
			{
				insize = iline.substr(0, foundname2);
				bittemp = stoi(insize);
				if (bittemp < bitsize)
					bitsize = bitsize;
				else
					bitsize = bittemp;
			}
			instr[m] = iline.substr(foundname2);
			m++;
			DW[i] = bittemp;
			i = i++;
		}
		foundname1 = iline.find("output"); // parse the output variables
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
			outstr[m] = iline.substr(foundname2);
			m++;
			DW[i] = bittemp;
			i = i++;
		}

		foundname1 = iline.find("wire");
		if ((foundname1 != string::npos) && (foundname1 == 0))
		{
			iline = iline.substr(foundname1 + 4);
			foundname1 = iline.find("Int");
			iline = iline.substr(foundname1 + 3);
			foundname2 = iline.find(" ");
			if (foundname2 != string::npos)
			{
				insize = iline.substr(0, foundname2);
				bittemp = stoi(insize);
			}
			wirestr[m] = iline.substr(foundname2);
			m++;
			DW[i] = bittemp;
			i = i++;
		}

		foundname1 = iline.find("register");
		if (foundname1 != string::npos)
		{
			iline = iline.substr(foundname1 + 8);
			foundname1 = iline.find("Int");
			iline = iline.substr(foundname1 + 3);
			foundname2 = iline.find(" ");
			if (foundname2 != string::npos)
			{
				insize = iline.substr(0, foundname2);
				bittemp = stoi(insize);
			}
			regstr[m] = iline.substr(foundname2);
			m++;
			DW[i] = bittemp;
			i = i++;
		}
	}
	for (int c = 0; c < i; c++)
	{
		if (instr[c] != "")
		{
			if (c == 0)
			{
				str = instr[c];				
			}
			else
			{
				str = str + "," + instr[c];
			}
		}
		if (outstr[c] != "")
		{
			str = str + "," + outstr[c];
		}
	}
	oline = "module " + filename + " #(parameter DATAWIDTH = " + std::to_string(bitsize) + ")(" + str + "); \n";
	myfile2 << oline;

	/**************
	This for loop block  creates declartions of inputs, outputs, wires and registers
	***************/

	for (int p = 0; p < i; p++)
	{
		if (instr[p] != "")
		{
			str = instr[p];
			if ((stoi(outsize)) >= (DW[p]))
			{
				string str2 = std::to_string(bitsize / (DW[p]));
				newline = newline + "input[DATAWIDTH/" + str2 + "-1:0]" + str + "; \n";
			}
				
			else
			{
				string str2 = std::to_string((DW[p]) / bitsize);
				newline = newline + "input[DATAWIDTH*" + str2 + "-1:0]" + str + "; \n";
			}				
		}
		if (outstr[p] != "")
		{
			str = outstr[p];
			if ((stoi(outsize)) >= (DW[p]))
			{
				string str2 = std::to_string(bitsize / (DW[p]));
				newline = newline + "output wire[DATAWIDTH/" + str2 + "-1:0]" + str + "; \n";
			}

			else
			{
				string str2 = std::to_string((DW[p]) / bitsize);
				newline = newline + "output wire[DATAWIDTH*" + str2 + "-1:0]" + str + "; \n";
			}
		}
		if (wirestr[p] != "")
		{
			str = wirestr[p];
			if ((stoi(outsize)) >= (DW[p]))
			{
				string str2 = std::to_string(bitsize / (DW[p]));
				newline = newline + "wire[DATAWIDTH/" + str2 + "-1:0]" + str + "; \n";
			}

			else
			{
				string str2 = std::to_string((DW[p]) / bitsize);
				newline = newline + "wire[DATAWIDTH*" + str2 + "-1:0]" + str + "; \n";
			}
		}
		if (regstr[p] != "")
		{
			str = regstr[p];
			if ((stoi(outsize)) >= (DW[p]))
			{
				string str2 = std::to_string(bitsize / (DW[p]));
				newline = newline + "register[DATAWIDTH/" + str2 + "-1:0]" + str + "; \n";
			}

			else
			{
				string str2 = std::to_string((DW[p]) / bitsize);
				newline = newline + "register[DATAWIDTH*" + str2 + "-1:0]" + str + "; \n";
			}
		}
	}
	oline = "";
	myfile1.close();
	myfile2 << newline;
	ifstream myfile3(filename1);

	if (myfile3.is_open()) // open input file check and write to output file check
	{
		myfile2 << '\n';
	}
	else cout << "Unable to open file";

	/**********************************
	this while loop bloc works through the input file looking for datapath components
	and writes a line to the output file for each datapath component
	***********************************/
	while (getline(myfile3, iline))
	{
 		found = iline.find(" = "); // detemine which operation is being performed and generate structure line to output
		if ((found != string::npos) && (found < 50))
		{
			temp = 0;
			newline = iline.substr(found + 2);
			found1 = iline.find(" + ");					//select ADD or INC
			if (found1 != string::npos)
			{
				temp = 1;				
				found1 = iline.find("+ 1");
				if (found1 != string::npos)
				{
					str = iline;
					(x, y, z) = iovalues(str, x, y, z);
					for (int m = 0; m < i; m++)
					{
						str2 = instr[m];
						str3 = outstr[m];
						str4 = wirestr[m];
						str5 = regstr[m];
						s = varcheck3(nx, nz, x, z, str2, str3, str4, str5);
						//here = here + s;
						if ((nx > 0) && (nz > 0))
							break;
					}
					if ((nx == 0) || (nz == 0))
					{
						cout << endl
							<< " Missing Variable " << endl;
						break;
					}
					here = 0;
					nw = 0, nx = 0, ny = 0, nz = 0;
					for (int g = 0; g < i; g++)
					{
						strc = outstr[g];
						if (strc.find(z) < 20)
						{
							z_dw[0] = std::to_string(DW[g]);
						}
						strc = wirestr[g];
						if (strc.find(z) < 20)
						{
							z_dw[0] = std::to_string(DW[g]);
						}
						strc = regstr[g];
						if (strc.find(z) < 20)
						{
							z_dw[0] = std::to_string(DW[g]);
						}
					}
					count_DPC[10]++;							//count instances of INC
					oline = oline + "INC #(.DATAWIDTH" + z_dw[0] + ") INC_" + std::to_string(count_DPC[10]) + "(.a(" + x + "), .d(" + z + ")); \n";
				}
				else
				{
					str = iline;
					(x, y, z) = iovalues(str, x, y, z);
					for (int m = 0; m < i; m++)
					{
						str2 = instr[m];
						str3 = outstr[m];
						str4 = wirestr[m];
						str5 = regstr[m];
						s = varcheck(nx, ny, nz, x, y, z, str2, str3, str4, str5);
						//here = here + s;
						if ((nx > 0) && (ny > 0) && (nz > 0))
							break;
					}
					if ((nx == 0) || (ny == 0) || (nz == 0))
					{
						cout << endl
							<< " Missing Variable " << endl;
						break;
					}
					here = 0;
					nw = 0, nx = 0, ny = 0, nz = 0;
					for (int g = 0; g < i; g++)
					{
						strc = outstr[g];
						if (strc.find(z) < 20)
						{
							z_dw[1] = std::to_string(DW[g]);
							break;
						}
						strc = wirestr[g];
						if (strc.find(z) < 20)
						{
							z_dw[1] = std::to_string(DW[g]);
							break;
						}
						strc = regstr[g];
						if (strc.find(z) < 20)
						{
							z_dw[1] = std::to_string(DW[g]);
							break;
						}
					}
					count_DPC[1]++;							//count instances of ADD
					oline = oline + "ADD #(.DATAWIDTH(" + z_dw[1] + ")) ADD_" + std::to_string(count_DPC[1]) + "(.a(" + x + "), .b(" + y + "), .sum(" + z + ")); \n";
				}
			}
			found2 = iline.find(" - ");					//select SUB or DEC
			if (found2 != string::npos)
			{
				temp = 2;
				found2 = iline.find("- 1");
				if (found2 != string::npos)
				{
					str = iline;
					(x, y, z) = iovaluesreg(str, x, z);
					for (int m = 0; m < i; m++)
					{
						str2 = instr[m];
						str3 = outstr[m];
						str4 = wirestr[m];
						str5 = regstr[m];
						s = varcheck3(nx, nz, x, z, str2, str3, str4, str5);
						//here = here + s;
						if ((nx > 0) && (nz > 0))
							break;
					}
					if ((nx == 0) || (nz == 0))
					{
						cout << endl
							<< " Missing Variable " << endl;
						break;
					}
					here = 0;
					nw = 0, nx = 0, ny = 0, nz = 0;
					for (int g = 0; g < i; g++)
					{
						strc = outstr[g];
						if (strc.find(z) < 20)
						{
							z_dw[2] = std::to_string(DW[g]);
							break;
						}
						strc = wirestr[g];
						if (strc.find(z) < 20)
						{
							z_dw[2] = std::to_string(DW[g]);
							break;
						}
						strc = regstr[g];
						if (strc.find(z) < 20)
						{
							z_dw[2] = std::to_string(DW[g]);
							break;
						}
					}
					count_DPC[11]++;							//count instances of DEC
					oline = oline + "DEC #(.DATAWIDTH(" + z_dw[2] + ")) DEC_" + std::to_string(count_DPC[11]) + "(.a(" + x + "), .d(" + z + ")); \n";
				}
				else
				{
					str = iline;
					(x, y, z) = iovalues(str, x, y, z);
					for (int m = 0; m < i; m++)
					{
						str2 = instr[m];
						str3 = outstr[m];
						str4 = wirestr[m];
						str5 = regstr[m];
						s = varcheck(nx, ny, nz, x, y, z, str2, str3, str4, str5);
						//here = here + s;
						if ((nx > 0) && (ny > 0) && (nz > 0))
							break;
					}
					if ((nx == 0) || (ny == 0) || (nz == 0))
					{
						cout << endl
							<< " Missing Variable " << endl;
						break;
					}
					here = 0;
					nw = 0, nx = 0, ny = 0, nz = 0;
					for (int g = 0; g < i; g++)
					{
						strc = outstr[g];
						if (strc.find(z) < 20)
						{
							z_dw[3] = std::to_string(DW[g]);
							break;
						}
						strc = wirestr[g];
						if (strc.find(z) < 20)
						{
							z_dw[3] = std::to_string(DW[g]);
							break;
						}
						strc = regstr[g];
						if (strc.find(z) < 20)
						{
							z_dw[3] = std::to_string(DW[g]);
							break;
						}
					}
					count_DPC[2]++;							//count instances of SUB
					oline = oline + "SUB #(.DATAWIDTH(" + z_dw[3] + ")) SUB_" + std::to_string(count_DPC[2]) + "(.a(" + x + "), .b(" + y + "), .diff(" + z + ")); \n";
				}
			}
			found3 = iline.find(" * ");					//select MUL
			if (found3 != string::npos)
			{
				str = iline;
				(x, y, z) = iovalues(str, x, y, z);
				for (int m = 0; m < i; m++)
				{
					str2 = instr[m];
					str3 = outstr[m];
					str4 = wirestr[m];
					str5 = regstr[m];
					s = varcheck(nx, ny, nz, x, y, z, str2, str3, str4, str5);
					//here = here + s;
					if ((nx > 0) && (ny > 0) && (nz > 0))
						break;
				}
				if ((nx == 0) || (ny == 0) || (nz == 0))
				{
					cout << endl
						<< " Missing Variable " << endl;
					break;
				}
				here = 0;
				nw = 0, nx = 0, ny = 0, nz = 0;
				for (int g = 0; g < i; g++)
				{
					strc = outstr[g];
					if (strc.find(z) < 20)
					{
						z_dw[4] = std::to_string(DW[g]);
						break;
					}
					strc = wirestr[g];
					if (strc.find(z) < 20)
					{
						z_dw[4] = std::to_string(DW[g]);
						break;
					}
					strc = regstr[g];
					if (strc.find(z) < 20)
					{
						z_dw[4] = std::to_string(DW[g]);
						break;
					}
				}
				count_DPC[3]++;							//count instances of MUL
				oline = oline + "MUL #(.DATAWIDTH(" + z_dw[4] + ")) MUL_" + std::to_string(count_DPC[3]) + "(.a(" + x + "), .b(" + y + "), .prod(" + z + ")); \n";
				temp = 3;
			}
			found4 = iline.find(" / ");					//select DIV
			if (found4 != string::npos)
			{
				str = iline;
				(x, y, z) = iovalues(str, x, y, z);
				for (int m = 0; m < i; m++)
				{
					str2 = instr[m];
					str3 = outstr[m];
					str4 = wirestr[m];
					str5 = regstr[m];
					s = varcheck(nx, ny, nz, x, y, z, str2, str3, str4, str5);
					//here = here + s;
					if ((nx > 0) && (ny > 0) && (nz > 0))
						break;
				}
				if ((nx == 0) || (ny == 0) || (nz == 0))
				{
					cout << endl
						<< " Missing Variable " << endl;
					break;
				}
				here = 0;
				nw = 0, nx = 0, ny = 0, nz = 0;
				for (int g = 0; g < i; g++)
				{
					strc = outstr[g];
					if (strc.find(z) < 20)
					{
						z_dw[5] = std::to_string(DW[g]);
						break;
					}
					strc = wirestr[g];
					if (strc.find(z) < 20)
					{
						z_dw[5] = std::to_string(DW[g]);
						break;
					}
					strc = regstr[g];
					if (strc.find(z) < 20)
					{
						z_dw[5] = std::to_string(DW[g]);
						break;
					}
				}
				count_DPC[8]++;							//count instances of DIV
				oline = oline + "DIV #(.DATAWIDTH(" + z_dw[5] + ")) DIV_" + std::to_string(count_DPC[8]) + "(.a(" + x + "), .b(" + y + "), .quot(" + z + ")); \n";
				temp = 4;
			}
			found5 = iline.find(" % ");					//select MOD
			if (found5 != string::npos)
			{
				str = iline;
				(x, y, z) = iovalues(str, x, y, z);
				for (int m = 0; m < i; m++)
				{
					str2 = instr[m];
					str3 = outstr[m];
					str4 = wirestr[m];
					str5 = regstr[m];
					s = varcheck(nx, ny, nz, x, y, z, str2, str3, str4, str5);
					//here = here + s;
					if ((nx > 0) && (ny > 0) && (nz > 0))
						break;
				}
				if ((nx == 0) || (ny == 0) || (nz == 0))
				{
					cout << endl
						<< " Missing Variable " << endl;
					break;
				}
				here = 0;
				nw = 0, nx = 0, ny = 0, nz = 0;
				for (int g = 0; g < i; g++)
				{
					strc = outstr[g];
					if (strc.find(z) < 20)
					{
						z_dw[6] = std::to_string(DW[g]);
						break;
					}
					strc = wirestr[g];
					if (strc.find(z) < 20)
					{
						z_dw[6] = std::to_string(DW[g]);
						break;
					}
					strc = regstr[g];
					if (strc.find(z) < 20)
					{
						z_dw[6] = std::to_string(DW[g]);
						break;
					}
				}
				count_DPC[9]++;							//count instances of MOD
				oline = oline + "MOD #(.DATAWIDTH(" + z_dw[6] + ")) MOD_" + std::to_string(count_DPC[9]) + "(.a(" + x + "), .b(" + y + "), .rem(" + z + ")); \n";
				temp = 5;
			}
			found6 = iline.find(" << ");					//select shift left, SHL
			if (found6 != string::npos)
			{
				str = iline;
				(x, y, z) = iovaluesshift(str, x, y, z);
				for (int m = 0; m < i; m++)
				{
					str2 = instr[m];
					str3 = outstr[m];
					str4 = wirestr[m];
					str5 = regstr[m];
					s = varcheck(nx, ny, nz, x, y, z, str2, str3, str4, str5);
					//here = here + s;
					if ((nx > 0) && (ny > 0) && (nz > 0))
						break;
				}
				if ((nx == 0) || (ny == 0) || (nz == 0))
				{
					cout << endl
						<< " Missing Variable " << endl;
					break;
				}
				here = 0;
				nw = 0, nx = 0, ny = 0, nz = 0;
				for (int g = 0; g < i; g++)
				{
					strc = outstr[g];
					if (strc.find(z) < 20)
					{
						z_dw[7] = std::to_string(DW[g]);
						break;
					}
					strc = wirestr[g];
					if (strc.find(z) < 20)
					{
						z_dw[7] = std::to_string(DW[g]);
						break;
					}
					strc = regstr[g];
					if (strc.find(z) < 20)
					{
						z_dw[7] = std::to_string(DW[g]);
						break;
					}
				}
				count_DPC[7]++;							//count instances of SHL
				oline = oline + "SHL #(.DATAWIDTH(" + z_dw[7] + ")) SHL_" + std::to_string(count_DPC[7]) + "(.a(" + x + "), .sh_amt(" + y + "), .d(" + z + ")); \n";
				temp = 6;
			}
			found7 = iline.find(" >> ");					//select shift right, SHR
			if (found7 != string::npos)
			{
				str = iline;
				(x, y, z) = iovaluesshift(str, x, y, z);
				for (int m = 0; m < i; m++)
				{
					str2 = instr[m];
					str3 = outstr[m];
					str4 = wirestr[m];
					str5 = regstr[m];
					s = varcheck(nx, ny, nz, x, y, z, str2, str3, str4, str5);
					//here = here + s;
					if ((nx > 0) && (ny > 0) && (nz > 0))
						break;
				}
				if ((nx == 0) || (ny == 0) || (nz == 0))
				{
					cout << endl
						<< " Missing Variable " << endl;
					break;
				}
				here = 0;
				nw = 0, nx = 0, ny = 0, nz = 0;
				for (int g = 0; g < i; g++)
				{
					strc = outstr[g];
					if (strc.find(z) < 20)
					{
						z_dw[8] = std::to_string(DW[g]);
						break;
					}
					strc = wirestr[g];
					if (strc.find(z) < 20)
					{
						z_dw[8] = std::to_string(DW[g]);
						break;
					}
					strc = regstr[g];
					if (strc.find(z) < 20)
					{
						z_dw[8] = std::to_string(DW[g]);
						break;
					}
				}
				count_DPC[6]++;							//count instances of SHR
				oline = oline + "SHR #(.DATAWIDTH(" + z_dw[8] + ")) SHR_" + std::to_string(count_DPC[6]) + "(.a(" + x + "), .sh_amt(" + y + "), .d(" + z + ")); \n";
				temp = 7;
			}
			found8 = iline.find(" ? ");					//select MUX
			if (found8 != string::npos)
			{
				str = iline;
				(w, x, y, z) = iovaluesmux(str, w, x, y, z);
				for (int m = 0; m < i; m++)
				{
					str2 = instr[m];
					str3 = outstr[m];
					str4 = wirestr[m];
					str5 = regstr[m];
					s = varcheck2(nw, nx, ny, nz, w, x, y, z, str2, str3, str4, str5);
					//here = here + s;
					if ((nw > 0) && (nx > 0) && (ny > 0) && (nz > 0))
						break;
				}
				if ((nw == 0) || (nx == 0) || (ny == 0) || (nz == 0))
				{
					cout << endl
						<< " Missing Variable " << endl;
					break;
				}
				here = 0;
				nw = 0, nx = 0, ny = 0, nz = 0;
				for (int g = 0; g < i; g++)
				{
					strc = outstr[g];
					if (strc.find(z) < 20)
					{
						z_dw[9] = std::to_string(DW[g]);
						break;
					}
					strc = wirestr[g];
					if (strc.find(z) < 20)
					{
						z_dw[9] = std::to_string(DW[g]);
						break;
					}
					strc = regstr[g];
					if (strc.find(z) < 20)
					{
						z_dw[9] = std::to_string(DW[g]);
						break;
					}
				}
				count_DPC[5]++;							//count instances of MUX
				oline = oline + "MUX2x1 #(.DATAWIDTH(" + z_dw[9] + ")) MUX2x1_" + std::to_string(count_DPC[5]) + "(.a(" + x + "), .b(" + y + "), .sel(" + w + "), .d(" + z + ")); \n";
				temp = 8;
			}
			found9 = iline.find(" == ");					//select COMP, eq output
			if (found9 != string::npos)
			{
				str = iline;
				(x, y, z) = iovaluescomp(str, x, y, z);
				for (int m = 0; m < i; m++)
				{
					str2 = instr[m];
					str3 = outstr[m];
					str4 = wirestr[m];
					str5 = regstr[m];
					s = varcheck(nx, ny, nz, x, y, z, str2, str3, str4, str5);
					//here = here + s;
					if ((nx > 0) && (ny > 0) && (nz > 0))
						break;
				}
				if ((nx == 0) || (ny == 0) || (nz == 0))
				{
					cout << endl
						<< " Missing Variable " << endl;
					break;
				}
				here = 0;
				nw = 0, nx = 0, ny = 0, nz = 0;
				for (int g = 0; g < i; g++)
				{
					strc = outstr[g];
					if (strc.find(z) < 20)
					{
						z_dw[10] = std::to_string(DW[g]);
						break;
					}
					strc = wirestr[g];
					if (strc.find(z) < 20)
					{
						z_dw[10] = std::to_string(DW[g]);
						break;
					}
					strc = regstr[g];
					if (strc.find(z) < 20)
					{
						z_dw[10] = std::to_string(DW[g]);
						break;
					}
				}
				count_DPC[4]++;							//count instances of COMP
				oline = oline + "COMP #(.DATAWIDTH(" + z_dw[10] + ")) COMP_" + std::to_string(count_DPC[4]) + "(.a(" + x + "), .b(" + y + "), .eq(" + z + ")); \n";
				temp = 9;
			}
			found10 = iline.find(" < ");					//select COMP, lt output 
			if ((found10 != string::npos) && (temp != 6))
			{
				str = iline;
				(x, y, z) = iovaluescomp(str, x, y, z);
				for (int m = 0; m < i; m++)
				{
					str2 = instr[m];
					str3 = outstr[m];
					str4 = wirestr[m];
					str5 = regstr[m];
					s = varcheck(nx, ny, nz, x, y, z, str2, str3, str4, str5);
					//here = here + s;
					if ((nx > 0) && (ny > 0) && (nz > 0))
						break;
				}
				if ((nx == 0) || (ny == 0) || (nz == 0))
				{
					cout << endl
						<< " Missing Variable " << endl;
					break;
				}
				here = 0;
				nw = 0, nx = 0, ny = 0, nz = 0;
				for (int g = 0; g < i; g++)
				{
					strc = outstr[g];
					if (strc.find(z) < 20)
					{
						z_dw[11] = std::to_string(DW[g]);
						break;
					}
					strc = wirestr[g];
					if (strc.find(z) < 20)
					{
						z_dw[11] = std::to_string(DW[g]);
						break;
					}
					strc = regstr[g];
					if (strc.find(z) < 20)
					{
						z_dw[11] = std::to_string(DW[g]);
						break;
					}
				}
				count_DPC[4]++;							//count instances of COMP
				oline = oline + "COMP #(.DATAWIDTH(" + z_dw[11] + ")) COMP_" + std::to_string(count_DPC[4]) + "(.a(" + x + "), .b(" + y + "), .lt(" + z + ")); \n";
				temp = 10;
			}
			found11 = iline.find(" > ");					//select COMP, gt output
			if ((found11 != string::npos) && (temp != 7))
			{
				str = iline;
				(x, y, z) = iovaluescomp(str, x, y, z);
				for (int m = 0; m < i; m++)
				{
					str2 = instr[m];
					str3 = outstr[m];
					str4 = wirestr[m];
					str5 = regstr[m];
					s = varcheck(nx, ny, nz, x, y, z, str2, str3, str4, str5);
					//here = here + s;
					if ((nx > 0) && (ny > 0) && (nz > 0))
						break;
				}
				if ((nx == 0) || (ny == 0) || (nz == 0))
				{
					cout << endl
						<< " Missing Variable " << endl;
					break;
				}
				here = 0;
				nw = 0, nx = 0, ny = 0, nz = 0;
				for (int g = 0; g < i; g++)
				{
					strc = outstr[g];
					if (strc.find(z) < 20)
					{
						z_dw[12] = std::to_string(DW[g]);
						break;
					}
					strc = wirestr[g];
					if (strc.find(z) < 20)
					{
						z_dw[12] = std::to_string(DW[g]);
						break;
					}
					strc = regstr[g];
					if (strc.find(z) < 20)
					{
						z_dw[12] = std::to_string(DW[g]);
						break;
					}
				}
				count_DPC[4]++;							//count instances of COMP
				oline = oline + "COMP #(.DATAWIDTH(" + z_dw[12] + ")) COMP_" + std::to_string(count_DPC[4]) + "(.a(" + x + "), .b(" + y + "), .gt(" + z + ")); \n";
				temp = 11;
			}
			if ((found != string::npos) && (temp == 0))		//select REG
			{
				str = iline;
				(x, z) = iovaluesreg(str, x, z);
				for (int m = 0; m < i; m++)
				{
					str2 = instr[m];
					str3 = outstr[m];
					str4 = wirestr[m];
					str5 = regstr[m];
					s = varcheck3(nx, nz, x, z, str2, str3, str4, str5);
					//here = here + s;
					if ((nx > 0) && (nz > 0))
						break;
				}
				if ((nx == 0) || (nz == 0))
				{
					cout << endl
						<< " Missing Variable " << endl;
					break;
				}
				here = 0;
				nw = 0, nx = 0, ny = 0, nz = 0;
				for (int g = 0; g < i; g++)
				{
					strc = outstr[g];
					if (strc.find(z) < 20)
					{
						strv = std::to_string(DW[g]);						
						break;
					}
					strc = wirestr[g];
					if (strc.find(z) < 20)
					{
						strv = std::to_string(DW[g]);
						break;
					}
					strc = regstr[g];
					if (strc.find(z) < 20)
					{
						strv = std::to_string(DW[g]);
						break;
					}
				}
				error = opcheck(newline, x, z);
				if (error == 1)
				{
					cout << endl
						<< " Invalid Operation" << endl;
					break;
				}					
				else
				{
					count_DPC[0]++;							//count instances of REG
					oline = oline + "REG #(.DATAWIDTH(" + strv + ")) REG_" + std::to_string(count_DPC[0]) + "(.d(" + x + "), .Clk(1), .Rst(0), .q(" + z + ")); \n";
				}				
			}			
		}
	}
	myfile2 << oline << '\n';
	myfile2 << "endmodule" << '\n';
	myfile3.close();
	myfile2.close();

	return 0;
}

