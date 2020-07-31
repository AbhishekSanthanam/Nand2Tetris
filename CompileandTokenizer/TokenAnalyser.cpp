#include <bits/stdc++.h>
using namespace std;
#define pts for(int fgh=0;fgh<space-2;fgh++) f<<" "; f<<x<<"\n"


class Entry {
	public:
		string name;
		string type;
		string sf;//kind=static/field/arguemnt/local
		int index;
} ;

class SymbolTable {
public:
	int kind1_count;  //field or local
	int kind2_count;  //static or argument
	int total_count;
	vector<Entry*> a;
	string names;
	void init(string x){
		//f<<"Initialising da funny boi\n";
		kind1_count=0;
		kind2_count=0;
		total_count=0;
		a.clear();
		names=x;
	}
};

SymbolTable Class_Symbol_table;
SymbolTable Subroutine_Symbol_table;
vector<string> fil;
int d_count=0;

void CompileSubroutine(ofstream& f,ofstream& cerr);
void compileTerm(ofstream& f,ofstream& cerr);
int compileExpressionList(ofstream& f,ofstream& cerr);
void compileExpression(ofstream& f,ofstream& cerr);
void compileLetStatement(ofstream& f,ofstream& cerr);
void compileDoStatement(ofstream& f,ofstream& cerr);
void compileWhileStatement(ofstream& f,ofstream& cerr);
void compileReturnStatement(ofstream& f,ofstream& cerr);
void compileIfStatement(ofstream& f,ofstream& cerr);
void compileVarDec(ofstream& f,ofstream& cerr);
void compileStatements(ofstream& f,ofstream& cerr);
void compileParameterList(ofstream& f,ofstream& cerr);
void Compileclassvardec(ofstream& f,ofstream& cerr);

int space=0;

string rettok(string s){
	if(s[3]=='k') return "keyword";
	if(s[3]=='s') return "symbol";
	if(s[3]=='S') return "StringConstant";
	if(s[4]=='n') return "integerConstant";
	if(s[4]=='d') return "identifier";
	else return "\0";
}


string sname(string s){
	int m=s.size();
	//f<<s;
	//f<<m<<"\n";
	int i,j;
	if(s[3]=='k') { i=12,j=m-11;}
	else if(s[3]=='s') { i=11;j=m-10;}
	else if(s[3]=='S') {i=19;j=m-18;}
	else if(s[4]=='n') {i=20;j=m-19;}
	else if(s[4]=='d') {i=15;j=m-14;}
	char a[j-i+4];
	int k=0;
	for(;i<j;i++){
		a[k++]=s[i];
	}
	a[k]='\0';
	return a;
}


bool issymbol(char a) {
	return (a=='('||a==')'||a=='{'||a=='}'||a=='['||a==']'||a=='.'||a==','||a==39||a=='+'||a=='_'||a=='-'||a=='/'||a=='&'||a=='|'||a=='<'||a=='>'||a=='='||a==';');
}

bool iskeyword(string s){
	return (s=="class"||s=="constructor"||s=="function"||s=="method"||s=="field"||s=="static"||s=="var"||s=="int"||s=="char"||s=="boolean"||s=="void"||s=="true"||s=="false"||s=="null"||s=="this"||s=="let"||s=="do"||s=="if"||s=="else"||s=="while"||s=="return" );
}

bool isintconstant(string s){
	for(int i=0;s[i];i++){
		if(!(s[i]>='0'&&s[i]<='9')) return false;
	}
	return true;
}

bool isStringconst(string s) {
	if(s[0]==34&&s[s.size()-1]==34) return true;
	return false;
}

bool isidentifier(string s){
	if(s[0]>='0'&&s[0]<='9')	return false;
	for(int i=0;i<s.size();i++){
		if(!((s[i]<='Z'&&s[i]>='A')||(s[i]>='a'&&s[i]<='z')||(s[i]>='0'&&s[i]<='9')||s[i]=='_')) return false;
	}
	return true;
}

bool iskeywordconst(string s){
	return (s=="this"||s=="true"||s=="false"||s=="this");
}

bool isop(string x){
	return (x=="+"||x=="-"||x=="*"||x=="/"||x=="&"||x=="|"||x=="<"||x==">");
}

