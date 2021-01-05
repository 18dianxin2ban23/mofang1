#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>
 
//数据结构：
typedef enum colors
{blue=1,red,yellow,green,white,orange}Colors;//6种颜色
 
typedef struct surface
{
	Colors s[4][4];
}Surface;//每个面有3*3个小格，从下标1开始表示，每一面的颜色是固定的
 
typedef struct cube
{
	Surface up,down,front,back,left,right;
}Cube;//魔方的6个面
 
typedef struct snode
{
	char *chbuf;
	int times;
	struct snode *next;
}SNode;
 
typedef struct sequence
{
	SNode *head;//存储魔方转换序列
	int num;//魔方转换的次数
}Sequence;
 
Sequence CD;
 
//程序：
void SaveChBuf(char *sur,int i)//将cb序列存入chbuf中
{
	char *str;
	int len=strlen(sur);
	SNode *p,*q;
	if(i%4)
	{
		str=(char *)malloc(sizeof(char)*(len+2));
		if(!str)
		{
			printf("内存不足!\n");
			exit(0);
		}
		strcpy(str,sur);
		q=(SNode *)malloc(sizeof(SNode));
		if(!q)
		{
			printf("内存不足!\n");
			exit(0);
		}
		q->chbuf=str;
		q->times=i;
		q->next=NULL;
		if(CD.head==NULL)
		{
			CD.head=q;
			CD.num++;
		}
		else
		{
			for(p=CD.head;p->next;p=p->next);
			if(!strcmp(p->chbuf,q->chbuf))
			{
				p->times=(p->times+q->times)%4;
				free(q->chbuf);free(q);
				if(!(p->times))
				{
					if(p==CD.head)
					{
						CD.head=NULL;
						free(p->chbuf);free(p);
						CD.num--;
					}
					else
					{
						for(q=CD.head;q->next!=p;q=q->next);
						q->next=NULL;
						free(p->chbuf);free(p);
						CD.num--;
					}
				}
			}
			else
			{
				p->next=q;
				CD.num++;
			}
		}
	}
}
 
void clockwise(Surface *sur,int i)//将sur面顺时针旋转i次
{
	Surface t;
	for(;i>0;i--)
	{
		t=*sur;
		sur->s[1][1]=t.s[3][1];
		sur->s[1][2]=t.s[2][1];
		sur->s[1][3]=t.s[1][1];
		sur->s[2][1]=t.s[3][2];
		sur->s[2][2]=t.s[2][2];
		sur->s[2][3]=t.s[1][2];
		sur->s[3][1]=t.s[3][3];
		sur->s[3][2]=t.s[2][3];
		sur->s[3][3]=t.s[1][3];
	}
}
 
void F(Cube *m,int i)//将魔方的正面顺时针转i次
{
	Cube n;
	for(;i>0;i--)
	{
		n=*m;
		clockwise(&m->front,1);
		m->right.s[1][1]=n.up.s[3][1];
		m->right.s[2][1]=n.up.s[3][2];
		m->right.s[3][1]=n.up.s[3][3];
		m->down.s[1][1]=n.right.s[3][1];
		m->down.s[1][2]=n.right.s[2][1];
		m->down.s[1][3]=n.right.s[1][1];
		m->left.s[1][3]=n.down.s[1][1];
		m->left.s[2][3]=n.down.s[1][2];
		m->left.s[3][3]=n.down.s[1][3];
		m->up.s[3][1]=n.left.s[3][3];
		m->up.s[3][2]=n.left.s[2][3];
		m->up.s[3][3]=n.left.s[1][3];
	}
}
 
void B(Cube *m,int i)//将魔方的背面顺时针转i次
{
	Cube n;
	for(;i>0;i--)
	{
		n=*m;
		clockwise(&m->back,1);
		m->right.s[1][3]=n.down.s[3][3];
		m->right.s[2][3]=n.down.s[3][2];
		m->right.s[3][3]=n.down.s[3][1];
		m->down.s[3][1]=n.left.s[1][1];
		m->down.s[3][2]=n.left.s[2][1];
		m->down.s[3][3]=n.left.s[3][1];
		m->left.s[1][1]=n.up.s[1][3];
		m->left.s[2][1]=n.up.s[1][2];
		m->left.s[3][1]=n.up.s[1][1];
		m->up.s[1][1]=n.right.s[1][3];
		m->up.s[1][2]=n.right.s[2][3];
		m->up.s[1][3]=n.right.s[3][3];
	}
}
 
void R(Cube *m,int i)//将魔方的右面顺时针转i次
{
	Cube n;
	for(;i>0;i--)
	{
		n=*m;
		clockwise(&m->right,1);
		m->up.s[1][3]=n.front.s[1][3];
		m->up.s[2][3]=n.front.s[2][3];
		m->up.s[3][3]=n.front.s[3][3];
		m->front.s[1][3]=n.down.s[1][3];
		m->front.s[2][3]=n.down.s[2][3];
		m->front.s[3][3]=n.down.s[3][3];
		m->down.s[1][3]=n.back.s[3][1];
		m->down.s[2][3]=n.back.s[2][1];
		m->down.s[3][3]=n.back.s[1][1];
		m->back.s[3][1]=n.up.s[1][3];
		m->back.s[2][1]=n.up.s[2][3];
		m->back.s[1][1]=n.up.s[3][3];
	}
}
 
void L(Cube *m,int i)//将魔方的左面顺时针转i次
{
	Cube n;
	for(;i>0;i--)
	{
		n=*m;
		clockwise(&m->left,1);
		m->up.s[1][1]=n.back.s[3][3];
		m->up.s[2][1]=n.back.s[2][3];
		m->up.s[3][1]=n.back.s[1][3];
		m->back.s[1][3]=n.down.s[3][1];
		m->back.s[2][3]=n.down.s[2][1];
		m->back.s[3][3]=n.down.s[1][1];
		m->down.s[1][1]=n.front.s[1][1];
		m->down.s[2][1]=n.front.s[2][1];
		m->down.s[3][1]=n.front.s[3][1];
		m->front.s[1][1]=n.up.s[1][1];
		m->front.s[2][1]=n.up.s[2][1];
		m->front.s[3][1]=n.up.s[3][1];
	}
}
 
void U(Cube *m,int i)//将魔方的上面顺时针转i次
{
	Cube n;
	for(;i>0;i--)
	{
		n=*m;
		clockwise(&m->up,1);
		m->front.s[1][1]=n.right.s[1][1];
		m->front.s[1][2]=n.right.s[1][2];
		m->front.s[1][3]=n.right.s[1][3];
		m->right.s[1][1]=n.back.s[1][1];
		m->right.s[1][2]=n.back.s[1][2];
		m->right.s[1][3]=n.back.s[1][3];
		m->back.s[1][1]=n.left.s[1][1];
		m->back.s[1][2]=n.left.s[1][2];
		m->back.s[1][3]=n.left.s[1][3];
		m->left.s[1][1]=n.front.s[1][1];
		m->left.s[1][2]=n.front.s[1][2];
		m->left.s[1][3]=n.front.s[1][3];
	}
}
 
void D(Cube *m,int i)//将魔方的底面顺时针转i次
{
	Cube n;
	for(;i>0;i--)
	{
		n=*m;
		clockwise(&m->down,1);
		m->front.s[3][1]=n.left.s[3][1];
		m->front.s[3][2]=n.left.s[3][2];
		m->front.s[3][3]=n.left.s[3][3];
		m->le;
		} 
