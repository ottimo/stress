#ifndef _LOG_H
#define _LOG_H

#include <memory>
#include <sstream>
#include <state.h>
#include <composite.h>
#include <output.h>

#define LOG Log::getInstance() 

class Log{
	public:
		static Log* getInstance();
		~Log();

		void printTree(State*, uint64_t, int);
		void printTree(Composite*);
		void closeAll();
		
	protected:
		Log();

		void printTree(std::ostringstream&, std::string);

	private:
		static std::auto_ptr<Log> _instance;
		std::string abnfTreeFileName;
		std::string stateTreeFileName;
		AbstractOutput* output;
};

#endif
