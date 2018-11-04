// Assignment2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

//global variables

string ins_list = string();		//comma delimited list of inputs to the circuit
int circuit_clocks = 0;			//number of clock periods required to schedule this circuit

//global structure contains attributes of Data Path Components (DPC)
struct dp_comp {				//Data Path Component (DPC) structure with attributes for DPC
    int function;				//enumeration of the components ( REG = 0, ADD = 1, SUB = 2, etc)
    int order;					//order as received from text file
    int top_order;				//topological order used to find critical datapath
    string out_line;			//output line to be sent to output file (verilog file)
    int d_width;				//datpath width
    float latency;				//value from estimated latency table, used to find critical datapath
    int i_clock;				//clock latency number
    string dp_ins_str;			//comma delimited list of inputs
    string dp_ins[2];
    string dp_outs_str;			//comma delimited list of outputs
    string dp_outs[3];
};								//Data Path Component (DPC) structure with DPC attributes
struct dp_comp dpc_list[20];	//create array of above structure

//global structure contains attributes of the Circuit
struct cir_desc {			//circuit description
    string inp_str;			//input string
    string ins[4];			//array of inputs to the circuit
    string inp_type;		//Int or UInt
    string out_str;
    string outs[4];			//array of outputs of the circuit
    string wires[4];
    string reg[4];
};
struct cir_desc cir_list;

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
    string ops[11] = { "+", "-", "*", "/", "%", "<", ">", "==", "<<", ">>", "?" };
    h = newline.find(" ");
    newline = newline.substr(h + 1);
    h = newline.find(" ");
    newline = newline.substr(h + 1);
    op = newline.substr(0, (newline.find(" ")));
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

void parse_cir_inputs(void)
{
    string temp_str = string();
    int p = 0;
    int findLoc = 0;
    int beg_pos = 0;
    int end_pos = 0;
    int index = 0;
    string var_str = string();

    temp_str = cir_list.inp_str;
    temp_str = temp_str + '\0';			//terminate with NULL

    end_pos = 0;
    p = -1;
    do {
        p++;
        while( ((temp_str[end_pos] == ' ') || (temp_str[end_pos] == ',')) && (temp_str[end_pos]))
        {
            end_pos++;			//step over spaces and comma to eliminate from variable
        }
        beg_pos = end_pos++;
        while (temp_str[end_pos] != ',' && (temp_str[end_pos]))
        {
            ++end_pos;
        }
        cir_list.ins[p] = temp_str.substr(beg_pos, end_pos - beg_pos);
    } while (temp_str[end_pos] && p < 4);
}

