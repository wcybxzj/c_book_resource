//优化的快速排序

#include "iostream.h"
#include "stdlib.h"
#include "string.h"
#include "Compare.h"
#include "ImprovedQuickSorter.h"

// 设定随即函数的种子
inline void Randomize() 
  { srand(1); }

//返回一个0到n-1之间的随机数
inline int Random(int n)
  { return rand() % (n); }

void main()
{
	//产生随机数组,长度为100
	Randomize();	
	int * sortarray =new int[100];
	for(int i=0;i<100;i++)
		sortarray[i]=Random(100);
	//实例化优化的快速排序类
	ImprovedQuickSorter<int,Compare> sorter;
	//输出排序前数组内容
	cout<<"排序前：";cout<<endl;
	sorter.PrintArray(sortarray,100);
	//排序
	sorter.Sort(sortarray,0,99);
	//输出排序后数组内容
	cout<<"排序后：";cout<<endl;
	sorter.PrintArray(sortarray,100);
}