void Compileclass(ofstream& f,ofstream& cerr) {
	f<<"<class>\n";
	space+=2;
	string x;
	x=fil[d_count++];
	//f<<d.tellg();
	x=fil[d_count++];
	//f<<" "<<d.tellg();
	if(!(x=="  <keyword> class </keyword>")) cerr<<"Syntax Error: class expected\n";
	pts;
	x=fil[d_count++];
	//f<<" "<<d.tellg();
	if(!(rettok(x)=="identifier")) cerr<<"Syntax Error: Class Name expected\n";
	//cout<<x;
	pts;
	//f<<"Calling for initialization\n";
	Class_Symbol_table.init(sname(x));
	x=fil[d_count++];
	//f<<" "<<d.tellg();
	if(!(x=="  <symbol> { </symbol>")) cerr<<"Syntax Error: { expected\n";
	pts;
	Compileclassvardec(f,cerr);
	CompileSubroutine(f,cerr);
	x=fil[d_count++];
	if(!(sname(x)=="}")) cerr<<"Syntax Error: } expected\n";
	pts;
	space-=2;
	f<<"</class>\n";
}

int isterm(ofstream& f,ofstream& cerr){
	string x;
	x=fil[d_count++];
	string s;
	s=fil[d_count++];
	d_count--;
	d_count--;
	if(rettok(x)=="integerConstant") return 1;
	if(rettok(x)=="StringConstant") return 2;
	if(iskeywordconst(sname(x))) return 3;
	if(rettok(x)=="identifier"&&sname(s)=="[") return 4;
	if(rettok(x)=="identifier"&&sname(s)=="(") return 5;
	if(rettok(x)=="identifier"&&sname(s)==".") return 6;
	if(rettok(x)=="identifier") return 7;
	if(sname(x)=="(") return 8;
	if(sname(x)=="~"||sname(x)=="-") return 9;
	return 0;
}

void CompileSubroutine(ofstream& f,ofstream& cerr) {
	//f<<"In CompileSubroutine\n";

	string x;
	//f<<" "<<d.tellg();
	x=fil[d_count++];
	//f<<x;
	while(sname(x)=="constructor"||sname(x)=="function"||sname(x)=="method"){
		string tpp=sname(x);
		for(int i=0;i<space;i++) f<<" ";
		f<<"<subroutineDec>\n";
		space+=2;
		pts;
		x=fil[d_count++];
		if(!(rettok(x)=="identifier"||rettok(x)=="keyword")) cerr<<"Syntax Error: Return type Error\n";
		pts;
		string rettype=sname(x);
		x=fil[d_count++];
		if(!(rettok(x)=="identifier")) cerr<<"Syntax Error: proper identifier expected\n";
		Subroutine_Symbol_table.init(sname(x));
		pts;
		if(tpp=="method"){
			Entry* ent = new Entry;
			ent->name="this";
			ent->index=0;
			ent->type=Class_Symbol_table.names;
			ent->sf="argument";
			Subroutine_Symbol_table.a.push_back(ent);
			Subroutine_Symbol_table.kind2_count++;
			Subroutine_Symbol_table.total_count++;
		}
		x=fil[d_count++];
		if(!(x=="  <symbol> ( </symbol>")) cerr<<"Syntax Error : ( expected\n";
		pts;
		compileParameterList(f,cerr);
		x=fil[d_count++];
		if(!(x=="  <symbol> ) </symbol>")) cerr<<"Syntax Error : ) expected\n";
		pts;
		for(int fgh=0;fgh<space;fgh++) f<<" ";
		f<<"<subroutineBody>\n";
		space+=2;
		x=fil[d_count++];
		if(!(x=="  <symbol> { </symbol>")) cerr<<"Syntax Error : { expected\n";
		pts;
		compileVarDec(f,cerr);
		compileStatements(f,cerr);
		x=fil[d_count++];
		if(!(x=="  <symbol> } </symbol>")) cerr<<"Syntax Error : } expected\n";
		pts;
		space-=2;
		for(int fgh=0;fgh<space;fgh++) f<<" ";
		f<<"</subroutineBody>\n";
		space-=2;
		for(int fgh=0;fgh<space;fgh++) f<<" ";
		f<<"</subroutineDec>\n";
		x=fil[d_count++];
	}
	d_count--;
}

