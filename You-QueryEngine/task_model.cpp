#include "stdafx.h"
#include "task_model.h"

using boost::posix_time::ptime;
using boost::posix_time::hours;
using boost::gregorian::date;
using boost::gregorian::max_date_time;

namespace You {
namespace QueryEngine {

void Task::setDescription(const Task::Description &description) {
	this->description = description;
}

void Task::setDeadline(Task::Time time) {
	this->deadline = time;
}

void Task::setDependencies(const Task::Dependencies &dependencies) {
	this->dependencies = dependencies;
}

void Task::setPriority(Task::Priority priority) {
	this->priority = priority;
}

Task Task::nextNewTask() {
	return Task(
		DUMMY_ID,  // TODO(evansb) Change this when DS ready.
		DEFAULT_DESCRIPTION,
		DEFAULT_DEADLINE,
		DEFAULT_DEPENDENCIES,
		DEFAULT_PRIORITY);
}

const Task::Description Task::DEFAULT_DESCRIPTION = L"";
const Task::Time Task::NEVER = ptime(date(max_date_time), hours(0));
const Task::Time Task::DEFAULT_DEADLINE = Task::NEVER;
const Task::Dependencies Task::DEFAULT_DEPENDENCIES;
const Task::Priority Task::DEFAULT_PRIORITY = Task::Priority::NORMAL;

const Task::ID Task::DUMMY_ID = 42L;

}  // namespace QueryEngine
}  // namespace You
