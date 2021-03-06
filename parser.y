%{
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include "memc.hpp"
#include "structs.hpp"
#include "code.hpp"
#include <exception>

int yylex();
int yyparse();
void yyerror(std::string er);
extern FILE *yyin;
extern int yylineno;

std::shared_ptr<MemoryController> vars = std::make_shared<MemoryController>();
std::shared_ptr<Coder> code = std::make_shared<Coder>(vars);

bool declareError = false;
string inputName;
string outputName;

%}
%define parse.error verbose
%union {
  std::string * str;
  long long number;
}
//Tokens
//%start program
%token <str> DECLARE BEGINA END
%token <str> IF WHILE DO
%token <str> FOR 
%token <str> THEN ELSE ENDIF FROM TO DOWNTO ENDFOR ENDWHILE ENDDO
%token <str> READ WRITE       
%token <str> LE GE LEQ GEQ EQ NEQ
%token <str> ASSIGN
%token <str> ERROR
%token <str> pidentifier
%token <number> num


//Types
%start program
//%type <number> value
//%type <number> identifier 
%type <number> condition //VM -> k
//%type <string> declarations
//%type <string> commands
//%type <string> command
%type <number> expression //VM -> k

//Operators precedence
%left PLUS MINUS
%left TIMES DIV MOD
%%      

program:        DECLARE declarations BEGINA { if(declareError){YYABORT;
                                         }}
                commands END {
                   
                   //vars->printAll();
                   std::cerr << "\033[1;32m[SUCCESS] " << inputName <<", " << outputName <<"\033[0m \t" << std::endl;
                   code->end(); code->printVM();
                   }                                                   
                |
                BEGINA commands END { std::cerr << "\033[1;32m[SUCCESS] " << inputName <<", " << outputName <<"\033[0m \t" << std::endl;
                code->end(); code->printVM(); }
;

declarations:   declarations ',' pidentifier                
                {
                  auto pid = $3; try{
                    code->declareVariable(*pid); 
                  }catch(std::runtime_error & err){
                    yyerror(err.what());
                    declareError = true;
                  }

                }

                | declarations ',' pidentifier '(' num ':' num ')'    
                {
                  auto pid = $3; auto beg = $5; auto end = $7; 
                  try{
                    code->declareArray(*pid,beg,end);
                  }catch(std::runtime_error & err){
                    yyerror(err.what());
                    declareError = true;
                  }
                }                                          
                | pidentifier
                {
                  auto pid = $1; try{
                    code->declareVariable(*pid); 
                  }catch(std::runtime_error & err){
                    yyerror(err.what());
                    declareError = true;
                  }

                }
                | pidentifier '('num ':' num ')'  
                {
                  auto pid = $1; auto beg = $3; auto end = $5;
                  try{
                    code->declareArray(*pid,beg,end);
                  }catch(std::runtime_error & err){
                    yyerror(err.what());
                    declareError = true;
                  }
                }
;

commands:       commands command
                | command       
;


