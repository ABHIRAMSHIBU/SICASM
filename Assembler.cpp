/*
 * Assembler.cpp
 *
 *  Created on: Oct 1, 2018
 *      Author: abhiram
 */

#include<iostream>
#include<map>
#include<functional>
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
int main(int argc, char **argv) {
	Table t;
	t.init();
	t.PSEUDOOPTAB["RESW"]("Meh",1);
}


