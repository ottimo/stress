#ifndef _STATE_H
#define _STATE_H

#include <vector>
#include <string>
#include <sstream>
#include <stdint.h>
#include <iostream>

class State{
	friend class StateTest;
	private:

		virtual void resetValues();
		
	protected:

		virtual void printTestTree(std::ostringstream*);

	public:

		State();
		State(int);
		State(State&);
		~State();

		virtual void setStringValue(std::string);
		virtual void setRawValue(uint8_t*, int);
		virtual void setIntegerValue(int);

		// spostare queste implementazioni a value state e rifare un  return con concatenazione
		virtual std::string getStringValue();
		virtual const uint8_t* getRawValue();
		virtual int getRawLenght();
		virtual int getIntegerValue();

		virtual const State& operator=(const State&);
		virtual void operator<<(State*);
		virtual void operator<<(std::vector<State*>&);

		virtual void setId(int i){ composite_id = i; };
		virtual int  getId(){ return composite_id; };

		virtual std::string getName(){ return my_name; };
		virtual void		 setName(std::string s){ // my_name = s; 
			std::vector<State*>::iterator it;
			for(it = derived_state.begin(); it!= derived_state.end(); it++)
				(*it)->setName(s);
		}
		virtual std::string& getType() {return my_type;};
		virtual const char* getTag() {return "state";};

		virtual void printTree(std::ostringstream* oss){ printTestTree(oss); };

		virtual std::vector<State*>& getSons(){ return derived_state; };

		virtual std::string getStringToSend();
		virtual std::string getValueForLog();
		virtual void setFaultValue(){state_value = FAULT_VALUE;};
		virtual void setPacketValue(){state_value = PACKET_VALUE;};
                virtual void setFailValue(){state_value = FAIL;};

                virtual bool isFaultValue(){return (state_value==FAULT_VALUE);};
		virtual bool isPacketValue(){return (state_value==PACKET_VALUE);};
                virtual bool isFailValue(){ bool result = (state_value == FAIL) ? true : false;
                                            return result; };

		virtual void setAbnfString	(std::string);
		virtual void setAbnfHex		(std::string);
		virtual void setAbnfInt		(std::string);
		virtual void setAbnfBin		(std::string);
		
	private:

	protected:

		const uint8_t* raw_value;
		int raw_lenght;
		std::string string_value;
		int int_value;
		enum state{
			OK,
			FAIL,
			FAULT_VALUE,
			PACKET_VALUE,
			NOT_RESULT
		} state_value;

		int composite_id;
		std::string my_name;
		std::string my_type;
		std::vector<State*> derived_state;

	public:

};

#endif
