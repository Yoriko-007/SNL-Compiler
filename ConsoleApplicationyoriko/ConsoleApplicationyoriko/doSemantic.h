#pragma once
#include<vector>
#include<iostream>
#include"doToken.h"
#include"doGrammerll1.h"
#include"util.h"
using namespace std;
//↑所以我该不该把你注释了，一会好使一会儿不好使的，很搞心态的！！！

typedef enum { typeKind, varKind, procKind }IdKind;//标识符的类型
typedef enum { dir, indir }AccessKind;//Access表示是直接变量还是间接变量
typedef enum { intTy, charTy, arrayTy, recordTy, boolTy }TypeKind;//SNL类型
typedef enum { total, one }Flag;

//标识符信息域
typedef struct attribute
{
	struct typeIR* idtype; /*指向标识符的类型内部表示*/
	IdKind kind;			/*标识符的类型*/
	union //struct
	{
		struct
		{
			AccessKind access;
			int level;
			int off;
			bool isParam;
		}VarAttr;			/*变量标识符的属性*/
		struct
		{
			int level;
			struct ParamTable* param; /*参数表*/
			int mOff;
			int nOff;
			int procEntry;
			int codeEntry;
			int code;
			int size;
		}ProcAttr;			/*过程名标识符的属性*/
	}More;					/*标识符的不同类型有不同的属性*/
}AttributeIR;
//符号表
typedef struct symbtable                 //symbtable 
{
	char idname[10];//id
	AttributeIR attrIR;//AttributeIR
	struct symbtable* next;
}SymbTable;
//形参信息表
struct ParamTable
{
	symbtable* entry;
	ParamTable* next;
};
//记录域表
struct fieldChain
{
	char idname[10]; // 变量名
	typeIR* unitType; // 域中成员的类型
	int	offset; // 所在记录中的偏移
	fieldChain* next; // 链表指针
};
//内部类型结构(标准：整数字符型，非标准：数组，记录型）
typedef struct typeIR   //指向标识符的类型内部表示
{
	int size; /*类型所占空间大小*/
	TypeKind kind;//类型
	struct                 
	{
		struct
		{
			struct typeIR* indexTy;//数组下标类型
			struct typeIR* elemTy;//数组元素类型
			int low = 0;
			int up = 0;
		}ArrayAttr;
		fieldChain* body; /*记录类型中的域链*/
	} More;
}TypeIR;

class Semantic {
public:
	Semantic() {
		level = -1;
	}
	symbtable* NewTable();
	void CreateTable();//创建符号表
	void DestroyTable();//撤销符号表
	bool Enter(char* Id, AttributeIR* AttribP, symbtable*& Entry);//登记标识符和属性到符号表
	bool FindEntry(char* id, symbtable*& Entry);//符号表中查找标识符
	typeIR* NewTy(TypeKind kind);//
	fieldChain* NewBody();//
	void initialize();
	typeIR* TypeProcess(TreeNode* t, DecKind deckind);
	typeIR* nameType(TreeNode* t);
	typeIR* arrayType(TreeNode* t);
	typeIR* recordType(TreeNode* t);
	void TypeDecPart(TreeNode* t);
	void VarDecList(TreeNode* t);
	void ProcDecPart(TreeNode* t);
	symbtable* HeadProcess(TreeNode* t);
	ParamTable* ParaDecList(TreeNode* t);
	void Body(TreeNode* t);
	typeIR* Expr(TreeNode* t, AccessKind* Ekind);
	typeIR* arrayVar(TreeNode* t);
	typeIR* recordVar(TreeNode* t);
	void statement(TreeNode* t);
	void assignstatement(TreeNode* t);
	void callstatement(TreeNode* t);
	void ifstatement(TreeNode* t);
	void whilestatement(TreeNode* t);
	void readstatement(TreeNode* t);
	void writestatement(TreeNode* t);
	void returnstatement(TreeNode* t);
	bool analyze(TreeNode* t);
	int level;
	int Off;
	int mainOff;
	bool isError;
	vector<symbtable*> scope;
	typeIR* intPtr = NULL;
	typeIR* charPtr = NULL;
	typeIR* boolPtr = NULL;
};
//主程序
bool Semantic::analyze(TreeNode* t) {
	CreateTable();
	initialize();//类型内部初始化函数
	TreeNode* p = t->child[1];//声明
	while (p != NULL) {//声明部分处理
		if (p->nodekind == NodeKind::TypeK)
			TypeDecPart(p->child[0]);//类型声明部分分析处理函数
		else if (p->nodekind == NodeKind::VarK)
			VarDecList(p->child[0]);
		else if (p->nodekind == NodeKind::ProcDecK)
			ProcDecPart(p);
		p = p->sibling;
	}
	t = t->child[2];
	if (t->nodekind == NodeKind::StmLK)//处理主程序体
		Body(t);
	else cout << "error1" << endl;
	//	if (level != -1)
	DestroyTable();
	//	cout << "平安走完了语义分析" << endl;
	//	if (isError) cout << "有语义错误，语义分析失败" << endl;
	//	else cout << "语义分析成功" << endl;
	return !isError;
}

