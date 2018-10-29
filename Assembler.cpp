/*
 * Assembler.cpp
 *
 *  Created on: Oct 1, 2018
 *      Author: abhiram
 */

#include<iostream>
#include<map>
#include<functional>
#include<fstream>
#include <string>
//#include <boost/algorithm/string/replace.hpp>
//#include<boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string.hpp>
#include <sstream>
using namespace std;
bool VERBOSE=false;
//typedef function<void(string,int)> fun;
class Table{
public:
	map< string, string > OPTAB;
//	map< string, fun > PSEUDOOPTAB;
	static int RESW(string oper){
		stringstream temp;
		temp<<oper;
		int pc=0;
		temp>>pc;
		pc=3*pc;
		return pc;
	}
	static int WORD(string oper){
		return 3;
	}
	static int RESB(string oper){
		stringstream temp;
		temp<<oper;
		int pc=0;
		temp>>pc;
		return pc;
	}
	static int BYTE(string oper){
		if(oper.c_str()[0]=='C'){
			if(VERBOSE)
				cout<<"Abhiram Debug code C:"<<oper.substr(1,oper.length()-2)<<endl;
			return oper.length()-3;
		}
		else{
			if(VERBOSE)
				cout<<"Abhiram Debug code X:"<<oper.substr(2,oper.length()-2)<<endl;
			if((oper.length()-3)%2!=0){
				return (oper.length()-3+1)/2;
			}
			else{
				return (oper.length()-3)/2;
			}
		}
	}
	void init(){
//		PSEUDOOPTAB["RESW"]=RESW;
//		PSEUDOOPTAB["WORD"]=WORD;
//		PSEUDOOPTAB["RESB"]=RESB;
//		PSEUDOOPTAB["BYTE"]=BYTE;
		OPTAB["ADD"]="18";
		OPTAB["AND"]="40";
		OPTAB["COMP"]="28";
		OPTAB["DIV"]="24";
		OPTAB["J"]="3C";
		OPTAB["JEQ"]="30";
		OPTAB["JGT"]="34";
		OPTAB["JLT"]="38";
		OPTAB["JSUB"]="48";
		OPTAB["LDA"]="00";
		OPTAB["LDB"]="68";
		OPTAB["LDCH"]="50";
		OPTAB["LDL"]="08";
		OPTAB["LDX"]="04";
		OPTAB["LPS"]="D0";
		OPTAB["MUL"]="20";
		OPTAB["OR"]="44";
		OPTAB["RD"]="D8";
		OPTAB["RSUB"]="4C";
		OPTAB["SSK"]="EC";
		OPTAB["STA"]="0C";
		OPTAB["STB"]="78";
		OPTAB["STCH"]="54";
		OPTAB["STI"]="D4";
		OPTAB["STL"]="14";
		OPTAB["STSW"]="E8";
		OPTAB["STX"]="10";
		OPTAB["SUB"]="1C";
		OPTAB["TD"]="E0";
		OPTAB["TIX"]="B8";
		OPTAB["WD"]="DC";
	}
};
class fileHandler{
private:
	fstream f;
	bool backEn;
	int prevLocation;
public:
	bool eof;
	int location;
	int line;
	bool w;
	fileHandler(string name,char operation){
		if(operation=='r'){
			f.open(name.c_str(),ios::in);
			w=false;
		}
		if(operation=='w'){
			f.open(name.c_str(),ios::out);
			w=true;
		}
		eof=f.eof();
		location=f.tellg();
		line=0;
		prevLocation=0;
		backEn=true;
	}
	string readLine(){
		char buff[1];
		string data="";
		while(!f.eof() && buff[0]!='\n'){
			f.read(buff,1);
			data+=buff[0];
		}
		prevLocation=location;
		location=f.tellg();
		eof=f.eof();
		line++;
		backEn=true;
		return data;
	}
	void print(string data){
		f.write(data.c_str(),data.length());
		f.flush();  //Automatic flush not working for some reason.
	}
	void write(string data){
		print(data);
	}
	void println(string data){
		f.write((data+"\n").c_str(),(data+"\n").length());
		f.flush(); //Automatic flush not working for some reason.
	}
	void writeln(string data){
		println(data);
	}
	string front(){
		return		 readLine();
	}
	string next(){
		return readLine();
	}
	bool previous(){
		return back();
	}
	bool back(){
		if(backEn){
			f.seekg(prevLocation);
			return 0;
		}
		else{
			return 1;
		}
		backEn=false;
	}
	void seekLine(int lineNo){
		f.seekg(0);
		for(int i=lineNo;i>0;i++){
			readLine();
		}
	}
	void close(){
		f.close();
	}
	~fileHandler(){
		if(VERBOSE)
			cout<<"Destructor";
//		f.flush();
//		f.close();
	}
};