//routine to find schedule, that is populate the .i_clock value of the structure for each DPC
void get_schedule(void)
{
    int p = 0;								//counter used in for loop
    int n = 0;
    int k = 0;
    int inp_idx;							//index for inputs arrays
    int dpc_idx;							//index for data path components arrays
    int i_clock = 1;						//clock periods used for scheduling
    int count_dpc_scheduled = 0;			//count the Data Path Components (DPC) scheduled
    string ready_inputs[15];				//array of inputs that are ready, allowing datapaths to be scheduled
    string dpc_inputs[12][10];				//DPC inputs [dpc_list.order][inp_index]
    string dpc_outputs[12][10];				//DPC inputs [dpc_list.order][inp_index]
    string temp_str = string();				//temporary string
    int beg_pos = 0;						//string index
    int end_pos = 0;
    int count_dpc = 0;						//count the components
    int ready_ins_count;					//number of inputs in ready_inputs list
    int dpc_item_count = 0;					//number of DPC in circuit
    int dpc_schd = 0;						//count dpc scheduled
    int rdy_idx = 0;
    int inp_found = 0;						//flag inputs as found in ready input list

    parse_cir_inputs();

    for (p = 0; p < 3; ++p) {
        cout << cir_list.ins[p] << endl;
    }
    for (p = 0; p < 3; ++p) {
        ready_inputs[p] = cir_list.ins[p];		//copy circuit inpputs
        cout << ready_inputs[p] << ", ";
    }
    cout << endl;

    for (p = 4; p < 15; p++) {
        ready_inputs[p] = string();		//empty string
    }

    ready_ins_count = 0;
    do {
        ready_ins_count++;				//count the number of ready inpuuts
    } while (ready_inputs[ready_ins_count] != "\0");
    cout << "ready inputs:  " << ready_ins_count << endl;

    //DPC that can run in first clock period have inputs in the circuit inputs list
    dpc_idx = 0;

    while (dpc_list[dpc_idx].order) {			//check every dpc against the circuit inputs
        if ((dpc_list[dpc_idx].dp_ins[0] == cir_list.ins[0]) || (dpc_list[dpc_idx].dp_ins[0] == cir_list.ins[1]) ||
            (dpc_list[dpc_idx].dp_ins[0] == cir_list.ins[2]) || (dpc_list[dpc_idx].dp_ins[3] == cir_list.ins[3]))
        {
            dpc_list[dpc_idx].i_clock = i_clock;			//schedule the dpc
            //cout << ".i_clock  " << dpc_idx << ": " << dpc_list[p].i_clock << endl;
            dpc_schd++;
            p = 0;											//add datapath outputs to ready_ouputs array
        }
        dpc_idx++;
    }
    dpc_item_count = dpc_idx;				//capture the number of DPC

    dpc_idx = 0;
    do {		//add outputs to ready_inputs
        if (dpc_list[dpc_idx].i_clock == 1)
        {
            p = 0;
            do {											//all dpc have at least 1 input
                ready_inputs[ready_ins_count] = dpc_list[dpc_idx].dp_outs[p];
                p++;
                ready_ins_count++;
            } while (!dpc_list[dpc_idx].dp_outs[p].empty());					//dpc_list[dpc_idx].dp_outs[p]);
        }
        dpc_idx++;
    } while (dpc_list[dpc_idx].order);

    //check every DPC against the ready inputs list; then increment the clock
    //dpc_idx = 0;
    do {						//clock loop
        i_clock++;				//advance to next clock period
        dpc_idx = 0;
        do {										//dpc loop
            if (!dpc_list[dpc_idx].i_clock)			//if this DPC is NOT scheduled, compare inputs to ready_inputs
            {
                inp_idx = 0;
                do {								//dpc inputs loop
                    rdy_idx = 0;
                    inp_found = 0;
                    do {							//ready inputs loop
                        if (dpc_list[dpc_idx].dp_ins[inp_idx] == ready_inputs[rdy_idx]) {
                            inp_found = 1;
                        }
                        else {
                            rdy_idx++;
                        }
                    } while (!inp_found && !ready_inputs[rdy_idx].empty());		//until input is found or no more ready inputs
                    inp_idx++;						//advance to next input
                } while (inp_found && !dpc_list[dpc_idx].dp_ins[inp_idx].empty());			//until all inputs are checked or an input is not found
                //if ( inp_found && !dpc_list[dpc_idx].dp_ins[inp_idx].empty() )
                if (inp_found)
                {
                    dpc_list[dpc_idx].i_clock = i_clock;			//schedule the DPC for this clock period
                    dpc_schd++;										//count dpc scheduled
                }
            }  //if (!dpc_list[dpc_idx].i_clock)
            dpc_idx++;			//advance to next dpc
        } while (dpc_list[dpc_idx].order);							//.order is zero if array is empty

        dpc_idx = 0;
        do {		//add outputs to ready_inputs
            if (dpc_list[dpc_idx].i_clock == i_clock)
            {
                p = 0;
                do {											//all dpc have at least 1 input
                    ready_inputs[ready_ins_count] = dpc_list[dpc_idx].dp_outs[p];
                    p++;
                    ready_ins_count++;
                } while (!dpc_list[dpc_idx].dp_outs[p].empty());					//dpc_list[dpc_idx].dp_outs[p]);
            }
            dpc_idx++;
        } while (dpc_list[dpc_idx].order);

    } while ((dpc_schd < dpc_item_count) && (i_clock < 11));			//repeat until all components have been scheduled

    circuit_clocks = i_clock;			//number of clock cycles required for this circuit
    //for (p = 0; p < dpc_item_count; p++) {
    //cout << "Component #: " << p << "  " << "Period" << dpc_list[p].i_clock << endl;
    //}
}


