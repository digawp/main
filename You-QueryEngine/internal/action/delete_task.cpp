//@author A0112054Y
#include "stdafx.h"

#include "../../../You-Utils/log.h"
#include "../../../You-DataStore/datastore.h"
#include "../../../You-DataStore/transaction.h"

#include "../state.h"
#include "../controller.h"
#include "add_task.h"
#include "batch_add_subtasks.h"
#include "delete_task.h"

namespace You {
namespace QueryEngine {
namespace Internal {
namespace Action {

using Transaction = You::DataStore::Transaction;
using DataStore = You::DataStore::DataStore;
using Log = You::Utils::Log;

const std::wstring DeleteTask::logCategory =
	Query::logCategory + L"[DeleteTask]";

std::unique_ptr<Query> DeleteTask::getReverse() {
	if (deletedTask.getSubtasks().empty()) {
		return std::unique_ptr<Query>(new AddTask(
			deletedTask.getID(),
			deletedTask.getDescription(),
			deletedTask.getStartTime(),
			deletedTask.getDeadline(),
			deletedTask.getPriority(),
			deletedTask.getDependencies(),
			deletedTask.getSubtasks()));
	} else {
		std::vector<std::unique_ptr<Query>> subtaskAddQueries;
		// Construct add query for each subtask
		for (const auto& cid : deletedTask.getSubtasks()) {
			auto c = children[cid];
			subtaskAddQueries.push_back(
				std::unique_ptr<Query>(
					new AddTask(
						cid,
						c.getDescription(),
						c.getStartTime(),
						c.getDeadline(),
						c.getPriority(),
						c.getDependencies(),
						c.getSubtasks())));
		}
		return std::unique_ptr<Query>(new BatchAddSubTasks(
			deletedTask.getID(),
			deletedTask.getDescription(),
			deletedTask.getStartTime(),
			deletedTask.getDeadline(),
			deletedTask.getPriority(),
			deletedTask.getDependencies(),
			std::move(subtaskAddQueries)));
	}
}

void DeleteTask::makeTransaction(const Task::ID id) {
	Log::debug << (boost::wformat(L"%1% : ERASE \"%2%\"")
		% logCategory % id).str();
	Transaction t(DataStore::get().begin());
	DataStore::get().erase(id);
	t.commit();
}

void DeleteTask::deleteTree(State& state, Task::ID id) {
	Task c = state.get().graph().getTask(id);
	children.insert({ id, c });
	for (auto cid : c.getSubtasks()) {
		deleteTree(state, cid);
	}
	Controller::Graph::deleteTask(state.graph(), id);
	Controller::Graph::deleteTask(state.sgraph(), id);
	makeTransaction(id);
}

void DeleteTask::disconnectDependencies(State& state) {
	Task c = state.get().graph().getTask(id);
	auto allTasks = state.get().graph().asTaskList();
	for (auto& task : allTasks) {
		if (task.isDependOn(id)) {
			auto dep = task.getDependencies();
			auto cchild = c.getDependencies();
			// Remove it from depending task.
			dep.erase(id);
			// Reparent task.
			for (const auto& cid : cchild) {
				dep.insert(cid);
			}
			task.setDependencies(dep);
			QueryEngine::UpdateTask(task)->execute(state);
		}
	}
}

Response DeleteTask::execute(State& state) {
	deletedTask = state.get().graph().getTask(id);
	// Remove the task from the parent's task.
	if (!deletedTask.isTopLevel()) {
		auto parent = deletedTask.getParent();
		QueryEngine::RemoveSubtask(parent, id)->execute(state);
	}
	disconnectDependencies(state);
	// If it has subtasks, delete the entire tree.
	if (!deletedTask.getSubtasks().empty()) {
		deleteTree(state, id);
	} else {
		Controller::Graph::deleteTask(state.graph(), id);
		Controller::Graph::deleteTask(state.sgraph(), id);
		makeTransaction(id);
	}
	return this->id;
}

}  // namespace Action
}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You
