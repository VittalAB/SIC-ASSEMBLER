/*
date : - 26/10/21
coder : - VittalAB
*/

// Generating tokens from instructions

#include <iostream>
#include <fstream>
#include <string>
#include <bits/stdc++.h>
#include <cstring>

using namespace std;

class tokens
{
public:
    vector<string> label;
    vector<string> opcode; // class for label,opcode,operand,loc
    vector<string> operand;
    vector<string> loc;
} tk;

class operror
{
public:
    string msg;

    operror(string str)
    {
        msg = str;
    }

    string error()
    {
        return msg;
    }
};

class symerror
{
public:
    string msg;

    symerror(string str)
    {
        msg = str;
    }

    string error()
    {
        return msg;
    }
};

/*Symbol table data structure*/

typedef map<string, string> symy;
typedef map<string, string> oppy;

symy symbol_table;
oppy opcode_table;

int loca = 0;
int starting_address = 0;
string program_name;


string int_to_hex(int);
int hex_to_int(string);
bool is_valid_opcode(string);
void get_opcode(string);
void validate_symtab();


fstream IRfile;

void get_tokenize(string tp)
{ // function to generate tokens
    vector<string> words;
    string word = "";

    int within_qoute = 0;

    for (auto x : tp)
    {
        if (x == 39)
        {
            within_qoute = 1;
        }
        if ((x == ' ' || x == '\n') && !within_qoute)
        {
            words.push_back(word);
            word = "";
        }
        else if ((x == ' ' || x == '\n') && within_qoute)
        {
            word = word + x;
        }
        else
        {
            word = word + x;
        }
    }
    words.push_back(word);

    if (words.size() == 2)
    {

        if (!is_valid_opcode(words[0]) && words[0] != "START")
        {
            throw operror("invalid opcode " + words[0] + " !");
        }
        if (words[0] != "START")
        {
            tk.opcode.push_back(words[0]);
            tk.operand.push_back(words[1]);

            string loca1 = int_to_hex(loca);

            if(loca1.size()==1){
                loca1 = "000"+loca1; 
            }else if(loca1.size()==2){
                loca1 = "00"+loca1; 
            }else if(loca1.size()==3){
                loca1 = "0"+loca1; 
            }

            tk.loc.push_back((loca1));

            IRfile << loca1 << " " << words[0] + " " + words[1] << endl;

            loca = loca + 3;
        }
        else
        {
            loca = hex_to_int(words[1]);
            starting_address = loca;

            return;
        }
    }
    else if (words.size() == 3)
    {

        if (!is_valid_opcode(words[1]) && words[1] != "RESB" && words[1] != "BYTE" && words[1] != "WORD" && words[1] != "RESW" && words[1] != "START" && words[1]!="EQU")
        {
            throw operror("invalid opcode !");
        }

        if (words[1] != "START")
        {

            tk.label.push_back(words[0]);
            tk.opcode.push_back(words[1]);
            tk.operand.push_back(words[2]);

            string loca1 = int_to_hex(loca);

            if(loca1.size()==1){
                loca1 = "000"+loca1; 
            }else if(loca1.size()==2){
                loca1 = "00"+loca1; 
            }else if(loca1.size()==3){
                loca1 = "0"+loca1; 
            }
            if(words[1]!="EQU")
            symbol_table[loca1] = words[0];
            else{
                symbol_table[words[2]] = words[0];
            }

            // if(words[0]=="LENGTH"){
            //     cout << "Length processesd " << loca1 << endl;
            // cout << loca1 << " " << words[0] + " " + words[1] + " " + words[2] << endl;
            // exit(0);
            // }

            tk.loc.push_back((loca1));

            IRfile << loca1 << " " << words[0] + " " + words[1] + " " + words[2] << endl;


            if (words[1] == "WORD")
            {
                loca = loca + 3;
            }
            else if (words[1] == "BYTE")
            {
                if (words[2].at(0) == 'X')
                {
                    loca += (words[2].size() - 3) / 2;
                }
                else
                {
                    loca = loca + words[2].size() - 3;
                }
            }
            else if (words[1] == "RESB")
            {
                int disp = stoi(words[2]);
                loca = loca + disp;
            }
            else if (words[1] == "RESW")
            {
                loca = loca + stoi(words[2]) * 3;
            }
            else
            {
                loca += 3;
            }
        }
        else
        {
            loca = hex_to_int(words[2]);
            starting_address = loca;
            program_name = words[0];

            return;
        }
    }
    else
    {
        if(words[0]!="END"){
        tk.opcode.push_back(words[0]);
        string word = words[0];

        string loca1 = int_to_hex(loca);
        if(loca1.size()==1){
                loca1 = "000"+loca1; 
            }else if(loca1.size()==2){
                loca1 = "00"+loca1; 
            }else if(loca1.size()==3){
                loca1 = "0"+loca1; 
            }
        tk.loc.push_back((loca1));

        if (word == "FIX" || word == "FLOAT" || word == "NORM" || word == "SIO" || word == "TIO")
        {
            loca = loca + 1;
        }
        else
        {
            IRfile << loca1 << " " << words[0] << endl;
            loca += 3;
        }
        }
    }
    return;
}

