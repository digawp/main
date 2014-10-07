//@author A0094446X
#pragma once
#ifndef YOU_GUI_BASE_MANAGER_H_
#define YOU_GUI_BASE_MANAGER_H_
#include "you_main_gui.h"

/// The class from which all components inherit.
class YouMainGUI::BaseManager : public QObject{
	Q_OBJECT
public:
	/// The constructor that assigns to the manager the parent GUI.
	explicit BaseManager(YouMainGUI * parentGUI);

	/// Tasks needed to be run on startup
	void setup();

protected:
	/// Pointer to the main GUI
	YouMainGUI * const parentGUI;

private:
	/// BaseManager constructor. Not used.
	BaseManager() = delete;
};

#endif  // YOU_GUI_BASE_MANAGER_H_
