%{
#include "tokens.hpp"
%}

%option yylineno
%option noyywrap

digit [0-9]
letter [a-zA-Z]
whitespace ([\t\n\r ])
relop [<>]=?|[!=]=
binop [\*\+\-\/]
word ({letter}+[0-9a-zA-Z]*) 
num ([1-9]{digit}*)|[0]
comment \/\/[^\n\r]*

%x BEGIN_STRING

%%

void return VOID;
int return INT;
byte return BYTE;
bool return BOOL;
b return B;
and return AND;
or return OR;
not return NOT;
true return TRUE;
false return FALSE;
return return RETURN;
if return IF;
else return ELSE;
while return WHILE;
break return BREAK;
continue return CONTINUE;
\; return SC;
\, return COMMA;
\( return LPAREN;
\) return RPAREN;
\{ return LBRACE;
\} return RBRACE;
{relop} return RELOP;
{binop} return BINOP;
(\=) return ASSIGN;
{word} return ID;
{num} return NUM;
{comment} return COMMENT;
{whitespace}    ;

\" BEGIN(BEGIN_STRING);
<BEGIN_STRING>([^\"\n\r(\n\r)]*[\n\r][^\"\n\r(\n\r)]*)+\" {BEGIN(INITIAL); return UNCLOSED_STRING;}
<BEGIN_STRING>([^\"\\]|(\\.))*\" {BEGIN (INITIAL); return STRING;}
<BEGIN_STRING>([^\"\\]|(\\.))* {BEGIN(INITIAL); return UNCLOSED_STRING;}
<BEGIN_STRING>. {BEGIN(INITIAL); return ERROR;}

. return ILEAGAL_CHAR;
%%