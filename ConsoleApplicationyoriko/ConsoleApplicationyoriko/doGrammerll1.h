#pragma once
#include"doToken.h"
#include "stdlib.h"
#include"util.h"
#include<stack>
//���ռ���non-terminal






using namespace std;


class doGrammerll1
{
public:
	bool getTree();

	void Init_table();//��ʼ�����ű�
	void allProcess(int id);//104������Ĵ������
	string tree_out(TreeNode* p);//����﷨��
	string get_names(const TreeNode* node);//��ȡ���ڵ�����
	int Priosity(LexType  op);//�жϲ�����������˳��
	void drawtree(TreeNode*, int level);//����﷨��

	stack<LexType> symbal_stack;//����ջ//��ԭ���е����
	stack<TreeNode**> syntaxtree_stack;//�﷨��ջ
	stack<TreeNode*> op_stack;//������ջ
	stack<TreeNode*> num_stack;//������ջ
	TreeNode* root;
	//������
	map<pair<LexType, LexType>, int> table;
};

bool getExpResult = true;
int  expflag = 0;
int  getExpResult2 = false;
void doGrammerll1::Init_table() {
	table = {
		{{Program,PROGRAM},1},
		{{ProgramHead,PROGRAM},2},
		{{ProgramName,ID},3},
		{{DeclarePart,TYPE},4},
		{{DeclarePart,VAR},4},
		{{DeclarePart,PROCEDURE},4},
		{{DeclarePart,BEGIN},4},
		{{TypeDec,VAR},5},
		{{TypeDec,PROCEDURE},5},
		{{TypeDec,BEGIN},5},
		{{TypeDec,TYPE},6},
		{{TypeDeclaration,TYPE},7},
		{{TypeDecList,ID},8},
		{{TypeDecMore,VAR},9},
		{{TypeDecMore,PROCEDURE},9},
		{{TypeDecMore,BEGIN},9},
		{{TypeDecMore,ID},10},
		{{TypeId,ID},11},
		{{TypeName,INTEGER},12},
		{{TypeName,CHAR},12},
		{{TypeName,ARRAY},13},
		{{TypeName,RECORD},13},
		{{TypeName,ID},14},
		{{BaseType,INTEGER},15},
		{{BaseType,CHAR},16},
		{{StructureType,ARRAY},17},
		{{StructureType,RECORD},18},
		{{ArrayType,ARRAY},19},
		{{Low,INTC},20},
		{{Top,INTC},21},
		{{RecType,RECORD},22},
		{{FieldDecList,INTEGER},23},
		{{FieldDecList,CHAR},23},
		{{FieldDecList,ARRAY},24},
		{{FieldDecMore,END},25},
		{{FieldDecMore,INTEGER},26},
		{{FieldDecMore,CHAR},26},
		{{FieldDecMore,ARRAY},26},
		{{IdList,ID},27},
		{{IdMore,SEMI},28},
		{{IdMore,COMMA},29},
		{{VarDec,PROCEDURE},30},
		{{VarDec,BEGIN},30},
		{{VarDec,VAR},31},
		{{VarDeclaration,VAR},32},
		{{VarDecList,INTEGER},33},
		{{VarDecList,CHAR},33},
		{{VarDecList,ARRAY},33},
		{{VarDecList,RECORD},33},
		{{VarDecList,ID},33},
		{{VarDecMore,PROCEDURE},34},
		{{VarDecMore,BEGIN},34},
		{{VarDecMore,INTEGER},35},
		{{VarDecMore,CHAR},35},
		{{VarDecMore,ARRAY},35},
		{{VarDecMore,RECORD},35},
		{{VarDecMore,ID},35},
		{{VarIdList,ID},36},
		{{VarIdMore,SEMI},37},
		{{VarIdMore,COMMA},38},
		{{ProcDec,BEGIN},39},
		{{ProcDec,PROCEDURE},40},
		{{ProcDeclaration,PROCEDURE},41},
		{{ProcDecMore,PROCEDURE},41},
		{{ProcDecMore,BEGIN},42},
		{{ProcName,ID},44},





		{{ParamList,RMIDPAREN},45},
		{{ParamList,INTEGER},46},
		{{ParamList,CHAR},46},
		{{ParamList,ARRAY},46},
		{{ParamList,RECORD},46},
		{{ParamList,VAR},46},
		{{ParamList,ID},46},
		{{ParamDecList,INTEGER},47},
		{{ParamDecList,CHAR},47},
		{{ParamDecList,ARRAY},47},
		{{ParamDecList,RECORD},47},
		{{ParamDecList,VAR},47},
		{{ParamDecList,ID},47},
		{{ParamMore,SEMI},49},
		{{ParamMore,RPAREN},48},
		{{Param,INTEGER},50},
		{{Param,CHAR},50},
		{{Param,ARRAY},50},
		{{Param,RECORD},50},
		{{Param,END},51},
		{{Param,ID},50},

		{{Param,VAR},51},
		{{FormList,ID},52},
		{{FidMore,SEMI},53},

		{{FidMore,COMMA},54},
		{{FidMore,RPAREN},53},

		{{ProcDecPart,TYPE},55},
		{{ProcDecPart,VAR},55},
		{{ProcDecPart,PROCEDURE},55},

		{{ProcDecPart,BEGIN} , 55},

		{{ProcBody,BEGIN},56},

		{{ProgramBody,BEGIN},57},
		{{StmList,IF},58},
		{{StmList,WHILE},58},

		{{StmList,READ},58},
		{{StmList,WRITE},58},
		{{StmList,RETURN},58},
		{{StmList,ID},58},
		{{StmMore,END},59},

		{{StmMore,ELSE},59},
		{{StmMore,FI},59},
		{{StmMore,ENDWH},59},
		{{StmMore,SEMI},60},

		{{Stm,IF},61},
		{{Stm,WHILE},62},
		{{Stm,READ},63},
		{{Stm,WRITE},64},
		{{Stm,RETURN},65},
		{{Stm,ID},66},






		//TODO shushang cuole
		{{AssCall,ASSIGN},67},
		{{AssCall,DOT},67},
		{{AssCall,LMIDPAREN},67},

		{{AssCall,LPAREN},68},
		{{AssignmentRest,DOT},69},
		{{AssignmentRest,LMIDPAREN},69},
		{{AssignmentRest,ASSIGN},69},
		{{ConditionalStm,IF},70},
		{{LoopStm,WHILE},71},
		{{InputStm,READ},72},
		{{InVar,ID},73},
		{{OutputStm,WRITE},74},
		{{ReturnStm,RETURN},75},
		{{CallStmRest,LPAREN},76},
		{{ActParamList,INTC},78},
		{{ActParamList,ID},78},
		{{ActParamList,LPAREN},78},
		{{ActParamList,RPAREN},77},
		{{ActParamMore,COMMA},80},
		{{ActParamMore,RPAREN},79},
		{{RelExp,INTC},81},
		{{RelExp,ID},81},
		{{RelExp,LMIDPAREN},81},
		{{OtherRelE,LT},82},
		{{OtherRelE,EQ},82},

		{{Exp,INTC},83},

		{{OtherFactor,END},87},
		{{OtherFactor,THEN},87},
		{{OtherFactor,ELSE},87},
		{{OtherFactor,FI},87},
		{{OtherFactor,DO},87},
		{{OtherFactor,ENDWH},87},
		{{OtherFactor,SEMI},87},
		{{OtherFactor,COMMA},87},
		{{OtherFactor,RPAREN},87},
		{{OtherFactor,RMIDPAREN},87},
		{{OtherFactor,LT},87},
		{{OtherFactor,EQ},87},
		{{OtherFactor,PLUS},87},
		{{OtherFactor,MINUS},87},


		{{OtherFactor,TIMES},88},
		{{OtherFactor,DIVIDE},88},







		{{Term,INTC},86},
		{{Factor,INTC},90},

		{{OtherTerm,END},84},
		{{Term,END},87},
		{{VariMore,END},93},
		{{FieldVarMore,END},97},

		{{OtherTerm,THEN},84},
		{{Term,THEN},87},
		{{VariMore,THEN},93},
		{{FieldVarMore,THEN},97},

		{{OtherTerm,ELSE},84},
		{{Term,ELSE},87},
		{{VariMore,ELSE},93},
		{{FieldVarMore,ELSE},97},

		{{OtherTerm,FI},84},
		{{Term,FI},87},
		{{VariMore,FI},93},
		{{FieldVarMore,FI},97},

		{{OtherTerm,DO},84},
		{{Term,DO},87},
		{{VariMore,DO},93},
		{{FieldVarMore,DO},97},

		{{OtherTerm,ENDWH},84},
		{{Term,ENDWH},87},
		{{VariMore,ENDWH},93},
		{{FieldVarMore,ENDWH},97},

		{{Exp,ID},83},
		{{Term,ID},86},
		{{Factor,ID},91},
		{{Variable,ID},92},
		{{FieldVar,ID},96},

		{{VariMore,DOT},95},

		{{OtherTerm,SEMI},84},
		{{Term,SEMI},87},
		{{VariMore,SEMI},93},
		{{FieldVarMore,SEMI},97},

		{{OtherTerm,COMMA},84},
		{{Term,COMMA},87},
		{{VariMore,COMMA},93},
		{{FieldVarMore,COMMA},97},

		{{Exp,LPAREN},83},
		{{Term,LPAREN},86},
		{{Factor,LPAREN},89},

		{{OtherTerm,RPAREN},84},
		{{Term,RPAREN},87},
		{{VariMore,RPAREN},93},
		{{FieldVarMore,RPAREN},97},

		{{VariMore,LMIDPAREN},94},
		{{FieldVarMore,LMIDPAREN},98},

		{{OtherTerm,RMIDPAREN},84},
		{{Term,RMIDPAREN},87},
		{{VariMore,RMIDPAREN},93},
		{{FieldVarMore,RMIDPAREN},97},

		{{OtherTerm,LT},84},
		{{Term,LT},87},
		{{VariMore,LT},93},
		{{FieldVarMore,LT},97},
		{{CmpOp,LT},99},

		{{OtherTerm,EQ},84},
		{{Term,EQ},87},
		{{VariMore,EQ},93},
		{{FieldVarMore,EQ},97},
		{{CmpOp,EQ},100},

		{{OtherTerm,PLUS},85},
		{{Term,PLUS},87},
		{{VariMore,PLUS},93},
		{{FieldVarMore,PLUS},97},
		{{AddOp,PLUS},101},

		{{OtherTerm,MINUS},85},
		{{Term,MINUS},87},
		{{VariMore,MINUS},93},
		{{FieldVarMore,MINUS},97},
		{{AddOp,MINUS},102},

		{{Term,TIMES},88},
		{{VariMore,TIMES},93},
		{{FieldVarMore,TIMES},97},
		{{MultOp,TIMES},103},

		{{Term,DIVIDE},88},
		{{VariMore,DIVIDE},93},
		{{FieldVarMore,DIVIDE},97},
		{{MultOp,DIVIDE},104},

		{{VariMore,ASSIGN},93},
		{{FieldVarMore,ASSIGN},97},


		{{CmpOp,LT},99},
		{{CmpOp,EQ},100},
		{{AddOp,PLUS},101},
		{{AddOp,MINUS},102},
		{{MultOp,TIMES},103},
		{{MultOp,DIVIDE},104},

	};
}
int pos = 0;//��ʾ��ǰ���˵ڼ���

