 /* c9-4.h ˫�����Ĵ洢�ṹ */
 #define MAXKEYLEN 16 /* �ؼ��ֵ���󳤶� */
 typedef struct
 {
   char ch[MAXKEYLEN]; /* �ؼ��� */
   int num; /* �ؼ��ֳ��� */
 }KeysType; /* �ؼ������� */

 typedef struct
 {
   KeysType key; /* �ؼ��� */
   Others others; /* ��������(�����̶���) */
 }Record; /* ��¼���� */

 typedef enum{LEAF,BRANCH}NodeKind; /* �������:{Ҷ��,��֧} */
 typedef struct DLTNode /* ˫�������� */
 {
   char symbol;
   struct DLTNode *next; /* ָ���ֵܽ���ָ�� */
   NodeKind kind;
   union
   {
     Record *infoptr; /* Ҷ�ӽ��ļ�¼ָ�� */
     struct DLTNode *first; /* ��֧���ĺ�����ָ�� */
   }a;
 }DLTNode,*DLTree;