string int_to_hex(int my_integer)
{
    std::stringstream sstream;
    sstream << std::hex << my_integer;
    std::string result = sstream.str();
    return result;
}

int hex_to_int(string my_hex)
{
    unsigned int x;
    std::stringstream ss;
    ss << std::hex << my_hex;
    ss >> x;
    return x;
}

void get_opcode(string op)
{

    vector<string> words;
    string word = "";

    // int within_qoute = 0;

    for (auto x : op)
    {
        // if(x == 39){
        //     // within_qoute = 1;
        // }
        if ((x == ' ' || x == '\n'))
        {
            words.push_back(word);
            word = "";
        }
        else if ((x == ' ' || x == '\n'))
        {
            word = word + x;
        }
        else
        {
            word = word + x;
        }
    }
    words.push_back(word);

    opcode_table[words[0]] = words[1];
}

bool is_valid_opcode(string op)
{

    oppy::iterator itr;
    int found = 0;

    for (itr = opcode_table.begin(); itr != opcode_table.end(); itr++)
    {
        if (itr->first == op)
        {
            found = 1;
        }
    }

    if (found)
    {
        return true;
    }
    else
    {
        return false;
    }
}


void validate_symtab(){
    vector<string>labels;
    labels = tk.label;
    bool isduplicate=false;

    typedef map<string,int> arry;
    arry arr;

    for(int i=0;i<labels.size();i++){
        arr[labels[i]]++;
        // cout << labels[i]  << endl;
        if(arr[labels[i]] > 1){
            isduplicate=true;

        }
    }

    if(isduplicate){
        throw symerror("Duplicate Symbols Found ! ");
    }
}