bool doGrammerll1::getTree() {
	bool isError=false;
	symbal_stack.push(Program);
	root = newSpecNode(ProK);
	syntaxtree_stack.push(&root->child[2]);
	syntaxtree_stack.push(&root->child[1]);
	syntaxtree_stack.push(&root->child[0]);
	lineno = tok.tokens[0].line;//��¼�к�

	if (tok.tokens.size() == 0) return false;//?
	while (!symbal_stack.empty()) {//�ߵ���59��END�����滹������Ԫ��ò��
		//����ջջ��Ϊfei�ռ���
	//	cout << "top:" << symbal_stack.top() << endl;
		if (NTSet.find(symbal_stack.top()) != NTSet.end()) {
			auto ss = symbal_stack.top();
			symbal_stack.pop();
	/*		if (!ss) {
				continue;
			}*///?
			map<pair<LexType, LexType>, int>::iterator iter = table.find(pair<LexType, LexType>(ss, tok.tokens[pos].lex));
//			cout << lexName[ss] << " " << lexName[tok.tokens[pos].lex] << endl;
			if (table.cend() != iter) {//LL1����������
				allProcess(iter->second);
			}
			else {
		//		cout << lexName[ss] << " " << lexName[tok.tokens[pos].lex] << endl;
				cout << lineno <<" �����ڶ�Ӧ�Ĺ�Լ��������ʧ��" << endl;
				isError = true;
				
			}
		}
		//����ջջ��Ϊ�ռ���//???
		else if (TTSet.find(symbal_stack.top()) != TTSet.end()) {
			if (symbal_stack.top() == tok.tokens[pos].lex) {
			//	cout <<symbal_stack.top() << endl;
				symbal_stack.pop();
				pos++;
				lineno = tok.tokens[pos].line;
				
			}
			else {
				cout << "Ҳ�Ǵ��ˣ�ԭ����" <<lineno << "�У����ռ���������ûƥ����" << endl;
				pos++;
				isError = true;
			}
		}
	}
//	cout << "ƽ���������﷨����" << endl;
	return !isError;
}
int k = 0;
TreeNode* currentP;
DecKind* temp;
TreeNode* saveP;

