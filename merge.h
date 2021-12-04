#include<bits/stdc++.h>
using namespace std;

int merge(){
    fstream h_file,t_file,e_file,obj_program;

    h_file.open("header_record.txt",ios::in);
    t_file.open("t_record.txt",ios::in);
    e_file.open("end_record.txt",ios::in);
    obj_program.open("object_code.txt",ios::out);

    if (h_file.is_open())
    {
        string op;

        while (getline(h_file, op))
        {
            obj_program << op << endl;
        }

        h_file.close();
    }
    if (t_file.is_open())
    {
        string op;

        while (getline(t_file, op))
        {
            obj_program << op << endl;
        }

        t_file.close();
    }
    if (e_file.is_open())
    {
        string op;

        while (getline(e_file, op))
        {
            obj_program << op << endl;
        }

        e_file.close();
    }

    obj_program.close();

    return 0;
}