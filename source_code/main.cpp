#include <iostream>
#include <fstream>
#include <string>
#include "assert.h"
#define Maxnum 9999
using namespace std;

class kw
{
public:
    string re_exp;//正规表达式
    string mark;//记号
    string nature;//属性
    kw()//无参数构造函数
    {
        re_exp = "";
        mark = "";
        nature = "";
    }
    kw(string a, string b, string c)//有参数构造函数
    {
        re_exp = a;
        mark = b;
        nature = c;
    }
};

//关键字表
kw kw_ex[32] = { kw("auto", "auto", "Z"), 
                 kw("short", "stort", "Z"), 
                 kw("int", "int", "Z"), 
                 kw("long", "long", "Z"), 
                 kw("float", "float", "Z"), 
                 kw("double", "double", "Z"), 
                 kw("char", "char", "Z"), 
                 kw("struct", "struct", "Z"), 
                 kw("union", "union", "Z"), 
                 kw("enum", "enum", "Z"), 
                 kw("typedef", "typedef", "Z"), 
                 kw("const", "const", "Z"), 
                 kw("unsigned", "unsigned", "Z"), 
                 kw("signed", "signed", "Z"), 
                 kw("extern", "extern", "Z"), 
                 kw("register", "register", "Z"), 
                 kw("static", "static", "Z"), 
                 kw("volatile", "volatile", "Z"), 
                 kw("void", "void", "Z"), 
                 kw("if", "if", "Z"), 
                 kw("else", "else", "Z"), 
                 kw("switch", "switch", "Z"), 
                 kw("case", "case", "Z"), 
                 kw("for", "for", "Z"), 
                 kw("do", "do", "Z"), 
                 kw("while", "while", "Z"), 
                 kw("goto", "goto", "Z"), 
                 kw("continue", "continue", "Z"), 
                 kw("break", "break", "Z"), 
                 kw("default", "default", "Z"), 
                 kw("sizeof", "sizeof", "Z"), 
                 kw("return", "return", "Z") 
};

//用户自定义标识符
kw tag[100];
//用户自定义标识符”指针“
int i_tag = 0;

string ch[11] = { "{", "[", "(", ")", "]", "}", ";", ",", "\'", "\"","\\"};

string code[Maxnum];
char C;
string token;
int err = 0;//错误数
int all_line = 0;//语句行数
int word_num = 0;//单词个数
int all_ch = 0;//字符总数

class fw
{
public:
    int row;
    int line;
    fw()
    {
        row = 0;
        line = 0;
    }
};//指针forward类

void readTxt(string file)
{
    ifstream infile;
    infile.open(file);   //将文件流对象与文件连接起来
    assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行
    string s;
    int i = 0;
    while (getline(infile, s))
    {
        code[i] = s;
        all_ch += s.length();
        code[i] += '\n';
        i++;
        std::cout << s << endl;
    }
    all_line = i;
    infile.close();             //关闭文件输入流
    code[i] = "#";
    for (auto a : ch) {
        cout << a << " ";
    }
    cout << endl;
}

fw f;
fw rf;//存储之前字符的位置

void get_char()
{
    rf.row = f.row;
    rf.line = f.line;
    if (code[f.row][f.line] != '\n')
    {
        C = code[f.row][f.line];
        f.line++;
    }
    else
    {
        f.row++;
        f.line = 0;
        C = code[f.row][f.line];
        f.line++;
    }
}//根据向前指针forward的指示从输入缓冲区中读出一个字符并放入C中

void get_nbc()
{
    while ((C == ' ') || (C == '\t'))
    {
        get_char();
    }
}//检查空格和\t

void cat()
{
    token += C;
}//将C中字符拼接在token中的字符串后面

bool letter()
{
    if (((C >= 'a') && (C <= 'z')) || ((C >= 'A') && (C <= 'Z')) || (C == '_'))//字符        待处理
        return true;
    else
        return false;
}//判断字母或下划线

bool digit()
{
    if (((C >= '0') && (C <= '9')))
        return true;
    else
        return false;
}//判断数字

void restract()
{
    f.row = rf.row;
    f.line = rf.line;
}//指针forward后退一个字符

