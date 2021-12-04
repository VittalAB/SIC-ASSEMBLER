#include <bits/stdc++.h>
#include "merge.h"

using namespace std;

class tokens
{
public:
    vector<string> label;
    vector<string> opcode; // class for label,opcode,operand,loc
    vector<string> operand;
    vector<string> object_code;
} tk;

int is_symbol_present(string);

typedef map<string, string> symy;
typedef map<string, string> oppy;
typedef map<string, string> hashy;

symy symbol_table;
oppy opcode_table;
hashy xhash;

fstream loc_opcode;

void generate_text_record();

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

string program_name, program_length, starting_address;

void get_header_record(string name, string length, string start)
{
    fstream header_rec;
    header_rec.open("header_record.txt", ios::out);
    if (name.size() == 1)
    {
        name += "-----";
    }
    else if (name.size() == 2)
    {
        name += "----";
    }
    else if (name.size() == 3)
    {
        name += "---";
    }
    else if (name.size() == 4)
    {
        name += "--";
    }
    else if (name.size() == 5)
    {
        name += "-";
    }
    if (start.size() == 1)
    {
        start = "00000" + start;
    }
    else if (start.size() == 2)
    {
        start = "0000" + start;
    }
    else if (start.size() == 3)
    {
        start = "000" + start;
    }
    else if (start.size() == 4)
    {
        start = "00" + start;
    }
    else if (start.size() == 5)
    {
        start = "0" + start;
    }
    if (length.size() == 1)
    {
        length = "00000" + length;
    }
    else if (length.size() == 2)
    {
        length = "0000" + length;
    }
    else if (length.size() == 3)
    {
        length = "000" + length;
    }
    else if (length.size() == 4)
    {
        length = "00" + length;
    }
    else if (length.size() == 5)
    {
        length = "0" + length;
    }
    header_rec << "H^" + name + "^" + start + "^" + length << endl;
}

void process_header_file()
{
    fstream prgname, prglength, startadd;

    prgname.open("prgname.txt", ios::in);
    prglength.open("prglength.txt", ios::in);
    startadd.open("startadd.txt", ios::in);

    if (prgname.is_open())
    {
        string line;
        while (getline(prgname, line))
        {
            program_name = line;
        }
        prgname.close();
    }
    if (prglength.is_open())
    {
        string line;
        while (getline(prglength, line))
        {
            program_length = line;
        }
        prglength.close();
    }
    if (startadd.is_open())
    {
        string line;
        while (getline(startadd, line))
        {
            starting_address = line;
        }
        startadd.close();
    }

    get_header_record(program_name, program_length, starting_address);
}

void process_end_file()
{
    fstream endfile;
    endfile.open("end_record.txt", ios::out);
    if (starting_address.size() == 1)
    {
        starting_address = "00000" + starting_address;
    }
    if (starting_address.size() == 2)
    {
        starting_address = "0000" + starting_address;
    }
    if (starting_address.size() == 3)
    {
        starting_address = "000" + starting_address;
    }
    if (starting_address.size() == 4)
    {
        starting_address = "00" + starting_address;
    }
    else if (starting_address.size() == 5)
    {
        starting_address = "0" + starting_address;
    }
    endfile << "E^" + starting_address << endl;
}

