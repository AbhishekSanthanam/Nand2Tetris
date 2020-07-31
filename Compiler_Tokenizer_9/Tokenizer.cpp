#include<bits/stdc++.h>
#define ll long long int
#define dd double double
#define pb push_back
#define ins insert
#define mp make_pair
#define F first
#define S second
#define For(i,x,y) for(size_t i=size_t(x);i<size_t(y);i++)
#define rfor(i,x,y) for(size_t i=size_t(x);i>=size_t(y);i--)
using namespace std;
void initialise(string *fname)
{
	size_t it = fname->find(".");
	fname->erase(it);
}
int main(int argc, char **argv)
{
	vector<string> keyword,symbol;
	keyword.pb("class");keyword.pb("constructor");keyword.pb("function");keyword.pb("method");keyword.pb("field");keyword.pb("static");keyword.pb("var");keyword.pb("int");keyword.pb("char");
keyword.pb("boolean");keyword.pb("void");keyword.pb("true");keyword.pb("false");keyword.pb("null");keyword.pb("this");keyword.pb("let");keyword.pb("do");keyword.pb("if");keyword.pb("else");
keyword.pb("while");keyword.pb("return");
symbol.pb("(");symbol.pb(")");symbol.pb("{");symbol.pb("}");symbol.pb("[");symbol.pb("]");symbol.pb(".");symbol.pb(",");symbol.pb(";");symbol.pb("+");symbol.pb("-");symbol.pb("*");
symbol.pb("/");symbol.pb("&"); symbol.pb("|");symbol.pb(">");symbol.pb("<");symbol.pb("=");symbol.pb("~");

	string fname(argv[1]); // Get name of VM file
	initialise(&fname);
	ifstream fin((fname + ".jack").c_str());
	ofstream fout((fname + "T.xml").c_str());
	string str;
	bool label=true;
	fout<<"<token>\n";
	while(getline(fin,str)){
		string s="";
		cout<<str.length()<<"!\n";
		int dq=0;
		for(int i=0;i<str.length();i++){
			string temp="";
			temp+=str[i];
			if(str[i]=='/' && str[i+1]=='/') break;
			else if(str[i]=='/' && str[i+1]=='*') label=false;
			else if(str[i]=='*'&& str[i+1]=='/') label=true;
			else if(temp=="\"") {
				dq++;
			}
			else if(dq%2==0 &&label && str[i]!=' ' && (find(symbol.begin(),symbol.end(),temp)==symbol.end() || s=="")){
				s+=str[i];
			}
			else if(dq%2==0 &&label && (str[i]==' ' || (find(symbol.begin(),symbol.end(),temp)!=symbol.end())) && s!=""){
				if(find(keyword.begin(),keyword.end(),s)!=keyword.end()) fout<<"\t<keyword>"<<s<<"</keyword>\n";
				else if(find(symbol.begin(),symbol.end(),s)!=symbol.end()) fout<<"\t<symbol>"<<s<<"</symbol>\n";
				else{
					bool alpha=true,num=true,ident=true;
					if(isdigit(s[0])) ident=false;
					For(j,0,s.length()){
						if(isdigit(s[j])) alpha=false;
						else num=false;
					}
					if(num) fout<<"\t<IntegerConstant>"<<s<<"</IntegerConstant>\n";
					else if(alpha && str[i]=='"'&& str[i-1-s.length()]=='"') fout<<"\t<StringConstant>"<<s<<"</StringConstant>\n";
					else if(ident) fout<<"\t<identifier>"<<s<<"</identifier>\n";
					else {
						ofstream fout((fname+".err").c_str());
						fout<<"Unidentified symbol "<<s;
						return 0;
					}
				}
				cout<<"str[i]: "<<str[i]<<"!!\n";
				if(str[i]==' ')	s="";
				else {
					fout<<"\t<symbol>"<<str[i]<<"</symbol>\n";
					s="";
				}
			}
			if(dq%2==0 &&label && (i==str.length()-1 || (find(symbol.begin(),symbol.end(),s)!=symbol.end()) ) && s!=""){
				cout<<s<<"\n";
				if(find(keyword.begin(),keyword.end(),s)!=keyword.end()) fout<<"\t<keyword>"<<s<<"</keyword>\n";
				else if(find(symbol.begin(),symbol.end(),s)!=symbol.end()) fout<<"\t<symbol>"<<s<<"</symbol>\n";
				else{
					bool alpha=true,num=true,ident=true;
					if(isdigit(s[0])) ident=false;
					For(j,0,s.length()){
						if(isdigit(s[j])) alpha=false;
						else num=false;
					}
					if(num) fout<<"\t<IntegerConstant>"<<s<<"</IntegerConstant>\n";
					else if(alpha) fout<<"\t<StringConstant>"<<s<<"</StringConstant>\n";
					else if(ident) fout<<"\t<identifier>"<<s<<"</identifier>\n";
					else {
						ofstream fout((fname+".err").c_str());
						fout<<"Unidentified symbol "<<s;
						return 0;
					}
				}
				s="";
			}
			if(dq%2==1 && str[i]!='"'){
				s+=str[i];
				cout<<"!!\n";
			}
			if(s==" ") s="";
		}
	}
}
