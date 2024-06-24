#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<sstream>
using namespace std;


string op_TAB[] = {
		"ADD", "ADDF", "ADDR", "AND", "CLEAR", "COMP", "COMPF", "COMPR", "DIV", "DIVF", "DIVR",
		"FIX", "FLOAT", "HIO", "J", "JEQ", "JGT", "JLT", "JSUB", "LDA", "LDB", "LDCH", "LDF", "LDL", "LDS",
		"LDT", "LDX", "LPS", "UML", "MULF", "MULR", "NORM", "OR", "RD", "RMO", "RSUB", "SHIFTL", "SHIFTR",
		"SIO", "SSK", "STA", "STB", "STCH", "STF", "STI", "STL", "STS", "STSW", "STT", "STX", "SUB", "SUBF",
		"SUBR", "SVC", "TD", "TIO", "TIX", "TIXR", "WD", 
		};
string opCode[] = {
		"18", "58", "90", "40", "B4", "28", "88", "A0", "24", "64", "9C", "C4", "C0", "F4", "3C",
		"30", "34", "38", "48", "00", "68", "50", "70", "08", "6C", "74", "04", "E0", "20", "60", 
		"98", "C8",	"44", "D8", "AC", "4C", "A4", "A8", "F0", "EC", "0C", "78", "54", "80", "D4",
		"14", "7C", "E8", "84",	"10", "1C", "5C", "94", "B0", "E0", "F8", "2C", "B8", "DC",
		};
void temp(string s, vector<string>& buf)
{
	int current = 0; //最初由 0 的位置開始找
	int next;
	
	while (1)
	{
		next = s.find_first_of(" ", current);
		if (next != current)
		{
			string tmp = s.substr(current, next - current);
			if (tmp.size() != 0) //忽略空字串
				buf.push_back(tmp);
		}
		if (next == string::npos) break;
		current = next + 1; //下次由 next + 1 的位置開始找起。
	}
	
}

int main()
{
	
	ifstream in;
	in.open("source.txt");
	
	if (!in) {

		cout << "fall" << endl;
		return 0;

	}
	int n = 0;
	string file;
	string file_copy = "";
	string buffer[1000];
	while (getline(in,file))
	{
		file_copy = file;
		buffer[n] = file_copy;
		n++;
	}
	//cout << file_copy << endl;
	in.close();
	//cout << op_TAB[1] << " " << opCode[1] << " <--test";
	//cout << buffer[0] <<endl;
	//cout << buffer[1][0]<<endl;
	/*for (int i = 0; i < n; i++)
	{
		cout << buffer[i] << endl;
	}*/
	bool start_objectcode = false;
	vector<string> buf,ObjectCode,Label;
	vector<int>ObjectLoction, LabelInt, ObjectCode_back,WordOBJC;
	string start_adress;
	int test_start_adress = pow(16,3)*4;
	for (int num = 0; num < n; num++)
	{
		
		temp(buffer[num], buf);
		for (int i = 0; i < buf.size(); i++)
		{
			if (buf[i] == "START")
			{
				start_adress = buf[i + 1];
			}
			if (buf[i] == "FIRST")
			{
				start_objectcode = true;
			}
			if (buf[i] == "END")
			{
				start_objectcode = false;
			}
			if (buf[i] == "WORD")
			{
				int val;				 
				istringstream is(buf[i+1]);
				is >> val;
				WordOBJC.push_back(val);
			}
			if (start_objectcode)
			{
				for(int optab_size=0 ; optab_size < 59 ; optab_size++)
					if (buf[i] == op_TAB[optab_size])
					{
						ObjectCode.push_back(opCode[optab_size]);
					}
				
			}
		}
		if (buf.size() == 3)
		{
			Label.push_back(buf[0]);
			LabelInt.push_back(test_start_adress);
		}
		ObjectLoction.push_back(test_start_adress);
		test_start_adress += 3;
		

		buf.clear();
		
	}
	/*for (int temp : LabelInt)
		cout << std::hex<< temp << " " << endl;*/
	/*for (string temp : Label)
		cout << temp << " " << endl;*/
	for (int num = 0; num < n; num++)
	{
		temp(buffer[num], buf);
		for (int i = 0; i < buf.size(); i++)
		{			
			for (int optab_size = 0; optab_size < 59; optab_size++)
			{
				if (buf[i] == op_TAB[optab_size])
				{
					if (buf[i] == "RSUB")
					{
						ObjectCode_back.push_back(0);
					}
					else
						for (int j = 0; j < Label.size(); j++)
						{
							if (buf[i + 1] == Label[j])
							{
								ObjectCode_back.push_back(LabelInt[j]);
							}
						}

				}
					
			}		
		}
		buf.clear();
	}
	

	cout << std::hex << ObjectLoction[0] << "\t" << buffer[0] << "\t" << endl;
	int distance = 0;
	for(int i = 0; i < ObjectCode.size(); i++)
	{
		
		if (ObjectCode_back[i] == 0)
		{
			break;
		}
		cout << std::hex<< ObjectLoction[i]<<"\t"<< buffer[i+1] <<"\t"<<ObjectCode[i] << std::hex << ObjectCode_back[i] << endl;
		distance++;
	}
	cout << std::hex << ObjectLoction[distance] << "\t" << buffer[distance+1] << "\t\t" << ObjectCode[distance] +"0000" << endl;
	int wordnum = 0;
	bool wordchk = true;
	for (int i = distance+2; i < n; i++)
	{
		buf.clear();
		wordchk = true;
		temp(buffer[i], buf);
		for (int j = 0; j < buf.size(); j++)
		{
			/*if (buf[j] == "WORD")
			{
				cout << std::hex << ObjectLoction[i-1] << "\t" << buffer[i+1]  <<WordOBJC[wordnum]<<"\t" <<endl;
				
				wordchk = false;
				break;
			
			}			*/
			if (buf[j] == "WORD")
			{				
				cout << std::hex << ObjectLoction[i]<< "\t" << buffer[i] << "\t\t0000"<<WordOBJC[wordnum]  << endl;
				wordnum++;
				wordchk = false;
				break;
			}
			
		}	

		if (wordchk)
		{
			cout << std::hex << ObjectLoction[i] << "\t " << buffer[i] << endl ;		

		}
		
		
		buf.clear();

	}
	

	

	
	
	
	/*for(string temp : ObjectCode)
		cout << temp << " \n";
	for (int temp : ObjectCode_back)
		cout << temp << " \n";*/
	/*for (int temp : ObjectLoction)
		printf("%x \n", temp);
	for (string temp : ObjectCode)
		cout << temp << " \n";*/
	//cout << start_adress;

















	
	system("PAUSE");
}