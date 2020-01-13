%{
#include <iostream>
#include <string>
#include <memory>
#include <vector>


int yylex();
int yyparse();
void yyerror(char const *s);
extern FILE *yyin;
extern int yylineno;

int addr;

%}
%union sem_rec {
    std::string *pidentifier;
    long long num;
    struct for_label *lfor;
    struct cond_label *cond;
    struct label *lbl;
    struct variable *var;
    struct variable *id;
}

//Tokens
%start program
%token DECLARE BEGIN END
%token <lbl> IF WHILE DO
%token <lfor> FOR 
%token THEN ELSE ENDIF FROM TO DOWNTO ENDFOR ENDWHILE ENDDO
%token READ WRITE       
%token LE GE LEQ GEQ EQ NEQ
%token ASSIGN
%token ERROR
%token <pidentifier> pidentifier
%token <num> num

//Types
%type <var> value
%type <id> identifier
%type <cond> condition;

//Operators precedence
%left PLUS MINUS
%left TIMES DIV MOD
%%      

program:        DECLARE declarations BEGIN commands END                                                       
                |
                BEGIN  commands END
;

declarations:   declarations',' pidentifier
                | declarations',' pidentifier'('num':'num')'                                                   
                | pidentifier
                | pidentifier '('num ':' num ')'
;

commands:       commands command
                | command       
;


command:        identifier ASSIGN expression ';'                                                               

                /* IF_ELSE */
                | IF condition THEN commands                                                             
                  ELSE commands ENDIF                                                   //{ cg->if_else_block_second($1->go_to); }               

                /* IF */
                | IF condition THEN commands ENDIF                                      //{ cg->if_block($2->go_to); }             

                /* WHILE */
                | WHILE condition DO                                                                                                                                                             
                  commands ENDWHILE                                                     //{ cg->while_block($2); }    

                /* DO_WHILE */
                | DO                                                                    
                  commands WHILE condition ENDDO                                        //{ cg->do_while_block_second($1, $5->go_to); }
                                                                                        


                /*== FOR_FROM_TO ==*/
                | FOR pidentifier FROM value TO value DO                                
                        commands 
                  ENDFOR              

                /* FOR_FROM_DOWNTO */
                | FOR pidentifier FROM value DOWNTO value DO                          
                        commands 
                  ENDFOR                                                     


                | READ identifier';'                                                     

                | WRITE value';'                                                        //{ cg->write($2); }
;

expression:     value                                                                   //{ cg->constant($1); }
                | value PLUS value                                                       //{ cg->add($1, $3); }
                | value MINUS value                                                       //{ cg->sub($1, $3); }
                | value TIMES value                                                       //{ cg->mul($1, $3); }
                | value DIV value                                                       //{ cg->div($1, $3); }
                | value MOD value                                                       //{ cg->mod($1, $3); }
;

condition:      value EQ value                                                         //{ $$ = cg->eq($1, $3); }
                | value NEQ value                                                      //{ $$ = cg->neq($1, $3); }
                | value LE value                                                       //{ $$ = cg->lt($1, $3); }
                | value GE value                                                       //{ $$ = cg->gt($1, $3); }
                | value LEQ value                                                      //{ $ = cg->leq($1, $3); }
                | value GEQ value                                                      //{ $$ = cg->geq($1, $3); }
;

value:          num                                                                     //{ $$ = d->get_value_num($1); }
                | identifier                                                            //{ $$ = d->get_value($1, current_id); }
;

identifier:     pidentifier                                                             
                | pidentifier'('pidentifier')'                                          
                | pidentifier'('num')'                                                  
;


%%

int main(int argc, char** argv) {

        yyparse();
        
        return 0;
}