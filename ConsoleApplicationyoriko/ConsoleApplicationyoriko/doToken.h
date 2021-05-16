#pragma once
#include<string>
#include<iostream>
#include<map>
#include<vector>
using namespace std;
//Token的种类
typedef enum
{
    /*薄记单词符号*/
    ENDFILE, ERROR,
    /*保留字*/
    PROGRAM, PROCEDURE, TYPE, VAR, IF,
    THEN, ELSE, FI, WHILE, DO,
    ENDWH, BEGIN, END, READ, WRITE,
    ARRAY, OF, RECORD, RETURN, INTEGER, CHAR,
    /*类型*/
    INTEGER_T, CHAR_T,
    /*多字符单词序号*/
    ID, INTC, CHARC, INTC_VAL, CHARC_VAL,
    /*特殊符号*/
    ASSIGN, EQ, LT, PLUS, MINUS,
    TIMES, DIVIDE, LPAREN, RPAREN, DOT,//÷书上是OVER
    COLON, SEMI, COMMA, LMIDPAREN, RMIDPAREN,
    UNDERRANGE,

    Program, ProgramHead, ProgramName, DeclarePart,
    TypeDec, TypeDeclaration, TypeDecList, TypeDecMore,
    TypeId, TypeName, BaseType, StructureType,//TypeName其实真正应该是TypeDef
    ArrayType, Low, Top, RecType,
    FieldDecList, FieldDecMore, IdList, IdMore,
    VarDec, VarDeclaration, VarDecList, VarDecMore,
    VarIdList, VarIdMore, ProcDec, ProcDeclaration,
    ProcDecMore, ProcName, ParamList, ParamDecList,
    ParamMore, Param, FormList, FidMore,
    ProcDecPart, ProcBody, ProgramBody, StmList,
    StmMore, Stm, AssCall, AssignmentRest,
    ConditionalStm, StmL, LoopStm, InputStm,
    InVar, OutputStm, ReturnStm, CallStmRest,
    ActParamList, ActParamMore, RelExp, OtherRelE,
    Exp, OtherTerm, Term, OtherFactor,
    Factor, Variable, VariMore, FieldVar,
    FieldVarMore, CmpOp, AddOp, MultOp
}LexType;

map<LexType, string> lexName = { {PROGRAM, "PROGRAM"},
                                  {TYPE, "TYPE"},
                                  {VAR, "VAR"},
                                  {PROCEDURE, "PROCEDURE"},
                                  {BEGIN, "BEGIN"},
                                  {END, "END"},
                                  {ARRAY, "ARRAY"},
                                  {OF, "OF"},
                                  {RECORD, "RECORD"},
                                  {IF, "IF"},
                                  {THEN, "THEN"},
                                  {ELSE, "ELSE"},
                                  {FI, "FI"},
                                  {WHILE, "WHILE"},
                                  {DO, "DO"},
                                  {ENDWH, "ENDWH"},
                                  {READ, "READ"},
                                  {WRITE, "WRITE"},
                                  {RETURN, "RETURN"},
                                  {INTEGER, "INTEGER"},
                                  {CHAR, "CHAR"},
                                  {ID, "ID"},
                                  {INTC, "INTC"},
                                  {CHARC, "CHAR"},
                                  {ASSIGN, "ASSIGN"},
                                  {EQ, "EQ"},
                                  {LT, "LT"},
                                  {PLUS, "PLUS"},
                                  {MINUS, "MINUS"},
                                  {TIMES, "TIMES"},
                                  {DIVIDE, "DIVIDE"},
                                  {LPAREN, "LPAREN"},
                                  {RPAREN, "RPAREN"},
                                  {DOT, "DOT"},
                                  {COLON, "COLON"},
                                  {SEMI, "SEMI"},
                                  {COMMA, "COMMA"},
                                  {LMIDPAREN, "LMIDPAREN"},
                                  {RMIDPAREN, "RMIDPAREN"},
                                  {UNDERRANGE, "UNDERRANGE"},
                                  {ENDFILE, "EOF"},
                                  {ERROR, "ERROR"} };
/*定义保留字数字常量MAXRESERVED为21*/
#define MAXRESERVED 21
//???
map<string, LexType>sym_lex;
//下面这个可能会有问题
void Initsym_lex() {
    sym_lex["("] = LPAREN;
    sym_lex[")"] = RPAREN;
    sym_lex["["] = LMIDPAREN;//这里更加让我确定了书和资料总有一个有病！
    sym_lex["]"] = RMIDPAREN;
    sym_lex[","] = COMMA;
    sym_lex[";"] = SEMI;
    sym_lex["+"] = PLUS;
    sym_lex["-"] = MINUS;
    sym_lex["*"] = TIMES;
    sym_lex["/"] = DIVIDE;
    sym_lex["<"] = LT;
    sym_lex["="] = ASSIGN;
}
vector<string> asd;

struct Token {
    int line;
    LexType lex;
    string sem;
};
class Tokens {
public:
    Tokens();
    bool getTokenlist(string str);
    void addToken(LexType lex, string sem);
    string readStr();
    bool isNum(string str);
    bool isString(string str);//保留字
    bool isSeparator(char ch);//判断分隔符
    string str_lex(string x);
    string readIn;//读入的内容
    vector<Token> tokens;
    int flag;
    int line;//当前处理的地方
    LexType lex;
    string sem;
};

