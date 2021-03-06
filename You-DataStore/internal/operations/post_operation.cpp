//@author A0114171W
#include "stdafx.h"
#include "../../task_typedefs.h"
#include "branch_operation.h"
#include "post_operation.h"

namespace You {
namespace DataStore {
namespace Internal {

PostOperation::PostOperation(std::wstring branch, std::wstring id,
	const KeyValuePairs& kvp) {
	branchName = branch;
	nodeId = id;
	task = kvp;
}

bool PostOperation::run(pugi::xml_node& node) {
	pugi::xml_node xmlBranch = BranchOperation::get(node, branchName);
	pugi::xml_node toAdd =
		xmlBranch.find_child_by_attribute(L"id", nodeId.c_str());

	// Checks if the id of the node to be added already exists
	if (toAdd) {
		return false;
	}

	// The name of the node is just the non-plural form of branchName
	std::wstring nodeName(branchName);
	nodeName.pop_back();

	pugi::xml_node newNode = xmlBranch.append_child(nodeName.c_str());
	pugi::xml_attribute id = newNode.append_attribute(L"id");
	id.set_value(nodeId.c_str());
	serialize(task, newNode);
	return true;
}

}  // namespace Internal
}   // namespace DataStore
}   // namespace You
