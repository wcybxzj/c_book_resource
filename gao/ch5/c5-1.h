 /* c5-1.h �����˳��洢��ʾ */
 #include<stdarg.h> /* ��׼ͷ�ļ����ṩ��va_start��va_arg��va_end�� */
                    /* ���ڴ�ȡ�䳤������ */
 #define MAX_ARRAY_DIM 8 /* ��������ά�������ֵΪ8 */
 typedef struct
 {
   ElemType *base; /* ����Ԫ�ػ�ַ����InitArray���� */
   int dim; /* ����ά�� */
   int *bounds; /* ����ά���ַ����InitArray���� */
   int *constants; /* ����ӳ����������ַ����InitArray���� */
 }Array;