int main()
{
    fstream newfile, output, optab, symtab , startadd, prglength , prgname;

    IRfile.open("Intermediate_File_of_Pass_1.txt", ios::out);

    symtab.open("symtab.txt", ios::out);

    newfile.open("input2.txt", ios::in);

    output.open("output1.txt", ios::out);

    optab.open("optab.txt", ios::in);

    cout << "\n\nGiven Assembly Language : - \n\n"
         << endl;
    output << "\n\nGiven Assembly Language : - \n\n"
           << endl;

    int itr1 = 0;

    // Getting opcode table ready

    if (optab.is_open())
    {
        string op;

        while (getline(optab, op))
        {
            get_opcode(op);
        }

        optab.close();
    }

    if (newfile.is_open())
    {
        string tp;
        while (getline(newfile, tp))
        {
            cout << tp << "\n";
            output << tp << "\n";
            itr1++;

            try
            {
                get_tokenize(tp);
            }
            catch (operror e)
            {
                cout << e.error() << endl;
            }
        }
        newfile.close();
    }
    // cout << "this is it ! re baba";
    //    cout << tk.operand[5] << endl << endl;

    cout << "\n ----------------------------------------------------------------------------------------------------\n";
    output << "\n ----------------------------------------------------------------------------------------------------\n";
    cout << "Opcode are : - {";
    output << "Opcode are : - {";

    for (int i = 0; i < tk.opcode.size(); i++)
    {
        cout << tk.opcode[i] + " , ";
        output << tk.opcode[i] << ", ";
    }
    cout << "}";
    output << "}";

    cout << "\n";
    output << "\n";

    cout << "\n ----------------------------------------------------------------------------------------------------\n";
    output << "\n ----------------------------------------------------------------------------------------------------\n";
    cout << "Labels are :- {";
    output << "Labels are :- {";

    for (int i = 0; i < tk.label.size(); i++)
    {
        cout << tk.label[i] + " , ";
        output << tk.label[i] << ", ";
    }
    cout << " }";
    output << " }";
    cout << "\n";
    output << "\n";

    cout << "\n ----------------------------------------------------------------------------------------------------\n";
    output << "\n ----------------------------------------------------------------------------------------------------\n";
    cout << "Operand are :-  { ";
    output << "Operand are :-  { ";
    for (int i = 0; i < tk.operand.size(); i++)
    {
        cout << tk.operand[i] + " , ";
        output << tk.operand[i] << ", ";
    }

    cout << "}\n";
    output << "}\n";

    cout << "\n ----------------------------------------------------------------------------------------------------\n";
    output << "\n ----------------------------------------------------------------------------------------------------\n";
    cout << "Locations are : -  { ";
    output << "Locations are : -  { ";
    for (int i = 0; i < tk.loc.size(); i++)
    {
        cout << tk.loc[i] + " , ";
        output << tk.loc[i] << ", ";
    }
    cout << " }\n";
    output << " }\n";
    cout << "\n ----------------------------------------------------------------------------------------------------\n";
    output << "\n ----------------------------------------------------------------------------------------------------\n";

    cout << "Symbol table : - " << endl
         << endl
         << endl;
    output << "Symbol table : - " << endl
           << endl
           << endl;
    symy::iterator itr;
    cout << "Symbol\t\t\t"
         << "Value" << endl;
    output << "Symbol\t\t\t"
           << "Value" << endl;

    for (itr = symbol_table.begin(); itr != symbol_table.end(); itr++)
    {

        output << itr->first << "\t<-------\t" << itr->second << endl;
        cout << itr->second << "\t<-------\t" << itr->first << endl;
        symtab << itr->first << " " << itr->second << endl;
    }

    cout << "\n";

    try{
        validate_symtab();
    }
    catch(symerror s){
        cout << s.error() << endl;
    }

    cout << endl << endl << endl;

    string start;

    start=int_to_hex(starting_address);

    if(start.size()==1){
        start="00000"+start;
    }else if(start.size()==2){
        start="0000"+start;
    }else if(start.size()==3){
        start="000"+start;
    }else if(start.size()==4){
        start="00"+start;
    }else if(start.size()==5){
        start="0"+start;
    }

    cout << "Starting address :- " << start << endl;

    string program_length;

    program_length = int_to_hex(loca - starting_address);

    if(program_length.size()==1){
        program_length="00000"+program_length;
    }else if(program_length.size()==2){
        program_length="0000"+program_length;
    }else if(program_length.size()==3){
        program_length="000"+program_length;
    }else if(program_length.size()==4){
        program_length="00"+program_length;
    }else{
        program_length="0"+program_length;
    }

    cout << "Program length :- " << program_length << endl;

    IRfile << "END" << endl;

    IRfile.close();

    output.close();

    startadd.open("startadd.txt",ios::out);
    startadd << int_to_hex(starting_address) << endl;
    startadd.close();

    prglength.open("prglength.txt",ios::out);
    prglength << int_to_hex(loca-starting_address) << endl;
    prglength.close();


    prgname.open("prgname.txt",ios::out);
    prgname << program_name << endl;
    
    return 0;
}