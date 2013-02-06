
%skeleton "lalr1.cc"                         /*  -*- C++ -*- */
%require "2.3"
%defines
%define "parser_class_name" "Parser"


%code requires{
# include <string>
#include <symbol.h>
#include <composite.h>
class ParserDriver;



}

// The parsing context.

%parse-param { ParserDriver& driver}
%parse-param { CompositeFactory& factory }
%parse-param { std::string file }
%parse-param { Composite** root }


%lex-param   { ParserDriver& driver }
%lex-param	 { CompositeFactory& factory }




%locations
%initial-action
{
  // Initialize the initial location.
  @$.begin.filename = @$.end.filename = &file;
};

%debug
%error-verbose

// Symbols.
%union
{
  int          intval;
  std::string *stringval;
  Symbol*		sval;
};

%{
# include <driver.h>
%}

%token        END      0 "end of file"
%token <sval> RULENAME "rulename"
%token <sval> CHARVAL "charval"
%token <sval>  BINVAL "binval"
%token <sval>  DECVAL "decval"
%token <sval>  HEXVAL "hexval"
%token <sval> COMMAND "command"
%token <sval> REPEAT "repeat"
%token  COMMENT "comment"
%token       WSP	
%token        CRLF	
%token      EQUAL		"="
%token        INCREQUAL	"=/"
%token 		  LEFTBRACKETSQ "["
%token 		  RIGHTBRACKETSQ "]"
%token		  LEFTBRACKET "("
%token		  RIGHTBRACKET ")"
%token		  SLASH "/"




%type <sval>	  numval
%type <sval>	  element
%type <sval>	  elements
%type <sval>	  repetition
%type <sval>	  concatenation
%type <sval>	  group
%type <sval>	  option
%type <sval>	  alternation
%type <sval>	  definedas
%type <sval>	  rule
%type <sval>	  rules
%type <sval>	  cnl

%printer    { debug_stream () << $$->getName(); } "rulename"
%destructor { delete $$; } "rulename"
%destructor { delete $$; }  "charval"
%destructor { delete $$; }  "binval"
%destructor { delete $$; }  "decval"
%destructor { delete $$; }  "hexval"
%destructor { delete $$; }  "command"
%destructor { delete $$; }  "repeat"








%%




%start rulelist;

rulelist: rules {
			
				if (!($1->getNodes().empty())) 
				{	
				(*root)=$1->getNodes().front();
				}
				$1->clear();delete $1;
				}
			;

rules: rules rule{$$=new Symbol(std::string("rules"),$1,$2);$1->clear();delete $1;$2->clear();delete $2;}
	 | rules cnl {$$=new Symbol(std::string("rules"),$1);$1->clear();delete $1;}
	 | rule {$$=new Symbol(std::string("rules"),$1);$1->clear();delete $1;}
	 | cnl {$$=new Symbol(std::string("rules"));}
	 ;
	 		
rule: "rulename" definedas elements cnl {if ($1->addTreeLink($3)){
										driver.addValue($1); $$=new Symbol(std::string("rule"),$1,$3); 
										$1->clear();delete $1;
										$3->clear();delete $3;
										}
										else
										{
										$1->clear();delete $1;
										$3->clear();delete $3;
										error(@$, std::string("invalid rule: a symbol can't be defined by itself"));
										 
										YYABORT;
										}
										}
	|"rulename" definedas "command" elements cnl {
										Symbol* totElements=new Symbol(std::string("elements"),$3,$4);
										if ($1->addTreeLink(totElements)){
										driver.addValue($1); $$=new Symbol(std::string("rule"),$1,totElements); 
										$1->clear();delete $1;
										$3->clear();delete $3;
										$4->clear();delete $4;
										totElements->clear();delete totElements;
										}
										else
										{error(@$, std::string("invalid rule: a symbol can't be defined by itself"));
										 
										YYABORT;
										}
										}
	|"rulename" definedasincr elements cnl{if (driver.addIncrAlternative($1,$3)){
										
										$$=new Symbol(std::string("rule"),$1,$3); 
										$1->clear();delete $1;
										$3->clear();delete $3;
										}
										else
										{
											$1->clear();delete $1;
										$3->clear();delete $3;
										error(@$, std::string("invalid rule: symbol not previously defined"));
										YYABORT;
										}}
	;

