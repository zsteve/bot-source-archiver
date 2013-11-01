#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H
#include "parser.h"

class serializable{
    public:
	~serializable(){};
    virtual string serialize()=0;
    virtual bool deserialize(char* data)=0;
    protected:
    parser myParser;
};

#endif