symbtable* Semantic::NewTable() {
	symbtable* table = new symbtable;
	strcpy_s(table->idname, "\0");
	table->next = NULL;
	table->attrIR.kind = IdKind::typeKind;
	table->attrIR.idtype = NULL;
	table->attrIR.More.VarAttr.isParam = false;
	return table;
}

#define initOff 7;
void Semantic::CreateTable()
{
	level++;
	scope.push_back(NULL);
	Off = initOff;
}

void Semantic::DestroyTable() {
	if(isError==false)
	scope.pop_back();
	level--;
}

bool Semantic::Enter(char* Id, AttributeIR* AttribP, symbtable*& Entry) { //登记标识符和属性到符号表
	bool present = false;
	bool result = false;
	symbtable* currentry = scope[level];
	symbtable* prentry = scope[level];
	if (scope[level] == NULL) {
		currentry = NewTable();
		scope[level] = currentry;
	}
	else {
		while (currentry != NULL) {
			prentry = currentry;
			if (strcmp(Id, currentry->idname) == 0) {
				result = true;
				isError = true;
				present = true;
				break;
			}
			else {
				result = false;
				/*		if (result) {
							isError = true;
							present = true;
							break;
						}*/
			//else 
			currentry = prentry->next;
			}
		}
		if (!present) {
			currentry = NewTable();
			prentry->next = currentry;
		}
	}
	strcpy_s(currentry->idname, Id);
	currentry->attrIR.idtype = AttribP->idtype;
	currentry->attrIR.kind = AttribP->kind;
	if (AttribP->kind == IdKind::typeKind) {}
	else if (AttribP->kind == IdKind::varKind) {
		currentry->attrIR.More.VarAttr.level = AttribP->More.VarAttr.level;
		currentry->attrIR.More.VarAttr.off = AttribP->More.VarAttr.off;
		currentry->attrIR.More.VarAttr.access = AttribP->More.VarAttr.access;
	}
	else if (AttribP->kind == IdKind::procKind) {
		currentry->attrIR.More.ProcAttr.level = AttribP->More.ProcAttr.level;
		currentry->attrIR.More.ProcAttr.param = AttribP->More.ProcAttr.param;
	}
	Entry = currentry;

	return present;
}

/*符号表中查找标识符*/
bool Semantic::FindEntry(char* id, symbtable*& Entry) {
	bool present = false;
	int lev = level;
	symbtable* findentry = scope[lev];//查找当前符号表头
	while (lev != -1 && present == false) {
		while (findentry != NULL && present == false) {
			if (strcmp(id, findentry->idname) == 0) {
				present = true;
			}
			else findentry = findentry->next;
		}
		if (present == false) {//没有找到
			lev--;
			if (lev >= 0)
				findentry = scope[lev];
		}
	}
	if (present == false) {//到最后也没找到
		Entry = NULL;
	}
	else Entry = findentry;
	return present;
}


//初始化内部类型表示函数
void Semantic::initialize() {
	intPtr = NewTy(TypeKind::intTy);
	charPtr = NewTy(TypeKind::charTy);
	boolPtr = NewTy(TypeKind::boolTy);
}

