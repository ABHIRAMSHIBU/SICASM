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
using namespace std;
typedef function<void(string,int)> fun;
class Table{
public:
	map< string, string > OPTAB;
	map< string, fun > PSEUDOOPTAB;
	static void RESW(string line, int number){
		cout<<"TEST CODE"<<endl;
	}
	static void WORD(string line, int number){

	}
	static void RESB(string line, int number){

	}
	static void BYTE(string line, int number){

	}
	void init(){
		PSEUDOOPTAB["RESW"]=RESW;
		PSEUDOOPTAB["WORD"]=WORD;
		PSEUDOOPTAB["RESB"]=RESB;
		PSEUDOOPTAB["BYTE"]=BYTE;
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
	fileHandler(string name,char operation){
		if(operation=='r'){
			f.open(name.c_str(),ios::in);
		}
		if(operation=='w'){
			f.open(name.c_str(),ios::out);
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
	}
	void write(string data){
		print(data);
	}
	void println(string data){
		f.write((data+"\n").c_str(),(data+"\n").length());
	}
	void writeln(string data){
		println(data);
	}
	string front(){
		return readLine();
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
};
class processASM{
	string fileName;
public:
	processASM(string fileName){
		this->fileName=fileName;
	}
	static void process(processASM p){
		fileHandler f(p.fileName,'r');
		cout<<f.readLine();
	}
};
int main(int argc, char **argv) {
	Table t;
	t.init();
	if(argc>1){
		processASM p(argv[1]);
		processASM::process(p);
	}
}


