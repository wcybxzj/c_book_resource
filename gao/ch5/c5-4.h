 /* c5-4.h ϡ������ʮ�������洢��ʾ */
 typedef struct OLNode
 {
   int i,j; /* �÷���Ԫ���к����±� */
   ElemType e; /* ����Ԫ��ֵ */
   struct OLNode *right,*down; /* �÷���Ԫ�����б����б��ĺ������ */
 }OLNode,*OLink;
 typedef struct
 {
   OLink *rhead,*chead; /* �к�������ͷָ��������ַ,��CreatSMatrix_OL()���� */
   int mu,nu,tu; /* ϡ�����������������ͷ���Ԫ���� */
 }CrossList;