//内部类型：标准：bool，int，char。非标准的：数组记录型
typeIR* Semantic::NewTy(TypeKind kind) {
	typeIR* table = new typeIR;
		if (kind == TypeKind::boolTy || kind == TypeKind::intTy || kind == TypeKind::charTy) {
			table->kind = kind;
			table->size = 1;
		}
		else if (kind == TypeKind::arrayTy) {
			table->kind = TypeKind::arrayTy;
			table->More.ArrayAttr.indexTy = NULL;
			table->More.ArrayAttr.elemTy = NULL;
		}
		else if (kind == TypeKind::recordTy) {
			table->kind = TypeKind::recordTy;
			table->More.body = NULL;
		}
	return table;
}

fieldChain* Semantic::NewBody() {
	fieldChain* Ptr = new fieldChain;
	Ptr->next = NULL;
	Ptr->offset = 0;
	return Ptr;
}

//类型分析处理函数
typeIR* Semantic::TypeProcess(TreeNode* t, DecKind deckind) {//处理语法树的当前结点类型。构造出当前类型的内部表
	typeIR* Ptr = NULL;
	if (deckind == DecKind::IdK)
		Ptr = nameType(t);
	else if (deckind == DecKind::ArrayK)
		Ptr = arrayType(t);
	else if (deckind == DecKind::RecordK)
		Ptr = recordType(t);
	else if (deckind == DecKind::IntegerK)
		Ptr = intPtr;
	else if (deckind == DecKind::CharK)
		Ptr = charPtr;
	return Ptr;
}

//自定义类型内部结构分析函数
typeIR* Semantic::nameType(TreeNode* t) {
	typeIR* Ptr = NULL;
	symbtable* entry = NULL;
	bool present = FindEntry(t->type_name, entry);
	if (present == true) {
		if (entry->attrIR.kind != IdKind::typeKind) {
			isError = true;
			cout << t->lineno << " : " << t->type_name << "非类型标识符错误" << endl;
		}
		else
			Ptr = entry->attrIR.idtype;
	}
	else {
		isError = true;
		cout << t->lineno << " : " << t->type_name << " 未声明" << endl;
	}
	return Ptr;
}

//数组类型内部表示处理函数,数组
typeIR* Semantic::arrayType(TreeNode* t) {
	typeIR* ptr0 = NULL;
	typeIR* ptr1 = NULL;
	typeIR* ptr = NULL;

	if (t->attr.ArrayAttr.low > t->attr.ArrayAttr.up) {
		cout << t->lineno << " : " << "数组越界" << endl;
		isError = true;
	}
	else {
		ptr0 = TypeProcess(t, DecKind::IntegerK);//处理下标类型
		ptr1 = TypeProcess(t, t->attr.ArrayAttr.childtype);//处理元素类型
		ptr = NewTy(TypeKind::arrayTy);//创建新的类型信息表
		if (ptr != NULL && ptr1 != NULL) {
			ptr->size = ((t->attr.ArrayAttr.up - t->attr.ArrayAttr.low) + 1) * ptr1->size;//计算类型长度
			ptr->More.ArrayAttr.indexTy = ptr0;
			ptr->More.ArrayAttr.elemTy = ptr1;
			ptr->More.ArrayAttr.low = t->attr.ArrayAttr.low;
			ptr->More.ArrayAttr.up = t->attr.ArrayAttr.up;
		}
	}
	return ptr;
}

//处理记录类型的内部表示函数，记录
typeIR* Semantic::recordType(TreeNode* t) {//处理记录类型的内部表示
	fieldChain* Ptr1 = NULL;
	fieldChain* Ptr2 = NULL;
	fieldChain* body = NULL;
	typeIR* Ptr = NewTy(TypeKind::recordTy);//新节点
	t = t->child[0];
	while (t != NULL) {//循环处理域变量
		int i = 0;
		while (i < t->idnum) {
			Ptr2 = NewBody();
			strcpy_s(Ptr2->idname, t->name[i]);
			Ptr2->unitType = TypeProcess(t, t->kind.dec);
			Ptr2->next = NULL;
//			if (Ptr2 != Ptr1) {
				Ptr2->offset = Ptr1->offset + Ptr1->unitType->size;
				Ptr1->next = Ptr2;
				Ptr1 = Ptr2;
//			}
			i++;
		}
		t = t->sibling;
	}
	Ptr->size = Ptr2->offset + Ptr2->unitType->size;//整个记录的size部分
	Ptr->More.body = body;//将域链入记录类型的body部分
	return Ptr;
}