void Compileclassvardec(ofstream& f,ofstream& cerr) {
	string x;
	x=fil[d_count++];
	//f<<"In classvardec\n";
	//f<<x;
	while(sname(x)=="field"||sname(x)=="static"){
		string ky=sname(x);
		for(int i=0;i<space;i++) f<<" ";
		f<<"<classVarDec>\n";
		space+=2;
		pts;
		x=fil[d_count++];
		if(!(rettok(x)=="identifier"||rettok(x)=="keyword")) cerr<<"Syntax Error: type expected\n";
		string sk=sname(x);

		do {
			pts;
			x=fil[d_count++];
			Entry* new_entry=new Entry;
			new_entry->sf=ky;
			new_entry->type=sk;
			if(ky=="field"){
				new_entry->index=Class_Symbol_table.kind1_count++;
				Class_Symbol_table.total_count++;
			}
			else {
				new_entry->index=Class_Symbol_table.kind2_count++;
				Class_Symbol_table.total_count++;
			}
			if(!(rettok(x)=="identifier")) cerr<<"Syntax Error: proper identifier expected\n";
			new_entry->name=sname(x);
			pts;
			Class_Symbol_table.a.push_back(new_entry);
			x=fil[d_count++];
		}while(x=="  <symbol> , </symbol>");
		if(!(x=="  <symbol> ; </symbol>")) cerr<<"Syntax Error: ; expected\n";
		pts;
		space-=2;
		for(int i=0;i<space;i++) f<<" ";
		f<<"</classVarDec>\n";

		x=fil[d_count++];
	}
	//f<<x.size();
	//f<<" "<<d.tellg();
	d_count--;
	//f<<" "<<d.tellg();
}

void compileParameterList(ofstream& f,ofstream& cerr){
	for(int fgh=0;fgh<space;fgh++) f<<" ";
	f<<"<parameterList>\n";
	space+=2;
	string x;
	x=fil[d_count++];
	if(x=="  <symbol> ) </symbol>") {
        space-=2;
        for(int fgh=0;fgh<space;fgh++) f<<" ";
        f<<"</parameterList>\n";
		d_count--;
		return;
	}
	if(!(rettok(x)=="identifier"||rettok(x)=="keyword")) cerr<<"Syntax Error: type expected\n";
	string sk=sname(x);

	do {
		pts;
		x=fil[d_count++];
		Entry* new_entry=new Entry;
		new_entry->sf="argument";
		new_entry->type=sk;
		if(!(rettok(x)=="identifier")) cerr<<"Syntax Error: proper identifier expected\n";
		new_entry->name=sname(x);
		pts;
		new_entry->index=Subroutine_Symbol_table.kind2_count++;
		Subroutine_Symbol_table.total_count++;
		Subroutine_Symbol_table.a.push_back(new_entry);
		x=fil[d_count++];
	}while(x=="  <symbol> , </symbol>");

	space-=2;
	for(int fgh=0;fgh<space;fgh++) f<<" ";
	f<<"</parameterList>\n";
	d_count--;

}

void compileStatements(ofstream& f,ofstream& cerr) {
	string x;
	x=fil[d_count++];
	if(!(sname(x)=="let"||sname(x)=="return"||sname(x)=="do"||sname(x)=="if"||sname(x)=="while")) {
		d_count--;
		return;
	}
	for(int fgh=0;fgh<space;fgh++) f<<" ";
		f<<"<statements>\n";
		space+=2;
	while(sname(x)=="let"||sname(x)=="return"||sname(x)=="do"||sname(x)=="if"||sname(x)=="while") {

		d_count--;
		if(sname(x)=="do") {
			compileDoStatement(f,cerr);
		}
		else if(sname(x)=="let")  compileLetStatement(f,cerr);
		else if(sname(x)=="return")  compileReturnStatement(f,cerr);
		else if(sname(x)=="if") compileIfStatement(f,cerr);
		else if(sname(x)=="while") compileWhileStatement(f,cerr);

		x=fil[d_count++];
	}
	space-=2;
	for(int fgh=0;fgh<space;fgh++) f<<" ";
	f<<"</statements>\n";
	d_count--;
}