bool Tokens::getTokenlist(string str) {
    bool isError = false;
    string s;//
    for (int i = 0; i < str.size(); i++) {
        if (str[i] != ' ' && str[i] != '\n' && str[i] != '\t' && !isSeparator(str[i])) {
            s.push_back(str[i]);
        }//除了分隔符要处理
        else if(!s.empty()){//lex
         //   cout << s << endl;
            if (isNum(s)) {
                lex = INTC;//数字
            //    cout << "加了一个数字" +s<< endl;
                asd.push_back(s);
                addToken(lex, s);//输出这个
            }
            else if (isString(s)) {//保留字//标识符
                string str2 = str_lex(s);
            //    cout << str2 << " ";
                if (str2 != "ID") {
                    sem = "NULL";
                }
                else { sem = s; }
 //               cout << "加了一个字符串"+s << endl;
           //     cout << s << endl;
                asd.push_back(s);
                addToken(lex, sem);
            }
            else {
                //出现了既不是字符串也不是数字串的东西//出错
                cout << line<<":出现了既不是字符串也不是数字串的东西:"<<s << endl;
                isError = true;
            }
            s.clear();
        }
        if (str[i] == ':') {
            if ((i + 1) != str.length() && str[i + 1] == '=') {
//                cout << "加了一个:=" << endl;
                asd.push_back(":=");
                addToken(ASSIGN, "NULL");
                i++;
                continue;
            }
            else {
                cout << "单独出现：" << endl;
            }
        }
        else if (str[i] == '.') {
            if ((i + 1) != str.length() && str[i + 1] == '.') {
 //               cout << "加了一个.." << endl;
                asd.push_back("..");
                addToken(UNDERRANGE, "NULL");//emmmm
                i++;
                continue;
            }
            else {
                if ((i + 1) != str.length()) {
                    cout <<line <<" 程序结束符以及出现，但是后面还有程序"<<endl;
                    isError = true;
                }
            }
        }
        else if (str[i] == '{') {//注释内容
            while (str[i] != '}') {
                i++;
            }
            s.clear();
        }//放在最后 
        else if (isSeparator(str[i])) {
            string str2;
            str2 += str[i];
            lex = sym_lex[str2];
            asd.push_back(str2);
            addToken(lex, "NULL");
        }
           
        if (str[i] == '\n') {
            line++;
        }
    }
    if (str[str.size() - 1] == '.') {
        addToken(DOT, "NULL");
    }
    else {
        cout << "没有结束符号" << endl;
        isError = true;
    }
    return !isError;
}
void Tokens::addToken(LexType lex, string sem) {
    struct Token tok;
    tok.lex = lex;
    tok.sem = sem;
    tok.line = line;
    tokens.push_back(tok);
}
Tokens::Tokens() {
    flag = 0;
    line = 0;
}



//判断是不是数字
bool Tokens::isNum(string str) {
    if (str.length() == 1 && !isdigit(str[0])) {
        return false;
    }
    if (str[0] == '0') {
        if (str.length() == 1) return true;
        else return false;
    }
    for (int i = 0; i < str.length(); i++) {
        if (!isdigit(str[i])) {
            return false;
        }
    }
    return true;
}
//判断标识符
bool Tokens::isString(string str) {//字母+数字
    if (!isalpha(str[0])) {
        return false;
    }
    for (int i = 1; i < str.length(); i++) {
        if (!isalnum(str[i])) {
            return false;
        }
    }
    return true;
}
//判断是不是分隔符(另外还有:=,..
bool Tokens::isSeparator(char ch) {
    if (ch == '(' || ch == ')' || ch == '[' || ch == ']' || ch == '.' || ch == ',' || ch == ':' || ch == ';')
        return true;
    if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '<' || ch == '=')
        return true;
    if (ch == '{' || ch == '}') {
        return true;
    }
    return false;

}

string Tokens::str_lex(string x) {
    if (x == "program") { lex = PROGRAM; return "PROGRAM"; }
    else if (x == "type") { lex = TYPE; return "TYPE"; }
    else if (x == "var") { lex = VAR; return "VAR"; }
    else if (x == "procedure") { lex = PROCEDURE; return "PROCEDURE"; }
    else if (x == "begin") { lex = BEGIN; return "BEGIN"; }
    else if (x == "end") { lex = END; return "END"; }
    else if (x == "array") { lex = ARRAY; return "ARRAY"; }
    else if (x == "of") { lex = OF; return "OF"; }
    else if (x == "record") { lex = RECORD; return "RECORD"; }
    else if (x == "if") { lex = IF; return "IF"; }
    else if (x == "then") { lex = THEN; return "THEN"; }
    else if (x == "else") { lex = ELSE; return "ELSE"; }
    else if (x == "fi") { lex = FI; return "FI"; }
    else if (x == "while") { lex = WHILE; return "WHILE"; }
    else if (x == "do") { lex = DO; return "DO"; }
    else if (x == "endwh") { lex = ENDWH; return "ENDWH"; }
    else if (x == "read") { lex = READ; return "READ"; }
    else if (x == "write") { lex = WRITE; return "WRITE"; }
    else if (x == "return") { lex = RETURN; return "RETURN"; }
    else if (x == "integer") { lex = INTEGER; return "INTEGER"; }
    else if (x == "char") { lex = CHAR; return "CHAR"; }
    else { lex = ID; return "ID"; }
}


Tokens tok;
