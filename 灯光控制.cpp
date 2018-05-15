#include<bits/stdc++.h>

using namespace std;

typedef long long lld;
/*
描述
小Hi学校的礼堂有N × M盏灯，排成N行M列的矩阵。  

为了给校庆联欢晚会增加气氛，小Hi编写了一段程序控制灯光效果：

0. 晚会开始时，小Hi会手动点亮位于(X, Y)的灯；

1. (X, Y)被点亮后，位于(X+A, Y+B)、(X+2A, Y+2B)、(X+3A, Y+3B) ... 的灯(如果该位置存在灯)也会被依次点亮；

2. (X, Y)被点亮后，位于(X+C, Y+D)、(X+2C, Y+2D)、(X+3C, Y+3D) ... 的灯(如果该位置存在灯)也会被依次点亮；

给定ABCD，初始时所有灯都是熄灭的。假设晚会开始时小Hi点亮了位于(X, Y)的灯，请你计算最终一共有多少盏灯是亮着的。

输入
第一行包含4个整数，N, M, X, Y。  

第二行包含4个整数，A, B, C, D。  

对于50%的数据，1 ≤ N, M ≤ 1000  

对于100%的数据， 1 ≤ N, M ≤ 1000000000, 1 ≤ X ≤ N, 1 ≤ Y ≤ M, -1000000 ≤ A, B, C, D ≤ 1000000  

输出
最终亮着的灯的数目。

样例输入
4 4 2 2   
1 1 0 1
样例输出
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