void compileVarDec(ofstream& f,ofstream& cerr){
	string x;
	x=fil[d_count++];
	while(sname(x)=="var") {
        for(int fgh=0;fgh<space;fgh++) f<<" ";
		f<<"<varDec>\n";
		space+=2;
		pts;
		x=fil[d_count++];
		if(!(rettok(x)=="identifier"||rettok(x)=="keyword")) cerr<<"Syntax Error: type expected\n";
		string sk=sname(x);

		do {
			pts;
			x=fil[d_count++];
			Entry *new_entry=new Entry;
			new_entry->sf="local";
			new_entry->type=sk;
			new_entry->index=Subroutine_Symbol_table.kind1_count++;
			Subroutine_Symbol_table.total_count++;
			if(!(rettok(x)=="identifier")) cerr<<"Syntax Error: proper identifier expected\n";
			new_entry->name=sname(x);
			pts;
			Subroutine_Symbol_table.a.push_back(new_entry);
			x=fil[d_count++];
		}while(x=="  <symbol> , </symbol>");
		if(!(x=="  <symbol> ; </symbol>")) cerr<<"Syntax Error: ; expected\n";
		pts;
		space-=2;
		for(int i=0;i<space;i++) f<<" ";
		f<<"</varDec>\n";

		x=fil[d_count++];
	}
	d_count--;
}


void compileIfStatement(ofstream& f,ofstream& cerr){
	for(int i=0;i<space;i++) f<<" ";
	f<<"<ifStatement>\n";
	space+=2;
	string x;
	x=fil[d_count++];
	pts;
	x=fil[d_count++];
	if(!(sname(x)=="(")) cerr<<"Syntax Error: ( expected\n";
	pts;
	compileExpression(f,cerr);
	x=fil[d_count++];
	if(!(sname(x)==")")) cerr<<"Syntax Error: ) expected\n";
	pts;
	x=fil[d_count++];
	if(!(sname(x)=="{")) cerr<<"Syntax Error: { expected\n";
	pts;
	compileStatements(f,cerr);
	x=fil[d_count++];
	if(!(sname(x)=="}")) cerr<<"Syntax Error: } expected\n";
	pts;
	x=fil[d_count++];
	if(!(sname(x)=="else")) {
		d_count--;
		return;
	}
	pts;
	x=fil[d_count++];
	if(!(sname(x)=="{")) cerr<<"Syntax Error: { expected\n";
	pts;
	compileStatements(f,cerr);
	x=fil[d_count++];
	if(!(sname(x)=="}")) cerr<<"Syntax Error: } expected\n";
	pts;
	space-=2;
	for(int i=0;i<space;i++) f<<" ";
	f<<"</ifStatement>\n";
}

void  compileReturnStatement(ofstream& f,ofstream& cerr){
	for(int i=0;i<space;i++) f<<" ";
	f<<"<returnStatement>\n";
	space+=2;
	string x;
	x=fil[d_count++];
	pts;
	x=fil[d_count++];
	if(sname(x)==";") {
		pts;
	}
	else {
		compileExpression(f,cerr);
		x=fil[d_count++];
		if(sname(x)!=";") cerr<<" Syntax Error : ; expected\n";
		pts;
	}
	space-=2;
	for(int i=0;i<space;i++) f<<" ";
	f<<"</returnStatement>\n";
}

void compileWhileStatement(ofstream& f,ofstream& cerr){
	for(int i=0;i<space;i++) f<<" ";
	f<<"<whileStatement>\n";
	space+=2;
	string x;
	x=fil[d_count++];
	pts;
	x=fil[d_count++];
	if(!(sname(x)=="(")) cerr<<"Syntax Error: ( expected\n";
	pts;
	compileExpression(f,cerr);
	x=fil[d_count++];
	if(!(sname(x)==")")) cerr<<"Syntax Error: ) expected\n";
	pts;
	x=fil[d_count++];
	if(!(sname(x)=="{")) cerr<<"Syntax Error: { expected\n";
	pts;
	compileStatements(f,cerr);
	x=fil[d_count++];
	if(!(sname(x)=="}")) cerr<<"Syntax Error: } expected\n";
	pts;
	space-=2;
	for(int i=0;i<space;i++) f<<" ";
	f<<"</whileStatement>\n";
}

