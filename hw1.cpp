#include <stdio.h>
#include <iostream>
#include "tokens.hpp"
#include <string.h>

using namespace std;

void showToken(const char *name)
{
    printf("%d %s %s\n", yylineno, name, !strcmp(name, "COMMENT") ? "//" : yytext);
}

int messageLength(char *msg)
{
    int i = 0;
    while (msg[i] != '\0')
    {
        i++;
    }
    return i;
}

int convertHexToDec(char ch)
{
    if (ch >= '0' && ch <= '9')
        return ch - '0';
    if (ch >= 'A' && ch <= 'F')
        return ch - 'A' + 10;
    if (ch >= 'a' && ch <= 'f')
        return ch - 'a' + 10;

    return -1;
}

void handleString()
{
    char *content = yytext;
    int str_len = messageLength(content) - 1;
    char buffer[1024];
    int indexbuff = 0;
    int i = 0;

    if (str_len == 0)
    {
        printf("%d STRING \n", yylineno);
        return;
    }
    while (i < str_len)
    {
        if (content[i] == '\\')
        {
            char c = content[i + 1];
            switch (c)
            {
            case 'n':
                buffer[indexbuff++] = '\n';
                break;
            case '0':
                buffer[indexbuff++] = '\0';
                break;
            case 'r':
                buffer[indexbuff++] = '\r';
                break;
            case 't':
                buffer[indexbuff++] = '\t';
                break;
            case '\\':
                buffer[indexbuff++] = '\\';
                break;
            case '\"':
                if ((i + 1) == str_len)
                {
                    printf("Error unclosed string\n");
                    std::exit(0);
                }
                else
                    buffer[indexbuff++] = '\"';
                break;
            case 'x':
                if (i + 3 < str_len) // /x or /xD or /xDD
                {
                    const char hex1 = content[i + 2];
                    const char hex2 = content[i + 3];
                    if (hex1 >= '0' && hex1 <= '7' && ((hex2 >= 'A' && hex2 <= 'F') || (hex2 >= '0' && hex2 <= '9') || (hex2 >= 'a' && hex2 <='f')))
                    {
                        int num1 = convertHexToDec(hex1);
                        int num2 = convertHexToDec(hex2);
                        int numTotal = num1 * 16 + num2;
                        buffer[indexbuff++] = (char)numTotal;
                    }
                    else
                    {
                        printf("Error undefined escape sequence x%c%c\n", hex1, hex2);
                        std::exit(0);
                    }
                }
                else
                {
                    if (i + 2 < str_len)
                    {
                        printf("Error undefined escape sequence x%c\n", content[i + 2]);
                    }
                    else if (i + 1 < str_len)
                    {
                        printf("Error undefined escape sequence x\n");
                    }
                    std::exit(0);
                }
                break;
            default:
                printf("Error undefined escape sequence %c\n", c);
                std::exit(0);
            }

            if (c == 'x')
                i += 4;
            else
                i += 2;
        }
        else
        {
            char to_insert= content[i];
            if(to_insert < 0x20 || to_insert > 0x7E)
            {
                if(to_insert != 0x09 && to_insert != 0x0A && to_insert != 0x0D)
                {
                    printf("Error %c\n", to_insert);
                    std::exit(0);
                }
            }
            buffer[indexbuff++] = content[i];
            i++;
        }
    }

    buffer[indexbuff] = '\0';
    printf("%d STRING %s\n", yylineno, buffer);
}

void handleError(int t)
{
    if (t == ILEAGAL_CHAR)
        printf("Error %s\n", yytext);
    else if (t == UNCLOSED_STRING)
        printf("Error unclosed string\n");
    else if (t == UNCLOSED_COMMENT)
        printf("Error unclosed comment\n");
    std::exit(0);
}

int main()
{
    int token;
    while ((token = yylex()))
    {
        switch (token)
        {
        case VOID:
            showToken("VOID");
            break;
        case INT:
            showToken("INT");
            break;
        case BYTE:
            showToken("BYTE");
            break;
        case B:
            showToken("B");
            break;
        case BOOL:
            showToken("BOOL");
            break;
        case AND:
            showToken("AND");
            break;
        case OR:
            showToken("OR");
            break;
        case NOT:
            showToken("NOT");
            break;
        case TRUE:
            showToken("TRUE");
            break;
        case FALSE:
            showToken("FALSE");
            break;
        case RETURN:
            showToken("RETURN");
            break;
        case IF:
            showToken("IF");
            break;
        case ELSE:
            showToken("ELSE");
            break;
        case WHILE:
            showToken("WHILE");
            break;
        case BREAK:
            showToken("BREAK");
            break;
        case CONTINUE:
            showToken("CONTINUE");
            break;
        case SC:
            showToken("SC");
            break;
        case COMMA:
            showToken("COMMA");
            break;
        case LPAREN:
            showToken("LPAREN");
            break;
        case RPAREN:
            showToken("RPAREN");
            break;
        case LBRACE:
            showToken("LBRACE");
            break;
        case RBRACE:
            showToken("RBRACE");
            break;
        case RELOP:
            showToken("RELOP");
            break;
        case BINOP:
            showToken("BINOP");
            break;
        case ASSIGN:
            showToken("ASSIGN");
            break;
        case ID:
            showToken("ID");
            break;
        case NUM:
            showToken("NUM");
            break;
        case COMMENT:
            showToken("COMMENT");
            break;
        case STRING:
            handleString();
            break;
        default:
            handleError(token);
            break;
        }
    }
    return 0;
}