#ifndef _COMPOSITE_H
#define _COMPOSITE_H

#include <state.h>
#include <branch.h>
#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <action.h>
class Composite {
private:
	
protected:
	
public:
	
	Composite();
	Composite(const Composite& c);
	Composite(std::string);
	
	virtual Composite* clone();
	virtual ~Composite();
	
	virtual void printTree();
	virtual void printTree(std::ostringstream* out);
	virtual std::vector<Composite*> buildTree(std::map<std::string,Branch>* values);
	virtual int getId(){return id;}

	virtual std::string getName(){ return my_name; };
	virtual const std::string& getType() {return type;};
	virtual std::string getFullName(){
		if(strategy == 0)
			return my_name;
		else
			return std::string(my_name+" "+strategy->getName ());
	}
	
	virtual std::vector<Composite*>& getSons(){return sons;}
	virtual void setSons( std::vector<Composite*> s){	sons.clear();sons=s;}
	virtual std::vector<Composite*> cloneSons();
	virtual void addSons(std::vector<Composite*> v);
	virtual void operator<<(Composite*);

	virtual State* runTest();
	virtual Action* getAction();
	
	virtual bool isLeaf(){ return false; };
	virtual std::vector<Composite**> getDataSons();
private:
	
protected:
	
	std::string my_name;
	std::string type;
	
	std::vector<Composite*> sons;
	int id;

	Action* strategy;	

public:
	
};

#endif
