#include <iostream>
#include <stdlib.h>
#include <string.h> 
#include "count.h" 
#include "Bit.h" 
using namespace std; 
typedef struct {
	int weight;
	int parent,lchild,rchild;
}HTnode, *HuffmanTree;
typedef struct {
	int weight;
	int No; 
}temp;
typedef char * * HuffmanCode;
void HeapAdjust(temp array[],int i,int nLength)
{
    int nChild;
    temp nTemp;
    for(; 2*i+1 < nLength; i = nChild)
    {
        nChild = 2*i+1;
        if(nChild < nLength-1 && array[nChild+1].weight > array[nChild].weight)
			++nChild;
        if(array[i].weight < array[nChild].weight)
        {
            nTemp.No = array[i].No;
            nTemp.weight = array[i].weight;
            array[i].No = array[nChild].No;
            array[i].weight = array[nChild].weight;
            array[nChild].No = nTemp.No;
			array[nChild].weight = nTemp.weight;  
        }
        else break;
    }
}
void HeapSort(temp array[],int length)
{
	temp tem; 
    int i;
    for(i = length/2-1; i >= 0; --i)
    	HeapAdjust(array,i,length);
    for(i = length-1; i > 0; --i)
    {
		tem.No = array[i].No;
		tem.weight = array[i].weight;
		array[i].No = array[0].No;
		array[i].weight = array[0].weight;
		array[0].No = tem.No; 
		array[0].weight = tem.weight; 
        HeapAdjust(array,0,i);
    }
}
void Select(HuffmanTree HT, int width, int &s1, int &s2)	//用堆实现 
{
	s1 = 0;
	s2 = 0;
	temp t[width];
	int j = 0;
	for(int i = 1; i <= width; i++)
	{
		if(HT[i].parent == 0)
		{
			t[j].No = i;
			t[j].weight = HT[i].weight;
			j++;
		}
	}
	HeapSort(t,j);
	s1 = t[0].No;
	s2 = t[1].No;

//	s1 = 0;
//	s2 = 0;
//	HT[0].weight = INT_MAX;
//	int i;
//	for(i = 1; i <= width; i++)
//	{
//		if(HT[i].parent == 0 && HT[i].weight < HT[s1].weight)
//			s1 = i;
//	}
//	for(i = 1; i <= width; i++)
//	{
//		if(HT[i].parent == 0 && HT[i].weight < HT[s2].weight && i != s1)
//			s2 = i;
//	}
}
void HuffmanCoding(LinkStack List)
{
	HuffmanCode HC;
	HuffmanTree HT;
	int n = size(List);
	int i = 0,m = 2 * n - 1,c,f;
	fstream fp;
	fp.open("CodeTable.txt",ios::out);
	int *w = (int *)malloc(n * sizeof(int));
	LinkStack q = List->next; 
	while(q)
	{
		w[i++] = q->data.count;
		q = q->next;
	}
	if(n <= 1)
		return;
	HuffmanTree p;
	HT = (HuffmanTree)malloc((m + 1) * sizeof(HTnode));
	p = HT;
	p++;
	for (i = 1; i <= n; ++i, ++p, ++w)
	{
		p->weight = *w;
		p->parent = 0;
		p->lchild = 0;
		p->rchild = 0;
	}
	for(; i <= m; ++i, ++p)
	{
		p->weight = 0;
		p->parent = 0;
		p->lchild = 0;
		p->rchild = 0;
	}
	for (i = n + 1; i <= m; ++i)
	{
		int s1,s2;
		Select(HT,i-1,s1,s2);
		HT[s1].parent = i;
		HT[s2].parent = i;
		HT[i].lchild = s1;
		HT[i].rchild = s2;
		HT[i].weight = HT[s1].weight + HT[s2].weight;
	}
	HC = (HuffmanCode)malloc((n+1) * sizeof(char *));
	char *cd = (char *)malloc(n * sizeof(char));
	cd[n-1] = '\0';
	for (i = 0; i <= n; ++i)
	{
		int start = n-1;
		for (c = i, f = HT[i].parent; f != 0; c = f, f = HT[f].parent)
		{
			if (HT[f].lchild == c)
				cd[--start] = '0';
			else
				cd[--start] = '1';
		}
		HC[i] = (char *)malloc((n - start) * sizeof(char));
		strcpy(HC[i],&cd[start]);
	}
	i = 1;
	q = List->next;
	while(q)
	{
		q->data.HuffCode = (char *)malloc((strlen(HC[i])+1) * sizeof(char));
		strcpy(q->data.HuffCode,HC[i]);
		fp<<q->data.ch<<"`"<<q->data.count<<"`"<<q->data.HuffCode<<endl;
		i++;
		q = q->next;
	}
	fp.close();
} 
void HuffmanToCode(LinkStack top)
{
	fstream fp_in,fp_out;
	char c;
	LinkStack p;
	fp_in.open("test.txt",ios::in);
	fp_out.open("TextToCode.txt",ios::out);
	while(1)
    {
    	p = top->next;
        c = fp_in.get();
        if(c == EOF)
			break;
		if(c == '\n')
			c = '|';
		while(p)
		{
			if(p->data.ch == c)
			{
				fp_out<<p->data.HuffCode;
				break;
			}
			p = p->next;
		}
    }
	fp_in.close();
	fp_out.close();
}
void ToBit()
{
	fstream fp;
	fp.open("TextToCode.txt",ios::in);
	string s;
	getline(fp,s);
	writeHuffman(s);
	fp.close();
	remove ("TextToCode.txt");
}
int main()
{
	LinkStack List;
	InitCharacterList(List); 
	CountCharacter(List);
	reverse(List);
	HuffmanCoding(List);	//生成码表 
	HuffmanToCode(List);	//对文章进行转码 
	ToBit();				//压缩成二进制 
	cout<<"转码完成！"; 
	return 0;
}

