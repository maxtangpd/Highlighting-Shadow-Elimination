#include<stdio.h>
#include<iostream>

void quicksort(int a[], int left, int right)
{
	int i, j, t, temp;
	if (left > right)
		return;
	temp = a[left]; //temp中存的就是基准数
	i = left;
	j = right;
	while (i != j) { //顺序很重要，要先从右边开始找
		while (a[j] >= temp && i < j)
			j--;
		while (a[i] <= temp && i < j)//再找右边的
			i++;
		if (i < j)//交换两个数在数组中的位置
		{
			t = a[i];
			a[i] = a[j];
			a[j] = t;
		}
	}
	//最终将基准数归位
	a[left] = a[i];
	a[i] = temp;
	quicksort(a,left, i - 1);//继续处理左边的，这里是一个递归的过程
	quicksort(a,i + 1, right);//继续处理右边的 ，这里是一个递归的过程
}

int main()
{
	int x;
	int a[9] = { 5,4,9,10,7,6,2,3,8 };
	quicksort(a, 0, 8);
	for (int i = 0; i<9; i++)
	{
		printf("%d ", a[i]);
	}
	x = 1 + 2 * 2;
	printf("%d ", x);
	system("pause");
	return 0;
}