#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;
class VM
{
private:
    char buffer[40];
    char Memory[100][4];
    char IR[4];
    char R[4];
    // bool C;
    int IC;
    int SI;
    fstream infile;
    ofstream outfile;

    void init()
    {
        fill(buffer, buffer + sizeof(buffer), '\0');
        fill(&Memory[0][0], &Memory[0][0] + sizeof(Memory), '\0');
        fill(IR, IR + sizeof(IR), '\0');
        fill(R, R + sizeof(R), '\0');
        IC = 0;
        // C = true;
        SI = 0;
    }

    void resetBuffer()
    {
        fill(buffer, buffer + sizeof(buffer), '\0');
    }

    void MOS()
    {
        cout << "\nPrinting IR : \n";
        for (char x : IR)
        {
            cout << x << " ";
        }
        switch (SI)
        {
        case 1:
            READ();
            break;

        case 2:
            WRITE();
            break;

        case 3:
            TERMINATE();
            break;
        }
        SI = 0;
    }
    void LOAD()
    {

        if (infile.is_open())
        {
            string s;
            while (getline(infile, s))
            {
                if (s[0] == '$' && s[1] == 'A' && s[2] == 'M' && s[3] == 'J')
                {
                    init();
                    cout << "\nNew Job started\n";
                }
                else if (s[0] == '$' && s[1] == 'D' && s[2] == 'T' && s[3] == 'A')
                {
                    cout << "Data card loding\n";
                    resetBuffer();
                    STARTEXE();
                }

                else if (s[0] == '$' && s[1] == 'E' && s[2] == 'N' && s[3] == 'D')
                {
                    cout << "END of Job\n";
                }

                else
                {

                    cout << "Program Card loding\n";
                    int length = s.size();
                    resetBuffer();

                    for (int i = 0; i < length; i++)
                    {
                        buffer[i] = s[i];
                    }

                    int buff = 0;
                    int ref = 0;

                    while (buff < 40 && buffer[buff] != '\0') // Untill Buffer is empty
                    {
                        for (int j = 0; j < 4; j++)
                        {
                            if (buffer[buff] == 'H')
                            {
                                Memory[IC][j] = 'H';
                                buff++;
                                break;
                            }
                            Memory[IC][j] = buffer[buff];
                            buff++;
                        }
                        IC++;
                    }
                }
                for (int i = 0; i < 100; i++)
                {
                    cout << "M[" << i << "]\t";
                    for (int j = 0; j < 4; j++)
                    {
                        cout << Memory[i][j];
                    }
                    cout << endl;
                }
            }
            infile.close();
        }
    }

    void STARTEXE()
    {
        IC = 0;
        EXECUTEUSERPROGRAM();
    }

    void READ()
    {
        cout << "Read function called\n";

        string data;
        getline(infile, data);
        int len = data.size();
        for (int i = 0; i < len; i++)
        {
            buffer[i] = data[i]; // fills buffer from input file 
        }
        cout<<"\nIR[2] "<<IR[2]<<" "<<" IR[2]-'0' "<<IR[2]-'0'<<"\n";
        int buff = 0, mem_ptr = (IR[2] - '0') * 10; // Mem_ptr= adress from IR 
        while (buff < 40 && buffer[buff] != '\0')
        {
            for (int i = 0; i < 4; i++)
            {
                Memory[mem_ptr][i] = buffer[buff];
                buff++;
            }
            mem_ptr++;
        }
        resetBuffer();
    }

    void WRITE()
    {
        cout << "Write function called\n";
        outfile.open("output.txt", ios::app);

        for (int i = (IR[2] - '0') * 10; i < (IR[2] - '0' + 1) * 10; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                if (Memory[i][j] != '\0')
                {
                    outfile << Memory[i][j];
                }
            }
        }
        outfile << "\n";
        outfile.close();
    }

    void TERMINATE()
    {
        outfile.open("output.txt", ios::app);
        cout << "Terminate called\n\n";
        outfile << "\n\n";
        outfile.close();
    }

    void EXECUTEUSERPROGRAM()
    {
        while (IC < 99 && Memory[IC][0] != '\0')
        {
            cout<<"In ExeUserProg: IC :-  "<<IC<<"\n";

            for (int i = 0; i < 4; i++)
            {
                IR[i] = Memory[IC][i];
            }
            IC++;

            if (IR[0] == 'G' && IR[1] == 'D')
            {
                SI = 1;
                MOS();
            }

            else if (IR[0] == 'P' && IR[1] == 'D')
            {
                SI = 2;
                MOS();
            }

            else if (IR[0] == 'H')
            {
                SI = 3;
                MOS();
                return;
            }
        }
    }

public:
    VM()
    {
        infile.open("./input.txt", ios::in);
        init();
        LOAD();
    }
};
int main()
{
    VM v;
    return 0;
}
