#ifndef _JOB_H_
#define _JOB_H_

namespace asyncply
{

class multithread_API job
{
public:
	job()
	{ ; }
	virtual ~job(void) {}

	virtual void execute() = 0;
};

}

#endif
