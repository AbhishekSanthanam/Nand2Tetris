#include<iostream>
#include<string>
#include<fstream>
#include<algorithm>
using namespace std;
int noofparse = 0;
void initialise(string *fname)
{
	size_t it = fname->find(".");
	fname->erase(it);
}
string parse(string str)  //Function for Parsing
{
	noofparse++;
	string p = str;
	p.erase(remove_if(p.begin(), p.end(), ::isspace), p.end());
	size_t it = p.find("//"); size_t end = p.length(); string t;
	if (it == string::npos)
	{
		return p;
	}
	else if (it != string::npos)
	{
		//cout<<it<<"is the index of comment ";
		p.erase(it, end);
		//cout<<p;cout<<"Returning null\n";
		return p;
	}
}
int findins(string str) // Function to find what kinda instruction
{
	if (str.size() == 0)
		return 0;
	if ((str.find("add") == 0) || (str.find("sub") == 0) || (str.find("and") == 0) || (str.find("or") == 0) || (str.find("not") == 0) || (str.find("neg") == 0) || (str.find("eq") == 0) || (str.find("gt") == 0) || (str.find("lt") == 0))
		return 1; //Arithmetic and Logic Instruction
	if ((str.find("push") == 0) || (str.find("pop") == 0))
		return 2; //Memory Access Instruction
	return 0;
}
int ex(string str, int start)
{
	int ret = 0, i, t = 1;
	for (i = str.size() - 1; i >= start; i--)
	{
		ret += (str[i] - 48)*t;
		t *= 10;
	}
	return ret;
}
void push(string str, ofstream &fout)
{
	int index;
	if (str.find("constant") == 4)
	{
		index = ex(str, 11);
		fout << "@" << index << "\n" << "D=A\n" << "@SP\nAM=M+1\nA=A-1\nM=D\n";
	}
	else if (str.find("local") == 4)
	{
		index = ex(str, 8);
		fout << "@LCL\nD=M\n" << "@" << index << "\n" << "A=D+A\nD=M\n@SP\nAM=M+1\nA=A-1\nM=D\n";
	}
	else if (str.find("argument") == 4)
	{
		index = ex(str, 11);
		fout << "@ARG\nD=M\n" << "@" << index << "\n" << "A=D+A\nD=M\n@SP\nAM=M+1\nA=A-1\nM=D\n";
	}
	else if (str.find("pointer") == 4)
	{
		index = ex(str, 11);
		if (index == 0)
			fout << "@R3\nA=M\nD=M\n" << "@SP\nAM=M+1\nA=A-1\nM=D\n";
		else
			fout << "@R4\nA=M\nD=M\n" << "@SP\nAM=M+1\nA=A-1\nM=D\n";
	}
	/*else if(str.find("static")==4)
	{
		index=ex(str,9);
		fout<<"@LCL\nD=M\n"<<"@"<<index<<"\n"<<"A=D+A\nD=M\n@SP\nAM=M+1\nA=A-1\nM=D\n";
	}
	else if(str.find("temp")==4)
	{
		index=ex(str,7);
		fout<<"@LCL\nD=M\n"<<"@"<<index<<"\n"<<"A=D+A\nD=M\n@SP\nAM=M+1\nA=A-1\nM=D\n";
	}*/
	//Static and Temp not done	
}
void pop(string str, ofstream &fout)
{
	int index;
	if (str.find("local") == 3)
	{
		index = ex(str, 8);
		fout << "@LCL\nD=M\n@" << index << "\nD=D+A\n@R13\nM=D\n@SP\nAM=M-1\nD=M\n@R13\nA=M\nM=D\n";
	}
	else if (str.find("argument") == 3)
	{
		index = ex(str, 11);
		fout << "@ARG\nD=M\n@" << index << "\nD=D+A\n@R13\nM=D\n@SP\nAM=M-1\nD=M\n@R13\nA=M\nM=D\n";
	}
	else if (str.find("this") == 3)
	{
		fout << "@SP\nAM=M-1\nD=M\n@R3\nA=M\nM=D\n";
	}
	else if (str.find("that") == 3)
	{
		fout << "@SP\nAM=M-1\nD=M\n@R4\nA=M\nM=D\n";
	}
}
int main(int argc, char **argv)
{
	string fname(argv[1]); // Get name of VM file
	initialise(&fname);
	ifstream fin((fname + ".vm").c_str());
	ofstream fout((fname + ".asm").c_str());
	string str, l; string out;
	int instype, index, stat = 0;
	char eq = 'a', gt = 'a', lt = 'a';
	//Creating files is done
	while (getline(fin, l))
	{
		str = parse(l);
		cout<<str<<"\n";
		instype = findins(str);
		//cout<<str<<instype<<endl;
		switch (instype)
		{
		case 1:
		{
			if ((str.find("add") == 0))
			{
				fout << "@SP\n" << "AM=M-1\n" << "D=M\n" << "A=A-1\n" << "M=D+M\n"; break;
			}
			else if ((str.find("sub") == 0))
			{
				fout << "@SP\n" << "AM=M-1\n" << "D=M\n" << "A=A-1\n" << "M=M-D\n"; break;
			}
			else if ((str.find("and") == 0))
			{
				fout << "@SP\n" << "AM=M-1\n" << "D=M\n" << "A=A-1\n" << "M=D&M\n"; break;
			}
			else if ((str.find("or") == 0))
			{
				fout << "@SP\n" << "AM=M-1\n" << "D=M\n" << "A=A-1\n" << "M=D|M\n"; break;
			}
			else if ((str.find("neg") == 0))
			{
				fout << "@SP\n" << "A=M-1\n" << "M=-M\n"; break;
			}
			else if ((str.find("not") == 0))
			{
				fout << "@SP\n" << "A=M-1\n" << "M=!M\n"; break;
			}
			else if ((str.find("gt") == 0))
			{
				fout << "@SP\n" << "AM=M-1\n" << "D=M\n" << "A=A-1\n" << "D=M-D\n" << "M=-1\n" << "@gt" << gt << "\n" << "D;JGT\n" << "@SP\n" << "A=M-1\n" << "M=0\n" << "(gt" << gt << ")\n"; gt++; break;
			}
			else if ((str.find("eq") == 0))
			{
				fout << "@SP\n" << "AM=M-1\n" << "D=M\n" << "A=A-1\n" << "D=M-D\n" << "M=-1\n" << "@eq" << eq << "\n" << "D;JEQ\n" << "@SP\n" << "A=M-1\n" << "M=0\n" << "(eq" << eq << ")\n"; eq++; break;
			}
			else if ((str.find("lt") == 0))
			{
				fout << "@SP\n" << "AM=M-1\n" << "D=M\n" << "A=A-1\n" << "D=M-D\n" << "M=-1\n" << "@lt" << lt << "\n" << "D;JLT\n" << "@SP\n" << "A=M-1\n" << "M=0\n" << "(lt" << lt << ")\n"; lt++; break;
			}
			break;
		}
		case 2:
		{
			if (str.find("push") == 0)
			{
				if (str.find("constant") == 4)
				{
					index = ex(str, 12);
					fout << "@" << index << "\n" << "D=A\n" << "@SP\nAM=M+1\nA=A-1\nM=D\n";
				}
				else if (str.find("local") == 4)
				{
					index = ex(str, 9);
					fout << "@LCL\nD=M\n" << "@" << index << "\n" << "A=D+A\nD=M\n@SP\nAM=M+1\nA=A-1\nM=D\n";
				}
				else if (str.find("argument") == 4)
				{
					index = ex(str, 12);
					fout << "@ARG\nD=M\n" << "@" << index << "\n" << "A=D+A\nD=M\n@SP\nAM=M+1\nA=A-1\nM=D\n";
				}
				else if (str.find("pointer") == 4)
				{
					index = ex(str, 11);
					if (index == 0)
						fout << "@R3\nD=M\n" << "@SP\nAM=M+1\nA=A-1\nM=D\n";
					else
						fout << "@R4\nD=M\n" << "@SP\nAM=M+1\nA=A-1\nM=D\n";
				}
				else if ((str.find("this") == 4))
				{
					index = ex(str, 8);					     		
					fout << "@R3\nD=M\n" << "@" << index << "\n" << "A=D+A\nD=M\n@SP\nAM=M+1\nA=A-1\nM=D\n";
				}
				else if ((str.find("that") == 4))
				{
					index = ex(str, 8);
					fout << "@R4\nD=M\n" << "@" << index << "\n" << "A=D+A\nD=M\n@SP\nAM=M+1\nA=A-1\nM=D\n";
				}
				else if (str.find("static") == 4)
				{
					index = ex(str, 10);					
					fout << "@" << fname << index << "\n" << "D=M\n@SP\nAM=M+1\nA=A-1\nM=D\n";
				}
				else if (str.find("temp") == 4)
				{
					index = ex(str, 8);
					fout << "@" << 5 + index << "\nD=M\n" << "@SP\nAM=M+1\nA=A-1\nM=D\n";
				}
			}
			else if (str.find("pop") == 0)
			{
					if (str.find("pointer") == 3)
				{
					index = ex(str, 10);
					if (index == 0)
						fout << "@SP\nAM=M-1\nD=M\n" << "@R3\nM=D\n";
					else
						fout << "@SP\nAM=M-1\nD=M\n" << "@R4\nM=D\n";
				}
				else if (str.find("local") == 3)
				{
					index = ex(str, 8);
					fout << "@LCL\nD=M\n@" << index << "\nD=D+A\n@R13\nM=D\n@SP\nAM=M-1\nD=M\n@R13\nA=M\nM=D\n";
				}
				else if (str.find("argument") == 3)
				{
					index = ex(str, 11);
					fout << "@ARG\nD=M\n@" << index << "\nD=D+A\n@R13\nM=D\n@SP\nAM=M-1\nD=M\n@R13\nA=M\nM=D\n";
				}
				else if (str.find("this") == 3)
				{
					index = ex(str, 7);
					fout << "@R3\nD=M\n@" << index << "\nD=D+A\n@R13\nM=D\n@SP\nAM=M-1\nD=M\n@R13\nA=M\nM=D\n";
				}
				else if (str.find("that") == 3)
				{
					index = ex(str, 7);
					fout << "@R4\nD=M\n@" << index << "\nD=D+A\n@R13\nM=D\n@SP\nAM=M-1\nD=M\n@R13\nA=M\nM=D\n";
				}
				else if (str.find("static") == 3)
				{
					index = ex(str, 9);
					fout << "@" << fname << index << "\nD=A\n@R13\nM=D\n@SP\nAM=M-1\nD=M\n@R13\nA=M\nM=D\n";
				}
				else if (str.find("temp") == 3)
				{
					index = ex(str, 7);					fout << "@5\nD=A\n@" << index << "\nD=D+A\n@R13\nM=D\n@SP\nAM=M-1\nD=M\n@R13\nA=M\nM=D\n";
				}
			}

			break;
		}
		default:
		{
			break;
		}
		}
	}
	fout << "(END)\n@END\n0;JMP\n";
	fin.close();
	fout.close();
	return 0;
}
