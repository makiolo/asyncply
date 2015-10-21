#include "api.h"
#include "multithread.h"
#include "worker.h"

namespace asyncply
{

void worker::execute()
{
	circular_queue<job>* queue = _owner->get_queue();
	while (!_interrupted)
	{
		job* j = queue->pop();
		if(j)
		{
			j->execute();
		}
		else
		{
			// no hay trabajo, esperar a que haya trabajo
			// Tambien puede ser que me despierten porque me han interrumpido
			_owner->_any_job->wait();
		}
	}
}

}

