//@author A0094446X
#pragma once
#ifndef YOU_GUI_TASK_PANEL_MANAGER_H_
#define YOU_GUI_TASK_PANEL_MANAGER_H_
#include <QApplication>
#include "base_manager.h"
#include "You-Controller/controller.h"
#include "You-Controller/result.h"
#include "You-Controller/controller_context.h"

/// The component that deals with everything in the task panel. It handles the
/// presentation of logic of a set of tasks presented to it by the NLP engine.
/// It inherits from the BaseManager class.
class YouMainGUI::TaskPanelManager : public YouMainGUI::BaseManager{
	Q_OBJECT

public:
	/// Constructor inherited from BaseManager.
	explicit TaskPanelManager(YouMainGUI* const parentGUI);

	/// Destructor.
	~TaskPanelManager();

	/// Initializes the taskTreePanel by setting column count and headers.
	/// Called in the constructor of YouMainGUI.
	void setup();

	/// Adds a task to the taskTreePanel. Only deals with top-level tasks.
	void addTask(const You::Controller::Task& task);

	/// Edits a task.
	void editTask(const You::Controller::Task& task);

	/// Deletes a task.
	void deleteTask(You::Controller::Task::ID taskID);

private:
	QStringList taskToStrVec(const You::Controller::Task& task);

	/// Produces a generic QTreeWidgetItem from a task. It is an
	/// intermediate step to adding headings and tasks.
	std::unique_ptr<QTreeWidgetItem> createItem(
		const You::Controller::Task& task);

	/// Produces a generic QTreeWidgetItem from an item. It is an
	/// intermediate step to adding headings and tasks.
	std::unique_ptr<QTreeWidgetItem> createItem(
		const QStringList& cells);

	/// Adds a subtask to the taskTreePanel. Requires the specification of a
	/// parent task.
	void addSubtask(QTreeWidgetItem* parent, const QStringList& rowStrings);

	/// Deletes a task or subtask. Memory management is automagically dealt
	/// with by QT's parent/child structure, so all child objects are
	/// automatically deleted.
	void deleteTask(QTreeWidgetItem* task);

private:
	/// String/numeric constants for the GUI
	/// Number of columns in task panel
	static const int TASK_COLUMN_COUNT;

	/// Header string for column 1
	static const QString TASK_COLUMN_1;

	/// Header string for column 2
	static const QString TASK_COLUMN_2;

	/// Header string for column 3
	static const QString TASK_COLUMN_3;

	/// Header string for column 4
	static const QString TASK_COLUMN_4;

	/// Header string for column 5
	static const QString TASK_COLUMN_5;

};

#endif  // YOU_GUI_TASK_PANEL_MANAGER_H_