//routine to add estimated latency of each DPC, that is populate the .latency of the structure for each DPC
void get_est_lat(void)
{
    int p;		//used as counter in for loop

    float est_lat_tab[12][6] = {							//  [datawidth] [ component]  estimated latency table from assignment
            { 2.616, 2.644, 2.879, 3.061, 3.602, 3.966},		//reg
            { 2.704, 3.713, 4.924, 5.638, 7.270, 9.566},		//add
            { 3.024, 3.412, 4.890, 5.569, 7.253, 9.566},		//sub
            { 2.438, 3.651, 7.453, 7.811, 12.395, 15.354},		//mul
            { 3.031, 3.934, 5.949, 6.256, 7.264, 8.416},		//comp
            { 4.083, 4.115, 4.815, 5.623, 8.079, 8.766},		//mux
            { 3.644, 4.007, 5.178, 6.460, 8.819, 11.095},		//shr
            { 3.614, 3.980, 5.152, 6.549, 8.565, 11.220},		//shl
            { 0.619, 2.144, 15.439, 33.093, 89.312, 243.233},	//div
            { 0.758, 2.149, 16.078, 35.563, 88.142, 250.583},	//mod
            { 1.792, 2.218, 3.111, 3.471, 4.347, 6.200},		//inc
            { 1.792, 2.218, 3.108, 3.701, 4.685, 6.503}, };		//dec

    for (p = 0; dpc_list[p].function != 99; p++) {
        dpc_list[p].latency = est_lat_tab[dpc_list[p].function][dpc_list[p].d_width];
        //cout << ".schedule " << p << dpc_list[p].i_clock;
    }
}

//routine to calculate the Critical Data Path
//Using the schedule stored in dpc_list[].i_clock find the maximum latency for each clock period and take the sum
float calc_cr_dp(void)
{
    int clock_count = 0;			//count the clock period, used in for loop
    int p = 0;						//used to index for loops
    float cr_dp = 0.0;				//critical datapath
    float max_lat[15];				//maximum latecncy for each clock period

    //initialize the max_lat array
    for (p = 0; p < 15; p++)
    {
        max_lat[p] = 0.0;
    }
    //get the maximum latency for each clock period
    for (clock_count = 0; clock_count < circuit_clocks; clock_count++)
    {
        //check every DPC
        for (p = 0; dpc_list[p].function != 99; p++)
        {
            if ((dpc_list[p].i_clock == clock_count) && ((max_lat[p]) < (dpc_list[p].latency)))
            {
                max_lat[clock_count] = dpc_list[p].latency;
            }
        }
    }
    // sum the maximum latency values
    for (p = 0; p < 15; p++)
    {
        cr_dp = cr_dp + max_lat[p];
    }
    return cr_dp;
}