void compileDoStatement(ofstream& f,ofstream& cerr){
	for(int i=0;i<space;i++) f<<" ";
	f<<"<doStatement>\n";
	space+=2;
	string x;
	x=fil[d_count++];
	pts;
	x=fil[d_count++];
	if(rettok(x)!="identifier") cerr<<"Error : identifier expected\n";
	pts;
	x=fil[d_count++];
	if(sname(x)==".") {
		pts;
		x=fil[d_count++];
		if(rettok(x)!="identifier") cerr<<"Error : identifier expected\n";
		pts;
		x=fil[d_count++];
	}
	if(sname(x)!="(") cerr<<"Syntax Error : ( expected\n";
	pts;
	int np=compileExpressionList(f,cerr);
	x=fil[d_count++];
	if(!(sname(x)==")")) cerr<<"Syntax Error : ) expected\n";
	pts;
	x=fil[d_count++];
	if(sname(x)!=";") cerr<<"Syntax Error : ; expected\n";
	pts;
	space-=2;
	for(int i=0;i<space;i++) f<<" ";
	f<<"</doStatement>\n";
}

void compileLetStatement(ofstream& f,ofstream& cerr){
	for(int i=0;i<space;i++) f<<" ";
	f<<"<letStatement>\n";
	space+=2;
	string x;
	x=fil[d_count++];
	pts;
	x=fil[d_count++];
	if(!(rettok(x)=="identifier")) cerr<<"Identifier name expected\n";
	pts;
	x=fil[d_count++];
	if(sname(x)=="[") {
		pts;
		compileExpression(f,cerr);
		x=fil[d_count++];
		if(!(sname(x)=="]")) cerr<<"Syntax Error : ] expected\n";
		pts;
		x=fil[d_count++];
	}
	if(!(sname(x)=="=")) cerr<<"Error : = expected\n";
	pts;
	compileExpression(f,cerr);
	x=fil[d_count++];
	if(sname(x)!=";") cerr<<"Syntax Error : ; expected\n";
	pts;
	space-=2;
	for(int i=0;i<space;i++) f<<" ";
	f<<"</letStatement>\n";
}

void compileExpression(ofstream& f,ofstream& cerr){
	string x;
	x=fil[d_count++];
	for(int i=0;i<space;i++) f<<" ";
	f<<"<expression>\n";
	space+=2;
	d_count--;
	compileTerm(f,cerr);
	x=fil[d_count++];
	while(isop(sname(x))) {
		pts;
		compileTerm(f,cerr);
		x=fil[d_count++];
	}
	d_count--;
	space-=2;
	for(int i=0;i<space;i++) f<<" ";
	f<<"</expression>\n";
}

int compileExpressionList(ofstream& f,ofstream& cerr){

	int coo=0;
	for(int i=0;i<space;i++) f<<" ";
	f<<"<expressionList>\n";
	string x;
	if(isterm(f,cerr)==0) {
		for(int i=0;i<space;i++) f<<" ";
		f<<"</expressionList>\n";
		return 0;
	}
	space+=2;
	do {
		coo++;
		compileExpression(f,cerr);
		x=fil[d_count++];
		if(sname(x)==",") pts;

	}while(sname(x)==",");
	space-=2;
	for(int i=0;i<space;i++) f<<" ";
	f<<"</expressionList>\n";
	d_count--;
	return coo;

}

void compileTerm(ofstream& f,ofstream& cerr){
	string x;
	int z=isterm(f,cerr);
	if(z==0) {

		return;
	}
	for(int i=0;i<space;i++) f<<" ";
	f<<"<term>\n";
	space+=2;
	x=fil[d_count++];
	if(z==1) {
		pts;

	}
	else if(z==2){
		pts;
	}
	else if(z==3){
		pts;
	}
	else if(z==4){
		pts;
		x=fil[d_count++];
		pts;
		compileExpression(f,cerr);
		x=fil[d_count++];
		if(!(sname(x)=="]")) cerr<<"Syntax Error : ] expected\n";
		pts;
	}
	else if(z==5){
		pts;
		x=fil[d_count++];
		pts;
		int elc=compileExpressionList(f,cerr);
		x=fil[d_count++];
		if(!(sname(x)==")")) cerr<<"Syntax Error : ) expected\n";
		pts;
	}
	else if(z==6){
		pts;
		x=fil[d_count++];
		pts;
		x=fil[d_count++];
		if(!(rettok(x)=="identifier")) cerr<<"Proper identifier expected\n";
		pts;
		x=fil[d_count++];
		if(!(sname(x)=="(")) cerr<<"Syntax Error : ( expected";
		pts;
		int elc=compileExpressionList(f,cerr);
		x=fil[d_count++];
		if(!(sname(x)==")")) cerr<<"Syntax Error : ) expected\n";
		pts;
	}
	else if(z==7){
		pts;
	}
	else if(z==8){
		pts;
		compileExpression(f,cerr);
		x=fil[d_count++];
		if(!(sname(x)==")")) cerr<<"Syntax Error : ) expected\n";
		pts;
	}
	else if(z==8){
		if(sname(x)=="-"){
			pts;
		}
		else if(sname(x)=="~"){
			pts;
		}
	}
	space-=2;
	for(int i=0;i<space;i++) f<<" ";
	f<<"</term>\n";
}