//类型声明部分分析处理函数
void Semantic::TypeDecPart(TreeNode* t) {
	bool present = false;
	symbtable* entry = NULL;
	AttributeIR* attrIr = new AttributeIR;
	attrIr->kind = IdKind::typeKind;
	while (t != NULL) {
		present = Enter(t->name[0], attrIr, entry);//登记属性函数
		if (present) {
			//重复声明报错
			isError = true;
			cout << t->lineno << " : " << t->name[0] << " 重复定义" << endl;
			entry = NULL;
		}
		else {
			entry->attrIR.idtype = TypeProcess(t, t->kind.dec);//内部类型表示调用类型处理函数
		}
		t = t->sibling;
	}
}

//变量声明部分分析处理函数
void Semantic::VarDecList(TreeNode* t) {
	bool present = false;
	symbtable* entry = NULL;
	AttributeIR* attrIr = new AttributeIR;
	while (t != NULL) {
		attrIr->kind = IdKind::varKind;
		int i = 0;
		while (i < t->idnum) {
			attrIr->idtype = TypeProcess(t, t->kind.dec);
			//值参还是变参(dir,indir)
			if (t->attr.ProcAttr.paramt == ParamType::varparamType) {//valparamType, varparamType
				attrIr->More.VarAttr.access = AccessKind::indir;
				attrIr->More.VarAttr.level = level;
				attrIr->More.VarAttr.off = Off;
				Off++;
			}
			else {
				attrIr->More.VarAttr.access = AccessKind::dir;
				attrIr->More.VarAttr.level = level;
				/*计算值参的偏移*/
				if (attrIr->idtype != NULL) {
					attrIr->More.VarAttr.off = Off;
					Off = Off + attrIr->idtype->size;
				}
			}
			if (attrIr->idtype != NULL) {
				present = Enter(t->name[i], attrIr, entry);
				if (present != false) {
					isError = true;
					cout << t->lineno << " : " << t->name[i] << "重复定义" << endl;
				}
				else t->table[i] = entry;
			}
			i++;
		}
		if (t != NULL)t = t->sibling;
	}
	if (level == 0) {
		mainOff = Off;
	}
}

//过程声明部分分析处理函数
void Semantic::ProcDecPart(TreeNode* t) {//过程声明
	TreeNode* p = t;
	symbtable* entry = HeadProcess(t);
	t = t->child[1];
	while (t != NULL) {//过程内部存在声明部分
		if (t->nodekind == NodeKind::TypeK)
			TypeDecPart(t->child[0]);
		else if (t->nodekind == NodeKind::VarK)
			VarDecList(t->child[0]);
		else if (t->nodekind == NodeKind::ProcDecK) {}
		else {
			isError = true;
			cout << t->lineno << " : " << "语法树没有这种节点" << endl;
		}
		if (t->nodekind == NodeKind::ProcDecK) {}
		else t = t->sibling;
	}
	/*过程活动记录的长度等于nOff加上display表的长度*
	 diplay表的长度等于过程所在层数加一*/
	entry->attrIR.More.ProcAttr.nOff = mainOff;
	entry->attrIR.More.ProcAttr.mOff = entry->attrIR.More.ProcAttr.nOff + entry->attrIR.More.ProcAttr.level + 1;
	mainOff++;
	while (t != NULL) {
		ProcDecPart(t);
		t = t->sibling;
	}
	t = p;
	Body(t->child[2]);
	if (level != -1)
		DestroyTable();
}

//过程声明头分析函数
symbtable* Semantic::HeadProcess(TreeNode* t) {
	AttributeIR* attrIr = new AttributeIR;
	bool present = false;
	symbtable* entry = NULL;
	attrIr->kind = IdKind::procKind;
	attrIr->idtype = NULL;
	attrIr->More.ProcAttr.level = level + 1;
	if (t != NULL) {
		present = Enter(t->name[0], attrIr, entry);
		t->table[0] = entry;
	}
	entry->attrIR.More.ProcAttr.param = ParaDecList(t);
	return entry;
}