command:        identifier ASSIGN expression ';'         { auto injectPoint = $3; 
                                                            try{
                                                              code->assignValueToVar(injectPoint);
                                                            }catch(std::runtime_error & err){
                                                              yyerror(err.what());
                                                              YYABORT;
                                                            } 
                                                          }                                                       

                
                | IF condition 
                   THEN 
                   commands                                                             
                  ELSE { code->startelse(); } 
                  commands 
                  ENDIF { code->endif(); }                                                                

                | IF condition 
                  THEN 
                  commands 
                  ENDIF { code->endif(); }                                                 

                | WHILE condition DO {auto conditionIndex = $2;code->incDepth(); code->stackJump(conditionIndex);}
                  commands ENDWHILE   {code->endWhile();
                  try{code->decDepth();
                    }catch(std::runtime_error & err){
                      yyerror(err.what());YYABORT;
                  }}                                  

                | DO                              {code->stackJump();code->incDepth();  }                                                                 
                  commands WHILE condition ENDDO  {code->endDoWhile();
                  try{code->decDepth();
                  }catch(std::runtime_error & err){
                    yyerror(err.what());YYABORT;
                  }}                                      
                                                                                        
                | FOR pidentifier FROM value TO value DO      
                  {auto pid = $2; 
                    try{code->handleToFor(*pid);code->incDepth();}
                    catch(std::runtime_error & err){
                    yyerror(err.what()); YYABORT;
                    }
                  }                           
                  commands 
                  ENDFOR                                      {try{code->endFor(false);code->decDepth();}
                    catch(std::runtime_error & err){yyerror(err.what()); YYABORT;}}    

                | FOR pidentifier FROM value DOWNTO value DO  {auto pid = $2; try{code->handleDownToFor(*pid);code->incDepth();}
                    catch(std::runtime_error & err){yyerror(err.what()); YYABORT;}}                              
                  commands 
                  ENDFOR                                      {try{code->endFor(true);code->decDepth();}
                    catch(std::runtime_error & err){yyerror(err.what()); YYABORT;}}                        

                | READ identifier ';' { code->read(); }                                                     

                | WRITE value ';'     { try{code->write();}catch(std::runtime_error & err){yyerror(err.what()); YYABORT;
                } }                                                     
;

expression:     value                 { $$ = code->getCurrentPosition(); code->getValue(); }                                              
                | value PLUS value    { $$ = code->getCurrentPosition(); code->addVars();  }                                                
                | value MINUS value   { $$ = code->getCurrentPosition(); code->subVars();  }                                                  
                | value TIMES value   { $$ = code->getCurrentPosition(); code->mulVars();  }                                                     
                | value DIV value     { $$ = code->getCurrentPosition(); code->divVars();  }                                              
                | value MOD value     { $$ = code->getCurrentPosition(); code->modVars();  }                                             
;

condition:      value EQ value      { $$ = code->getCurrentPosition(); code->processEQ();   }              
                | value NEQ value   { $$ = code->getCurrentPosition(); code->processNEQ();  }              
                | value LE value    { $$ = code->getCurrentPosition(); code->processLE();   }              
                | value GE value    { $$ = code->getCurrentPosition(); code->processGE();   }                
                | value LEQ value   { $$ = code->getCurrentPosition(); code->processLEQ();  }              
                | value GEQ value   { $$ = code->getCurrentPosition(); code->processGEQ();  }              
;

value:          num                           { auto val = $1; code->stackValue(val); }                                                 
                | identifier                  { try{code->verifyStack();}catch(std::runtime_error & err){yyerror(err.what()); YYABORT;
                                              } }                                                            
;

identifier:     pidentifier                           { auto pid = $1; try{code->stackVariable(*pid);}
                                                            catch(std::runtime_error & err){yyerror(err.what()); YYABORT;
                                                      } }    
                | pidentifier '(' pidentifier ')'     { auto pid = $3; auto name = $1; try{code->stackArrayWithVariable(*pid,*name);}
                                                            catch(std::runtime_error & err){yyerror(err.what()); YYABORT;
                                                      } }                                        
                | pidentifier '(' num ')'             { auto pid = $1; auto val = $3; try{code->stackArrayWithConst(val,*pid);}
                                                            catch(std::runtime_error & err){yyerror(err.what()); YYABORT;
                                                      } }                                       
;


%%

void yyerror(std::string err){
  std::cerr << "\033[1;31m[ERROR] " << inputName <<"\033[0m \033[1;34m[LINE:" << yylineno << "]\033[0m\t" << err << std::endl;
}

int main(int argc, char** argv) {
  if(argc < 3){
    std::cerr << "input.imp \t output.mr\n";
    return 0;
  }
  inputName = argv[1];
  outputName = argv[2];

  yyin = fopen(argv[1], "r"); 
  code->setOutput(argv[2]);

  yyparse();  
  return 0;

}
