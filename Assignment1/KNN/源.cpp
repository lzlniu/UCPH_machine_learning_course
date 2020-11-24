#define _CRT_SECURE_NO_WARNINGS
#define FILENAMEMAX 63
#include <cstdio>
#include <cstdlib>

typedef struct Node {
	int x1,x2,cls,dist;
	struct Node* next;
}Node, * LinkList;

LinkList Init() {
	LinkList L = (LinkList)malloc(sizeof(Node)); //给头结点分配空间
	if (!L) { printf("overflow!\n"); exit(0); } //空间开辟失败
	L->dist = 0; //头结点的dist位用于计数，初始化为0
	L->next = nullptr; //指针域指空
	return L;
} //初始化

void read(char file_name[], int X1, int X2, LinkList L) {
	Node* pre, * pn;
	FILE* input_file;
	input_file = fopen(file_name, "r");
	if (input_file == nullptr) { printf("cann't open %s!\n", file_name); return; }
	if (L != nullptr) {
		while (!feof(input_file)) {
			pre = L; //初始化pre等于头结点
			pn = (LinkList)malloc(sizeof(Node));
			if (!pn) { printf("overflow!\n"); return; }
			fscanf_s(input_file, "%d,%d,%d", &pn->x1, &pn->x2, &pn->cls);
			pn->dist = ((X1 - pn->x1) * (X1 - pn->x1) + (X2 - pn->x2) * (X2 - pn->x2));
			while (pre->next != nullptr && pn->dist < pre->next->dist) pre = pre->next; //找到本元素按降距排列应插入的位置
			pn->next = pre->next;
			pre->next = pn;
			++L->dist; //给输入计数
		}
	}
	return;
}
void printlist(LinkList L) {
	Node* p = L->next;
	printf_s("\nx1\tx2\tclass\tdistance\n");
	while (p != nullptr) {
		printf("%d\t%d\t%d\t%d\n",p->x1,p->x2,p->cls,p->dist);
		p = p->next;
	}
}
void count(int K, LinkList L) {
	int stat1 = 0, stat2 = 0;
	Node* p = L->next;
	while (K < p->dist) p = p->next;
	while (p != nullptr) {
		if (p->cls == 1) ++stat1;
		else if (p->cls == 0) ++stat2;
		else printf("error status!\n");
		p = p->next;
	}
	if (stat1 > stat2) printf("hypo is state 1\n");
	else if (stat2 > stat1) printf("hypo is state 0\n");
	else printf("confused!\n");
}
int main() { //K = 3; X1 = 4; X2 = 7;
	LinkList LL = Init();
	char file_name[FILENAMEMAX];
	int x1 = 0, x2 = 0, k = 0;
	printf_s("define k:");
	scanf_s("%d", &k);
	printf_s("new element x1:");
	scanf_s("%d", &x1);
	printf_s("new element x2:");
	scanf_s("%d", &x2);
	printf_s("input file name:");
	scanf_s("%s", &file_name, FILENAMEMAX);
	read(file_name, x1, x2, LL);
	printlist(LL);
	count(k, LL);
	return 0;
}