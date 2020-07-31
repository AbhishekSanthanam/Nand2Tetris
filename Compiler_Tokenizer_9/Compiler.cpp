/* * in procstrin(), each word is treated as new string. Would it lead to problem?
 * Handle multi line commenting
*/
#include<bits/stdc++.h>
#define printstr printspace(space); fout<<str<<endl
#define printprevtok printspace(space); fout<<previoustoken<<endl
#define checkshouldquit if(shouldquit==true) return true
using namespace std;
string classname;
string subroutinename;
int labelnum;
string subroutinetype;
class elementtype
{
	public:
	string name,kind,type;int index;
	void print()
	{
		cout<<"Name: "<<name<<endl<<"Kind: "<<kind<<endl;
		cout<<"Type: "<<type<<endl<<"Index: "<<index<<endl;
	}
};
class classst
{
	public:
	string classname;
	int staticount;
	int fieldcount;
	int totalcount;
	vector<elementtype>classvar;
	void init()
	{
		totalcount=0;labelnum=0;
	}
	void set(string str)
	{
		classname=str;
	}
	void print()
	{
		cout<<"ClassName: "<<classname;
		cout<<"Number of static variables: "<<staticount<<endl;
		cout<<"Number of field variables: "<<fieldcount<<endl;
		cout<<"Number of total variables: "<<totalcount<<endl;
		for(int i=0;i<classvar.size();i++)
		{
			classvar[i].print();
		}
	}
	void del()
	{
		staticount=0;
		fieldcount=0;
		totalcount=0;
		classvar.clear();
		classname.clear();
	}
	void add(string name,string kind,string type)
        {
                elementtype s;
		if(kind=="static")
		{
			s.index=staticount;
			staticount++;totalcount++;
			s.kind=kind;
		}
		else if(kind=="field")
		{
			s.kind="this";
			s.index=fieldcount;
			fieldcount++;totalcount++;
		}
                s.name=name;s.type=type;
               classvar.push_back(s);
        }

}cst;
class subroutinest
{
	public:
	string subroutinename;
	string returntype;
	int argumentcount;
	int localcount;
	int totalcount;
	vector<elementtype>var;
	void init()
	{
		argumentcount=0;localcount=0;totalcount=0;
	}
	void set(string str)
        {
               subroutinename=str;
        }
	void setreturn(string str)
	{
		returntype=str;
	}
	void del()
	{
		argumentcount=0;
		totalcount=0;
		localcount=0;
		subroutinename.clear();
		var.clear();
		returntype.clear();
	}
	void add(string name,string kind,string type)
	{
		elementtype s;
		if(kind=="local")
		{
			s.index=localcount;
			localcount++;totalcount++;
		}
		else if(kind=="argument")
		{
			s.index=argumentcount;
			argumentcount++;totalcount++;
		}
		s.name=name;s.kind=kind;s.type=type;
		var.push_back(s);
	}
	void print()
        {
		cout<<"Subroutine is a "<<subroutinetype<<endl;
                cout<<"SubroutineName: "<<subroutinename<<"\t Subroutinetype: "<<returntype;
                cout<<"Number of local variables: "<<localcount<<endl;
                cout<<"Number of arguments: "<<argumentcount<<endl;
                cout<<"Number of total variables: "<<totalcount<<endl;
                for(int i=0;i<var.size();i++)
                {
                        var[i].print();
                }
        }
}srst;
int findvar(string name,string *kind,int *index,string *type)
{
	bool found=false;
	for(int i=0;i<srst.var.size();i++)
	{
		if(srst.var[i].name==name)
		{
			found=true;
			*kind=srst.var[i].kind;
			*index=srst.var[i].index;
			*type=srst.var[i].type;
		}
	}
	if(found==true)
		return true;
	for(int i=0;i<cst.classvar.size();i++)
	{
		if(cst.classvar[i].name==name)
		{
			found=true;
			*kind=cst.classvar[i].kind;
			*index=cst.classvar[i].index;
			*type=cst.classvar[i].type;
		}
	}
	return found;
}
ofstream fvm;
ofstream fout;
ifstream fin;
ofstream ferr;
string previoustoken;
int space=0;

