 /* main5-51.c ����bo5-5.c��bo5-51.c(�ַ���ΪSString����)�������� */
 #include"c1.h"
 typedef char AtomType; /* ����ԭ������Ϊ�ַ��� */
 #include"c5-5.h" /* ����������ͷβ�����洢 */
 #include"bo5-5.c"
 #include"bo5-51.c"

 void visit(AtomType e)
 {
   printf("%c ", e);
 }

 void main()
 {
   char p[80];
   SString t;
   GList l,m;
   InitGList(&l);
   InitGList(&m);
   printf("�չ����l�����=%d l�Ƿ�գ�%d(1:�� 0:��)\n",GListDepth(l),GListEmpty(l));
   printf("����������l(��д��ʽ���ձ�:(),��ԭ��:a,����:(a,(b),b)):\n");
   gets(p);
   StrAssign(t,p);
   CreateGList(&l,t);
   printf("�����l�ĳ���=%d\n",GListLength(l));
   printf("�����l�����=%d l�Ƿ�գ�%d(1:�� 0:��)\n",GListDepth(l),GListEmpty(l));
   printf("���������l��\n");
   Traverse_GL(l,visit);
   printf("\n���ƹ����m=l\n");
   CopyGList(&m,l);
   printf("�����m�ĳ���=%d\n",GListLength(m));
   printf("�����m�����=%d\n",GListDepth(m));
   printf("���������m��\n");
   Traverse_GL(m,visit);
   DestroyGList(&m);
   m=GetHead(l);
   printf("\nm��l�ı�ͷ�����������m��\n");
   Traverse_GL(m,visit);
   DestroyGList(&m);
   m=GetTail(l);
   printf("\nm��l�ı�β�����������m��\n");
   Traverse_GL(m,visit);
   InsertFirst_GL(&m,l);
   printf("\n����lΪm�ı�ͷ�����������m��\n");
   Traverse_GL(m,visit);
   printf("\nɾ��m�ı�ͷ�����������m��\n");
   DestroyGList(&l);
   DeleteFirst_GL(&m,&l);
   Traverse_GL(m,visit);
   printf("\n");
   DestroyGList(&m);
 }