#include "stdafx.h"
#include "CppUnitTest.h"

#include "../dummy_values.h"
#include "internal/operations/erase_operation.h"
#include "internal/operations/post_operation.h"
#include "internal/operations/put_operation.h"
#include "internal/internal_datastore.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace You {
namespace DataStore {
namespace UnitTests {

using DataStore = You::DataStore::Internal::DataStore;

/// Unit Test Class for DataStore class
TEST_CLASS(DataStoreTest) {
public:
	TEST_METHOD(beginTransaction) {
		DataStore& sut = DataStore::get();
		Assert::IsTrue(sut.transactionStack.empty());
		Transaction t(sut.begin());
		Assert::AreEqual(1U, sut.transactionStack.size());
	}

	TEST_METHOD(pushPostOperation) {
		DataStore& sut = DataStore::get();
		Transaction t(sut.begin());
		sut.post(12, task1);
		Assert::AreEqual(1U, t->operationsQueue.size());
	}

	TEST_METHOD(getAllTasks) {
		DataStore& sut = DataStore::get();
		sut.document.append_child(L"task").
			append_child(pugi::xml_node_type::node_pcdata).set_value(L"what");
		std::vector<SerializedTask> result = sut.getAllTask();
		Assert::AreEqual(1U, result.size());

		sut.document.reset();
		sut.saveData();
	}

	TEST_METHOD(saveThenLoad) {
		DataStore& sut = DataStore::get();
		sut.document.append_child(L"task").
			append_child(pugi::xml_node_type::node_pcdata).set_value(L"what");
		bool result = sut.saveData();
		Assert::IsTrue(result);
		sut.loadData();
		std::wstring value = sut.document.child(L"task").child_value();
		Assert::AreEqual(std::wstring(L"what"), value);

		sut.document.reset();
		sut.saveData();
	}

	TEST_METHOD(pushOperationToTransaction) {
		Internal::Transaction sut;

		std::unique_ptr<Internal::IOperation> post =
			std::make_unique<Internal::PostOperation>(0, task1);
		sut.push(std::move(post));
		Assert::AreEqual(1U, sut.operationsQueue.size());

		std::unique_ptr<Internal::IOperation> put =
			std::make_unique<Internal::PutOperation>(0, task1);
		sut.push(std::move(put));
		Assert::AreEqual(2U, sut.operationsQueue.size());

		std::unique_ptr<Internal::IOperation> erase =
			std::make_unique<Internal::EraseOperation>(0);
		sut.push(std::move(erase));
		Assert::AreEqual(3U, sut.operationsQueue.size());

		sut.operationsQueue.clear();
	}
};
}  // namespace UnitTests
}  // namespace DataStore
}  // namespace You