string strip(string line){
	boost::replace_all(line,"\t"," ");
	boost::replace_all(line,"\n","");
	return line;
}
char *tocharArray(string line){
	char *array=new char(line.length()+1);
	for(int i=0;i<(int)line.length();i++){
		array[i]=line[i];
	}
	array[line.length()]='\0';
	return array;
}
map< string, string > SYMTAB;
class processASM{
	string fileName;

public:
	processASM(string fileName){
		this->fileName=fileName;
	}
	static void process(processASM p,bool verbose){
		VERBOSE=verbose;
		string intermediate_file_name=p.fileName+"-Intermediate";
		string symbol_table_file_name=p.fileName+"-Symbol_Table";
		fileHandler f(p.fileName,'r');
		fileHandler fi(intermediate_file_name,'w'); //fi = File Intermediate
		fileHandler fst(symbol_table_file_name,'w');//fst- File Symbol Table

		int PC=0;
		string hex_pc="";
		bool start=true;
		string pgm_name;
		int starting_address=0;
		int program_length;
		while(true){
			stringstream hex_pc_ss;
			string line=f.readLine();
			if(f.eof){
				break;
			}
			std::size_t found = line.find(".");
			if (found != string::npos) //Check if its a comment
			    continue;
			if(verbose)
					cout<<"Line is "<<line<<endl;
			line=strip(line);
			if(line==""){
				continue;
			}
			line="f"+line+" f";
			string label,operand,opcode;
			label=string(strtok(tocharArray(line.c_str())," "));
			boost::replace_all(label,"f","");
			opcode=strtok(NULL," ");
			operand=strtok(NULL," ");
			boost::replace_all(opcode,"f","");
			boost::to_upper(label);
			boost::to_upper(operand);
			boost::to_upper(opcode);
			if(verbose)
				cout<<"Label:"<<label<<" opcode:"<<opcode<<" operand "<<operand<<endl;

			if(start){//checking for the START label
				if(opcode=="START"){
					if(verbose)
						cout<<"START LABEL IDENTIFIED"<<endl;
					stringstream temp;
					temp<<hex<<operand;
					temp>>PC;
					if(verbose)
						cout<<"PC="<<PC<<endl;
					fi.println(temp.str()+"\t"+opcode+"\t"+operand);
					pgm_name=label;
					starting_address=PC;
				}
			}
			else{ //if not 'START' write to intermediate file the split line with the current PC value
				if(verbose)
					cout<<"PC="<<PC<<endl;
				hex_pc_ss<<hex<<PC;
				fi.println(hex_pc_ss.str()+"\t"+opcode+"\t"+operand);
			}
			//code to create SYMBTAB
			if(label=="" && verbose){
				cout<<"NO LABEL HERE"<<endl;
			}

			if(SYMTAB.find(label)==SYMTAB.end() && label!="" && !start ){
				SYMTAB[label]=PC;
				fst.println(label+"\t"+hex_pc_ss.str());
			}

			if(!start){
				if(opcode=="RESW"){
					PC+=Table::RESW(operand);
				}
				else if(opcode=="RESB"){
					PC+=Table::RESB(operand);
				}
				else if(opcode=="WORD"){
					PC+=Table::WORD(operand);
				}
				else if(opcode=="BYTE"){
					PC+=Table::BYTE(operand);
				}
				else{
					PC+=3;
				}
			}
			start=false;


//			cout<<what.str();
//			cout<<"label - "<<label<<endl;
//			cout<<"operand - "<<operand<<endl;
//			cout<<"opcode - "<<opcode<<endl;
		}
		//converting length of the program into hex
		program_length=PC-starting_address-3;
		stringstream hex_program_length;
		hex_program_length<<hex<<program_length;

		cout<<"Length of the Assembly program in hex is "<<hex_program_length.str()<<endl;
		fi.close();
	}
};
int main(int argc, char **argv) {
	Table t;
	t.init();
	if(argc==1){
		processASM p("add.asm");
		processASM::process(p,false);
	}
	if(argc==2){
		processASM p(argv[1]);
		processASM::process(p,false);
	}


}