void doGrammerll1::allProcess(int id)
{
//	cout << k++<<endl;
	if (id == 19) {
//		cout << "stop" << endl;
	}
	switch (id) {
	case 1:
	{
		symbal_stack.push(ProgramBody);
		symbal_stack.push(DeclarePart);
		symbal_stack.push(ProgramHead);
		break;
	}
	case 2:
	{
		symbal_stack.push(ProgramName);
		symbal_stack.push(PROGRAM);
		currentP = newSpecNode(PheadK);
		auto item = syntaxtree_stack.top();
		syntaxtree_stack.pop();
		(*item) = currentP;
		//ʹ����ͷ�ڵ���ڵ�Ķ��ӽڵ�ָ�����ͷ�ڵ�
		break;
	}
	case 3:
	{
		symbal_stack.push(ID);
		strcpy_s(currentP->name[0], tok.tokens[pos].sem.c_str());
		currentP->idnum++;
		break;
	}
	case 4:
	{
		symbal_stack.push(ProcDec);
		symbal_stack.push(VarDec);
		symbal_stack.push(TypeDec);

		break;
	}
	case 5:
	{
		break;
	}
	case 6:
	{
		symbal_stack.push(TypeDeclaration);
		break;
	}
	case 7://����������
	{
		symbal_stack.push(TypeDecList);
		symbal_stack.push(TYPE);

		auto t = syntaxtree_stack.top();
		syntaxtree_stack.pop();
		currentP = newSpecNode(TypeK);

		(*t) = currentP;
		syntaxtree_stack.push(&(currentP->sibling));
		syntaxtree_stack.push(&(currentP->child[0]));

		break;
	}
	case 8:
	{

		symbal_stack.push(TypeDecMore);
		symbal_stack.push(SEMI);
		symbal_stack.push(TypeName);//TypeDef//������ô����
		symbal_stack.push(ASSIGN);//
		symbal_stack.push(TypeId);

		auto t = syntaxtree_stack.top();
		syntaxtree_stack.pop();
		currentP = newSpecNode(DecK);
		(*t) = currentP;

		syntaxtree_stack.push(&(currentP->sibling));
		break;
	}
	case 9:
	{
		syntaxtree_stack.pop();
		break;
	}
	case 10:
	{
		symbal_stack.push(TypeDecList);
		break;
	}
	case 11:
	{
		symbal_stack.push(ID);
		strcpy_s(currentP->name[0], tok.tokens[pos].sem.c_str());
		currentP->idnum++;
		break;
	}
	case 12:
	{
		symbal_stack.push(BaseType);
		temp = &(currentP->kind.dec);
		break;
	}
	case 13:
	{

		symbal_stack.push(StructureType);
		break;
	}
	case 14: {

		symbal_stack.push(ID);
		currentP->kind.dec = IdK;
		strcpy_s(currentP->type_name, tok.tokens[pos].sem.c_str());
		break;
	}
	case 15: {
		symbal_stack.push(INTEGER);
		(*temp) = IntegerK;
		break;
	}
	case 16: {
		symbal_stack.push(CHAR);
		(*temp) = CharK;
		break;
	}
	case 17: {
		symbal_stack.push(ArrayType);

		break;
	}
	case 18: {
		symbal_stack.push(RecType);
		break;
	}
	case 19: {
		symbal_stack.push(BaseType);
		symbal_stack.push(OF);
		symbal_stack.push(RMIDPAREN);
		symbal_stack.push(Top);
		symbal_stack.push(UNDERRANGE);
		symbal_stack.push(Low);
		symbal_stack.push(LMIDPAREN);
		symbal_stack.push(ARRAY);

		currentP->kind.dec = ArrayK;
		temp = &currentP->attr.ArrayAttr.childtype;
		break;
	}
	case 20: {
		symbal_stack.push(INTC);
		currentP->attr.ArrayAttr.low = atoi(tok.tokens[pos].sem.c_str());
		//���ַ���ת��Ϊ���ֵĳ�����������atoi���Ұ���

		break;
	}
	case 21: {
		symbal_stack.push(INTC);
		currentP->attr.ArrayAttr.up = atoi(tok.tokens[pos].sem.c_str());
		break;
	}
	case 22: {
		symbal_stack.push(END);
		symbal_stack.push(FieldDecList);
		symbal_stack.push(RECORD);

		currentP->kind.dec = RecordK;

		saveP = currentP;

		syntaxtree_stack.push(&(currentP->child[0]));
		break;
	}
	case 23: {
		symbal_stack.push(FieldDecMore);
		symbal_stack.push(SEMI);
		symbal_stack.push(IdList);
		symbal_stack.push(BaseType);


		auto t = syntaxtree_stack.top();
		syntaxtree_stack.pop();
		currentP = newSpecNode(DecK);
		temp = &currentP->kind.dec;
		(*t) = currentP;

		syntaxtree_stack.push(&currentP->sibling);
		break;

	}
	case 24: {
		symbal_stack.push(FieldDecMore);
		symbal_stack.push(SEMI);
		symbal_stack.push(IdList);
		symbal_stack.push(ArrayType);

		TreeNode** t = syntaxtree_stack.top();
		syntaxtree_stack.pop();
		currentP = newDecNode(); /*���ɼ�¼���͵���һ���򣬲����κ���Ϣ*/
		(*t) = currentP; /*���ǵ�һ��������record���͵��ӽ��ָ��ǰ��㣬
						   ��������һ����¼���������ֵܽ��*/
		syntaxtree_stack.push(&((*currentP).sibling));

		break;
	}

	case 25:
	{
		/*����û�м�¼���͵���һ�����ˣ��ָ���ǰ��¼���ͽڵ��ָ��*/
		syntaxtree_stack.pop();
		currentP = saveP;
		break;
	}

	case 26:
	{
		symbal_stack.push(FieldDecList);

		break;
	}
	case 27:
	{
		symbal_stack.push(IdMore);
		symbal_stack.push(ID);

		/*��¼һ�����и���������������Ϣ*/
		strcpy_s(currentP->name[currentP->idnum], tok.tokens[pos].sem.c_str());
		currentP->idnum++;
		break;
	}
	case 28:
	{
		break;
	}

	case 29:
	{
		symbal_stack.push(IdList);
		symbal_stack.push(COMMA);

		break;
	}
	case 30:
	{
		break;
	}

	case 31:
	{
		symbal_stack.push(VarDeclaration);

		break;
	}
	case 32:
	{
		symbal_stack.push(VarDecList);
		symbal_stack.push(VAR);

		currentP = newVarNode();   /*����һ����־���������Ľڵ�*/
		TreeNode** t = syntaxtree_stack.top();
		syntaxtree_stack.pop();
		(*t) = currentP;
		syntaxtree_stack.push(&((*currentP).sibling));  /*ѹ��ָ����������ָ��*/
		syntaxtree_stack.push(&((*currentP).child[0])); /*ѹ��ָ���һ�����������ڵ��ָ��*/

		break;
	}
	case 33:
	{
		symbal_stack.push(VarDecMore);
		symbal_stack.push(SEMI);
		symbal_stack.push(VarIdList);
		symbal_stack.push(TypeName);

		TreeNode** t = syntaxtree_stack.top();
		syntaxtree_stack.pop();
		currentP = newDecNode();/*����һ���µ������ڵ㣬�����ʾ��������*/
		(*t) = currentP;       /*���ǵ�һ���ڵ㣬�����������ͷָ��ָ������
								 ��������ǰһ�����������ĺ��*/
		syntaxtree_stack.push(&((*currentP).sibling));

		break;
	}
	case 34:
	{
		syntaxtree_stack.pop();
		break;
	}

	case 35:
	{
		symbal_stack.push(VarDecList);
		break;

	}
	case 36:
	{
		symbal_stack.push(VarIdMore);
		symbal_stack.push(ID);

		strcpy_s(currentP->name[currentP->idnum], tok.tokens[pos].sem.c_str());
		currentP->idnum++;
		break;

	}
	case 37:
	{
		break;
	}

	case 38:
	{
		symbal_stack.push(VarIdList);
		symbal_stack.push(COMMA);

		break;
	}
	case 39:
	{
		break;
	}

	case 40:
	{
		symbal_stack.push(ProcDeclaration);

		break;
	}
	case 41:
	{
		symbal_stack.push(ProcDecMore);
		symbal_stack.push(ProcBody);
		symbal_stack.push(ProcDecPart);
		symbal_stack.push(SEMI);
		symbal_stack.push(RPAREN);
		symbal_stack.push(ParamList);
		symbal_stack.push(LPAREN);
		symbal_stack.push(ProcName);
		symbal_stack.push(PROCEDURE);


		currentP = newProcNode();
		TreeNode** t = syntaxtree_stack.top();
		syntaxtree_stack.pop();
		(*t) = currentP;

		syntaxtree_stack.push(&(currentP->sibling));

		syntaxtree_stack.push(&(currentP->child[2])); /*ָ���������*/

		syntaxtree_stack.push(&(currentP->child[1]));  /*ָ��������������*/

		syntaxtree_stack.push(&(currentP->child[0]));  /*ָ�������������*/
		break;
	}

	case 42:
	{   /*�������̽ڵ���ֵܽڵ�ָ��*/
		//syntaxtree_stack.pop();  /*Ϊ��ͳһ�����������ڵ���*/
		break;
	}

	case 43:
	{
		symbal_stack.push(ProcDeclaration);
		break;
	}

	case 44:
	{
		symbal_stack.push(ID);

		strcpy_s(currentP->name[0], tok.tokens[pos].sem.c_str());
		currentP->idnum++;
		break;
	}

	case 45:
	{
		/*�ββ���Ϊ�գ�����ָ���βε�ָ��*/
		syntaxtree_stack.pop();
		break;
	}

	case 46:
	{
		symbal_stack.push(ParamDecList);
		break;
	}
	case 47:
	{
		symbal_stack.push(ParamMore);
		symbal_stack.push(Param);
		break;
	}
	case 48:
	{
		syntaxtree_stack.pop();
		break;
	}

	case 49:
	{
		symbal_stack.push(ParamDecList);
		symbal_stack.push(SEMI);
		break;

	}
	case 50:
	{
		symbal_stack.push(FormList);
		symbal_stack.push(TypeName);

		TreeNode** t = syntaxtree_stack.top();
		syntaxtree_stack.pop();
		currentP = newDecNode();
		/*�����Ĳ���������ֵ����*/
		currentP->attr.ProcAttr.paramt = valparamType;
		(*t) = currentP;
		syntaxtree_stack.push(&(currentP->sibling));

		break;

	}
	case 51:
	{
		symbal_stack.push(FormList);
		symbal_stack.push(TypeName);
		symbal_stack.push(VAR);

		TreeNode** t = syntaxtree_stack.top();
		syntaxtree_stack.pop();
		currentP = newDecNode();
		/*�����Ĳ��������Ǳ�������*/
		currentP->attr.ProcAttr.paramt = varparamType;
		(*t) = currentP;
		syntaxtree_stack.push(&(currentP->sibling));

		break;
	}
	case 52:
	{
		symbal_stack.push(FidMore);
		symbal_stack.push(ID);

		strcpy_s(currentP->name[currentP->idnum], tok.tokens[pos].sem.c_str());
		currentP->idnum++;

		break;
	}
	case 53:
	{
		break;
	}

	case 54:
	{
		symbal_stack.push(FormList);
		symbal_stack.push(COMMA);
		break;
	}

	case 55:
	{
		symbal_stack.push(DeclarePart);
		break;
	}
	case 56:
	{
		symbal_stack.push(ProgramBody);
		break;
	}

	case 57:
	{
		symbal_stack.push(END);
		symbal_stack.push(StmList);
		symbal_stack.push(BEGIN);

		/*ע�⣬��û���������֣��򵯳����ǳ������̸��ڵ���ָ��
		  �������ֵ�ָ��child[1];�����������֣��򵯳������������ǰ
		  �����һ��������ʶ�ڵ���ֵ�ָ�룻���������������������
		  ��Ҫ�����﷨��ջ�е�һ��ָ��*/
		syntaxtree_stack.pop();

		/*����������б�ʶ�ڵ�*/
		TreeNode** t = syntaxtree_stack.top();
		syntaxtree_stack.pop();
		currentP = newStmlNode();
		(*t) = currentP;
		syntaxtree_stack.push(&(currentP->child[0]));
		break;

	}
	case 58:
	{
		symbal_stack.push(StmMore);
		symbal_stack.push(Stm);
		break;
	}
	case 59:
	{
		syntaxtree_stack.pop();
		break;
	}

	case 60:
	{
		symbal_stack.push(StmList);
		symbal_stack.push(SEMI);

		break;
	}
	case 61:
	{
		symbal_stack.push(ConditionalStm);

		currentP = newStmtNode(IfK);
		//currentP->kind.stmt=;

		TreeNode** t = syntaxtree_stack.top();
		syntaxtree_stack.pop();
		(*t) = currentP;
		syntaxtree_stack.push(&currentP->sibling);

		break;

	}
	case 62:
	{
		symbal_stack.push(LoopStm);

		currentP = newStmtNode(WhileK);
		//currentP->kind.stmt=;

		TreeNode** t = syntaxtree_stack.top();
		syntaxtree_stack.pop();
		(*t) = currentP;
		syntaxtree_stack.push(&currentP->sibling);

		break;
	}

	case 63:
	{
		symbal_stack.push(InputStm);

		TreeNode** t = syntaxtree_stack.top();
		syntaxtree_stack.pop();
		currentP = newStmtNode(ReadK);
		//currentP->kind.stmt=;
		(*t) = currentP;
		syntaxtree_stack.push(&currentP->sibling);

		break;
	}
	case 64:
	{
		symbal_stack.push(OutputStm);

		TreeNode** t = syntaxtree_stack.top();
		syntaxtree_stack.pop();
		currentP = newStmtNode(WriteK);
		//currentP->kind.stmt=;
		(*t) = currentP;
		syntaxtree_stack.push(&currentP->sibling);

		break;
	}
	case 65:
	{
		symbal_stack.push(ReturnStm);

		TreeNode** t = syntaxtree_stack.top();
		syntaxtree_stack.pop();
		currentP = newStmtNode(ReturnK);
		//currentP->kind.stmt=;
		(*t) = currentP;
		syntaxtree_stack.push(&currentP->sibling);

		break;
	}

	case 66:
	{
		symbal_stack.push(AssCall);
		symbal_stack.push(ID);

		currentP = newStmtNode(AssignK);

		/*��ֵ����󲿱����ڵ�*/
		TreeNode* t = newExpNode(VariK);
		strcpy_s(t->name[0], tok.tokens[pos].sem.c_str());
		t->idnum++;

		/*��ֵ����child[0]ָ���󲿵ı����ڵ�*/
		currentP->child[0] = t;

		TreeNode** t1 = syntaxtree_stack.top();
		syntaxtree_stack.pop();
		(*t1) = currentP;
		syntaxtree_stack.push(&currentP->sibling);

		break;
	}
	case 67:
	{
		symbal_stack.push(AssignmentRest);

		currentP->kind.stmt = AssignK;
		break;
	}
	case 68:
	{
		symbal_stack.push(CallStmRest);
		/*���̵���������Ǳ�ʶ����������ʾ������*/
		currentP->child[0]->attr.ExpAttr.varkind = IdV;

		currentP->kind.stmt = CallK;
		break;
	}
	case 69:
	{
		symbal_stack.push(Exp);
		symbal_stack.push(ASSIGN);
		symbal_stack.push(VariMore);
		/*ѹ��ָ��ֵ�Ҳ���ָ��*/
		syntaxtree_stack.push(&(currentP->child[1]));
		/*��ǰָ��ָ��ֵ��*/
		currentP = currentP->child[0];

		TreeNode* t = newExpNode(OpK);
		t->attr.ExpAttr.op = END;
		op_stack.push(t);  //������ջ��ջ�״���һ������Ĳ�������Ϊ��־
		break;
	}
	case 70:
	{
		symbal_stack.push(FI);
		symbal_stack.push(StmList);
		symbal_stack.push(ELSE);
		symbal_stack.push(StmList);
		symbal_stack.push(THEN);
		symbal_stack.push(RelExp);
		symbal_stack.push(IF);

		syntaxtree_stack.push(&(currentP->child[2]));
		syntaxtree_stack.push(&(currentP->child[1]));
		syntaxtree_stack.push(&(currentP->child[0]));

		break;
	}

	case 71:
	{
		symbal_stack.push(ENDWH);
		symbal_stack.push(StmList);
		symbal_stack.push(DO);
		symbal_stack.push(RelExp);
		symbal_stack.push(WHILE);

		syntaxtree_stack.push(&(currentP->child[1]));
		syntaxtree_stack.push(&(currentP->child[0]));

		break;
	}

	case 72:
	{
		symbal_stack.push(RPAREN);
		symbal_stack.push(InVar);
		symbal_stack.push(LPAREN);
		symbal_stack.push(READ);
		break;
	}
	case 73:
	{
		symbal_stack.push(ID);

		strcpy_s(currentP->name[0], tok.tokens[pos].sem.c_str());
		currentP->idnum++;
		break;
	}
	case 74:
	{
		symbal_stack.push(RPAREN);
		symbal_stack.push(Exp);
		symbal_stack.push(LPAREN);
		symbal_stack.push(WRITE);

		syntaxtree_stack.push(&(currentP->child[0]));

		TreeNode* t = newExpNode(OpK);
		t->attr.ExpAttr.op = END;
		op_stack.push(t);  //������ջ��ջ�״���һ������Ĳ�������Ϊ��־
		break;
	}
	case 75:
	{
		symbal_stack.push(RETURN);
		break;
	}

	case 76:
	{
		symbal_stack.push(RPAREN);
		symbal_stack.push(ActParamList);
		symbal_stack.push(LPAREN);

		syntaxtree_stack.push(&(currentP->child[1]));
		break;

	}
	case 77:
	{
		syntaxtree_stack.pop();
		break;
	}
	case 78:
	{
		symbal_stack.push(ActParamMore);
		symbal_stack.push(Exp);

		TreeNode* t = newExpNode(OpK);
		t->attr.ExpAttr.op = END;
		op_stack.push(t);  //������ջ��ջ�״���һ������Ĳ�������Ϊ��־
		break;
	}

	case 79:
	{
		break;
	}
	case 80:
	{
		symbal_stack.push(ActParamList);
		symbal_stack.push(COMMA);

		syntaxtree_stack.push(&(currentP->sibling));
		break;
	}




	/*********************���ʽ����************************/

	case 81:
	{
		symbal_stack.push(OtherRelE);
		symbal_stack.push(Exp);

		TreeNode* t = newExpNode(OpK);
		t->attr.ExpAttr.op = END;
		op_stack.push(t);  //������ջ��ջ�״���һ������Ĳ�������Ϊ��־

		getExpResult = false;
		break;
	}

	case 82:
	{
		symbal_stack.push(Exp);
		symbal_stack.push(CmpOp);

		TreeNode* currentP = newExpNode(OpK);
		currentP->attr.ExpAttr.op = tok.tokens[pos].lex;

		LexType  sTop = op_stack.top()->attr.ExpAttr.op;
		while (Priosity(sTop) >= Priosity(tok.tokens[pos].lex))
			/*���������ջ������������ȼ����ڻ���ڵ�ǰ�����Ĳ�����*/
		{
			TreeNode* t = op_stack.top();
			op_stack.pop();
			TreeNode* Rnum = num_stack.top();
			num_stack.pop();
			TreeNode* Lnum = num_stack.top();
			num_stack.pop();
			t->child[1] = Rnum;
			t->child[0] = Lnum;
			num_stack.push(t);

			sTop = op_stack.top()->attr.ExpAttr.op;
		}

		op_stack.push(currentP);
		/*�������ϵ�������Ҳ��ı��ʽʱ��Ҫ���﷨��ջ����
		  ����getExpResultΪ��*/
		getExpResult = true;
		break;
	}

	case 83:
	{
		symbal_stack.push(OtherTerm);
		symbal_stack.push(Term);
		break;
	}

	case 84:
	{
		if ((tok.tokens[pos].lex == RPAREN) && (expflag != 0))
			//˵����ǰ�������Ǳ��ʽ�е�һ����
		{
			while (op_stack.top()->attr.ExpAttr.op != LPAREN)
			{
				TreeNode* t = op_stack.top();
				op_stack.pop();
				TreeNode* Rnum = num_stack.top();
				num_stack.pop();
				TreeNode* Lnum = num_stack.top();
				num_stack.pop();
				t->child[1] = Rnum;
				t->child[0] = Lnum;
				num_stack.push(t);
			}
			op_stack.pop(); //����������
			expflag--;
		}
		else
		{
			if ((getExpResult) || (getExpResult2))
			{
				while (op_stack.top()->attr.ExpAttr.op != END)
				{
					TreeNode* t = op_stack.top();
					op_stack.pop();
					TreeNode* Rnum = num_stack.top();
					num_stack.pop();
					TreeNode* Lnum = num_stack.top();
					num_stack.pop();

					t->child[1] = Rnum;
					t->child[0] = Lnum;
					num_stack.push(t);
				}
				op_stack.pop();//����ջ�ױ�־
				currentP = num_stack.top();
				num_stack.pop();

				TreeNode** t = syntaxtree_stack.top();
				syntaxtree_stack.pop();
				(*t) = currentP;

				/*�����������������־�ָ���ʼֵ�٣�
				  ������һ�������±���ʽʱ���ٽ�������Ϊ��ֵ*/
				if (getExpResult2 == true)
					getExpResult2 = false;
			}
		}
		break;
	}

	case 85:
	{
		symbal_stack.push(Exp);
		symbal_stack.push(AddOp);

		TreeNode* currentP = newExpNode(OpK);
		currentP->attr.ExpAttr.op = tok.tokens[pos].lex;
		LexType  sTop = op_stack.top()->attr.ExpAttr.op;
		while (Priosity(sTop) >= Priosity(tok.tokens[pos].lex))
		{
			TreeNode* t = op_stack.top();
			op_stack.pop();
			TreeNode* Rnum = num_stack.top();
			num_stack.pop();
			TreeNode* Lnum = num_stack.top();
			num_stack.pop();
			t->child[1] = Rnum;
			t->child[0] = Lnum;
			num_stack.push(t);
			sTop = op_stack.top()->attr.ExpAttr.op;
		}
		op_stack.push(currentP);
		break;
	}

	case 86:
	{
		symbal_stack.push(OtherFactor);
		symbal_stack.push(Factor);
		break;
	}
	case 87:
	{
		break;
	}
	case 88:
	{
		symbal_stack.push(Term);
		symbal_stack.push(MultOp);

		TreeNode* currentP = newExpNode(OpK);
		currentP->attr.ExpAttr.op = tok.tokens[pos].lex;

		LexType  sTop = op_stack.top()->attr.ExpAttr.op;
		while (Priosity(sTop) >= Priosity(tok.tokens[pos].lex))
			/*���������ջ������������ȼ����ڻ���ڵ�ǰ�����Ĳ�����*/
		{
			TreeNode* t = op_stack.top();
			op_stack.pop();
			TreeNode* Rnum = num_stack.top();
			num_stack.pop();
			TreeNode* Lnum = num_stack.top();
			num_stack.pop();
			t->child[1] = Rnum;
			t->child[0] = Lnum;
			num_stack.push(t);

			sTop = op_stack.top()->attr.ExpAttr.op;
		}
		op_stack.push(currentP);
		break;
	}

	case 89:
	{
		symbal_stack.push(RPAREN);
		symbal_stack.push(Exp);
		symbal_stack.push(LPAREN);

		TreeNode* t = newExpNode(OpK);
		t->attr.ExpAttr.op = tok.tokens[pos].lex; /*��������Ҳѹ��ջ��*/
		op_stack.push(t);
		expflag++;
		break;
	}

	case 90:
	{
		symbal_stack.push(INTC);

		TreeNode* t = newExpNode(ConstK);
		t->attr.ExpAttr.val = atoi(tok.tokens[pos].sem.c_str());
		/*�����ڵ��������ջ*/
		num_stack.push(t);

		break;
	}

	case 91:
	{
		symbal_stack.push(Variable);
		break;
	}

	case 92:
	{
		symbal_stack.push(VariMore);
		symbal_stack.push(ID);

		currentP = newExpNode(VariK);
		strcpy_s(currentP->name[0], tok.tokens[pos].sem.c_str());
		currentP->idnum++;
		/*�����ڵ��������ջ*/
		num_stack.push(currentP);

		break;
	}

	case 93:
	{
		/*��ʶ������*/
		currentP->attr.ExpAttr.varkind = IdV;
		break;
	}

	case 94:
	{
		symbal_stack.push(RMIDPAREN);
		symbal_stack.push(Exp);
		symbal_stack.push(LMIDPAREN);
		/*�����Ա����*/
		currentP->attr.ExpAttr.varkind = ArrayMembV;
		syntaxtree_stack.push(&currentP->child[0]);

		/*Ҫ������ʽ������ʼ��������ջ*/
		//������ջ��ջ�״���һ������Ĳ�������Ϊ��־
		TreeNode* t = newExpNode(OpK);
		t->attr.ExpAttr.op = END;
		op_stack.push(t);

		/*Ҫ���������±���ʽ�����ں���process84�����У�Ҫ
		  �����﷨��ջ���ʽ���־getExpResult2����Ϊ��ֵ*/
		getExpResult2 = true;

		break;

	}

	case 95:
	{
		symbal_stack.push(FieldVar);
		symbal_stack.push(DOT);
		/*���Ա����*/
		currentP->attr.ExpAttr.varkind = FieldMembV;
		syntaxtree_stack.push(&currentP->child[0]);
		break;
	}

	case 96:
	{
		symbal_stack.push(FieldVarMore);
		symbal_stack.push(ID);

		/*��¼��ĳ�Ա*/
		currentP = newExpNode(VariK);
		strcpy_s(currentP->name[0], tok.tokens[pos].sem.c_str());
		currentP->idnum++;

		TreeNode** t = syntaxtree_stack.top();
		syntaxtree_stack.pop();
		(*t) = currentP;

		break;

	}

	case 97:
	{
		/*���Ա�Ǳ�ʶ������*/
		currentP->attr.ExpAttr.varkind = IdV;
		break;
	}

	case 98:
	{
		symbal_stack.push(RMIDPAREN);
		symbal_stack.push(Exp);
		symbal_stack.push(LMIDPAREN);
		/*���Ա���������*/
		currentP->attr.ExpAttr.varkind = ArrayMembV;
		/*ָ�������Ա���ʽ*/
		syntaxtree_stack.push(&currentP->child[0]);

		//������ջ��ջ�״���һ������Ĳ�������Ϊ��־
		TreeNode* t = newExpNode(OpK);
		t->attr.ExpAttr.op = END;
		op_stack.push(t);

		/*Ҫ���������±���ʽ�����ں���process84�����У�Ҫ
		  �����﷨��ջ���ʽ���־getExpResult2����Ϊ��ֵ*/
		getExpResult2 = true;
		break;
	}
	case 99:
	{
		symbal_stack.push(LT);
		break;
	}

	case 100:
	{
		symbal_stack.push(EQ);
		break;
	}

	case 101:
	{
		symbal_stack.push(PLUS);
		break;
	}

	case 102:
	{
		symbal_stack.push(MINUS);
		break;
	}

	case 103:
	{
		symbal_stack.push(TIMES);
		break;
	}

	case 104:
	{
		symbal_stack.push(DIVIDE);
		break;
	}


	}


}


