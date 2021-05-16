#pragma once
#include<map>
#include<string>
#include<set>
#include"doToken.h"
#include"doGrammerll1.h"
using namespace std;

extern int lineno = 0;

typedef enum { ProK, PheadK, DecK, TypeK, VarK, ProcDecK, StmLK, StmtK, ExpK }NodeKind;
extern map<NodeKind, string> nodekind_map;

typedef enum { ArrayK, CharK, IntegerK, RecordK, IdK }  DecKind;
extern map<DecKind, string> deckind_map;

typedef enum { IfK, WhileK, AssignK, ReadK, WriteK, CallK, ReturnK } StmtKind;
extern map<StmtKind, string> stmtkind_map;

typedef enum { OpK, ConstK, VariK } ExpKind;
extern map<ExpKind, string> expkind_map;

typedef enum { IdV, ArrayMembV, FieldMembV } VarKind;
extern map<VarKind, string> varkind_map;

typedef enum { Void, Integer, Boolean } ExpType;
extern map<ExpType, string> exptype_map;

typedef enum { valparamType, varparamType } ParamType;//val只读
extern map<ParamType, string> paramtype_map;


map<NodeKind, string> nodekind_map = {
{ProK,"ProK"},
{PheadK,"PheadK"},
{DecK,"DecK"},
{TypeK,"TypeK"},
{VarK,"VarK"},
{ProcDecK,"ProcDecK"},
{StmLK,"StmLK"},
{StmtK,"StmtK"},
{ExpK,"ExpK"},
};
map<DecKind, string> deckind_map = {
{ArrayK,"ArrayK"},
{CharK,"CharK"},
{IntegerK,"IntegerK"},
{RecordK,"RecordK"},
{IdK,"IdK"},
};
map<StmtKind, string> stmtkind_map = {
{IfK,"IfK"},
{WhileK,"WhileK"},
{AssignK,"AssignK"},
{ReadK,"ReadK"},
{WriteK,"WriteK"},
{CallK,"CallK"},
{ReturnK,"ReturnK"},
};
map<ExpKind, string> expkind_map = {
{OpK,"OpK"},
{ConstK,"ConstK"},
{VariK,"VariK"},
};
map<VarKind, string> varkind_map = {
{IdV,"IdV"},
{ArrayMembV,"ArrayMembV"},
{FieldMembV,"FieldMembV"},
};
map<ExpType, string> exptype_map = {
{Void,"Void"},
{Integer,"Integer"},
{Boolean,"Boolean"},
};
map<ParamType, string> paramtype_map = {
{valparamType,"valparamType"},
{varparamType,"varparamType"},
};

//终极符
set<LexType> TTSet = {
	ENDFILE, ERROR,
	PROGRAM, PROCEDURE, TYPE, VAR, IF,
	THEN, ELSE, FI, WHILE, DO,
	ENDWH, BEGIN, END, READ, WRITE,
	ARRAY, OF, RECORD, RETURN, INTEGER, CHAR,
	INTEGER_T, CHAR_T,
	ID, INTC, CHARC, INTC_VAL, CHARC_VAL,
	ASSIGN, EQ, LT, PLUS, MINUS,
	TIMES, DIVIDE, LPAREN, RPAREN, DOT,
	COLON, SEMI, COMMA, LMIDPAREN, RMIDPAREN,
	UNDERRANGE,
};
//非终极符
set<LexType> NTSet = {
	//feizhongjiefu
	Program,	      ProgramHead,	    ProgramName,	DeclarePart,
	TypeDec,        TypeDeclaration,	TypeDecList,	TypeDecMore,
	TypeId,	      TypeName,			BaseType,	    StructureType,
	ArrayType,      Low,	            Top,            RecType,
	FieldDecList,   FieldDecMore,	    IdList,	        IdMore,
	VarDec,	      VarDeclaration,	VarDecList,		VarDecMore,
	VarIdList,	  VarIdMore,		ProcDec,		ProcDeclaration,
	ProcDecMore,    ProcName,		    ParamList,		ParamDecList,
	ParamMore,      Param,		    FormList,		FidMore,
	ProcDecPart,    ProcBody,	    	ProgramBody,	StmList,
	StmMore,        Stm,				AssCall,		AssignmentRest,
	ConditionalStm, StmL,			    LoopStm,		InputStm,
	InVar,          OutputStm,		ReturnStm,		CallStmRest,
	ActParamList,   ActParamMore,		RelExp,			OtherRelE,
	Exp,			  OtherTerm,		Term,           OtherFactor,
	Factor,         Variable,			VariMore,		FieldVar,
	FieldVarMore,   CmpOp,			AddOp,          MultOp,

};


#define MAXCHILDREN 3 

typedef struct treeNode
{
	struct treeNode* child[MAXCHILDREN];
	struct treeNode* sibling;
	int lineno;
	NodeKind nodekind;
	union
	{
		DecKind  dec;
		StmtKind stmt;
		ExpKind  exp;
	} kind;
	int idnum;
	char name[10][10];
	struct symbtable* table[10];
	char type_name[10];
	struct
	{
		struct
		{
			int low;
			int up;
			DecKind   childtype;
		}ArrayAttr;
		struct
		{
			ParamType  paramt;
		}ProcAttr;
		struct
		{
			LexType op;
			int val;
			VarKind  varkind;
			ExpType type;
		}ExpAttr;
	} attr;
}TreeNode;

TreeNode* newSpecNode(NodeKind kind)
{

	auto t = new TreeNode;

	int i;

	for (i = 0; i < MAXCHILDREN; i++) t->child[i] = nullptr;
	t->sibling = nullptr;
	t->nodekind = kind;
	t->lineno = lineno;
	t->idnum = 0;
	strcpy_s(t->type_name, "\0");

	for (i = 0; i < 10; i++) {
		strcpy_s(t->name[i], "\0");
		t->table[i] = nullptr;
	}
	return t;
}

TreeNode* newRootNode() {
	return newSpecNode(ProK);
}

TreeNode* newPheadNode() {
	return newSpecNode(PheadK);
}

TreeNode* newDecANode(NodeKind kind) {
	return newSpecNode(kind);
}

/********************************************************/
/* 功  能 类型标志语法树节点创建函数					*/
/********************************************************/
TreeNode* newTypeNode() {
	return newSpecNode(TypeK);
}

/********************************************************/
/* 功  能 变量声明标志语法树节点创建函数				*/
/********************************************************/
TreeNode* newVarNode() {
	return newSpecNode(VarK);
}

/********************************************************/
/* 功  能 创建声明类型语法树节点函数					*/
/********************************************************/
TreeNode* newDecNode() {
	return newSpecNode(DecK);
}

/********************************************************/
/* 功  能 创建函数类型语法树节点函数					*/
/********************************************************/
TreeNode* newProcNode() {
	return newSpecNode(ProcDecK);
}

/********************************************************/
/* 功  能 创建语句标志类型语法树节点函数				*/
/********************************************************/
TreeNode* newStmlNode() {
	return newSpecNode(StmLK);
}

/********************************************************/
/* 功  能 创建语句类型语法树节点函数					*/
/********************************************************/
TreeNode* newStmtNode(StmtKind kind) {
	auto t = newSpecNode(StmtK);

	t->kind.stmt = kind;
	return t;
}


/********************************************************/
/* 功  能 表达式类型语法树节点创建函数					*/
/********************************************************/
TreeNode* newExpNode(ExpKind kind) {
	auto t = newSpecNode(ExpK);
	t->kind.exp = kind;
	return t;
}




