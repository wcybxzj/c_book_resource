 /* c6-4.h ����˫�ױ��洢��ʾ */
 #define MAX_TREE_SIZE 100
 typedef struct
 {
   TElemType data;
   int parent; /* ˫��λ���� */
 } PTNode;
 typedef struct
 {
   PTNode nodes[MAX_TREE_SIZE];
   int n; /* ����� */
 } PTree;