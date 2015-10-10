#ifndef _JOB_FUNCTION_H_
#define _JOB_FUNCTION_H_

#include "Job.h"

namespace asyncply
{

class job_function : public job
{
public:
	job_function(const std::function<void()>& function)
        : _function( function )
        , _callback( true )
	{ ; }

	job_function()
        : _function( nullptr )
        , _callback(false)
    { ; }

	virtual ~job_function() {}

	void set_function(const std::function<void()>& function)
	{
		_function = function;
		_callback = true;
	}

	virtual void execute() override
	{
		_function();
	}

protected:
	std::function<void()> _function;
	bool _callback;
};

}

#endif  // _JOB_FUNCTION_H_
