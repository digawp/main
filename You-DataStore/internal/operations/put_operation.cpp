//@author A0114171W
#include "stdafx.h"
#include "../../task_typedefs.h"
#include "post_operation.h"
#include "put_operation.h"
#include "branch_operation.h"

namespace You {
namespace DataStore {
namespace Internal {

PutOperation::PutOperation(std::wstring branch, std::wstring id,
	const KeyValuePairs& kvp) {
	branchName = branch;
	nodeId = id;
	task = kvp;
}

bool PutOperation::run(pugi::xml_node& node) {
	pugi::xml_node xmlBranch = BranchOperation::get(node, branchName);
	pugi::xml_node toEdit =
		xmlBranch.find_child_by_attribute(L"id", nodeId.c_str());

	// Checks if the node to be edited exists
	if (!toEdit) {
		return false;
	}

	xmlBranch.remove_child(toEdit);

	PostOperation post(branchName, nodeId, task);
	return post.run(node);
}

}  // namespace Internal
}  // namespace DataStore
}  // namespace You
