 /* c6-3.h �������Ķ��������洢��ʾ */
 typedef enum{Link,Thread}PointerTag; /* Link(0):ָ��,Thread(1):���� */
 typedef struct BiThrNode
 {
   TElemType data;
   struct BiThrNode *lchild,*rchild; /* ���Һ���ָ�� */
   PointerTag LTag,RTag; /* ���ұ�־ */
 }BiThrNode,*BiThrTree;