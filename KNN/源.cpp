#define _CRT_SECURE_NO_WARNINGS
#define FILENAMEMAX 63
#include <cstdio>
#include <cstdlib>

typedef struct Node {
	float x1,x2,dist;
	int cls;
	struct Node* next;
}Node, * LinkList; //��¼�������ݣ��洢�ṹΪ������

typedef struct Cls {
	int cls,stat;
	struct Cls* next;
}Cls, * ClsLinkList; //ͳ���������ݵķ����Լ�����ͳ�Ƹ�����KNN�е��������洢�ṹΪ������

ClsLinkList ClsInit() {
	ClsLinkList CL = (ClsLinkList)malloc(sizeof(Cls));
	if (!CL) { printf("overflow!\n"); exit(0); }
	CL->cls = 0; //ͷ���cls���ڴ洢�����ȣ�Ԫ������������ʼ��Ϊ0��ͷ����stat����
	CL->next = nullptr;
	return CL;
}

void ClsNum(int cls, ClsLinkList CL) {
	Cls* pre, * pn;
	if (CL != nullptr) { //ȷ�����������ǿ�
		pre = CL; //������ǰ��������ͷ���
		pn = (ClsLinkList)malloc(sizeof(Cls)); //���ٿռ�洢��Ԫ��
		if (!pn) { printf("overflow!\n"); return; } //����ʧ��
		pn->cls = cls; //������𴢴浽�˽���cls
		pn->stat = 0; //ÿ���������KNN������ʱͳһΪ0
		while (pre->next != nullptr && pn->cls < pre->next->cls) pre = pre->next; //�ҵ���Ԫ����𰴽�������Ӧ�����λ��
		if (pre->next != NULL && pn->cls == pre->next->cls) free(pn); //Ԫ����ͬ���������
		else { 
			pn->next = pre->next; 
			pre->next = pn; 
			++CL->cls; //���ӷ������
		} //Ԫ�ز�ͬ������Ԫ��
	}
	return;
} //�˴�������cls��Ϊint����

LinkList Init() {
	LinkList L = (LinkList)malloc(sizeof(Node)); //��ͷ������ռ�
	if (!L) { printf("overflow!\n"); exit(0); } //�ռ俪��ʧ��
	L->dist = 0; //ͷ����dist���ڴ������ȣ�Ԫ������������ʼ��Ϊ0
	L->cls = 0; //ͷ����cls���ڴ�������Ԫ�������������ʼ��Ϊ0
	L->next = nullptr; //ָ����ָ��
	return L;
} //��ʼ��

ClsLinkList read(char file_name[], float X1, float X2, LinkList L) {
	Node* pre, * pn;
	FILE* input_file;
	ClsLinkList CL = ClsInit();
	input_file = fopen(file_name, "r");
	if (input_file == nullptr) { printf("cann't open %s!\n", file_name); exit(0); }
	if (L != nullptr) {
		while (!feof(input_file)) {
			pre = L; //��ʼ��pre����ͷ���
			pn = (LinkList)malloc(sizeof(Node)); //���ٿռ�洢��Ԫ��
			if (!pn) { printf("overflow!\n"); exit(0);} //����ʧ��
			fscanf_s(input_file, "%f,%f,%d", &pn->x1, &pn->x2, &pn->cls); //��ȡ�ļ����ݣ����ŷָ��ʽ��
			pn->dist = ((X1 - pn->x1) * (X1 - pn->x1) + (X2 - pn->x2) * (X2 - pn->x2)); //����ÿ��Ԫ�ص���Ԫ�صľ���
			ClsNum(pn->cls, CL); //��Ԫ������
			while (pre->next != nullptr && pn->dist < pre->next->dist) pre = pre->next; //�ҵ���Ԫ�ذ���������Ӧ�����λ��
			pn->next = pre->next; pre->next = pn; //���뱾Ԫ��
			++L->dist; //����Ԫ�ؼ������浽ͷ���dist��
		}
		L->cls = CL->cls; //Ԫ�������������ݵ�����L
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
} //��ʾ����ȫ��Ԫ��
void sort(ClsLinkList CL) {
	if (CL == nullptr || CL->next == nullptr) { //����Ϊ�ջ��ͷ���
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
	while (p->next != nullptr) {//��pfָ��Ľڵ�Ϊ��׼�ڵ�
		pb = p->next;//pb�ӻ�׼�����һ���ڵ㿪ʼ
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
} //����
void printCls(ClsLinkList CL) {
	Cls* p = CL->next;
	printf_s("\nclass\tnumber\n");
	while (p != nullptr) {
		printf("%d\t%d\n", p->cls, p->stat);
		p = p->next;
	}
	return;
} //��ʾ����ȫ��Ԫ��
void count(int K, LinkList L,ClsLinkList CL) {
	Node* p = L->next;
	Cls* c;
	//while (K < p->dist) p = p->next; //KΪ�뾶
	for (int i = L->dist; i > K; i--) p = p->next; //KΪ������, L->dist��������Ԫ�ظ���
	while (p != nullptr) { //����ѭ��ֱ��������ֹ
		c = CL->next; //c��ʼ������������ĵ�һ�����
		while (c != nullptr) { //�������з���
			if (p->cls == c->cls) ++c->stat; //�����ͳ��ֵ����
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