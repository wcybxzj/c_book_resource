 /* c8-2.h ���ϵͳ�����ÿռ���Ľṹ */
 #define m 10 /* �����ÿռ�������1024�ֵ�2���ݴ�,�ӱ��ĸ���Ϊm+1 */
 typedef struct WORD
 {
   struct WORD *llink; /* ָ��ǰ����� */
   int tag; /* ���־,0:����,1:ռ�� */
   int kval; /* ���С,ֵΪ2���ݴ�k */
   struct WORD *rlink; /* ͷ����,ָ���̽�� */
 }WORD,head,*Space; /* WORD:�ڴ�������,���ĵ�һ����Ҳ��Ϊhead */

 typedef struct HeadNode
 {
   int nodesize; /* �������Ŀ��п�Ĵ�С */
   struct WORD *first; /* �������ı�ͷָ�� */
 }FreeList[m+1]; /* ��ͷ�������� */