string reserve()
{
    for (int i = 0; i < 32; i++)
    {
        if (token == kw_ex[i].re_exp)
            return kw_ex[i].mark;
    }
    return "-1";
}//根据token中的单词查关键字表，若token中的单词是关键字，则返回值该关键字的记号，否则返回-1

int table_insert()
{
    for (int i = 0; tag[i].re_exp != ""; i++)
    {
        if (token == tag[i].re_exp)
        {
            return i;
        }
    }
    tag[i_tag].re_exp = token;
    tag[i_tag].mark = "id";
    tag[i_tag].nature = i_tag;
    i_tag++;
    return (i_tag - 1);
}//将识别出来的用户自定义标识符插入符号表，返回该单词在符号表中的位置指针

void ret(string A, string B)
{
    int flag = -1;
    for (int i = 0; i < 11; i++)
        if (ch[i] == A)
            flag = i;
    if (flag >= 0)
        std::cout << "<" << A << " , " << "symbol" << ">" << endl;
    else if (A == "relop")
        std::cout << "<" << "relop" << " , " << B << ">" << endl;
    else if (B == "Z")
        std::cout << "<" << A << " , " << "keyword" << ">" << endl;
    else
        std::cout << "<" << A << " , " << B << ">" << endl;

}//输出

void ret(string A, int B)
{
    std::cout << "<" << A << " , " << B << ">" << endl;

}

void ret(string A, float B)
{
    std::cout << "<" << A << " , " << B << ">" << endl;

}

void error(int row, int line)
{
    std::cout << "第" << row + 1 << "行" << "第" << line + 1 << "列出现错误" << endl;
    err++;
}//对发现的错误进行相应的处理

