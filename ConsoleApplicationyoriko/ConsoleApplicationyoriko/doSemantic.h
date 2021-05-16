#pragma once
#include<vector>
#include<iostream>
#include"doToken.h"
#include"doGrammerll1.h"
#include"util.h"
using namespace std;
//�������Ҹò��ð���ע���ˣ�һ���ʹһ�������ʹ�ģ��ܸ���̬�ģ�����

typedef enum { typeKind, varKind, procKind }IdKind;//��ʶ��������
typedef enum { dir, indir }AccessKind;//Access��ʾ��ֱ�ӱ������Ǽ�ӱ���
typedef enum { intTy, charTy, arrayTy, recordTy, boolTy }TypeKind;//SNL����
typedef enum { total, one }Flag;

//��ʶ����Ϣ��
typedef struct attribute
{
	struct typeIR* idtype; /*ָ���ʶ���������ڲ���ʾ*/
	IdKind kind;			/*��ʶ��������*/
	union //struct
	{
		struct
		{
			AccessKind access;
			int level;
			int off;
			bool isParam;
		}VarAttr;			/*������ʶ��������*/
		struct
		{
			int level;
			struct ParamTable* param; /*������*/
			int mOff;
			int nOff;
			int procEntry;
			int codeEntry;
			int code;
			int size;
		}ProcAttr;			/*��������ʶ��������*/
	}More;					/*��ʶ���Ĳ�ͬ�����в�ͬ������*/
}AttributeIR;
//���ű�
typedef struct symbtable                 //symbtable 
{
	char idname[10];//id
	AttributeIR attrIR;//AttributeIR
	struct symbtable* next;
}SymbTable;
//�β���Ϣ��
struct ParamTable
{
	symbtable* entry;
	ParamTable* next;
};
//��¼���
struct fieldChain
{
	char idname[10]; // ������
	typeIR* unitType; // ���г�Ա������
	int	offset; // ���ڼ�¼�е�ƫ��
	fieldChain* next; // ����ָ��
};
//�ڲ����ͽṹ(��׼�������ַ��ͣ��Ǳ�׼�����飬��¼�ͣ�
typedef struct typeIR   //ָ���ʶ���������ڲ���ʾ
{
	int size; /*������ռ�ռ��С*/
	TypeKind kind;//����
	struct                 
	{
		struct
		{
			struct typeIR* indexTy;//�����±�����
			struct typeIR* elemTy;//����Ԫ������
			int low = 0;
			int up = 0;
		}ArrayAttr;
		fieldChain* body; /*��¼�����е�����*/
	} More;
}TypeIR;