definedas: "=" {}
	  ;
	 
definedasincr: "=/" {}	   
		;
	  
elements: alternation  {$$=new Symbol(std::string("elements"),$1);$1->clear();delete $1;}
	    ;
				
alternation: alternation  "/"   concatenation {
												$$=driver.addNonTerminalNode(factory.getOrNode(std::string("/")),std::string("alternation"),$1,$3);
												$1->clear();delete $1;
												$3->clear();delete $3;
												}
			| concatenation {$$=new Symbol(std::string("alternation"),$1);$1->clear();delete $1;}
			;
				
concatenation: concatenation  repetition   {$$=new Symbol(std::string("concatenation"),$1,$2);
											$1->clear();delete $1;
											$2->clear();delete $2;}
			| repetition  {$$=new Symbol(std::string("concatenation"),$1);$1->clear();delete $1;}
			;
 
repetition: "repeat" element {$1->addTreeLink($2);
								$$=$1;
								$2->clear();delete $2;
								}
			| element {$$=new Symbol(std::string("repetition"),$1);$1->clear();delete $1;}
			;
			
element: "rulename" {$$=new Symbol(std::string("element"),$1);$1->clear();delete $1;}
		| group		{$$=new Symbol(std::string("element"),$1);$1->clear();delete $1;}
		| option	{$$=new Symbol(std::string("element"),$1);$1->clear();delete $1;}
		| "charval" {$$=new Symbol(std::string("element"),$1);$1->clear();delete $1;}
		| numval	{$$=new Symbol(std::string("element"),$1);$1->clear();delete $1;}
		;
		
group: "("  alternation  ")" {
								$$=driver.addNonTerminalNode(factory.getNode(std::string("()")),std::string("group"),$2);$2->clear();delete $2;
								}
		| "("  "command"  ")" {
								$$=driver.addNonTerminalNode(factory.getNode(std::string("()")),std::string("group"),$2);$2->clear();delete $2;
								}
		| "(" "command" alternation ")"{
								Symbol* totAlternation=new Symbol(std::string("alternation"),$2,$3);
								$$=driver.addNonTerminalNode(factory.getNode(std::string("()")),std::string("group"),totAlternation);
								$2->clear();delete $2;
								totAlternation->clear();delete totAlternation;
								}
		;

option: "["  alternation  "]" {
								$$=driver.addNonTerminalNode(factory.getOptionNode(std::string("[]")),std::string("option"),$2);$2->clear();delete $2;
								}
		|"["  "command"  "]" {
								$$=driver.addNonTerminalNode(factory.getOptionNode(std::string("[]")),std::string("option"),$2);$2->clear();delete $2;
								}
		| "[" "command" alternation "]"{
								Symbol* totAlternation=new Symbol(std::string("alternation"),$2,$3);
								$$=driver.addNonTerminalNode(factory.getOptionNode(std::string("[]")),std::string("option"),totAlternation);
								$2->clear();delete $2;
								totAlternation->clear();delete totAlternation;
								}
		;
				
numval: "binval" {$$=new Symbol(std::string("numval"),$1);$1->clear();delete $1;
					}
		| "decval" {$$=new Symbol(std::string("numval"),$1);$1->clear();delete $1;
					}
		| "hexval" {$$=new Symbol(std::string("numval"),$1);$1->clear();delete $1;
						}
		;
		
cnl: CRLF {}
     | "comment"{}
	;

%%



void
yy::Parser::error (const yy::Parser::location_type& l,
                          const std::string& m)
{
  driver.error (l, m);
}