string name,type;
bool compileexpression();
bool compiledoStatement();
bool compilewhileStatement();
bool compilereturnStatement();
bool compileifStatement();
bool tok(string str)
{
	if(str=="~"||str=="{"||str=="}"||str=="("||str==")"||str=="+"||str=="-"||str=="*"||str=="/"||str=="."||str=="["||str=="]"||str=="&"||str=="|"||str==">"||str=="<"||str=="="||str==","||str==";"||str=="-")
	{
		fout<<"<symbol> "<<str<<" </symbol>\n";
	}
	else if(str=="class"||str=="constructor"||str=="function"||str=="method"||str=="var"||str=="int"||str=="char"||str=="boolean"||str=="let"||str=="do"||str=="while"||str=="if"||str=="arr"||str=="field"||str=="static"||str=="void"||str=="true"||str=="false"||str=="null"||str=="this"||str=="else"||str=="return")
	{
		fout<<"<keyword> "<<str<<" </keyword>\n";
	}
	else if(str[0]>=48&&str[0]<=57)
	{
		int ctr=1;
		for(int yu=0;yu<str.size();yu++)
		{
			if(str[yu]<48||str[yu]>57)
				ctr=0;
		}
		if(ctr==1)
		fout<<"<integerConstant> "<<str<<" </integerConstant>\n";
		else
		{
			ferr<<"Lexical Error: "<<str<<"\n";	
			return false;
		}	
	}
	else if(str[0]=='"')
		;
	else 
		fout<<"<identifier> "<<str<<" </identifier>\n";
		
}
int extractintcons(string str)
{
	int ans=0;
	int i=0,mult=1;
	for(i=str.size()-20;str[i]!=' ';i--)
	{
		ans+=mult*(str[i]-48);
		mult*=10;
	}
	return ans;
}
string extractname(string str)
{
	string p;
	for(int i=13;i<str.size()&&str[i]!=' ';i++)
		p.push_back(str[i]);
	cout<<p<<" is extracted name from"<<str<<endl;
	return p;
}
string extracttype(string str)
{
	string p;
	if(str.find("<identifier>")==0)
	{
		p=extractname(str);
		return p;
	}
	for(int i=10;i<str.size()&&str[i]!=' ';i++)
		p.push_back(str[i]);
	cout<<p<<" is extracted type\n";
	return p;
}
bool iskeywordConstant(string str)
{
	if(str=="<keyword> this </keyword>"||str=="<keyword> false </keyword>"||str=="<keyword> null </keyword>"||str=="<keyword> true </keyword>")
	return false;
	return true;
}
bool isUnaryop(string str)
{
	if(str=="<symbol> - </symbol>"||str=="<symbol> ~ </symbol>")
		return false;
	return true;
}
string stringcons(string *str,string *b)
{
	string p;int j=str->size()-1;int ctr=0;int k,i;string q;
	const char *y=str->c_str();
	for(i=0;i<str->size()&&ctr==0;i++)
	{
		if(y[i]=='"')
		{
			ctr=1;
			for(k=i+1;k<str->size()&&(y[k])!='"';k++)
				p.push_back(y[k]);
			for(k=k+1;k<str->size();k++)
				q.push_back(y[k]);
		}
	}
	if(ctr==1)
        {
                str->erase(i-1);
		*b=q;
        }
	return p;
}
string parse(string str)
{
	int n = str.size(); 
  	int i = 0, j = 0;  
	bool spaceFound = false; 
  	while (j < n)
	{
		if(str[j]==' ')
			j++;
		else 
			break;
	}	
	while (j <n) 
    	{ 
        // if current characters is non-space 
        if (str[j] != ' ') 
        {         
		
		str[i++] = str[j++];spaceFound = false;			

        } 
        // if current character is a space 
        else if (str[j] == ' ') 
        { 
            if (!spaceFound) 
            { 
                str[i++] = ' '; 
                spaceFound = true; 
           }
	   j++; 
        } 
    }   
    // Remove trailing spaces 
    if (i <= 1) 
	    str.erase(str.begin() + i, str.end()); 
    else
        str.erase(str.begin() + i , str.end()); 
		
    size_t it=str.find("//");size_t end=str.length();string t;
    if(it==string::npos)
    {
    return str; 
    }
    else if(it!=string::npos)
    {
	str.erase(it,end);
	return str;
     }
	return str;
}
bool startmultcomm(string *str)
{
	size_t it1=str->find("/*");
	size_t end=str->length();
	if(it1==string::npos)
	{
		return false;
	}
	size_t it2=str->find("*/");
	if(it2==string::npos)
	{
		str->erase(it1,end);
		return true;
	}
	str->erase(it1,it2+2);
	return false;
}
bool endmultcomm(string *str)
{
	size_t it=str->find("*/");
	if(it==string::npos)
	{
		return false;
	}
	str->erase(0,it+2);	
	return true;	
}
string addspace(string str)
{
	string p;int j=0;
	for(int i=0;i<str.size();i++)
	{
		if(str[i]=='~'||str[i]=='{'||str[i]=='}'||str[i]=='('||str[i]==')'||str[i]=='+'||str[i]=='-'||str[i]=='*'||str[i]=='/'||str[i]=='.'||str[i]=='['||str[i]==']'||str[i]=='&'||str[i]=='|'||str[i]=='>'||str[i]=='<'||str[i]=='='||str[i]==','||str[i]==';'||str[i]=='-')
		{
			p.push_back(' ');
			p.push_back(str[i]);
			p.push_back(' ');
		}
		else
		{
			p.push_back(str[i]);
		}	
	}
	return p;
}
		
