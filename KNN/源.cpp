#define _CRT_SECURE_NO_WARNINGS
#define FILENAMEMAX 63
#include <cstdio>
#include <cstdlib>

typedef struct Node {
	float x1,x2,dist;
	int cls;
	struct Node* next;
}Node, * LinkList; //记录输入数据，存储结构为单链表

typedef struct Cls {
	int cls,stat;
	struct Cls* next;
}Cls, * ClsLinkList; //统计输入数据的分类以及最终统计该类在KNN中的数量，存储结构为单链表

ClsLinkList ClsInit() {
	ClsLinkList CL = (ClsLinkList)malloc(sizeof(Cls));
	if (!CL) { printf("overflow!\n"); exit(0); }
	CL->cls = 0; //头结点cls用于存储链表长度（元素数量），初始化为0，头结点的stat不管
	CL->next = nullptr;
	return CL;
}

void ClsNum(int cls, ClsLinkList CL) {
	Cls* pre, * pn;
	if (CL != nullptr) { //确定输入的链表非空
		pre = CL; //先设置前驱结点等于头结点
		pn = (ClsLinkList)malloc(sizeof(Cls)); //开辟空间存储新元素
		if (!pn) { printf("overflow!\n"); return; } //开辟失败
		pn->cls = cls; //分类类别储存到此结点的cls
		pn->stat = 0; //每个分类类别KNN计数此时统一为0
		while (pre->next != nullptr && pn->cls < pre->next->cls) pre = pre->next; //找到本元素类别按降序排列应插入的位置
		if (pre->next != NULL && pn->cls == pre->next->cls) free(pn); //元素相同，不予插入
		else { 
			pn->next = pre->next; 
			pre->next = pn; 
			++CL->cls; //增加分类计数
		} //元素不同，插入元素
	}
	return;
} //此处分类标记cls均为int型数

LinkList Init() {
	LinkList L = (LinkList)malloc(sizeof(Node)); //给头结点分配空间
	if (!L) { printf("overflow!\n"); exit(0); } //空间开辟失败
	L->dist = 0; //头结点的dist用于存链表长度（元素数量），初始化为0
	L->cls = 0; //头结点的cls用于存链表中元素类别数量，初始化为0
	L->next = nullptr; //指针域指空
	return L;
} //初始化

ClsLinkList read(char file_name[], float X1, float X2, LinkList L) {
	Node* pre, * pn;
	FILE* input_file;
	ClsLinkList CL = ClsInit();
	input_file = fopen(file_name, "r");
	if (input_file == nullptr) { printf("cann't open %s!\n", file_name); exit(0); }
	if (L != nullptr) {
		while (!feof(input_file)) {
			pre = L; //初始化pre等于头结点
			pn = (LinkList)malloc(sizeof(Node)); //开辟空间存储新元素
			if (!pn) { printf("overflow!\n"); exit(0);} //开辟失败
			fscanf_s(input_file, "%f,%f,%d", &pn->x1, &pn->x2, &pn->cls); //读取文件内容（逗号分割格式）
			pn->dist = ((X1 - pn->x1) * (X1 - pn->x1) + (X2 - pn->x2) * (X2 - pn->x2)); //计算每个元素到新元素的距离
			ClsNum(pn->cls, CL); //求元素种数
			while (pre->next != nullptr && pn->dist < pre->next->dist) pre = pre->next; //找到本元素按降距排列应插入的位置
			pn->next = pre->next; pre->next = pn; //插入本元素
			++L->dist; //输入元素计数储存到头结点dist中
		}
		L->cls = CL->cls; //元素种数参数传递到链表L
		printf("Class number: %d", L->cls);
	}
	return CL;
}
void printlist(LinkList L) {
	Node* p = L->next;
	printf_s("\nx1\tx2\tclass\tdistance\n");
	while (p != nullptr) {
		printf("%.2f\t%.2f\t%d\t%.2f\n",p->x1,p->x2,p->cls,p->dist);
		p = p->next;
	}
	return;
} //显示链表全部元素
void sort(ClsLinkList CL) {
	if (CL == nullptr || CL->next == nullptr) { //链表为空或仅头结点
		printf("needn't order.\n");
		return;
	}
	Cls* p, * pb, temp;
	/*
	p = CL->next;
	while (p != nullptr) {
		q = p;
		if (p->stat > CL->next->stat) {
			temp = CL->next;
			temp->next = p->next;
			CL->next = p;
			p->next = temp;
		}
		p = q->next;
	}
	*/
	p = CL->next;
	while (p->next != nullptr) {//以pf指向的节点为基准节点
		pb = p->next;//pb从基准点的下一个节点开始
		while (pb != nullptr) {
			if (pb->stat > p->stat) {
				temp = *p;
				*p = *pb;
				*pb = temp;
				temp.next = p->next;
				p->next = pb->next;
				pb->next = temp.next;
			}
			pb = pb->next;
		}
		p = p->next;
	}
	return;
} //排序
void printCls(ClsLinkList CL) {
	Cls* p = CL->next;
	printf_s("\nclass\tnumber\n");
	while (p != nullptr) {
		printf("%d\t%d\n", p->cls, p->stat);
		p = p->next;
	}
	return;
} //显示链表全部元素
void count(int K, LinkList L,ClsLinkList CL) {
	Node* p = L->next;
	Cls* c;
	//while (K < p->dist) p = p->next; //K为半径
	for (int i = L->dist; i > K; i--) p = p->next; //K为近邻数, L->dist储存链表元素个数
	while (p != nullptr) { //继续循环直到链表终止
		c = CL->next; //c初始化到分类类别表的第一个结点
		while (c != nullptr) { //遍历所有分类
			if (p->cls == c->cls) ++c->stat; //该类别统计值增加
			c = c->next;
		}
		p = p->next;
	}
	sort(CL);
	printCls(CL);
	if (CL->next != nullptr) {
		if (CL->next->next != nullptr) {
			if (CL->next->stat > CL->next->next->stat) printf("hypo is state %d\n", CL->next->cls);
			else printf("confused!\n");
		}
		else printf("hypo is state %d\n", CL->next->cls);
	}
	return;
}
int main() {
	LinkList LL = Init();
	char file_name[FILENAMEMAX]= "input.txt";
	float x1, x2;
	int k;
	printf_s("define k:");
	scanf_s("%d", &k);
	printf_s("new element x1:");
	scanf_s("%f", &x1);
	printf_s("new element x2:");
	scanf_s("%f", &x2);
	//printf_s("input file name:");
	//scanf_s("%s", &file_name, FILENAMEMAX);
	ClsLinkList CC = read(file_name, x1, x2, LL);
	printlist(LL);
	count(k, LL, CC);
	return 0;
}