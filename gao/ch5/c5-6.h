 /* c5-6.h ���������չ���������洢��ʾ */
 typedef enum{ATOM,LIST}ElemTag; /* ATOM==0:ԭ��,LIST==1:�ӱ� */
 typedef struct GLNode
 {
   ElemTag tag; /* ��������,��������ԭ�ӽ��ͱ���� */
   union /* ԭ�ӽ��ͱ��������ϲ��� */
   {
     AtomType atom; /* ԭ�ӽ���ֵ�� */
     struct GLNode *hp; /* �����ı�ͷָ�� */
   }a;
   struct GLNode *tp; /* �൱������������next,ָ����һ��Ԫ�ؽ�� */
 }*GList,GLNode; /* ���������GList��һ����չ���������� */