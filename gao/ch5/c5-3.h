 /* c5-3.h ϡ��������Ԫ�����߼����ӵ�˳����洢��ʾ */
 #define MAXSIZE 100 /* ����Ԫ���������ֵ */
 #define MAXRC 20 /* ��������� */
 typedef struct
 {
   int i,j; /* ���±�,���±� */
   ElemType e; /* ����Ԫ��ֵ */
 }Triple; /* ͬc5-2.h */
 typedef struct
 {
   Triple data[MAXSIZE+1]; /* ����Ԫ��Ԫ���,data[0]δ�� */
   int rpos[MAXRC+1]; /* ���е�һ������Ԫ�ص�λ�ñ�,��c5-2.h���ӵ��� */
   int mu,nu,tu; /* ����������������ͷ���Ԫ���� */
 }RLSMatrix;