//形参分析处理函数  //形参表
ParamTable* Semantic::ParaDecList(TreeNode* t) {
	TreeNode* p = NULL;
	symbtable* Ptr0 = NULL;
	ParamTable* Ptr1 = NULL;
	ParamTable* Ptr2 = NULL;
	ParamTable* head = NULL;
	if (t != NULL) {
		if (t->child[0] != NULL)
			p = t->child[0];
		CreateTable();
		Off = 7;
		VarDecList(p);
		Ptr0 = scope[level];
		while (Ptr0 != NULL) {
			Ptr2 = new ParamTable;
			if (head == NULL)
				head = Ptr1 = Ptr2;
			Ptr2->entry = Ptr0;
			Ptr2->next = NULL;
			if (Ptr2 != Ptr1) {
				Ptr1->next = Ptr2;
				Ptr1 = Ptr2;
			}
			Ptr0 = Ptr0->next;
		}
	}
	return head;
}

//表达式分析处理函数
typeIR* Semantic::Expr(TreeNode* t, AccessKind* Ekind) {
	bool present = false;
	symbtable* entry = NULL;
	typeIR* Eptr = NULL;
	typeIR* Eptr0 = NULL;
	typeIR* Eptr1 = NULL;
	while (t != NULL) {
		if (t->kind.exp == ExpKind::ConstK) {//处理常量
			Eptr = TypeProcess(t, DecKind::IntegerK);
			Eptr->kind = TypeKind::intTy;
			if (Ekind != NULL) {
				*Ekind = AccessKind::dir;
			}
		}
		else if (t->kind.exp == ExpKind::VariK) {
			if (t->child[0] == NULL) {
				present = FindEntry(t->name[0], entry);
				t->table[0] = entry;
				if (present != false) {
					if (entry->attrIR.kind != IdKind::varKind) {
						isError = true;
						cout << t->lineno << " : " << t->name[0] << " id不是变量" << endl;
						Eptr = NULL;
					}
					else {
						Eptr = entry->attrIR.idtype;
						if (Ekind != NULL)
							*Ekind = AccessKind::indir;//变量
					}
				}
				else {
					isError = true;
					cout << t->lineno << " : " << t->name[0] << "标识符无声明" << endl;
				}
			}
			else {
				if (t->attr.ExpAttr.varkind == VarKind::ArrayMembV)
					Eptr = arrayVar(t);
				else if (t->attr.ExpAttr.varkind == VarKind::FieldMembV)
					Eptr = recordVar(t);
			}
		}
		else if (t->kind.exp == ExpKind::OpK) {//操作符
			Eptr0 = Expr(t->child[0], NULL);
			if (Eptr0 == NULL)
				return NULL;
			Eptr1 = Expr(t->child[1], NULL);
			if (Eptr1 == NULL)
				return NULL;
			if (Eptr0 == Eptr1) {
				present = true;
			}
			if (present) {
				if (t->attr.ExpAttr.op == LexType::EQ)
					Eptr = boolPtr;
				else if (t->attr.ExpAttr.op == LexType::DIVIDE)
					Eptr = intPtr;
				else if (t->attr.ExpAttr.op == LexType::PLUS)
					Eptr = intPtr;
				else if (t->attr.ExpAttr.op == LexType::TIMES)
					Eptr = intPtr;
				else if (t->attr.ExpAttr.op == LexType::MINUS)
					Eptr = intPtr;
				if (Ekind != NULL) {
					*Ekind = AccessKind::dir;
				}
			}
			else {
				isError = true;
				cout << t->lineno << " : " << "操作符类型不匹配" << endl;
			}
		}
		return Eptr;
	}
}

typeIR* Semantic::arrayVar(TreeNode* t) {
	bool present = false;
	symbtable* entry = NULL;
	typeIR* Eptr = NULL;
	typeIR* Eptr0 = NULL;
	typeIR* Eptr1 = NULL;
	present = FindEntry(t->name[0], entry);
	t->table[0] = entry;
	if (present) {
		if (entry->attrIR.idtype->kind != IdKind::varKind) {
			isError = true;
			cout << t->lineno << " : " << t->name[0] << " 不是变量" << endl;
			Eptr = NULL;
		}
		else {
			if (entry->attrIR.idtype->kind != TypeKind::arrayTy) {
				isError = true;
				cout << t->lineno << " : " << t->name[0] << " 不是数组类型变量" << endl;
				Eptr = NULL;
			}
			else {
				Eptr0 = entry->attrIR.idtype->More.ArrayAttr.indexTy;
				if (Eptr0 == NULL) 	return NULL;
				Eptr1 = Expr(t->child[0], NULL);
				if (Eptr1 == NULL) return NULL;
				if (Eptr0 != Eptr1)
					present = false;
				else present = true;
				if (present != true) {
					isError = true;
					cout << t->lineno << " : " << "E类型是否与不下标类型相符" << endl;
					Eptr = NULL;
				}
				else Eptr = entry->attrIR.idtype->More.ArrayAttr.elemTy;
			}
		}
	}
	else {
		isError = true;
		cout << t->lineno << " : " << t->name[0] << " is not declarations!" << endl;
	}
	return Eptr;
}

