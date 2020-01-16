%option noyywrap
%option yylineno
     
%{
#include <iostream>
#include <string>
#include "parser_y.h"
%}

IDENTIFIER              [_a-z]
DIGIT                   [0-9]
PUNCT                   [\(\)\:\;\,]

%x OPS

%%
"[".*"]"
"DECLARE"               { return DECLARE; }
"BEGIN"                 { return BEGINA; }
"END"                   { return END; }

"READ"                  { return READ; }
"WRITE"                 { return WRITE; }

"ASSIGN"                { return ASSIGN; }

"IF"                    { return IF; }
"THEN"                  { return THEN; }
"ELSE"                  { return ELSE; }
"ENDIF"                 { return ENDIF; }

"WHILE"                 { return WHILE; }
"DO"                    { return DO; }
"ENDWHILE"              { return ENDWHILE; }
"ENDDO"                 { return ENDDO; }

"FOR"                   { return FOR; }
"FROM"                  { return FROM; }
"TO"                    { return TO; }
"DOWNTO"                { return DOWNTO; }
"ENDFOR"                { return ENDFOR; }


"EQ"                     { return EQ; }
"NEQ"                    { return NEQ; }
"LE"                     { return LE; }
"GE"                     { return GE; }
"LEQ"                    { return LEQ; }
"GEQ"                    { return GEQ; }

"PLUS"                   { return PLUS; }
"MINUS"                  { return MINUS; }
"TIMES"                  { return TIMES; }
"DIV"                    { return DIV; }
"MOD"                    { return MOD; }
{IDENTIFIER}+           { 
                            yylval.str = new std::string(yytext);
                            return pidentifier;
                        }
"-"?{DIGIT}+                { 
                            yylval.number = std::stoll(yytext);
                            return num;
                        }
{PUNCT}             { return yytext[0]; }


[ \t\r\n]+                { /*ignore whitespaces*/ }
.                       { return ERROR; }
%%