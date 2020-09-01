#include<stdio.h>
#include<iostream>
#include<string>

using namespace std;
int n = 0;
int m = 0;

int strlength(char x) {
	int i;
	char *p = &x;
	if (p == NULL) return 0;
	for (i = 0; *p != '\n'; p++) i++;
	return i;
}

int Index(string S, string T, int pos) {
	int i;
	if (pos > 0) {
		const char *q = S.c_str();
		const char *w = T.c_str();
		n = strlength(*q); m = strlength(*w); int  j = pos;
		while (i < n - m + 1)
			substr();
	}

}