int  doGrammerll1::Priosity(LexType  op)
{
	int  pri = 0;
	switch (op)
	{
	case END:
		pri = -1; break;//ջ�ױ�ʶ�����ȼ����
	case LPAREN:
		pri = 0;
	case LT:
	case EQ:
		pri = 1; break;
	case PLUS:
	case MINUS:
		pri = 2; break;
	case TIMES:
	case DIVIDE:
		pri = 3; break;
	default: {
		cout << "û�����������";
		pri = -1;
	}
	}
	return  pri;
}

string doGrammerll1::tree_out(TreeNode* p)
{
	string name;
	switch (p->nodekind) {
	case ProK:
		return "Prok";
	case PheadK:
		return "PheadK:" + string(p->name[0]);
	case TypeK:
		return "TypeK";
	case VarK:
		return "VarK";
	case ProcDecK:
		return "ProcDeck:" + string(p->name[0]) + " of Type " + paramtype_map[p->attr.ProcAttr.paramt];
	case StmLK:
		return "StmLK: ";
	case DecK:
	{
		switch (p->kind.dec) {
		case ArrayK:
			return "DecK Array:" + string(p->name[0]) + " of " + deckind_map[p->attr.ArrayAttr.childtype] + " f " + to_string(p->attr.ArrayAttr.low) + " t " + to_string(p->attr.ArrayAttr.up);
		case CharK:
			return "DecK Char";
		case IntegerK: {
			name = get_names(p);
			return "DecK IntegerK:" + name;
		}
		case RecordK:
			name = get_names(p);
			return "DecK RecordK:" + name;
		case IdK:
			name = get_names(p);
			return "DecK IdK:" + name + " of " + p->type_name;
		}
		break;
	}
	case StmtK:
	{
		switch (p->kind.stmt) {
		case IfK:
			return "IfK";
		case WhileK:
			return "WHileK";
		case AssignK:
			return "AssignK";
		case ReadK:
			return "ReadK";
		case WriteK:
			return "WriteK";
		case CallK:
			return "CallK";
		case ReturnK:
			return "ReturnK";
		}
		break;
	case ExpK:
	{
		switch (p->kind.exp) {
		case OpK:
			return "ExpK OpK:" + lexName[p->attr.ExpAttr.op];
		case ConstK:
			return "ExpK ConstK:" + to_string(p->attr.ExpAttr.val);
		case VariK:
			return "ExpK " + varkind_map[p->attr.ExpAttr.varkind] + ": " + string(p->name[0]);
		}
		break;
	}
	default:
		return "ERROR";
	}
	}
	return "ERROR";
}

void doGrammerll1::drawtree(TreeNode* head, int level) {
	if (head != NULL) {
		string s = tree_out(head);
		for (int i = 0; i < level; i++) {
			cout << "    ";
		}
		cout << s << endl;
		level++;
		for (int i = 0; i < 3; i++) {
			if (head->child[i] != NULL) {
				drawtree(head->child[i], level);
				TreeNode* child = head->child[i];
				while (child->sibling != NULL) {
					drawtree(child->sibling, level);
					child = child->sibling;
				}
			}
		}
	}
}

string doGrammerll1::get_names(const TreeNode* node)
{
	string name;
	for (auto i = 0; i != node->idnum; ++i) {
		name += node->name[i];
		if (i + 1 != node->idnum)
			name.append(",");
	};
	return name;
}

doGrammerll1 gra1;