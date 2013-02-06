
#ifndef _DRIVER_HH_
# define _DRIVER_HH_
# include <string>
# include <map>
# include "parser.hh"
#include <branch.h>
#include <parsertop.h>





// Announce to Flex the prototype we want for lexing function, ...
# define YY_DECL					\
yy::Parser::token_type                         \
yylex (yy::Parser::semantic_type* yylval,      \
yy::Parser::location_type* yylloc,      \
ParserDriver& driver,		\
CompositeFactory& factory)				
// ... and declare it for the parser's sake.
YY_DECL;

// Conducting the whole scanning and parsing of Calc++.
class ParserDriver
	{
	public:
		ParserDriver ();
		ParserDriver(const ParserDriver& p);
		ParserDriver (ParserTop* t);
	
		
		virtual ~ParserDriver ();
		
		// Handling the scanner.
		bool scan_begin ();
		bool scan_end ();
		bool trace_scanning;
		
		// Handling the parser.
		Composite* parse (const std::string& f);
		
		bool trace_parsing;
		
		// Error handling.
		void error (const yy::location& l, const std::string& m, const std::string& ch);
		void error (const yy::location& l, const std::string& m);
		void error (const std::string& m);
		
		
		Symbol* addNonTerminalNode(Composite* node,string name, Symbol* sx, Symbol* dx=NULL);
		bool addIncrAlternative(Symbol* rule, Symbol* alternative);
		
		void addValue(Symbol* s);
		void printValueMap();	
		
		
		
	private:
		std::string file;
		map<string,Branch> values;
		void addValue(Composite* value);
		void deleteValueMap();
		vector<Composite*> checkUnreferencedSymbols();
		void printUndefinedSymbols(vector<Composite*> undS);
		void printUnreferencedSymbols(vector<Composite*> unrS);
		
		ParserTop* top;
		CompositeFactory factory;
	
	};
#endif //  _DRIVER_HH_