void get_opcode(string op)
{
    vector<string> words;
    string word = "";

    for (auto x : op)
    {
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

void process_opcode()
{
    fstream optab;
    optab.open("optab.txt", ios::in);
    if (optab.is_open())
    {
        string op;

        while (getline(optab, op))
        {
            get_opcode(op);
        }

        optab.close();
    }
}

void get_symbols(string op)
{
    vector<string> words;
    string word = "";

    for (auto x : op)
    {
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
    symbol_table[words[1]] = words[0];
}

void process_symbol()
{
    fstream symtab;
    symtab.open("symtab.txt", ios::in);
    if (symtab.is_open())
    {
        string op;

        while (getline(symtab, op))
        {
            get_symbols(op);
        }

        symtab.close();
    }
}
string get_binary(string hexa)
{
    long int i = 0;
    string ans;

    while (hexa[i])
    {
        switch (hexa[i])
        {
        case '0':
            ans += "0000";
            break;
        case '1':
            ans += "0001";
            break;
        case '2':
            ans += "0010";
            break;
        case '3':
            ans += "0011";
            break;
        case '4':
            ans += "0100";
            break;
        case '5':
            ans += "0101";
            break;
        case '6':
            ans += "0110";
            break;
        case '7':
            ans += "0111";
            break;
        case '8':
            ans += "1000";
            break;
        case '9':
            ans += "1001";
            break;
        case 'A':
        case 'a':
            ans += "1010";
            break;
        case 'B':
        case 'b':
            ans += "1011";
            break;
        case 'C':
        case 'c':
            ans += "1100";
            break;
        case 'D':
        case 'd':
            ans += "1101";
            break;
        case 'E':
        case 'e':
            ans += "1110";
            break;
        case 'F':
        case 'f':
            ans += "1111";
            break;
        default:
            // ans+= "lease enter valid hexadecimal digit "<< hexa[i];
            cout << "error";
        }
        i++;
    }
    return ans;
}

string solve(int num)
{
    long int hexa_val = 0, i = 1, r;
    while (num != 0)
    {
        r = num % 10;
        hexa_val = hexa_val + r * i;
        i = i * 2;
        num = num / 10;
    }
    if (hexa_val == 10)
    {
        return "a";
    }
    else if (hexa_val == 11)
    {
        return "b";
    }
    else if (hexa_val == 12)
    {
        return "c";
    }
    else if (hexa_val == 13)
    {
        return "d";
    }
    else if (hexa_val == 14)
    {
        return "e";
    }
    else if (hexa_val == 15)
    {
        return "f";
    }
    else
    {
        return to_string(hexa_val);
    }
}

string process_binary(string bin)
{

    string p1, p2, p3, p4, p5, p6;

    p1 = p1 + bin[0] + bin[1] + bin[2] + bin[3];
    p2 = p2 + bin[4] + bin[5] + bin[6] + bin[7];

    bin[8] = '1';

    p3 = p3 + bin[8] + bin[9] + bin[10] + bin[11];
    p4 = p4 + bin[12] + bin[13] + bin[14] + bin[15];
    p5 = p5 + bin[16] + bin[17] + bin[18] + bin[19];
    p6 = p6 + bin[20] + bin[21] + bin[22] + bin[23];

    string hex;
    hex = hex + (solve(stoi(p1)));
    hex = hex + (solve(stoi(p2)));
    hex = hex + (solve(stoi(p3)));
    hex = hex + (solve(stoi(p4)));
    hex = hex + (solve(stoi(p5)));
    // cout << "final 54101a" << p1 << '\n' << p2<< '\n' << p3<< '\n' << p4<< '\n' << p5<< '\n' << p6 << '\n';
    hex = hex + (solve(stoi(p6)));
    return hex;
}
string process_index_operand(string objcode)
{

    int num = hex_to_int(objcode);
    num += 32768;

    string bin = get_binary(objcode);
    return process_binary(bin);
}

int is_symbol_present(string sym)
{
    symy::iterator itr;
    bool found = false;
    for (itr = symbol_table.begin(); itr != symbol_table.end(); itr++)
    {
        if (itr->first == sym)
            found = true;
    }
    if (!found)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

void get_tokens(string tp)
{
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
        if (words[0] != "START")
        {
            tk.opcode.push_back(words[0]);
            tk.operand.push_back(words[1]);

            if (words[1].at(words[1].size() - 1) != 'X')
            {
                string objcode;

                if (is_symbol_present(words[1]))
                {
                    objcode = objcode + opcode_table[words[0]] + symbol_table[words[1]];
                }
                else
                {
                    cout << "# undefined symbol " + words[1] + "!!!" << endl;
                    objcode = objcode + opcode_table[words[0]] + "0000";
                }

                tk.object_code.push_back(objcode);
            }
            else if (words[1].at(words[1].size() - 1) == 'X')
            {
                string operand;
                operand = words[1];
                operand.pop_back();
                operand.pop_back();
                string objcode;

                if (is_symbol_present(operand))
                    objcode = objcode + opcode_table[words[0]] + symbol_table[operand];
                else
                {
                    objcode = objcode + opcode_table[words[0]] + "0000";
                    cout << "# undefined symbol " + operand + "!!!" << endl;
                }
                objcode = process_index_operand(objcode);
                tk.object_code.push_back(objcode);
            }
        }
    }
    else if (words.size() == 3)
    {

        if (words[1] != "START")
        {

            tk.label.push_back(words[0]);
            tk.opcode.push_back(words[1]);
            tk.operand.push_back(words[2]);

            if (words[2].at(words[2].size() - 1) != 'X' && words[1] != "RESW" && words[1] != "RESB" && words[1] != "WORD" && words[1] != "BYTE" && words[1]!="EQU")
            {
                string objcode;

                if (is_symbol_present(words[2]))
                    objcode = objcode + opcode_table[words[1]] + symbol_table[words[2]];
                else
                {
                    objcode = objcode + opcode_table[words[1]] + "0000";
                    cout << "# undefined symbol " + words[2] + "!!!" << endl;
                }
                tk.object_code.push_back(objcode);
            }
            else if (words[2].at(words[2].size() - 1) != 'X' && words[1] == "WORD")
            {
                string temp;
                temp = int_to_hex(stoi(words[2]));
                if(temp.size()==1){
                    temp = "00000" + temp;
                }else if(temp.size()==2){
                    temp = "0000" + temp;
                }else if(temp.size()==3){
                    temp = "000"+temp;
                }else if(temp.size()==4){
                    temp = "00" + temp;
                }else if(temp.size()==5){
                    temp = "0" + temp;
                }
                tk.object_code.push_back(temp);
            }
            else if (words[2].at(words[2].size() - 1) != 'X' && words[1] == "BYTE")
            {
                if (words[2].at(0) != 'X')
                {
                    string operand = words[2];
                    operand.pop_back();
                    reverse(operand.begin(), operand.end());
                    operand.pop_back();
                    operand.pop_back();
                    reverse(operand.begin(), operand.end());
                    // cout << operand << endl;

                    string objcode;

                    for (int i = 0; i < operand.size(); i++)
                    {
                        objcode = objcode + int_to_hex((int)operand[i]);
                    }
                    tk.object_code.push_back(objcode);
                }
                else
                {
                    string objcode = words[2];
                    objcode.pop_back();
                    reverse(objcode.begin(), objcode.end());
                    objcode.pop_back();
                    objcode.pop_back();
                    reverse(objcode.begin(), objcode.end());
                    tk.object_code.push_back(objcode);
                }
            }
            else if (words[2].at(words[2].size() - 1) == 'X')
            {
                string operand;
                operand = words[2];
                operand.pop_back();
                operand.pop_back();
                string objcode;
                objcode = objcode + opcode_table[words[1]] + symbol_table[operand];
                objcode = process_index_operand(objcode);

                string temp;
                temp = objcode;

                if(temp.size()==1){
                    temp = "00000" + temp;
                }else if(temp.size()==2){
                    temp = "0000" + temp;
                }else if(temp.size()==3){
                    temp = "000"+temp;
                }else if(temp.size()==4){
                    temp = "00" + temp;
                }else if(temp.size()==5){
                    temp = "0" + temp;
                }

                tk.object_code.push_back(temp);
            }
            else if (words[1] == "RESW")
            {
                tk.object_code.push_back("--");
            }
            else if (words[1] == "RESB")
            {
                tk.object_code.push_back("--");
            }
            else if (words[1]=="EQU"){
                tk.object_code.push_back("--");
            }
        }
    }
    else
    {
        if (words[0] != "END")
        {
            tk.opcode.push_back(words[0]);
            string objcode;
            objcode = objcode + opcode_table[words[0]];
            if(objcode.size()==2){
                objcode+="0000";
            }
            tk.object_code.push_back(objcode);
        }
    }
    return;
}

int main()
{

    // Pass_1();

    loc_opcode.open("Location_Opcode.txt", ios::out);

    process_header_file();
    cout << "Header process finished ! \n";
    process_end_file();
    cout << "End process finifshed ! \n";
    process_opcode();
    cout << "Opcode initialized from pass 1 ! \n";
    process_symbol();
    cout << "Symbol table initialized from pass 1 ! \n";

    fstream in, irfile, new_ir;
    irfile.open("Intermediate_File_of_Pass_1.txt", ios::in);
    // new_ir.open("updated_irfile.txt",ios::out);

    in.open("input2.txt", ios::in);

    if (in.is_open())
    {
        string tp;
        while (getline(in, tp))
        {
            get_tokens(tp);
        }
        in.close();
    }

    vector<string> objcode;
    objcode = tk.object_code;
    cout << "ALP With Object Code > " << endl
         << endl;

    if (irfile.is_open())
    {
        string tp;

        cout << "Location "
             << "  "
             << "Stmts"
             << "        "
             << "Object Code \n\n";

        for (int i = 0; i < objcode.size(); i++)
        {
            getline(irfile, tp);
            string ocode;
            ocode = objcode.at(i);

            if (ocode.size() == 5)
            {
                ocode = "0" + ocode;
            }
            else if (ocode.size() == 4)
            {
                ocode = "00" + ocode;
            }
            else if (ocode.size() == 3)
            {
                ocode = "000" + ocode;
            }
            else if (ocode.size() == 2 && ocode != "--")
            {
                ocode = "0000" + ocode;
            }
            else if (ocode.size() == 1)
            {
                ocode = "00000" + ocode;
            }
            else if (ocode == "--")
            {
                ocode = "------";
            }
            cout << ocode + " ---> " + tp << endl;
            // new_ir << tp + "---->" + objcode.at(i) << endl;
            string location;
            location = location + tp[0] + tp[1] + tp[2] + tp[3];
            xhash[location] = objcode.at(i);
        }
    }

    // hashy::iterator itr;
    // for(itr=xhash.begin();itr!=xhash.end();itr++){
    //     cout << itr->first << itr->second << endl;
    //}

    hashy::iterator kk;

    // generate_text_record();

    void xgenerate_text_record();

    xgenerate_text_record();

    loc_opcode.close();

    void write_to_loca_opcode();
    
    write_to_loca_opcode();

    merge();

    return 0;
}

void write_to_loca_opcode(){
    fstream loca_opcode;
    loca_opcode.open("Location_Opcode.txt",ios::out);

    string opcode;
    hashy::iterator itr;
    for(itr=xhash.begin();itr!=xhash.end();itr++){
        opcode = itr->second;

        if(opcode.size()==2){
            opcode = "0000"+opcode;
        }

        if(itr->second != "--")
        loca_opcode << itr->first << " " <<  itr->second << endl;
     }

     loca_opcode.close();

}

int get_last_inst_length()
{
    hashy::iterator itr;
    int ans = 0;
    for (itr = xhash.begin(); itr != xhash.end(); itr++)
    {
        ans = hex_to_int(itr->first);
    }
    // cout << hex_to_int(program_length)+hex_to_int(starting_address) << ans << endl;
    return hex_to_int(program_length) + hex_to_int(starting_address) - ans;
}

void xgenerate_text_record()
{

    vector<string> locs;
    vector<string> opcodes;
    stack<int> s;
    fstream t_record;

    t_record.open("t_record.txt", ios::out);

    hashy::iterator itr, p_itr;
    int size = 0;
    string start = starting_address;
    // cout << "start address " << starting_address << endl;

    for (itr = xhash.begin(); itr != xhash.end(); itr++)
    {

        s.push(size);

        size = hex_to_int(itr->first) - hex_to_int(start);

        if (size < 30 && itr->second != "--")
        {
            // size=hex_to_int(itr->first)-hex_to_int(start);
            locs.push_back(itr->first);
            opcodes.push_back(itr->second);
            s.push(size);
        }
        else if (size == 30)
        {
            t_record << "T^" + start + "^" + int_to_hex(size);
            for (int i = 0; i < opcodes.size(); i++)
            {
                if (opcodes[i].size() == 4)
                {
                    opcodes[i] = "00" + opcodes[i];
                }
                else if (opcodes[i].size() == 3)
                {
                    opcodes[i] = "000" + opcodes[i];
                }
                else if (opcodes[i].size() == 2)
                {
                    opcodes[i] = "0000" + opcodes[i];
                }
                else if (opcodes[i].size() == 1)
                {
                    opcodes[i] = "00000" + opcodes[i];
                }
                t_record << "^" + opcodes[i];
            }
            t_record << "\n";
            start = itr->first;
            size = 0;
            opcodes.clear();
            opcodes.push_back(itr->second);
        }
        else if (itr->second == "--")
        {

            t_record << "T^" + start + "^" + int_to_hex(size);
            for (int i = 0; i < opcodes.size(); i++)
            {
                if (opcodes[i].size() == 4)
                {
                    opcodes[i] = "00" + opcodes[i];
                }
                else if (opcodes[i].size() == 3)
                {
                    opcodes[i] = "000" + opcodes[i];
                }
                else if (opcodes[i].size() == 2)
                {
                    opcodes[i] = "0000" + opcodes[i];
                }
                else if (opcodes[i].size() == 1)
                {
                    opcodes[i] = "00000" + opcodes[i];
                }
                t_record << "^" + opcodes[i];
            }
            t_record << "\n";
            size = 0;
            opcodes.clear();

            while (itr->second == "--")
            {
                p_itr = itr;
                itr++;
            }
            start = itr->first;
            opcodes.push_back(itr->second);
        }
        else if (size > 30)
        {
            t_record << "T^" + start + "^" + int_to_hex(s.top());

            for (int i = 0; i < opcodes.size() - 1; i++)
            {

                if (opcodes[i].size() == 4)
                {
                    opcodes[i] = "00" + opcodes[i];
                }
                else if (opcodes[i].size() == 3)
                {
                    opcodes[i] = "000" + opcodes[i];
                }
                else if (opcodes[i].size() == 2)
                {
                    opcodes[i] = "0000" + opcodes[i];
                }
                else if (opcodes[i].size() == 1)
                {
                    opcodes[i] = "00000" + opcodes[i];
                }

                t_record << "^" + opcodes[i];
            }

            t_record << "\n";
            itr--;
            start = itr->first;
            // cout << start << endl;
            itr++;
            s.push(size);
            size = 0;
            opcodes.clear();
            itr--;
            // itr--;
            opcodes.push_back(itr->second);
            // itr++;
            itr++;
            opcodes.push_back(itr->second);
        }
    }
    size = hex_to_int(itr->first) - hex_to_int(start);
    // cout <<  "final" << start;
    int last_size = get_last_inst_length();

    // cout << size+last_size << endl;
    // cout << s.top()+last_size << endl;

    int eof = hex_to_int(program_length) + hex_to_int(starting_address);

    if (s.top() + last_size <= 30)
    {
        t_record << "T^" + start + "^" + int_to_hex(s.top() + last_size);
        // cout << s.top() << endl;
        for (int i = 0; i < opcodes.size(); i++)
        {
            if (opcodes[i].size() == 4)
            {
                opcodes[i] = "00" + opcodes[i];
            }
            else if (opcodes[i].size() == 3)
            {
                opcodes[i] = "000" + opcodes[i];
            }
            else if (opcodes[i].size() == 2)
            {
                opcodes[i] = "0000" + opcodes[i];
            }
            else if (opcodes[i].size() == 1)
            {
                opcodes[i] = "00000" + opcodes[i];
            }
            t_record << "^" + opcodes[i];
        }
    }
    else
    {
    }
    // else{
    //     opcodes.clear();

    //     hashy::iterator it,pt;
    //     it=xhash.begin();
    //     while(it->first!=start){
    //         it++;
    //     }

    //     xhash[int_to_hex(eof)]="EOF";
    //     while(!s.empty()) {s.pop()};

    //     while(it!=xhash.end()){
    //         pt=it;
    //         size = hex_to_int(++pt->first)-hex_to_int(it->first);
    //         if(size<30){
    //             opcodes.push_back(it->second);
    //             t_record << "T^"++"^"+int_to_hex(s.top()+last_size);
    //             s.push(size);
    //         }else{
    //             // cout << s.top() << endl;
    //             for(int i=0;i<opcodes.size();i++){
    //             t_record << "^"+opcodes[i];
    // }
    //             opcodes.clear();

    //             size=0;

    //         }
    //     }

    // }
    t_record.close();
}