void initialise(string *fname)
{
	size_t it=fname->find(".");
	fname->erase(it);
}
void printspace(int t)
{
	for(int i=0;i<t;i++)
		fout<<" ";
}
bool istype(string str)
{
	if(str=="<keyword> int </keyword>"||str=="<keyword> char </keyword>"||str=="<keyword> boolean </keyword>"||str.find("<identifier>")==0)
		return false;
	return true;
}
bool isop(string str)
{
	if(str=="<symbol> + </symbol>"||str=="<symbol> - </symbol>"||str=="<symbol> & </symbol>"||str=="<symbol> | </symbol>"||str=="<symbol> > </symbol>" ||str=="<symbol> < </symbol>"||str=="<symbol> = </symbol>"||str=="<symbol> * </symbol>"||str=="<symbol> / </symbol>")
		return false;
	return true;
}
string getop(string str)
{
	switch(str[9])
	{
		case '+': return "add";
		case '-': return "sub";
		case '&': return "and";
		case '|': return "or";
		case '>': return "gt";
		case '<': return "lt";
		case '=': return "eq";
		case '*': return "call Math.multiply 2";
		case '/': return "call Math.divide 2";
	}
	return "";
}
bool compileclassvarDec(int yu)
{
	cout<<"CompileClassVarDec \n";
	string str;
	bool shouldquit=true;
	printspace(space);
	fout<<"<classVarDec>\n";
	space=space+2;
	printprevtok;
	getline(fin,str);
	shouldquit=istype(str);
	if(shouldquit==true)
	{
		ferr<<"Syntax error: type expected\n";
		return true;
	}
	type=extracttype(str);
	printstr;
	getline(fin,str);
	if(str.find("<identifier>")!=0)
	{
		ferr<<"Syntax error: Identifier expected\n";
		return true;
	}
	printstr;
	name=extractname(str);
	if(yu==0)
	cst.add(name,"static",type);
	else
	cst.add(name,"field",type);
	getline(fin,str);
	while(str=="<symbol> , </symbol>")
	{
		printstr;
		getline(fin,str);
		name=extractname(str);
		if(str.find("<identifier>")!=0)
		{
			ferr<<"Syntax error: identifier expected\n";
			return true;
		}
		printstr;
		if(yu==0)
	        cst.add(name,"static",type);
       		else
       		cst.add(name,"field",type);
		getline(fin,str);	
	}
	if(str!="<symbol> ; </symbol>")
	{
		ferr<<"Syntax error: ; expected\n";
		return true;
	}
	printstr;
	space-=2;
	printspace(space);
	fout<<"</classVarDec>\n";
	return false;
}
bool compileparameterlist()
{
	cout<<"Entered compileparameterlist\n";
	string str;
	printspace(space);
	fout<<"<parameterList>\n";
	space=space+2;
	getline(fin,str);
	while(istype(str)==false)
	{
		printstr;
		type=extracttype(str);
		getline(fin,str);
		if(str.find("<identifier>")!=0)
		{
			ferr<<"Syntax error: Identifier expected\n";
			return true;
		}
		printstr;
		name=extractname(str);
		getline(fin,str);
		srst.add(name,"argument",type);
		if(str=="<symbol> ) </symbol>")
			break;
		else if(str=="<symbol> , </symbol>")
		{
			printstr;
			getline(fin,str);
		}
	}
	if(str!="<symbol> ) </symbol>")
	{
		ferr<<"Syntax error: ) expected\n";
		return true;
	}
	space=space-2;
	printspace(space);
	fout<<"</parameterList>\n";
	previoustoken=str;
       return false;	
}
int isStatement(string str)
{
	if(str=="<keyword> let </keyword>")
		return 1;
	if(str=="<keyword> do </keyword>")
		return 2;
	if(str=="<keyword> while </keyword>")
		return 3;
	if(str=="<keyword> return </keyword>")
		return 5;
	if(str=="<keyword> if </keyword>")
		return 4;
	return 0;
}
bool compileexpressionList(int *np)
{
	int count=0;
	cout<<"Entered compileexpressionlist\n";
        string str;string dup;bool shouldquit;
        printspace(space);
        fout<<"<expressionList>\n";
	space=space+2;
	getline(fin,str);
	if(str!="<symbol> ) </symbol>")
	{
		previoustoken=str;
		shouldquit=compileexpression();
		count++;
		if(shouldquit==true)
			return true;
		while(previoustoken=="<symbol> , </symbol>")
		{
			printspace(space);
			fout<<previoustoken<<endl;
			getline(fin,str);
			previoustoken=str;
			shouldquit=compileexpression();
			count++;
			if(shouldquit==true)
				return true;
		}
	}
	else
	previoustoken=str;
	space=space-2;
	printspace(space);
	*np=count;
	fout<<"</expressionList>\n";
       	return false;
}
void procstring(string str)
{
	int size;
	size=str.size()-35;
	cout<<str<<"has size "<<size<<endl;
	fvm<<"push constant "<<size<<endl;
	fvm<<"call String.new 1"<<endl;
	for(int i=17;i<str.size()-18;i++)
	{
		fvm<<"push constant "<<(int)str[i]<<endl;
		fvm<<"call String.appendChar 2"<<endl;
		
	}
	return ;	
}
bool compileterm(int def=1)
{
	cout<<"Entered compileTerm\n";
        string str;string dup;bool shouldquit,varpresent;string tempkind;int tempindex;string id1,id2;int np;string temptype;bool extravar=true;
	if(def==1)
	{
		printspace(space);
        	fout<<"<term>\n";
	        space=space+2;
	}	
	str=previoustoken;
	if(str.find("<integerConstant>")==0||str.find("<stringConstant>")==0||iskeywordConstant(str)==false)
	{
		printstr;
		space=space-2;
		printspace(space);
		fout<<"</term>\n";
		if(str.find("<integerConstant>")==0)
                {
                        cout<<"IntegerConstant found\n";
                        int val=extractintcons(str);
                        fvm<<"push constant "<<val<<endl;
                }
		else if(str=="<keyword> true </keyword>")
		{
			fvm<<"push constant 0\n"<<"not\n";
		}
		else if(str=="<keyword> false </keyword>")
		{
			fvm<<"push constant 0\n";
		}
		else if(str=="<keyword> this </keyword>")
		{
			fvm<<"push pointer 0\n";
		}
		else if(str=="<keyword> null </keyword>")
		{
			fvm<<"push constant 0\n";
		}
		else if(str.find("<stringConstant>")==0)
		{
			procstring(str);
		}
		getline(fin,str);
		previoustoken=str;
		return false;
	}
	if(isUnaryop(str)==false)
	{
		int unop;
		printstr;
		if(str=="<symbol> - </symbol>")
		{
			unop=0;
		}
		else
		unop=1;
                getline(fin,str);
                previoustoken=str;
		shouldquit=compileterm();
		checkshouldquit;
		space=space-2;
		printspace(space);
		fout<<"</term>\n";
		str=previoustoken;
		if(unop==0)
		fvm<<"neg\n";
		else
		fvm<<"not\n";
                return false;
	}
	if(str=="<symbol> ( </symbol>")
	{
		cout<<str<<" is the string at start of term\n";
		printstr;
		getline(fin,str);
		previoustoken=str;
		shouldquit=compileexpression();
		checkshouldquit;
		cout<<previoustoken<<" after processing paranthesis\n";
		str=previoustoken;
		previoustoken=str;
		extravar=false;
	}
	dup=str;
	printstr;
	getline(fin,str);
	if(str=="<symbol> [ </symbol>"&&extravar==true)
	{
		printstr;
		getline(fin,str);
		previoustoken=str;
		varpresent=findvar(extractname(dup),&tempkind,&tempindex,&temptype);
                if(varpresent==false)
                {
			cout<<"Declaration error at 0\n"<<"Dup is: "<<dup<<endl;
                        ferr<<"Declaration error: "<<extractname(dup)<<" undeclared\n";
                        return true;
                }
		shouldquit=compileexpression();
		fvm<<"push "<<tempkind<<" "<<tempindex<<endl;
		if(shouldquit==true)
			return true;
		if(previoustoken!="<symbol> ] </symbol>")
		{
			ferr<<"Syntax error: ] expected";
			return true;
		}
		printspace(space);
		fout<<previoustoken<<endl;
		getline(fin,str);
		fvm<<"add\n"<<"pop pointer 1\n"<<"push that 0\n";
		/*previoustoken=str;
		fout<<"</term>";
		return false;*/
	}
	else if(str=="<symbol> ( </symbol>"&&extravar==true)
	{
		id1=extractname(dup);
		printstr;
		fvm<<"push pointer 0\n";
		shouldquit=compileexpressionList(&np);
		if(shouldquit==true)
			return true;
		if(previoustoken!="<symbol> ) </symbol>")
		{
			ferr<<"Syntax error: ) expected";
			return true;
		}
		fvm<<"call "<<classname<<"."<<id1<<" "<<np+1<<endl;
		if(def==0)
		fvm<<"pop temp 0\n";
		printspace(space);
		cout<<previoustoken;
		fout<<previoustoken<<endl;
		getline(fin,str);
	/*	previoustoken=str;
		fout<<"</term>\n";
		return false;*/
	}
	else if(str=="<symbol> . </symbol>"&&extravar==true)
	{
		id1=extractname(dup);
		printstr;
		getline(fin,str);
		printstr;
		id2=extractname(str);
		varpresent=findvar(id1,&tempkind,&tempindex,&temptype);
		getline(fin,str);
		if(str!="<symbol> ( </symbol>")
		{
			ferr<<"Syntax error: ( expected";
			return true;
		}
		printstr;
		shouldquit=compileexpressionList(&np);
		if(shouldquit==true)
			return true;
		if(varpresent==false)
		{
			fvm<<"call "<<id1<<"."<<id2<<" "<<np<<endl;
			if(def==0)
	                fvm<<"pop temp 0\n";
		}
		else
		{
			fvm<<"push "<<tempkind<<" "<<tempindex<<endl;
			fvm<<"call "<<temptype<<"."<<id2<<" "<<np+1<<endl;
			if(def==0)
	                fvm<<"pop temp 0\n";
		}
		printspace(space);
		fout<<previoustoken<<endl;
		getline(fin,str);
	/*	previoustoken=str;
		fout<<"</term>\n";
		return false;*/
	}
	else if(extravar==true)
	{
		varpresent=findvar(extractname(dup),&tempkind,&tempindex,&temptype);
		/*if(varpresent==false)
		{
			cout<<"Declaration error at 1\n"<<"Dup is: "<<dup<<endl;
			ferr<<"Declaration error: "<<extractname(dup)<<" undeclared\n";
			return true;
		}*/
		fvm<<"push "<<tempkind<<" "<<tempindex<<endl;
	}
	if(def==1)
	{	
		space=space-2;
       		printspace(space);
		previoustoken=str;
       		fout<<"</term>\n";
	}
	previoustoken=str;	
	return false;
}
bool compileexpression()
{
	cout<<"Entered Expression\n";
        string str;bool shouldquit;string temp;
        printspace(space);
        fout<<"<expression>\n";
        space=space+2;
	shouldquit=compileterm();
	if(shouldquit==true)
		return true;
	str=previoustoken;
	while(isop(str)==false)
	{
		printstr;
		temp=getop(str);
		getline(fin,str);
		previoustoken=str;
		shouldquit=compileterm();
		if(shouldquit==true)
			return true;
		fvm<<temp<<endl;
		str=previoustoken;
	}
        space=space-2;
        printspace(space);
        fout<<"</expression>\n";
        return false;
}
bool compileletStatement()
{
	cout<<"Entered letStatement\n";
        string str;bool shouldquit;bool arr=false,varpresent;string tempkind;int tempindex;string temptype;
        printspace(space);
        fout<<"<letStatement>\n";
        space=space+2;
        printspace(space);
        fout<<previoustoken<<endl;
	getline(fin,str);
	if(str.find("<identifier>")!=0)
	{
		ferr<<"Syntax error: identifier expected\n";
		return true;
	}
	printstr;
	string dup=str;
	getline(fin,str);
	if(str=="<symbol> [ </symbol>")
	{
		arr=true;
		printstr;
		getline(fin,str);
		previoustoken=str;
		shouldquit=compileexpression();
		if(shouldquit==true)
			return true;
		varpresent=findvar(extractname(dup),&tempkind,&tempindex,&temptype);
                if(varpresent==false)
                {
                        ferr<<"Declaration error: "<<extractname(dup)<<" undeclared\n";
                        return true;
                }
                fvm<<"push "<<tempkind<<" "<<tempindex<<endl;
		fvm<<"add\n";
		printspace(space);
		if(previoustoken!="<symbol> ] </symbol>")
		{
			ferr<<"Syntax error: ] expected\n";
			return true;
		}
		cout<<previoustoken<<endl;
		fout<<previoustoken<<endl;
		getline(fin,str);
	}
	if(str!="<symbol> = </symbol>")
	{
		ferr<<"Syntax error: = expected\n";
		return true;
	}
	printstr;
	getline(fin,str);
	previoustoken=str;
	shouldquit=compileexpression();
	if(arr==true)
	{
		fvm<<"pop temp 0\n"<<"pop pointer 1\n"<<"push temp 0\n"<<"pop that 0\n";
	}
	else
	{
		varpresent=findvar(extractname(dup),&tempkind,&tempindex,&temptype);
                if(varpresent==false)
                {
                        ferr<<"Declaration error: "<<extractname(dup)<<" undeclared\n";
                        return true;
                }
                fvm<<"pop "<<tempkind<<" "<<tempindex<<endl;
	}
	checkshouldquit;
	printprevtok;
        space=space-2;
        printspace(space);
        fout<<"</letStatement>\n";
        return false;
}
bool compiledoStatement()
{
	cout<<"Entered doStatement\n";
        string str;bool shouldquit;
        printspace(space);
        fout<<"<doStatement>\n";
        space=space+2;
        printspace(space);
        fout<<previoustoken<<endl;
	getline(fin,str);
	previoustoken=str;
	shouldquit=compileterm(0);
	if(shouldquit==true)
		return true;
	printprevtok;
        space=space-2;
        printspace(space);
        fout<<"</doStatement>\n";
        return false;
}
bool compileStatements()
{
	cout<<"Entered compileStatements\n";
	string str;bool shouldquit;int stattype;
        printspace(space);
        fout<<"<statements>\n";
	space=space+2;
	str=previoustoken;
        while(isStatement(str))
        {
                stattype=isStatement(str);
                switch(stattype)
                {
                        case 1:
                                previoustoken=str;
                                shouldquit=compileletStatement();
                                if(shouldquit==true)
                                        return true;
                                break;
                        case 2:
                                previoustoken=str;
				shouldquit=compiledoStatement();
                                if(shouldquit==true)
                                        return true;
                                break;
                        case 3:
                                  previoustoken=str;
  				  shouldquit=compilewhileStatement();
                                if(shouldquit==true)
                                        return true;
                                break;
                        case 4:
                                previoustoken=str;
                                shouldquit=compileifStatement();
                                if(shouldquit==true)
                                        return true;
				str=previoustoken;
                                break;
                        case 5:
                                previoustoken=str;
				shouldquit=compilereturnStatement();
                                if(shouldquit==true)
                                        return true;
                                break;
                }
		if(stattype!=4)
                getline(fin,str);
        }
        if(str!="<symbol> } </symbol>")
        {
                ferr<<"Syntax error: } expected in compilestatements\n";
                return true;
        }
        space=space-2;
        printspace(space);
	previoustoken=str;
        fout<<"</statements>\n";
	printstr;
	return false;
}
bool compilewhileStatement()
{
	cout<<"Entered whileStatement\n";
        string str;bool shouldquit;int stattype;int numline=labelnum;labelnum+=2;
        printspace(space);
        fout<<"<whileStatement>\n";
        space=space+2;
        printspace(space);
        fout<<previoustoken<<endl;
	getline(fin,str);
	if(str!="<symbol> ( </symbol>")
	{
		ferr<<"Syntax error: ( expected\n";
		return true;
	}
	printstr;
	getline(fin,str);
	previoustoken=str;
	fvm<<"label "<<classname<<"."<<numline<<endl;
	shouldquit=compileexpression();
	checkshouldquit;
	if(previoustoken!="<symbol> ) </symbol>")
	{
		ferr<<"Syntax error: ) expected\n";
		return true;	
	}
	printprevtok;
	fvm<<"not\n";
	fvm<<"if-goto "<<classname<<"."<<numline+1<<endl;
	getline(fin,str);
	if(str!="<symbol> { </symbol>")
	{
		ferr<<"Syntax error: { expected\n";
		return true;
	}
	printstr;
	getline(fin,str);
	previoustoken=str;
	shouldquit=compileStatements();
	checkshouldquit;
	fvm<<"goto "<<classname<<"."<<numline<<endl;
	fvm<<"label "<<classname<<"."<<numline+1<<endl;
        space=space-2;
        printspace(space);
	labelnum+2;
        fout<<"</whileStatement>\n";
        return false;
}
bool compileifStatement()
{
	cout<<"Entered ifStatement\n";
        string str;bool shouldquit;int numline=labelnum;labelnum+=2;
        printspace(space);
        fout<<"<ifStatement>\n";
        space=space+2;
        printspace(space);
        fout<<previoustoken<<endl;
	getline(fin,str);
	if(str!="<symbol> ( </symbol>")
	{
		ferr<<"Syntax error: ( expected";
		return true;
	}
	printstr;
	getline(fin,str);
	previoustoken=str;
	shouldquit=compileexpression();
	fvm<<"not\n";
	fvm<<"if-goto "<<classname<<"."<<numline<<endl;
	printprevtok;
	getline(fin,str);
	if(str!="<symbol> { </symbol>")
	{
		ferr<<"Syntax error: { expected";
		return true;
	}
	printstr;
	getline(fin,str);
	previoustoken=str;
	shouldquit=compileStatements();
	fvm<<"goto "<<classname<<"."<<numline+1<<endl;
	fvm<<"label "<<classname<<"."<<numline<<endl;
	getline(fin,str);
	if(str=="<keyword> else </keyword>")
	{
		printstr;
	        getline(fin,str);
	        if(str!="<symbol> { </symbol>")
	        {
                	ferr<<"Syntax error: { expected";
        	        return true;
		        }
	        printstr;
	        getline(fin,str);
	        previoustoken=str;
       		 shouldquit=compileStatements();
		getline(fin,str);
		previoustoken=str;
	}
	fvm<<"label "<<classname<<"."<<numline+1<<endl;
	previoustoken=str;
        space=space-2;
        printspace(space);
	labelnum+=2;
        fout<<"</ifStatement>\n";
        return false;
}
bool compilereturnStatement()
{
	cout<<"Entered returnStatement\n";
        string str;bool shouldquit=true;
        printspace(space);
        fout<<"<returnStatement>\n";
        space=space+2;
        printspace(space);
        fout<<previoustoken<<endl;
	getline(fin,str);
	previoustoken=str;
	if(str!="<symbol> ; </symbol>")
	{
		previoustoken=str;
		shouldquit=compileexpression();
		if(shouldquit==true)
		{
			return true;
		}
		fvm<<"return\n";
	}
	else
	{
		fvm<<"push constant 0\n"<<"return\n";
	}
	printprevtok;
        space=space-2;
        printspace(space);
        fout<<"</returnStatement>\n";
        return false;
}
bool compilevarDec()
{
	cout<<"Entered varDec\n";
	string str;
	printspace(space);
	fout<<"<varDec>\n";
	space=space+2;
	printspace(space);
	fout<<previoustoken<<endl;
	getline(fin,str);
	type=extracttype(str);
	if(istype(str)!=false)
	{

		ferr<<"Syntax error: type expected\n";
		return true;
	}
	printspace(space);
	fout<<str<<endl;
	getline(fin,str);
	name=extractname(str);
	if(str.find("<identifier>")!=0)
	{
		ferr<<"Syntax error: identifier expected\n";
		return true;
	}
	printspace(space);
	fout<<str<<endl;
	srst.add(name,"local",type);
	getline(fin,str);
	while(str=="<symbol> , </symbol>")
	{
		printspace(space);
		fout<<str<<endl;
		getline(fin,str);
		name=extractname(str);
		srst.add(name,"local",type);
		if(str.find("<identifier>")!=0)
	        {
        	        ferr<<"Syntax error: identifier expected\n";
               		 return true;
       		 }
       		printspace(space);
        	fout<<str<<endl;
	        getline(fin,str);
	}
	if(str!="<symbol> ; </symbol>")
	{
		ferr<<"Syntax error: ; expected\n";
		return true;
	}
	printspace(space);
	fout<<str<<endl;
	space=space-2;
	printspace(space);
	fout<<"</varDec>\n";
	return false;
}
bool compilesubroutineBody()
{
	cout<<"Entered compilesubroutineBody\n";
	string str;bool shouldquit;int stattype;
	printspace(space);
        fout<<"<subroutineBody>\n";
        space=space+2;
        printspace(space);
        fout<<previoustoken<<endl;
	getline(fin,str);
	while(str=="<keyword> var </keyword>")
	{
		previoustoken=str;
		shouldquit=compilevarDec();
		if(shouldquit==true)
			return true;
		getline(fin,str);
	}
	previoustoken=str;
	fvm<<"function "<<classname<<"."<<srst.subroutinename<<" "<<srst.localcount<<endl;
	if(subroutinetype=="constructor")
	{
		fvm<<"push constant "<<cst.fieldcount<<endl;
		fvm<<"call Memory.alloc 1\n"<<"pop pointer 0\n";
	}
	if(subroutinetype=="method")
	{
		fvm<<"push argument 0\n"<<"pop pointer 0\n";
	}
	shouldquit=compileStatements();
	space=space-2;
	printspace(space);
	fout<<"</subroutineBody>\n";
	return false;
}	
bool compilesubroutineDec()
{
	cout<<"Entered compilesubroutineDec\n";
	bool shouldquit=true;
	string str;
	printspace(space);
	fout<<"<subroutineDec>\n";
	space=space+2;
	printprevtok;
	srst.init();
	getline(fin,str);
	if(istype(str)!=false&&str!="<keyword> void </keyword>")
	{
		ferr<<"Syntax error: Return type expected\n";
		return true;
	}
	printstr;
	type=extracttype(str);
	srst.setreturn(type);
	//subroutinetype=type;
	getline(fin,str);
	if(str.find("<identifier>")!=0)
	{
		ferr<<"Syntax error: Identifier expected\n";
		return true;
	}
	printstr;
	name=extractname(str);
	getline(fin,str);
	srst.set(name);
	if(subroutinetype=="method")
	{
		srst.add("this","argument",classname);
	}
	if(str!="<symbol> ( </symbol>")
	{
		ferr<<"Syntax errror: ( expected\n";
		return true;
	}
	printstr;
	shouldquit=compileparameterlist();
	printprevtok;
	if(shouldquit==true)
		return true;
	getline(fin,str);
	if(str!="<symbol> { </symbol>")
	{
		ferr<<"Syntax error: { expected\n";
		return true;
	}
	previoustoken=str;
	shouldquit=compilesubroutineBody();
	if(shouldquit==true)
		return true;
	space-=2;
	printspace(space);
	srst.print();
	fout<<"</subroutineDec>\n";
	return false;
}
bool compileclass()
{
	cout<<"Entered compileclass\n";
       	string str,word;
	printspace(space);
	fout<<"<class>\n";
	space=space+2;
	printspace(space);
	fout<<previoustoken<<endl;
       	getline(fin,str); //Should read <identifier>
	bool shouldquit=true;
       	if(str.find("<identifier>")!=0)
	{
		ferr<<"Syntax error: identifier expected\n";
		return true;
	}
	printstr;
	classname=extractname(str);
	cst.init();
	cst.set(classname);	
	getline(fin,str); //Should read {
	if(str!="<symbol> { </symbol>")
	{
		ferr<<"Syntax error: { expected\n";
		return true;
	}
	printspace(space);
	fout<<str<<endl;	
	getline(fin,str);
	while(str=="<keyword> static </keyword>"||str=="<keyword> field </keyword>")
	{
		previoustoken=str;
		if(str=="<keyword> static </keyword>")
		shouldquit=compileclassvarDec(0);
		else
		shouldquit=compileclassvarDec(1);	
		if(shouldquit==true)
			return true;
		getline(fin,str);
		previoustoken=str;
	}
	while(str=="<keyword> constructor </keyword>"||str=="<keyword> function </keyword>"||str=="<keyword> method </keyword>")
	{
		previoustoken=str;
		if(str=="<keyword> constructor </keyword>")
		subroutinetype="constructor";
		else if(str=="<keyword> function </keyword>")
		subroutinetype="function";
		else
		subroutinetype="method";		
		shouldquit=compilesubroutineDec();
		if(shouldquit==true)
			return true;
		getline(fin,str);
		previoustoken=str;

	}
	space-=2;
	printspace(space);
	fout<<"  ";
	printprevtok;
	cst.print();
	cst.del();
	fout<<"</class>\n";
	return false;
}
int main(int argc,char **argv)
{
	int num;bool checkagain;bool shouldprocess=true;bool t;string partofstring,endstring;bool shouldquit=true;string fname,str,word;
	for(num=2;num<=(argv[1][0]-48)+1;num++)
	{
	fname=string(argv[num]);shouldprocess=true;shouldquit=true;
	initialise(&fname);string b;
	fin.open((fname+".jack").c_str());
	fout.open((fname+"T.xml").c_str());
	ferr.open((fname+".err").c_str());
	fout<<"<tokens>\n";
	while(getline(fin,str))
	{
      		str=parse(str);
		checkagain=true;
		if(shouldprocess==false)
		{
			
			t=endmultcomm(&str);
			if(t==true)
			{shouldprocess=true;checkagain=false;}
			else
			str.erase();
		}
		t=startmultcomm(&str);	
		if(shouldprocess==true)
		{	
			partofstring=stringcons(&str,&b); 
			str=addspace(str); 
			stringstream iss(str);
			while(iss>>word)
			{
				shouldquit=tok(word);
				if(shouldquit==false)
					continue;
			}
			if(partofstring!="")
			{
				fout<<"<stringConstant> "<<partofstring<<" </stringConstant>\n";
				b=addspace(b);
				stringstream iss(b);
				while(iss>>word)
				{
					shouldquit=tok(word);
					if(shouldquit==false)
						continue;
				}
			}	
			}
		if(t==true&&checkagain==true)
                {
                  shouldprocess=false;
		}
	}

	fout<<"</tokens>\n";
	fin.close();	
	fout.close();
	ferr.close();
	}
	for(num=2;num<=(argv[1][0]-48)+1;num++)
	{
		string fname(argv[num]);shouldprocess=true;shouldquit=true;
	        initialise(&fname);
	        fin.open((fname+"T.xml").c_str());
	        fout.open((fname+".xml").c_str());
	        ferr.open((fname+".err").c_str(),fstream::app);
		fvm.open((fname+".vm").c_str());
		getline(fin,str); // Read <tokens>
		getline(fin,str); // Read <class>
		shouldquit=false;space=0;
		if(str=="<keyword> class </keyword>")
		{
			previoustoken=str;
			shouldquit=compileclass();
			if(shouldquit==true)
				continue;
		}
		else
		{
			ferr<<"Syntax error: class expected\n";
			continue;
		}	
	}
	fin.close();
	fout.close();
	ferr.close();
	fvm.close();
	return 0;
}
