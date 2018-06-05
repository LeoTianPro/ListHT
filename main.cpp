#include<stdio.h>
#include<stdlib.h>
#include <string.h>

char buffer[1024];
int cur = 0;

//1.������
typedef struct GLNode {
    int tag;   //��������  (tag=0��ʾԭ�ӽ�㣬tag=1��ʾ����)
    union {
        char data[100];
        struct {
            struct GLNode *hr, *tr; //hrΪ��ͷָ�룬trΪ��βָ��
        };
    };
} *GList;

//2.�������
void CreatList(GList &Ls) {
    char c;
    c = buffer[cur++];  //ͨ��c=getchar()��ȡһ���Ϸ��ַ�;
    if (c == ' ') {  //�ձ�����
        Ls = NULL;
        c = buffer[cur++];  //c=getchar();
        if (c != ')')  //�ձ����һ���Ϸ��ַ�Ӧ����')'
            return;
    } else {   //����Ĺ����ǿ�ʱ
        GList p;
        Ls = (GList) malloc(sizeof(GLNode));
        memset(Ls, 0, sizeof(GLNode));
        Ls->tag = 1;
        //����
        //��ͷΪ��ԭ��
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
            //����ԭ�ӽ��
        } else {
            //  ��ͷΪ�����
            CreatList(Ls->hr);
            // �Դ˹����ݹ齨���洢�ṹ
        }
        c = buffer[cur++];  //c=getchar();
        if (c == ',')
            CreatList(Ls->tr);  //��ǰ�����δ�������ȴ�������һ���ӱ�
        else if (c == ')')
            Ls->tr = NULL; //��ǰ������������
    }
}

// 3.���ԭ��
void GL_Elem(GList p) {
    printf("%s", p->data);
}


//4.���Lsָ����ָ��Ĺ����
void printf_GL(GList Ls, int i) {
    if (Ls == NULL) return;
    GList p = Ls->hr; //��p��¼�����ͷָ��
    if (!p) {
        printf("(  )"); //����ձ�
    } else {
        //pָ�����
        if (p->tag == 1) {
            printf("(");
            i++;  //��i��¼������
            printf_GL(p, i);
        } else if (p->tag == 0) {
            GL_Elem(p); //��pָ��ԭ�ӽ�������ԭ��
        }
    }
    GList k = Ls->tr; //��k��¼�����βָ��
    if (k) {
        printf(","); //βָ����ڱ�ʾ�˱��л���Ԫ��
        printf_GL(k, i); //������һ���
    } else if (!k && i) {
        printf(")");
    }
}


//5.��������ı�ͷ
void GetHead(GList &Ls) {
    printf("\n�ϱ�ı�ͷΪ��      ");
    GList p = Ls; //����ͷָ��
    p->tr = NULL; //��Ls��ָ����ԭ������ж�����������������
    int i = 0;
    printf_GL(Ls, i);
    Ls = Ls->hr;
}


//6.��������ı�β
void GetTail(GList &Ls) {
    printf("\n�ϱ�ı�βΪ��      ");
    Ls = Ls->tr;
    if (!Ls)
        printf("( )");  //Lsָ��ձ�
    else {
        printf("(");  // ��β��һ���ַ�Ϊ"��"
        int i = 1; //���������ţ���i=1
        printf_GL(Ls, i);
    }
}


//7.�������ͷ��β���Ĳ�������
void Get_HT(GList Ls) {
    char ch = getchar();
    while (ch) {
        GList p = Ls; //��p�����ͷָ��
        if (!p) {
            printf("\n��ǰ��Ϊ�ձ�����ִ�����ͷ��β������!\n");
            break;
        } else {
            switch (ch) {
                case 't' :
                    GetTail(Ls);
                    break;
                case 'h' :
                    GetHead(Ls);
                    break;
                case ' ' : { //�մ�ʱ������������
                    printf("\n�ϱ�Ϊ��             ");
                    if (!Ls) printf("( )");  //   Lsָ��ձ�
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

//8.������
int main() {
    printf("\t\t\t\t*****ʶ������ġ�ͷ����β������ʾ********\n\n");
    printf("����������:\n\n");
    memset(buffer, 0, sizeof(buffer));
    int i = 0, j = 0;
    for (;;) {
        char c1 = getchar();
        if (c1 == '$') {
            break;
        }
        if (c1 == 10 || c1 == ' ') { //������������������մ�ʱ�������ո�
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

        if (c1 == ')' && buffer[i - 1] == '(') {  //����մ�ʱ�Զ������������Ͽո� ��ʹ�������
            buffer[i++] = ' ';
        }
        buffer[i++] = c1;
    }
    char c;
    c = buffer[cur++]; //c=getchar();
    if (c != '(') {
        printf("������һ���ַ�������'('��'��'�����������룡");
        return -1;  //������һ���ַ�������'('��������ֹ����
    }
    GList Ls;
    CreatList(Ls);
    while ((c = getchar()) != EOF && c != '\n');  //���������
    //Ϊ�����ۣ����淶���������Ĺ����
    printf("\n����Ĺ����Ϊ:\n\n");
    if (!Ls) printf("( )");  //Lsָ��ձ�
    else {
        printf("(");  // ������һ���ַ�Ϊ"��"
        int i = 1;  //�ڵ���printf_GL( )֮ǰ�������һ�������ţ���i=1
        printf_GL(Ls, i);
    }
    printf("\n");
    printf("\n������ͷβ�������У�   ");
    Get_HT(Ls);
    printf("\n\n");
    printf("**********���������ⰴ������**********");
    getchar();
    return 0;
}


