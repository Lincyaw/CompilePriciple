# CompilePriciple
## Lab2 LR(1)词法分析
上次上课的时候已经是11月12号了,到现在过去了将近10天,
现在看一下上次写了啥。

上次采用的是自顶向下的设计方法，先假设已经有了`LR(1)`
分析表。那么我就写了下面的一些东西：

1. LR(1)的分析过程,函数名为`Grammar_Analyzer::LR1(string Input)`
2. 初始化动作表`initActionMap()`
3. 初始化转移表`initGotoMap()`


### 第二天
再次思考了一下自己到底要怎么做。
```c
void bubble_sort(int a[], int n);
int number[10000000];
int a = 0x123FF;
int b = 1234.123;
int c = 0123.123;
void bubble_sort(int a[], int n)
{
    int i,j,temp;
    for (j=0;j<n-1;j++)
    {                           
        for (i=0;i<n-1-j;i++)
        {
            if(a[i]>a[i+1])
            {
                temp=a[i];
                a[i]=a[i+1];
                a[i+1]=temp;
            }
        }
    }    
}

int main()
{
    int i,n;
    scanf("%d",&n);
    for(int j=0;j<n;j++)
        scanf("%d",&number[j]) ;
    bubble_sort(number,n);
    for (i=0;i<n-1;i++)
        printf("%d ",number[i]);
    printf("%d\n",number[i]);
}
```

按该代码进行分析的话,我需要以下的文法:
1. 函数的定义
2. 基本类型的定义,如char,int,float,void以及对应的数组。不考虑2维数组。
3. 循环、分支、判断、赋值语句