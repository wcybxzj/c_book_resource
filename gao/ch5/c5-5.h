 /* c5-5.h �������ͷβ�����洢��ʾ */
 typedef enum{ATOM,LIST}ElemTag; /* ATOM==0:ԭ��,LIST==1:�ӱ� */
 typedef struct GLNode
 {
   ElemTag tag; /* ��������,��������ԭ�ӽ��ͱ���� */
   union /* ԭ�ӽ��ͱ��������ϲ��� */
   {
     AtomType atom; /* atom��ԭ�ӽ���ֵ��,AtomType���û����� */
     struct
     {
       struct GLNode *hp,*tp;
     }ptr; /* ptr�Ǳ�����ָ����,prt.hp��ptr.tp�ֱ�ָ���ͷ�ͱ�β */
   }a;
 }*GList,GLNode; /* ��������� */