#include <fstream>
#include <string>
#include <iostream>
#include <iomanip> 
#include"doToken.h"
#include"doGrammerll1.h"
#include"doSemantic.h"
using namespace std;
int main()
{
	ifstream in("in.txt", ios::in);
	istreambuf_iterator<char> beg(in), end;
	string strdata(beg, end);//或者string st;st.assign(beg,end);
	cout << strdata << endl;
	//in.close();

	bool flag = true;
	Initsym_lex();
	cout << "——————词法分析开始——————" << endl;
	if (tok.getTokenlist(strdata)) {
		for (int i = 0; i < tok.tokens.size(); i++) {// <<setw(10)<<asd[i] 
			cout << i << "  " << tok.tokens[i].line << setw(20) << lexName[tok.tokens[i].lex] << setw(20) << tok.tokens[i].sem << endl;
		}
	}
	else flag = false;
	cout << "——————词法分析结束——————" << endl;
	cout << endl;
	if (flag) {
		cout << "——————语法分析开始——————" << endl;
		gra1.Init_table();
		if (gra1.getTree()) {
			cout << "            没有语法错误" << endl;
			gra1.drawtree(gra1.root, 0);
		}
		else flag = false;
		cout << "——————语法分析结束——————" << endl;
		cout << endl;
	}
	if (flag) {
		Semantic a;
		if (flag && a.analyze(gra1.root)) {
			cout << "——————语义分析开始——————" << endl;
			cout << "            没有语义错误 " << endl;
			cout << "——————语义分析结束——————" << endl;
		}
	}
	return 0;
}