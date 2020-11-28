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

## Lab2完成
支持函数\for循环\常规的声明\加减乘除\一维数组,
以及函数和循环之间的嵌套

需要注意的是,在输入代码的时候,在同级语句之间需要加'#',末尾不需要也不能加'#'
这都是因为自己设计的操蛋的文法过于垃圾



# Lab3

## 11.28 准备工作

三地址码：

​	这种代码的每条指令最多只能包含三个地址，即一个操作数地址和一个结果地址。`a = b + c`，`b = a * b`都是三地址码。

四元式：

​	由4个域组成，分别是$op$,$arg_1$,$arg_2$,$result$. 

![image-20201128150500237](/home/llincyaw/.config/Typora/typora-user-images/image-20201128150500237.png)

通过4元式，可以清楚地表达三地址码。如第一行即表示了$t_2=c+d$



抽象语法树(abstract syntax tree)是表示程序层次结构的树，他把分析树中对语义无关紧要的的成分去掉，是分析树（语法树）的抽象形式，也成为语法结构树或结构树。构建一颗语法抽象树有以下函数：

1. mkleaf(id,entry)用于建立标记为id的标识符节点，其域entry指向该标识符在符号表中的对应表项。

