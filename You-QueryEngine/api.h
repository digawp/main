/// \file api.h
/// Defines the API for Query Engine.
/// \author a0112054y

#pragma once
#ifndef YOU_QUERYENGINE_API_H_
#define YOU_QUERYENGINE_API_H_

#include <memory>
#include <boost/variant.hpp>
#include "task_model.h"

namespace You {
namespace QueryEngine {

/// A synthesized type for query responses
typedef boost::variant <
	 std::vector<Task>,
	 Task,
	 Task::ID,
	 Task::Time,
	 Task::Dependencies,
	 Task::Description
	> Response;

/// Base class for all queries.
class Query {
	friend Response executeQuery(std::unique_ptr<Query> query);
private:
	/// Execute the query.
	virtual Response execute() = 0;
};

/// Execute the query and return a response
Response executeQuery(std::unique_ptr<Query> query);

/// Construct a query for adding a task
/// \note Please use Task::DEFAULT_xxx to replace incomplete fields.
std::unique_ptr<Query>
ADD_TASK(Task::Description description, Task::Time deadline,
Task::Priority priority, Task::Dependencies dependencies);

}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_API_H_
