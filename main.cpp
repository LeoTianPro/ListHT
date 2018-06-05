#include<stdio.h>
#include<stdlib.h>
#include <string.h>

char buffer[1024];
int cur = 0;

//1.定义结点
typedef struct GLNode {
    int tag;   //表结点类型  (tag=0表示原子结点，tag=1表示表结点)
    union {
        char data[100];
        struct {
            struct GLNode *hr, *tr; //hr为表头指针，tr为表尾指针
        };
    };
} *GList;

//2.建广义表
void CreatList(GList &Ls) {
    char c;
    c = buffer[cur++];  //通过c=getchar()获取一个合法字符;
    if (c == ' ') {  //空表的情况
        Ls = NULL;
        c = buffer[cur++];  //c=getchar();
        if (c != ')')  //空表的下一个合法字符应该是')'
            return;
    } else {   //输入的广义表非空时
        GList p;
        Ls = (GList) malloc(sizeof(GLNode));
        memset(Ls, 0, sizeof(GLNode));
        Ls->tag = 1;
        //表结点
        //表头为单原子
        if (c != '(') {
            Ls->hr = (GList) malloc(sizeof(GLNode));
            p = Ls->hr;
            p->tag = 0;

            int i = 0;
            memset(p->data, 0, sizeof(p->data));
            p->data[i++] = c;
            while (c = buffer[cur++]) { //c=getchar())
                if (c == ',' || c == ')') {
                    cur--;
                    p->data[i] = '\0';
                    break;
                }
                p->data[i++] = c;
            }
            //建立原子结点
        } else {
            //  表头为广义表
            CreatList(Ls->hr);
            // 对此广义表递归建立存储结构
        }
        c = buffer[cur++];  //c=getchar();
        if (c == ',')
            CreatList(Ls->tr);  //当前广义表未结束，等待输入下一个子表
        else if (c == ')')
            Ls->tr = NULL; //当前广义表输入结束
    }
}

// 3.输出原子
void GL_Elem(GList p) {
    printf("%s", p->data);
}


//4.输出Ls指针所指向的广义表
void printf_GL(GList Ls, int i) {
    if (Ls == NULL) return;
    GList p = Ls->hr; //用p记录表结点的头指针
    if (!p) {
        printf("(  )"); //输出空表
    } else {
        //p指向表结点
        if (p->tag == 1) {
            printf("(");
            i++;  //用i记录括号数
            printf_GL(p, i);
        } else if (p->tag == 0) {
            GL_Elem(p); //若p指向原子结点则输出原子
        }
    }
    GList k = Ls->tr; //用k记录表结点的尾指针
    if (k) {
        printf(","); //尾指针存在表示此表中还有元素
        printf_GL(k, i); //遍历下一结点
    } else if (!k && i) {
        printf(")");
    }
}


//5.输出广义表的表头
void GetHead(GList &Ls) {
    printf("\n上表的表头为：      ");
    GList p = Ls; //保存头指针
    p->tr = NULL; //将Ls所指结点从原广义表中独立开来，单独遍历
    int i = 0;
    printf_GL(Ls, i);
    Ls = Ls->hr;
}


//6.输出广义表的表尾
void GetTail(GList &Ls) {
    printf("\n上表的表尾为：      ");
    Ls = Ls->tr;
    if (!Ls)
        printf("( )");  //Ls指向空表
    else {
        printf("(");  // 表尾第一个字符为"（"
        int i = 1; //已有左括号，故i=1
        printf_GL(Ls, i);
    }
}


//7.求广义表表头（尾）的操作函数
void Get_HT(GList Ls) {
    char ch = getchar();
    while (ch) {
        GList p = Ls; //用p保存表头指针
        if (!p) {
            printf("\n当前表为空表，不能执行求表头（尾）操作!\n");
            break;
        } else {
            switch (ch) {
                case 't' :
                    GetTail(Ls);
                    break;
                case 'h' :
                    GetHead(Ls);
                    break;
                case ' ' : { //空串时输出整个广义表
                    printf("\n上表为：             ");
                    if (!Ls) printf("( )");  //   Ls指向空表
                    else {
                        printf("(");
                        int i = 1;
                        printf_GL(Ls, i);
                    }
                }
                    break;
                default :
                    return;
            }
        }
        ch = getchar();
    }

}

//8.主函数
int main() {
    printf("\t\t\t\t*****识别广义表的“头”或“尾”的演示********\n\n");
    printf("请输入广义表:\n\n");
    memset(buffer, 0, sizeof(buffer));
    int i = 0, j = 0;
    for (;;) {
        char c1 = getchar();
        if (c1 == '$') {
            break;
        }
        if (c1 == 10 || c1 == ' ') { //允许多行输入而且输入空串时允许多个空格
            continue;
        }
        if (c1 == -93) {
            continue;
        }
        if (c1 == -84) {
            c1 = ',';
        }
        if (c1 == -87) {
            c1 = ')';
        }
        if (c1 == -88) {
            c1 = '(';
        }

        if (c1 == ')' && buffer[i - 1] == '(') {  //输出空串时自动在括号中添上空格 ，使结果美观
            buffer[i++] = ' ';
        }
        buffer[i++] = c1;
    }
    char c;
    c = buffer[cur++]; //c=getchar();
    if (c != '(') {
        printf("广义表第一个字符必须是'('或'（'，请重新输入！");
        return -1;  //广义表第一个字符必须是'('，否则终止函数
    }
    GList Ls;
    CreatList(Ls);
    while ((c = getchar()) != EOF && c != '\n');  //清除缓冲区
    //为了美观，将规范输出所输入的广义表
    printf("\n输入的广义表为:\n\n");
    if (!Ls) printf("( )");  //Ls指向空表
    else {
        printf("(");  // 广义表第一个字符为"（"
        int i = 1;  //在调用printf_GL( )之前，已输出一个左括号，故i=1
        printf_GL(Ls, i);
    }
    printf("\n");
    printf("\n请输入头尾操作序列：   ");
    Get_HT(Ls);
    printf("\n\n");
    printf("**********请输入任意按键结束**********");
    getchar();
    return 0;
}


