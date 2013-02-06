%{                                            /* -*- C++ -*- */
# include <cstdlib>
# include <errno.h>
# include <limits.h>
# include <string>
# include <driver.h>
# include <parser.hh>
#include <composite-factory.h>
#include <stack>

/* Work around an incompatibility in flex (at least versions
   2.5.31 through 2.5.33): it generates code that does
   not conform to C89.  See Debian bug 333231
   <http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=333231>.  */
# undef yywrap
# define yywrap() 1

/* By default yylex returns int, we use token_type.
   Unfortunately yyterminate by default returns 0, which is
   not of token_type.  */
#define yyterminate() return token::END

stack<YY_BUFFER_STATE> buffers;
YY_BUFFER_STATE main_buffer;
YY_BUFFER_STATE expansion_buffer;

%}

%option noyywrap nounput batch debug


int   [0-9]+*/
blank [ \t]

bit		 (0|1)
digit    [0-9]
alpha    [a-zA-Z]
hexdig	 [0-9,A,B,C,D,E,F,a,b,c,d,e,f]
vchar	 [\x21-\x7e]
ast		 \X*
dquote	 "\"" 
sp		 \x20
htab	 \x09
cr		 \x0d
lf		 \x0a
wsp		 [\x20,\x09," "]
crlf   [\x0d,\x0a,\x0d\x0a]


%{
# define YY_USER_ACTION  yylloc->columns (yyleng);/*The macro YY_USER_ACTION can be defined to provide an action */
												  /*which is always executed prior to the matched rule's action. */
%}
%%
%{
  yylloc->step ();
%}

%{
  typedef yy::Parser::token token;
%}

"ALPHA" {
	 buffers.push(YY_CURRENT_BUFFER);     
     yy_switch_to_buffer(yy_scan_string("(%x41-5A / %x61-7A)"));
     }


"BIT" {
	 buffers.push(YY_CURRENT_BUFFER);
     yy_switch_to_buffer(yy_scan_string("\"0\" / \"1\""));
    
     }

"CHAR" {
	 buffers.push(YY_CURRENT_BUFFER);
     yy_switch_to_buffer(yy_scan_string("%x01-7F"));
    
     }

"CR" {
	 buffers.push(YY_CURRENT_BUFFER);
     yy_switch_to_buffer(yy_scan_string("%x0D"));
    
     }

"CRLF" {
	 buffers.push(YY_CURRENT_BUFFER);
     yy_switch_to_buffer(yy_scan_string("(CR LF)"));
    
     }

"CTL" {
	 buffers.push(YY_CURRENT_BUFFER);
     yy_switch_to_buffer(yy_scan_string("(%x00-1F / %x7F)"));
    
     }

"DIGIT" {
	 buffers.push(YY_CURRENT_BUFFER);
     yy_switch_to_buffer(yy_scan_string("%x30-39"));
    
     }

"DQUOTE" {
	 buffers.push(YY_CURRENT_BUFFER);
     yy_switch_to_buffer(yy_scan_string("%x22"));
    
     }

"HEXDIG" {
	 buffers.push(YY_CURRENT_BUFFER);
     yy_switch_to_buffer(yy_scan_string("(DIGIT / \"A\" \"B\" \"C\" \"D\" \"E\" \"F\")"));
    
     }     

"HTAB" {
	 buffers.push(YY_CURRENT_BUFFER);
     yy_switch_to_buffer(yy_scan_string("%x09"));
    
     }

"LF" {
	 buffers.push(YY_CURRENT_BUFFER);
     yy_switch_to_buffer(yy_scan_string("%x0A"));
    
     }

"LWSP" {
	 buffers.push(YY_CURRENT_BUFFER);
     yy_switch_to_buffer(yy_scan_string("*(WSP / CRLF WSP)"));
    
     }	 
		
"OCTET" {
	 buffers.push(YY_CURRENT_BUFFER);
     yy_switch_to_buffer(yy_scan_string("%x00-FF"));
    
     }

"SP" {
	 buffers.push(YY_CURRENT_BUFFER);
     yy_switch_to_buffer(yy_scan_string("%x20"));
    
     }

"VCHAR" {
	 buffers.push(YY_CURRENT_BUFFER);
     yy_switch_to_buffer(yy_scan_string("(%x21-7E)"));
    
     }

"WSP" {
	 buffers.push(YY_CURRENT_BUFFER);
     yy_switch_to_buffer(yy_scan_string("(SP / HTAB)"));
    
     }

	 			          
<<EOF>>	{
     if ( !buffers.empty() )
     {
     // We were doing an expansion, return to where
     // we were.
	 
     yy_switch_to_buffer(buffers.top());
	 // yy_delete_buffer(expansion_buffer);
	buffers.pop();
     }
     else
     yyterminate();
     }
	 
{blank}+   yylloc->step ();
{crlf}		{yylloc->lines (yyleng);return token::CRLF;}





"="	 { return token::EQUAL;}
"=/" { return token::INCREQUAL;}
"("	 { return token::LEFTBRACKET;}
")"	 { return token::RIGHTBRACKET;}
"["	 { return token::LEFTBRACKETSQ;}
"]"	 {return token::RIGHTBRACKETSQ;}

"/" {return token::SLASH;}

";"({wsp}|{vchar})*{crlf} {yylloc->lines ();return token::COMMENT;}//anche il commento deve incrementare il conto delle righe

{dquote}([\x20-\x21]|[\x23-\x7e])*{dquote} {std::string text=std::string(yytext);
											text=text.substr(1,text.size()-2);											
											yylval->sval =new Symbol(string("charval"),factory.getStringNode(string("\\\"")+text+string("\\\"")));
											return token::CHARVAL;}

"<"([\x20-\x3d]|[\x3f-\x7e])*">" {	yylval->sval =new Symbol(string("command"),factory.getCommandNode(string(yytext)));
									return token::COMMAND;}

("%b"|"%B"){bit}{1,}(("."{bit}{1,}){1,}|(-{bit}{1,})){0,1}	{yylval->sval =new Symbol(string("binval"),factory.getBinNode(string(yytext)));
															return token::BINVAL;}

("%d"|"%D"){digit}{1,}(("."{digit}{1,}){1,}|(-{digit}{1,})){0,1}	{yylval->sval =new Symbol(string("decval"),factory.getDecNode(string(yytext)));
																return token::DECVAL;}

("%x"|"%X"){hexdig}{1,}(("."{hexdig}{1,}){1,}|(-{hexdig}{1,})){0,1}	{
													yylval->sval =new Symbol(string("hexval"),factory.getHexNode(string(yytext)));
													return token::HEXVAL;}

({digit}{1,}|{digit}*"*"*{digit}*) {yylval->sval =new Symbol(string("repeat"),factory.getRepeatNode(string(yytext)));
									return token::REPEAT;}


{alpha}({alpha}|{digit}|-)*  	 		{yylval->sval =new Symbol(string("rulename"),factory.getNode(string(yytext)));
										
										return token::RULENAME;
										}


.          driver.error (*yylloc, "invalid character", std::string(yytext));
%%

bool
ParserDriver::scan_begin ()
{
  yy_flex_debug = trace_scanning;
  if (!(yyin = fopen (file.c_str (), "r")))
   {
    error (std::string ("cannot open ") + file);
		return false;
	}

	else 
	return true;
}

bool
ParserDriver::scan_end ()
{
 yy_delete_buffer(main_buffer);
 yy_delete_buffer(expansion_buffer); 
  fclose (yyin);
  return true;
}

