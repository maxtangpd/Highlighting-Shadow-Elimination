#include<stdio.h>
#include<iostream>

void quicksort(int a[], int left, int right)
{
	int i, j, t, temp;
	if (left > right)
		return;
	temp = a[left]; //temp�д�ľ��ǻ�׼��
	i = left;
	j = right;
	while (i != j) { //˳�����Ҫ��Ҫ�ȴ��ұ߿�ʼ��
		while (a[j] >= temp && i < j)
			j--;
		while (a[i] <= temp && i < j)//�����ұߵ�
			i++;
		if (i < j)//�����������������е�λ��
		{
			t = a[i];
			a[i] = a[j];
			a[j] = t;
		}
	}
	//���ս���׼����λ
	a[left] = a[i];
	a[i] = temp;
	quicksort(a,left, i - 1);//����������ߵģ�������һ���ݹ�Ĺ���
	quicksort(a,i + 1, right);//���������ұߵ� ��������һ���ݹ�Ĺ���
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