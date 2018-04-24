 /* algo9-7.c ����bo9-5.c�ĳ��� */
 #include"c1.h"
 #define N 16 /* ����Ԫ�ظ��� */
 typedef struct
 {
   int ord;
 }Others; /* ��¼���������� */
 #define Nil ' ' /* ���������Ϊ�ո�(��̿��鲻ͬ) */
 #include"c9-4.h"
 #include"bo9-5.c"

 void print(Record e)
 {
   int i;
   printf("(");
   for(i=0;i<e.key.num;i++)
     printf("%c",e.key.ch[i]);
   printf(",%d)",e.others.ord);
 }

 void main()
 {
   DLTree t;
   int i;
   char s[MAXKEYLEN+1];
   KeysType k;
   Record *p;
   Record r[N]={{{"CAI"},1},{{"CAO"},2},{{"LI"},3},{{"LAN"},4},
                {{"CHA"},5},{{"CHANG"},6},{{"WEN"},7},{{"CHAO"},8},
                {{"YUN"},9},{{"YANG"},10},{{"LONG"},11},{{"WANG"},12},
                {{"ZHAO"},13},{{"LIU"},14},{{"WU"},15},{{"CHEN"},16}};
   /* ����Ԫ��(�Խ̿���ʽ9-24Ϊ��) */
   InitDSTable(&t);
   for(i=0;i<N;i++)
   {
     r[i].key.num=strlen(r[i].key.ch);
     p=SearchDLTree(t,r[i].key);
     if(!p) /* t�в����ڹؼ���Ϊr[i].key���� */
       InsertDSTable(&t,&r[i]);
   }
   printf("���ؼ��ַ�����˳�����˫������:\n");
   TraverseDSTable(t,print);
   printf("\n����������Ҽ�¼�Ĺؼ��ַ���: ");
   scanf("%s",s);
   k.num=strlen(s);
   strcpy(k.ch,s);
   p=SearchDLTree(t,k);
   if(p)
     print(*p);
   else
     printf("û�ҵ�");
   printf("\n");
   DestroyDSTable(&t);
 }