int main(int argc, const char* argv[]) {
    string file = "E:\\lexical_analysis\\lexical_analysis\\Release\\lexical_analysis.txt";
    readTxt(file);
    int state = 0;//当前状态指示
    string iskey = "";
    int line = 0;
    int row = 0;//指针
    do
    {
        switch (state)
        {
        case 0://初始状态
            token = "";
            row = f.row;
            line = f.line;
            get_char();
            get_nbc();
            switch (C)
            {
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'n':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case '_':
                state = 1;//读入字符为字母切换到状态1
                break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                state = 2;//读入字符为数字，切换到状态2
                break;
            case '<':
                state = 8;
                break;
            case '>':
                state = 9;
                break;
            case '!':
                state = 10;
                break;
            case '=':
                state = 11;
                break;
            case '/':
                state = 12;
                break;
            case '+':
                state = 0;
                ret("+", "--");
                break;
            case '-':
                state = 0;
                ret("-", "--");
                break;
            case '*':
                state = 0;
                ret("*", "--");
                break;
            case '&':
                state = 0;
                ret("&", "--");
                break;
            case '%':
                state = 0;
                ret("%", "--");
                break;
            case ',':
                state = 0;
                ret(",", "Z");
                break;
            case '(':
                state = 0;
                ret("(", "Z");
                break;
            case ')':
                state = 0;
                ret(")", "Z");
                break;
            case '[':
                state = 0;
                ret("[", "Z");
                break;
            case ']':
                state = 0;
                ret("]", "Z");
                break;
            case '{':
                state = 0;
                ret("{", "Z");
                break;
            case '}':
                state = 0;
                ret("}", "Z");
                break;
            case ';':
                state = 0;
                ret(";", "Z");
                break;
            case '\'':
                state = 0;
                ret("\'", "Z");
                break;
            case '\"':
                state = 0;
                ret("\"", "Z");
                break;
            case '\\':
                state = 0;
                ret("\\", "Z");
                break;
            default:
                state = 14;
                break;

            }
            //初始状态
            break;
        case 1:
            //标识符状态
            cat();
            get_char();
            if (letter() || digit())
                state = 1;
            else
            {
                restract();
                state = 0;
                iskey = reserve();
                word_num++;
                if (iskey != "-1")
                    ret(iskey, "Z");
                else {
                    int identry = table_insert(); //返回标识符在符号表的入口指针
                    ret("id", identry);
                }
            }
            break;
        case 2:
            //常数状态
            cat();
            get_char();
            switch (C) {
            case '0': case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                state = 2;
                break;
            case '.':
                state = 3;
                break;
            case 'E':
            case 'e':
                state = 5;
                break;
            default:
                if (((C >= 'a') && (C <= 'z')) || ((C >= 'A') && (C <= 'Z')) || (C == '_'))
                {
                    while ((C != ' ') && (C != ',') && (C != ';'))
                        get_char();
                    state = 14;
                    restract();
                }
                else
                {
                    word_num++;
                    restract();
                    state = 0;
                    ret("num", stoi(token));//返回整数
                }
                break;
            }
            break;
        case 3:
            //小数点状态
            cat();
            get_char();
            if (digit())
                state = 4;
            else
            {
                error(row, line);
                state = 0;
            }
            break;
        case 4:
            //小数状态
            cat();
            get_char();
            switch (C) {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                state = 4;
                break;
            case 'E':
            case 'e':
                state = 5;
                break;
            default:
                if (((C >= 'a') && (C <= 'z')) || ((C >= 'A') && (C <= 'Z')) || (C == '_'))
                {
                    while ((C != ' ') && (C != ',') && (C != ';'))
                        get_char();
                    state = 14;
                    restract();
                }
                else
                {
                    word_num++;
                    restract();
                    state = 0;
                    ret("num", stof(token));
                }
                break;
            }
            break;
        case 5:
            //指数状态
            cat();
            get_char();
            switch (C) {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                state = 7;
                break;
            case '+':
            case '-':
                state = 6;
                break;
            default:
                restract();
                error(row, line);
                state = 0;
                break;
            }
            break;
        case 6:
            cat();
            get_char();
            if (digit())
                state = 7;
            else
            {
                restract();
                error(row, line);
                state = 0;
            }
            break;
        case 7:
            cat();
            get_char();
            if (digit())
                state = 7;
            else
            {
                if (((C >= 'a') && (C <= 'z')) || ((C >= 'A') && (C <= 'Z')) || (C == '_'))
                {
                    while ((C != ' ') && (C != ',') && (C != ';'))
                        get_char();
                    state = 14;
                    restract();
                }
                else
                {
                    word_num++;
                    restract();
                    state = 0;
                    ret("num", stof(token));//返回无符号数
                }
            }
            break;
        case 8:
            //'<'状态
            cat();
            get_char();
            switch (C) {
            case '=':
                cat();
                state = 0;
                ret("relop", "LE");
                break;
            default:
                restract();
                cat();
                state = 0;
                ret("relop", "LT");
                break;
            }
            break;
        case 9:
            //'>'状态
            cat();
            get_char();
            if (C == '='){
                cat();
                state = 0;
                ret("relop", "GE");
            }
            else{
                restract();
                state = 0;
                ret("relop", "GT");
            }
            break;
        case 10:
            //'!'状态
            cat();
            get_char();
            if (C == '=') {
                cat();
                state = 0;
                ret("relop", "NE");
            }
            else {
                restract();
                state = 0;
                ret("!", "--");
            }
            break;
        case 11:
            // '='状态
            cat();
            get_char();
            if (C == '=') {
                cat();
                state = 0;
                ret("relop", "EQ");
            }
            else {
                restract();
                state = 0;
                cout << "<=" << " , " << "assign_op>" << endl;
            }
            break;
        case 12:
            //'/'状态
            cat();
            get_char();
            if (C == '*') {
                cout << "</*" << " , " << "annotation_start>" << endl;
                state = 13; //设置注释处理状态
            }
            else if (C == '/') {
                cout << "//" << " , " << "annotation_start>" << endl;
                get_char();
                while ((f.row-rf.row)!=1)
                    get_char();
                state = 0;
            }
            else{
                restract();
                state = 0;
                std::cout << "<div_op>" << endl;
            }
            break;
        case 13:
            //注释处理状态
            get_char();
            while (C != '*'&&C!='#')
                get_char();
                if (C == '#') {
                    restract();
                }
            get_char();
            if (C == '/') {
                cout << "<*/" << " , " << "annotation_end>" << endl;
                state = 0;
            }
            else if (C == '#') {
                state = 14;
            }
            else state = 13;
            break;
        case 14:
            //错误处理状态
            error(row, line);
            state = 0;
            break;
        }
    } while (C != '#');
    std::cout << "错误个数为: " << err << endl;
    std::cout << "程序语句行数为: " << all_line << endl;
    std::cout << "单词个数为: " << word_num << endl;
    std::cout << "字符总数为: " << all_ch << endl;
    return 0;
}