typeIR* Semantic::recordVar(TreeNode* t) {
	bool present = false;
	bool result = true;
	symbtable* entry = NULL;
	typeIR* Eptr = NULL;
	typeIR* Eptr0 = NULL;
	typeIR* Eptr1 = NULL;
	fieldChain* currentP = NULL;
	present = FindEntry(t->name[0], entry);
	t->table[0] = entry;
	if (present) {
		if (entry->attrIR.kind != IdKind::varKind) {
			isError = true;
			cout << t->lineno << " : " << t->name[0] << "不是变量" << endl;
			Eptr = NULL;
		}
		else {
			if (entry->attrIR.idtype->kind != TypeKind::recordTy) {
				isError = true;
				cout << t->lineno << " : " << t->name[0] << "不是记录类型变量" << endl;
				Eptr = NULL;
			}
			else {
				Eptr0 = entry->attrIR.idtype;
				currentP = Eptr0->More.body;
				while (currentP != NULL && result != false) {
					result = (t->child[0]->name[0] == currentP->idname);
					if (result == false)
						Eptr = currentP->unitType;
					else currentP = currentP->next;
				}
				if (currentP == NULL) {
					if (result) {
						isError = true;
						cout << t->child[0]->lineno << " : " << t->child[0]->name[0] << " 没有该域名" << endl;
						Eptr = arrayVar(t->child[0]);
					}
				}
			}
		}
	}
	else {
		isError = true;
		cout << t->lineno << " : " << t->name[0] << "标识符无声明" << endl;
	}
	return Eptr;
}


void Semantic::Body(TreeNode* t) {
	if (t->nodekind == NodeKind::StmLK) {
		TreeNode* p = t->child[0];
		while (p != NULL) {
			statement(p);//语句
			p = p->sibling;
		}
	}
}

//语句序列分析处理函数
void Semantic::statement(TreeNode* t) {
	if (t->kind.stmt == StmtKind::IfK) {
		ifstatement(t);
	}
	else if (t->kind.stmt == StmtKind::WhileK)
		whilestatement(t);
	else if (t->kind.stmt == StmtKind::AssignK)
		assignstatement(t);
	else if (t->kind.stmt == StmtKind::ReadK)
		readstatement(t);
	else if (t->kind.stmt == StmtKind::WriteK)
		writestatement(t);
	else if (t->kind.stmt == StmtKind::CallK)
		callstatement(t);
	else if (t->kind.stmt == StmtKind::ReturnK)
		returnstatement(t);
	else {
		isError = true;
		cout << t->lineno << " : " << "error" << endl;
	}
}