int main(int argc, char *argv[]) {
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
    int sum_count_DPC = 0;		//count the datapath components, which is sum of count_DPC[]
    int c_period_req = 0;		//number of clock periods required by schedule
    float cr_dp = 0.0;			//critical data path

//initialize the cir_list array
    for (i = 0; i < 4; i++) {
        cir_list.ins[i] = "\0";		//array of inputs to the circuit
        cir_list.outs[i] = string();		//array of outputs of the circuit
        cir_list.wires[i] = string();
        cir_list.reg[i] = string();
    }
    cir_list.inp_str = "\0";


//initailize the dpc_list array
    for (i = 0; i < 20; i++) {
        dpc_list[i].function = 99;		//component enumeration uses 0 thru 12
        dpc_list[i].order = 0;
        dpc_list[i].top_order = 0;
        dpc_list[i].out_line = string();
        dpc_list[i].d_width = 0;
        dpc_list[i].latency = 0.0;
        dpc_list[i].i_clock = 0;
        dpc_list[i].dp_ins[0] = string();
        dpc_list[i].dp_ins[1] = string();
        dpc_list[i].dp_ins_str = string();
        dpc_list[i].dp_outs[0] = string();
        dpc_list[i].dp_outs[1] = string();
        dpc_list[i].dp_outs[2] = string();
        dpc_list[i].dp_outs_str = string();
    }
    i = 0;

    if (argc == 3) {
        filename1 = string(argv[1]);
        filename2 = string(argv[2]);
    } else {
        cerr << "Program Terminated: Invalid number of arguments";
        return 1;
    }
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
            if ((foundname1 = iline.find("UInt")) != string::npos) {
                iline = iline.substr(foundname1 + 4);
                cir_list.inp_type = "UInt";
            }
            if ((foundname1 = iline.find("Int")) != string::npos) {
                iline = iline.substr(foundname1 + 3);
                cir_list.inp_type = "UInt";
            }
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
            instr[m] = iline.substr(foundname2+1);
            m++;
            DW[i] = bittemp;
            i++;
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
            i++;
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
            i++;
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
            i++;
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
    oline = "module " + filename1 + " #(parameter DATAWIDTH = " + std::to_string(bitsize) + ")(" + str + "); \n";
    myfile2 << oline;

    /**************
    This for loop block creates declartions of inputs, outputs, wires and registers
    ***************/
    int inpt_count = 0;
    int outpt_count = 0;
    int wire_count = 0;
    int reg_count = 0;

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
            cir_list.inp_str = str;
            inpt_count++;
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
            cir_list.out_str = str;
            outpt_count++;
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
            cir_list.wires[wire_count] = str;
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
            cir_list.reg[reg_count] = str;
            reg_count++;
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
    int count_dpc = 0;					//count the datapath components found
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
                    dpc_list[count_dpc].dp_ins[0] = x;
                    dpc_list[count_dpc].dp_ins[1] = x;
                    dpc_list[count_dpc].dp_outs[0] = z;
                    dpc_list[sum_count_DPC].function = 10;
                    dpc_list[sum_count_DPC].order = sum_count_DPC+1;
                    dpc_list[sum_count_DPC].top_order = sum_count_DPC+1;
                    //dpc_list[sum_count_DPC].latency = std::stod(strv);
                    dpc_list[sum_count_DPC].out_line = oline;
                    dpc_list[sum_count_DPC].d_width = std::stoi(z_dw[0]);
                    sum_count_DPC++;
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
                    dpc_list[sum_count_DPC].dp_ins[0] = x;
                    dpc_list[sum_count_DPC].dp_ins[1] = y;
                    dpc_list[sum_count_DPC].dp_outs[0] = z;
                    dpc_list[sum_count_DPC].function = 1;
                    dpc_list[sum_count_DPC].order = sum_count_DPC+1;
                    dpc_list[sum_count_DPC].top_order = sum_count_DPC+1;
                    //dpc_list[sum_count_DPC].latency = std::stof(strv);
                    dpc_list[sum_count_DPC].out_line = oline;
                    dpc_list[sum_count_DPC].d_width = std::stoi(z_dw[1]);
                    sum_count_DPC++;
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
                    dpc_list[sum_count_DPC].dp_ins[0] = x;
                    //dpc_list[count_dpc].dp_ins[1] = y;
                    dpc_list[sum_count_DPC].dp_outs[0] = z;
                    dpc_list[sum_count_DPC].function = 11;
                    dpc_list[sum_count_DPC].order = sum_count_DPC+1;
                    dpc_list[sum_count_DPC].top_order = sum_count_DPC+1;
                    //dpc_list[sum_count_DPC].latency = std::stof(strv);
                    dpc_list[sum_count_DPC].out_line = oline;
                    dpc_list[sum_count_DPC].d_width = std::stoi(z_dw[2]);
                    sum_count_DPC++;

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
                    dpc_list[sum_count_DPC].dp_ins[0] = x;
                    dpc_list[sum_count_DPC].dp_ins[1] = y;
                    dpc_list[sum_count_DPC].dp_outs[0] = z;
                    dpc_list[sum_count_DPC].function = 2;
                    dpc_list[sum_count_DPC].order = sum_count_DPC+1;
                    dpc_list[sum_count_DPC].top_order = sum_count_DPC+1;
                    //dpc_list[sum_count_DPC].latency = std::stof(strv);
                    dpc_list[sum_count_DPC].out_line = oline;
                    dpc_list[sum_count_DPC].d_width = std::stoi(z_dw[3]);
                    sum_count_DPC++;

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
                dpc_list[sum_count_DPC].dp_ins[0] = x;
                dpc_list[sum_count_DPC].dp_ins[1] = y;
                dpc_list[sum_count_DPC].dp_outs[0] = z;
                dpc_list[sum_count_DPC].function = 3;
                dpc_list[sum_count_DPC].order = sum_count_DPC+1;
                dpc_list[sum_count_DPC].top_order = sum_count_DPC+1;
                //dpc_list[sum_count_DPC].latency = std::stof(strv);
                dpc_list[sum_count_DPC].out_line = oline;
                dpc_list[sum_count_DPC].d_width = std::stoi(z_dw[4]);
                sum_count_DPC++;

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
                dpc_list[sum_count_DPC].dp_ins[0] = x;
                dpc_list[sum_count_DPC].dp_ins[1] = y;
                dpc_list[sum_count_DPC].dp_outs[0] = z;
                dpc_list[sum_count_DPC].function = 8;
                dpc_list[sum_count_DPC].order = sum_count_DPC+1;
                dpc_list[sum_count_DPC].top_order = sum_count_DPC+1;
                //dpc_list[sum_count_DPC].latency = std::stod(strv);
                dpc_list[sum_count_DPC].out_line = oline;
                dpc_list[sum_count_DPC].d_width = std::stoi(z_dw[5]);
                sum_count_DPC++;

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
                dpc_list[sum_count_DPC].dp_ins[0] = x;
                dpc_list[sum_count_DPC].dp_ins[1] = y;
                dpc_list[sum_count_DPC].dp_outs[0] = z;
                dpc_list[sum_count_DPC].function = 9;
                dpc_list[sum_count_DPC].order = sum_count_DPC+1;
                dpc_list[sum_count_DPC].top_order = sum_count_DPC+1;
                //dpc_list[sum_count_DPC].latency = std::stof(strv);
                dpc_list[sum_count_DPC].out_line = oline;
                dpc_list[sum_count_DPC].d_width = std::stoi(z_dw[6]);
                sum_count_DPC++;
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
                dpc_list[sum_count_DPC].dp_ins[0] = x;
                dpc_list[sum_count_DPC].dp_ins[1] = y;
                dpc_list[sum_count_DPC].dp_outs[0] = z;
                dpc_list[sum_count_DPC].function = 7;
                dpc_list[sum_count_DPC].order = sum_count_DPC+1;
                dpc_list[sum_count_DPC].top_order = sum_count_DPC+1;
                //dpc_list[sum_count_DPC].latency = std::stof(strv);
                dpc_list[sum_count_DPC].out_line = oline;
                dpc_list[sum_count_DPC].d_width = std::stoi(z_dw[7]);
                sum_count_DPC++;
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
                dpc_list[sum_count_DPC].dp_ins[0] = x;
                dpc_list[sum_count_DPC].dp_ins[1] = y;
                dpc_list[sum_count_DPC].dp_outs[0] = z;
                dpc_list[sum_count_DPC].function = 6;
                dpc_list[sum_count_DPC].order = sum_count_DPC+1;
                dpc_list[sum_count_DPC].top_order = sum_count_DPC+1;
                //dpc_list[sum_count_DPC].latency = std::stof(strv);
                dpc_list[sum_count_DPC].out_line = oline;
                dpc_list[sum_count_DPC].d_width = std::stoi(z_dw[8]);
                sum_count_DPC++;
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
                dpc_list[sum_count_DPC].dp_ins[0] = x;
                dpc_list[sum_count_DPC].dp_ins[1] = y;
                dpc_list[sum_count_DPC].dp_ins[2] = w;
                dpc_list[sum_count_DPC].dp_outs[0] = z;
                dpc_list[sum_count_DPC].function = 5;
                dpc_list[sum_count_DPC].order = sum_count_DPC+1;
                dpc_list[sum_count_DPC].top_order = sum_count_DPC+1;
                //dpc_list[sum_count_DPC].latency = std::stof(strv);
                dpc_list[sum_count_DPC].out_line = oline;
                dpc_list[sum_count_DPC].d_width = std::stoi(z_dw[9]);
                sum_count_DPC++;
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
                dpc_list[sum_count_DPC].dp_ins[0] = x;
                dpc_list[sum_count_DPC].dp_ins[1] = y;
                dpc_list[sum_count_DPC].dp_outs[0] = z;
                dpc_list[sum_count_DPC].function = 4;
                dpc_list[sum_count_DPC].order = sum_count_DPC+1;
                dpc_list[sum_count_DPC].top_order = sum_count_DPC+1;
                //dpc_list[sum_count_DPC].latency = std::stof(strv);
                dpc_list[sum_count_DPC].out_line = oline;
                dpc_list[sum_count_DPC].d_width = std::stoi(z_dw[10]);
                sum_count_DPC++;

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
                dpc_list[sum_count_DPC].dp_ins[0] = x;
                dpc_list[sum_count_DPC].dp_ins[1] = y;
                dpc_list[sum_count_DPC].dp_outs[0] = z;
                dpc_list[sum_count_DPC].function = 4;
                dpc_list[sum_count_DPC].order = sum_count_DPC+1;
                dpc_list[sum_count_DPC].top_order = sum_count_DPC+1;
                //dpc_list[sum_count_DPC].latency = std::stof(strv);
                dpc_list[sum_count_DPC].out_line = oline;
                dpc_list[sum_count_DPC].d_width = std::stoi(z_dw[11]);
                sum_count_DPC++;

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
                dpc_list[sum_count_DPC].dp_ins[0] = x;
                dpc_list[sum_count_DPC].dp_ins[1] = y;
                dpc_list[sum_count_DPC].dp_outs[0] = z;
                dpc_list[sum_count_DPC].function = 4;
                dpc_list[sum_count_DPC].order = sum_count_DPC+1;
                dpc_list[sum_count_DPC].top_order = sum_count_DPC+1;
                //dpc_list[sum_count_DPC].latency = std::stof(strv);
                dpc_list[sum_count_DPC].out_line = oline;
                dpc_list[sum_count_DPC].d_width = std::stoi(z_dw[12]);
                sum_count_DPC++;
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
                    dpc_list[sum_count_DPC].dp_ins[0] = x;
                    dpc_list[sum_count_DPC].dp_outs[0] = z;
                    dpc_list[sum_count_DPC].function = 0;
                    dpc_list[sum_count_DPC].order = sum_count_DPC+1;
                    dpc_list[sum_count_DPC].top_order = sum_count_DPC+1;
                    //dpc_list[sum_count_DPC].latency = std::stof(strv);
                    dpc_list[sum_count_DPC].out_line = oline;
                    dpc_list[sum_count_DPC].d_width = std::stoi(strv);
                    sum_count_DPC++;
                }
            }
            count_dpc++;
        }
    }
    myfile2 << oline << '\n';
    myfile2 << "endmodule" << '\n';
    get_schedule();							//create schedule, calculates circuit_clocks
    get_est_lat();							//populate .latentcy in structure
    cr_dp = calc_cr_dp();					//calculate the critical data path
    myfile2 << endl << endl;
    myfile2 << "//Critical Path : " + std::to_string(cr_dp) << endl;		//print critical data path to output file
    cout << endl << "Critical Path : " << cr_dp << endl << endl;

    myfile3.close();
    myfile2.close();

    return 0;
}