int main(int argc,char* argv[]) {
	ifstream a;
	a.open(argv[1]);
	ofstream b;
	b.open("out.txt");
	if(!a ||!b) cout<<"Error in opening file";
	else {
	    char ch,pre2=0;
	    string str;
	    while(a.get(ch)) {
	    	int h=0;
			if(ch==9 ) ;
			else if(ch=='/') {
				char pre='*';
				if(a.get(ch))
					if(ch=='/') {
						getline(a,str);
						if(pre2!='\n'&&pre2!=0)
							b<<'\n';
					}
				else if(ch=='*') {
					while(a.get(ch)) {
           			    if(ch=='\n') h=1;
						if(ch=='/'&&pre=='*') break;
						pre=ch;
					}
					if(h==1) b<<'\n';
				}
				else b<<'/'<<ch;
			}
			else if(ch=='\n'&&(pre2=='\n'||pre2==0)) continue;
			else { b<<ch; pre2=ch; }
		}
	}

	a.close();
	b.close();
	ifstream c;
	ofstream d;
	c.open("out.txt");
	d.open(argv[2]);
	d<<"<tokens>\n";
	string s;
	while(getline(c,s)) {
		if(s=="\n") continue;
		int i=0,j=0;
		int ssize=s.size();
		char stt[ssize+1];
		while(j<ssize){
			if(s[j]=='\t'||(s[j]==' '&&i==0)) ;
			else if(s[j]==' '&&i!=0) {
				stt[i++]='\0';
				if(iskeyword(stt)) d<<"  <keyword> "<<stt<<" </keyword>\n";
				else if(isidentifier(stt)) d<<"  <identifier> "<<stt<<" </identifier>\n";
				else if(isintconstant(stt)) d<<"  <integerConstant> "<<stt<<" </integerConstant>\n";
				//else f<<"Unidentified command : "<<stt<<" in the line : "<<s<<"\n";
				i=0;
			}
			else if(issymbol(s[j])&&i!=0) {
				stt[i++]='\0';

				if(iskeyword(stt)) d<<"  <keyword> "<<stt<<" </keyword>\n";
				else if(isidentifier(stt)) d<<"  <identifier> "<<stt<<" </identifier>\n";
				else if(isintconstant(stt)) d<<"  <integerConstant> "<<stt<<" </integerConstant>\n";
				i=0;
				d<<"  <symbol> "<<s[j]<<" </symbol>\n";
			}
			else if(issymbol(s[j])&&i==0) {
				d<<"  <symbol> "<<s[j]<<" </symbol>\n";
			}
			else if(s[j]==34) {
				if(i!=0); //out<<"Error in command : "<<s<<"Wrong occurence of \" \n";
				else {

					j++;
					while(s[j]!=34) stt[i++]=s[j++];
					stt[i++]='\0';
					d<<"  <StringConstant> "<<stt<<" </StringConstant>\n";
					i=0;
				}
			}
			else stt[i++]=s[j];
			j++;
		}
	}
	d<<"</tokens>";
	d.close();
	c.close();
	ifstream e;
	e.open(argv[2]);
	string x;
    while(getline(e,x)){
    	cout<<x<<"\n";
        fil.push_back(x);
    }
	ofstream f,g;
	f.open(argv[3]);
	g.open("cerr.txt");
	Compileclass(f,g);
	return 0;
}
