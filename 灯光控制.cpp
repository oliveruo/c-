#include<bits/stdc++.h>

using namespace std;

typedef long long lld;
/*
����
СHiѧУ��������N �� Mյ�ƣ��ų�N��M�еľ���  

Ϊ�˸�У����������������գ�СHi��д��һ�γ�����Ƶƹ�Ч����

0. ��Ὺʼʱ��СHi���ֶ�����λ��(X, Y)�ĵƣ�

1. (X, Y)��������λ��(X+A, Y+B)��(X+2A, Y+2B)��(X+3A, Y+3B) ... �ĵ�(�����λ�ô��ڵ�)Ҳ�ᱻ���ε�����

2. (X, Y)��������λ��(X+C, Y+D)��(X+2C, Y+2D)��(X+3C, Y+3D) ... �ĵ�(�����λ�ô��ڵ�)Ҳ�ᱻ���ε�����

����ABCD����ʼʱ���еƶ���Ϩ��ġ�������ῪʼʱСHi������λ��(X, Y)�ĵƣ������������һ���ж���յ�������ŵġ�

����
��һ�а���4��������N, M, X, Y��  

�ڶ��а���4��������A, B, C, D��  

����50%�����ݣ�1 �� N, M �� 1000  

����100%�����ݣ� 1 �� N, M �� 1000000000, 1 �� X �� N, 1 �� Y �� M, -1000000 �� A, B, C, D �� 1000000  

���
�������ŵĵƵ���Ŀ��

��������
4 4 2 2   
1 1 0 1
�������
5
*/ 
int getNum(int n, int m,int x, int y, lld a, lld b )
{
	if( !a && !b) return 0;
	int lx = x - 1;
	int ly = y - 1;
	if(a > 0) lx = n -x;
	if(b > 0) ly = m - y;
	if(!a) return ly / abs(b);
	if(!b) return lx / abs(a);
	return min(lx/ abs(a), ly / abs(b));
}

lld getLcm(lld a, lld b)
{
	if(a * b == 0) return 1;
	if(a %b == 0) return b;
	return getLcm(b, a % b);
}

lld getGcd(lld a, lld b)
{
	a /= getLcm(a ,b);
	return a * b;
}
void slove()
{
	int n,m,x,y;
	lld a,b,c,d;
	cin>>n>>m>>x>>y;
	cin>>a>>b>>c>>d;
	if(a == 0 && b == 0 && c == 0 && d == 0)
	{
		cout<<1;
		return ;
	}
	lld sum =0;
	sum += getNum(n,m,x,y,a,b);
	sum += getNum(n,m,x,y,c,d);
	if(a * b == c* b && a * c > 0)
	{
		lld lcm = getGcd(abs(a) , abs(c)) * (a < 0 ? -1: 1);
		lld ylcm = getGcd(abs(b) ,abs(d) * (b < 0 ? -1 : 1));
		
		sum -= getNum(n,m,x,y,lcm,ylcm);
	}
	else if(a == 0 && c == 0 && b * d > 0)
	{
		lld ylcm = getGcd( abs(b), abs(d)) * (b < 0 ? -1 : 1);
		sum -= getNum(n,m,x,y,0,ylcm);
	}
	else if(a * c > 0 && b == 0 && d == 0)
	{
		lld lcm = getGcd(abs(b) , abs(d)) * (b < 0 ? -1 :1);
		sum -= getNum(n,m,x,y,lcm,0);
	}
	sum ++;
	cout<<sum<<endl;
}

int main()
{
	slove();
	return 0;
 } 