class Semantic {
public:
	Semantic() {
		level = -1;
	}
	symbtable* NewTable();
	void CreateTable();//�������ű�
	void DestroyTable();//�������ű�
	bool Enter(char* Id, AttributeIR* AttribP, symbtable*& Entry);//�ǼǱ�ʶ�������Ե����ű�
	bool FindEntry(char* id, symbtable*& Entry);//���ű��в��ұ�ʶ��
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
//������
bool Semantic::analyze(TreeNode* t) {
	CreateTable();
	initialize();//�����ڲ���ʼ������
	TreeNode* p = t->child[1];//����
	while (p != NULL) {//�������ִ���
		if (p->nodekind == NodeKind::TypeK)
			TypeDecPart(p->child[0]);//�����������ַ���������
		else if (p->nodekind == NodeKind::VarK)
			VarDecList(p->child[0]);
		else if (p->nodekind == NodeKind::ProcDecK)
			ProcDecPart(p);
		p = p->sibling;
	}
	t = t->child[2];
	if (t->nodekind == NodeKind::StmLK)//������������
		Body(t);
	else cout << "error1" << endl;
	//	if (level != -1)
	DestroyTable();
	//	cout << "ƽ���������������" << endl;
	//	if (isError) cout << "����������������ʧ��" << endl;
	//	else cout << "��������ɹ�" << endl;
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

bool Semantic::Enter(char* Id, AttributeIR* AttribP, symbtable*& Entry) { //�ǼǱ�ʶ�������Ե����ű�
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

/*���ű��в��ұ�ʶ��*/
bool Semantic::FindEntry(char* id, symbtable*& Entry) {
	bool present = false;
	int lev = level;
	symbtable* findentry = scope[lev];//���ҵ�ǰ���ű�ͷ
	while (lev != -1 && present == false) {
		while (findentry != NULL && present == false) {
			if (strcmp(id, findentry->idname) == 0) {
				present = true;
			}
			else findentry = findentry->next;
		}
		if (present == false) {//û���ҵ�
			lev--;
			if (lev >= 0)
				findentry = scope[lev];
		}
	}
	if (present == false) {//�����Ҳû�ҵ�
		Entry = NULL;
	}
	else Entry = findentry;
	return present;
}


//��ʼ���ڲ����ͱ�ʾ����
void Semantic::initialize() {
	intPtr = NewTy(TypeKind::intTy);
	charPtr = NewTy(TypeKind::charTy);
	boolPtr = NewTy(TypeKind::boolTy);
}

//�ڲ����ͣ���׼��bool��int��char���Ǳ�׼�ģ������¼��
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

//���ͷ���������
typeIR* Semantic::TypeProcess(TreeNode* t, DecKind deckind) {//�����﷨���ĵ�ǰ������͡��������ǰ���͵��ڲ���
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

//�Զ��������ڲ��ṹ��������
typeIR* Semantic::nameType(TreeNode* t) {
	typeIR* Ptr = NULL;
	symbtable* entry = NULL;
	bool present = FindEntry(t->type_name, entry);
	if (present == true) {
		if (entry->attrIR.kind != IdKind::typeKind) {
			isError = true;
			cout << t->lineno << " : " << t->type_name << "�����ͱ�ʶ������" << endl;
		}
		else
			Ptr = entry->attrIR.idtype;
	}
	else {
		isError = true;
		cout << t->lineno << " : " << t->type_name << " δ����" << endl;
	}
	return Ptr;
}

//���������ڲ���ʾ������,����
typeIR* Semantic::arrayType(TreeNode* t) {
	typeIR* ptr0 = NULL;
	typeIR* ptr1 = NULL;
	typeIR* ptr = NULL;

	if (t->attr.ArrayAttr.low > t->attr.ArrayAttr.up) {
		cout << t->lineno << " : " << "����Խ��" << endl;
		isError = true;
	}
	else {
		ptr0 = TypeProcess(t, DecKind::IntegerK);//�����±�����
		ptr1 = TypeProcess(t, t->attr.ArrayAttr.childtype);//����Ԫ������
		ptr = NewTy(TypeKind::arrayTy);//�����µ�������Ϣ��
		if (ptr != NULL && ptr1 != NULL) {
			ptr->size = ((t->attr.ArrayAttr.up - t->attr.ArrayAttr.low) + 1) * ptr1->size;//�������ͳ���
			ptr->More.ArrayAttr.indexTy = ptr0;
			ptr->More.ArrayAttr.elemTy = ptr1;
			ptr->More.ArrayAttr.low = t->attr.ArrayAttr.low;
			ptr->More.ArrayAttr.up = t->attr.ArrayAttr.up;
		}
	}
	return ptr;
}

//�����¼���͵��ڲ���ʾ��������¼
typeIR* Semantic::recordType(TreeNode* t) {//�����¼���͵��ڲ���ʾ
	fieldChain* Ptr1 = NULL;
	fieldChain* Ptr2 = NULL;
	fieldChain* body = NULL;
	typeIR* Ptr = NewTy(TypeKind::recordTy);//�½ڵ�
	t = t->child[0];
	while (t != NULL) {//ѭ�����������
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
	Ptr->size = Ptr2->offset + Ptr2->unitType->size;//������¼��size����
	Ptr->More.body = body;//���������¼���͵�body����
	return Ptr;
}

//�����������ַ���������
void Semantic::TypeDecPart(TreeNode* t) {
	bool present = false;
	symbtable* entry = NULL;
	AttributeIR* attrIr = new AttributeIR;
	attrIr->kind = IdKind::typeKind;
	while (t != NULL) {
		present = Enter(t->name[0], attrIr, entry);//�Ǽ����Ժ���
		if (present) {
			//�ظ���������
			isError = true;
			cout << t->lineno << " : " << t->name[0] << " �ظ�����" << endl;
			entry = NULL;
		}
		else {
			entry->attrIR.idtype = TypeProcess(t, t->kind.dec);//�ڲ����ͱ�ʾ�������ʹ�����
		}
		t = t->sibling;
	}
}

//�����������ַ���������
void Semantic::VarDecList(TreeNode* t) {
	bool present = false;
	symbtable* entry = NULL;
	AttributeIR* attrIr = new AttributeIR;
	while (t != NULL) {
		attrIr->kind = IdKind::varKind;
		int i = 0;
		while (i < t->idnum) {
			attrIr->idtype = TypeProcess(t, t->kind.dec);
			//ֵ�λ��Ǳ��(dir,indir)
			if (t->attr.ProcAttr.paramt == ParamType::varparamType) {//valparamType, varparamType
				attrIr->More.VarAttr.access = AccessKind::indir;
				attrIr->More.VarAttr.level = level;
				attrIr->More.VarAttr.off = Off;
				Off++;
			}
			else {
				attrIr->More.VarAttr.access = AccessKind::dir;
				attrIr->More.VarAttr.level = level;
				/*����ֵ�ε�ƫ��*/
				if (attrIr->idtype != NULL) {
					attrIr->More.VarAttr.off = Off;
					Off = Off + attrIr->idtype->size;
				}
			}
			if (attrIr->idtype != NULL) {
				present = Enter(t->name[i], attrIr, entry);
				if (present != false) {
					isError = true;
					cout << t->lineno << " : " << t->name[i] << "�ظ�����" << endl;
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

//�����������ַ���������
void Semantic::ProcDecPart(TreeNode* t) {//��������
	TreeNode* p = t;
	symbtable* entry = HeadProcess(t);
	t = t->child[1];
	while (t != NULL) {//�����ڲ�������������
		if (t->nodekind == NodeKind::TypeK)
			TypeDecPart(t->child[0]);
		else if (t->nodekind == NodeKind::VarK)
			VarDecList(t->child[0]);
		else if (t->nodekind == NodeKind::ProcDecK) {}
		else {
			isError = true;
			cout << t->lineno << " : " << "�﷨��û�����ֽڵ�" << endl;
		}
		if (t->nodekind == NodeKind::ProcDecK) {}
		else t = t->sibling;
	}
	/*���̻��¼�ĳ��ȵ���nOff����display��ĳ���*
	 diplay��ĳ��ȵ��ڹ������ڲ�����һ*/
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

//��������ͷ��������
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

//�βη���������  //�βα�
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

//���ʽ����������
typeIR* Semantic::Expr(TreeNode* t, AccessKind* Ekind) {
	bool present = false;
	symbtable* entry = NULL;
	typeIR* Eptr = NULL;
	typeIR* Eptr0 = NULL;
	typeIR* Eptr1 = NULL;
	while (t != NULL) {
		if (t->kind.exp == ExpKind::ConstK) {//������
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
						cout << t->lineno << " : " << t->name[0] << " id���Ǳ���" << endl;
						Eptr = NULL;
					}
					else {
						Eptr = entry->attrIR.idtype;
						if (Ekind != NULL)
							*Ekind = AccessKind::indir;//����
					}
				}
				else {
					isError = true;
					cout << t->lineno << " : " << t->name[0] << "��ʶ��������" << endl;
				}
			}
			else {
				if (t->attr.ExpAttr.varkind == VarKind::ArrayMembV)
					Eptr = arrayVar(t);
				else if (t->attr.ExpAttr.varkind == VarKind::FieldMembV)
					Eptr = recordVar(t);
			}
		}
		else if (t->kind.exp == ExpKind::OpK) {//������
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
				cout << t->lineno << " : " << "���������Ͳ�ƥ��" << endl;
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
			cout << t->lineno << " : " << t->name[0] << " ���Ǳ���" << endl;
			Eptr = NULL;
		}
		else {
			if (entry->attrIR.idtype->kind != TypeKind::arrayTy) {
				isError = true;
				cout << t->lineno << " : " << t->name[0] << " �����������ͱ���" << endl;
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
					cout << t->lineno << " : " << "E�����Ƿ��벻�±��������" << endl;
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
			cout << t->lineno << " : " << t->name[0] << "���Ǳ���" << endl;
			Eptr = NULL;
		}
		else {
			if (entry->attrIR.idtype->kind != TypeKind::recordTy) {
				isError = true;
				cout << t->lineno << " : " << t->name[0] << "���Ǽ�¼���ͱ���" << endl;
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
						cout << t->child[0]->lineno << " : " << t->child[0]->name[0] << " û�и�����" << endl;
						Eptr = arrayVar(t->child[0]);
					}
				}
			}
		}
	}
	else {
		isError = true;
		cout << t->lineno << " : " << t->name[0] << "��ʶ��������" << endl;
	}
	return Eptr;
}


void Semantic::Body(TreeNode* t) {
	if (t->nodekind == NodeKind::StmLK) {
		TreeNode* p = t->child[0];
		while (p != NULL) {
			statement(p);//���
			p = p->sibling;
		}
	}
}

//������з���������
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
	symbtable* entry = NULL;//��
	bool present = false;
	typeIR* Eptr = NULL;
	typeIR* ptr = NULL;
	treeNode* child1 = t->child[0];
	treeNode* child2 = t->child[1];
	if (child1->child[0] == NULL) {
		present = FindEntry(child1->name[0], entry);//���ű��в���
		if (present) {
			if (entry->attrIR.kind != IdKind::varKind) {
				isError = true;
				cout << child1->lineno << " : " << child1->name[0] << " id���ﲻ�Ǳ���" << endl;
			}
			else {//�Ǳ�������
				Eptr = entry->attrIR.idtype;
				child1->table[0] = entry;
			}
		}
		else {
			isError = true;
			cout << child1->lineno << " : " << child1->name[0] << " û�ڷ��ű�����鵽�����ʶ��" << endl;
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
				cout << t->lineno << " : " << "��ֵ�����߲��ȼ�" << endl;
			}
		}
	}
}
//������
void Semantic::callstatement(TreeNode* t) {
	AccessKind* Ekind = NULL;
	bool present = false;
	symbtable* Entry = NULL;
	present = FindEntry(t->child[0]->name[0], Entry);
	t->child[0]->table[0] = Entry;
	if (present == false) {
		isError = true;
		cout << t->lineno << " : " << t->child[0]->name[0] << "����δ����" << endl;
	}
	else {
		if (Entry->attrIR.kind != IdKind::procKind) {
			isError = true;
			cout << t->lineno << " : " << t->child[0]->name[0] << "���ǹ�����" << endl;
		}
		else {//������ʵ�ν��
			TreeNode* p = t->child[1];
			ParamTable* paramP = Entry->attrIR.More.ProcAttr.param;
			while (p != NULL && paramP != NULL) {
				symbtable* paraEntry = paramP->entry;
				typeIR* Etp = Expr(p, Ekind);
				if ((paraEntry->attrIR.More.VarAttr.access == AccessKind::indir) && (Ekind != NULL) && (*Ekind == AccessKind::dir)) {
					isError = true;
					cout << t->lineno << " : " << "�������ƥ��" << endl;
				}
				else {
					if (paraEntry->attrIR.idtype != Etp) {
						isError = true;
						cout << t->lineno << " : " << "�������Ͳ�ƥ��" << endl;
					}
				}
				p = p->sibling;
				paramP = paramP->next;
			}
			if (p != NULL || paramP != NULL) {
				isError = true;
				cout << t->child[1]->lineno << " : " << "����������ƥ��" << endl;
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
			cout << t->lineno << " : " << "�������ʽ����bool����" << endl;
		}
		else {
			TreeNode* p = t->child[1];
			while (p != NULL) {//then����
				statement(p);
				p = p->sibling;
			}
			p = t->child[2];
			while (p != NULL) {//else����
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
			cout << t->lineno << " : " << "�������ʽ����bool����" << endl;
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
		cout << t->lineno << " : " << t->name[0] << " δ����" << endl;
	}
	else if (entry->attrIR.kind != varKind)
	{
		isError = true;
		cout << t->lineno << " : " << t->name[0] << "���Ǳ�����ʶ��" << endl;
	}
}

void Semantic::writestatement(TreeNode* t) {//˼����Ϊʲô������bool���ͣ�
	typeIR* Etp = Expr(t->child[0], NULL);
	if (Etp != NULL)
		if (Etp->kind == TypeKind::boolTy) {
			isError = true;
			cout << t->lineno << " : " << "���ʽ���ͳ���" << endl;
		}
}

void Semantic::returnstatement(TreeNode* t) {
	if (level == 0) {
		isError = true;
		cout << t->lineno << " : " << "����������������г���" << endl;
	}
}