void Semantic::assignstatement(TreeNode* t) {
	symbtable* entry = NULL;//？
	bool present = false;
	typeIR* Eptr = NULL;
	typeIR* ptr = NULL;
	treeNode* child1 = t->child[0];
	treeNode* child2 = t->child[1];
	if (child1->child[0] == NULL) {
		present = FindEntry(child1->name[0], entry);//符号表中查找
		if (present) {
			if (entry->attrIR.kind != IdKind::varKind) {
				isError = true;
				cout << child1->lineno << " : " << child1->name[0] << " id这里不是变量" << endl;
			}
			else {//是变量类型
				Eptr = entry->attrIR.idtype;
				child1->table[0] = entry;
			}
		}
		else {
			isError = true;
			cout << child1->lineno << " : " << child1->name[0] << " 没在符号表里面查到这个标识符" << endl;
		}
	}
	else {
		if (child1->attr.ExpAttr.varkind == VarKind::ArrayMembV) {
			Eptr = arrayVar(child1);
		}
		else {
			if (child1->attr.ExpAttr.varkind == VarKind::FieldMembV) {
				Eptr = recordVar(child1);
			}
		}
	}
	if (Eptr != NULL) {
		if ((t->nodekind == NodeKind::StmtK) && (t->kind.stmt == StmtKind::AssignK)) {
			ptr = Expr(child2, NULL);
			if (ptr != Eptr) {
				isError = true;
				cout << t->lineno << " : " << "赋值号两边不等价" << endl;
			}
		}
	}
}
//？？？
void Semantic::callstatement(TreeNode* t) {
	AccessKind* Ekind = NULL;
	bool present = false;
	symbtable* Entry = NULL;
	present = FindEntry(t->child[0]->name[0], Entry);
	t->child[0]->table[0] = Entry;
	if (present == false) {
		isError = true;
		cout << t->lineno << " : " << t->child[0]->name[0] << "函数未定义" << endl;
	}
	else {
		if (Entry->attrIR.kind != IdKind::procKind) {
			isError = true;
			cout << t->lineno << " : " << t->child[0]->name[0] << "不是过程名" << endl;
		}
		else {//处理形实参结合
			TreeNode* p = t->child[1];
			ParamTable* paramP = Entry->attrIR.More.ProcAttr.param;
			while (p != NULL && paramP != NULL) {
				symbtable* paraEntry = paramP->entry;
				typeIR* Etp = Expr(p, Ekind);
				if ((paraEntry->attrIR.More.VarAttr.access == AccessKind::indir) && (Ekind != NULL) && (*Ekind == AccessKind::dir)) {
					isError = true;
					cout << t->lineno << " : " << "参数类别不匹配" << endl;
				}
				else {
					if (paraEntry->attrIR.idtype != Etp) {
						isError = true;
						cout << t->lineno << " : " << "参数类型不匹配" << endl;
					}
				}
				p = p->sibling;
				paramP = paramP->next;
			}
			if (p != NULL || paramP != NULL) {
				isError = true;
				cout << t->child[1]->lineno << " : " << "参数个数不匹配" << endl;
			}
		}
	}


}

void Semantic::ifstatement(TreeNode* t) {
	AccessKind* Ekind = NULL;
	typeIR* Etp = Expr(t->child[0], Ekind);
	if (Etp != NULL) {
		if (Etp->kind != TypeKind::boolTy) {
			isError = true;
			cout << t->lineno << " : " << "条件表达式不是bool类型" << endl;
		}
		else {
			TreeNode* p = t->child[1];
			while (p != NULL) {//then部分
				statement(p);
				p = p->sibling;
			}
			p = t->child[2];
			while (p != NULL) {//else部分
				statement(p);
				p = p->sibling;
			}
		}
	}
}

void Semantic::whilestatement(TreeNode* t) {
	typeIR* Etp = Expr(t->child[0], NULL);
	if (Etp != NULL)
		if (Etp->kind != TypeKind::boolTy) {
			isError = true;
			cout << t->lineno << " : " << "条件表达式不是bool类型" << endl;
		}
		else {
			t = t->child[1];
			while (t != NULL) {
				statement(t);
				t = t->sibling;
			}
		}
}

void Semantic::readstatement(TreeNode* t) {
	symbtable* entry = NULL;
	bool present = false;
	present = FindEntry(t->name[0], entry);
	t->table[0] = entry;
	if (present == false) {
		isError = true;
		cout << t->lineno << " : " << t->name[0] << " 未声明" << endl;
	}
	else if (entry->attrIR.kind != varKind)
	{
		isError = true;
		cout << t->lineno << " : " << t->name[0] << "不是变量标识符" << endl;
	}
}

void Semantic::writestatement(TreeNode* t) {//思考，为什么不能是bool类型？
	typeIR* Etp = Expr(t->child[0], NULL);
	if (Etp != NULL)
		if (Etp->kind == TypeKind::boolTy) {
			isError = true;
			cout << t->lineno << " : " << "表达式类型出错" << endl;
		}
}

void Semantic::returnstatement(TreeNode* t) {
	if (level == 0) {
		isError = true;
		cout << t->lineno << " : " << "返回语句在主程序中出现" << endl;
	}
}