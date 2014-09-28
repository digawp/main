#include "stdafx.h"
#include <QApplication>
#include <QList>
#include "you_main_gui.h"
#include "system_tray_manager.h"

YouMainGUI::SystemTrayManager::~SystemTrayManager() {
}

void YouMainGUI::SystemTrayManager::setup() {
	createActions();
	setIcon();
	makeContextMenu();
	connectTrayActivatedSlot();
	parentGUI->setVisible(true);
}

void YouMainGUI::SystemTrayManager::setIcon() {
	QIcon icon("icon.png");
	trayIcon.setIcon(icon);
	parentGUI->setWindowIcon(icon);
	trayIcon.show();
}

void YouMainGUI::SystemTrayManager::makeContextMenu() {
	trayIconMenu = new QMenu(parentGUI);
	trayIconMenu->addAction(minimizeAction);
	trayIconMenu->addAction(maximizeAction);
	trayIconMenu->addAction(restoreAction);
	trayIconMenu->addSeparator();
	trayIconMenu->addAction(quitAction);
	trayIcon.setContextMenu(trayIconMenu);
}

void YouMainGUI::SystemTrayManager::connectTrayActivatedSlot() {
	connect(&trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
		this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
}
void YouMainGUI::SystemTrayManager::iconActivated(
	QSystemTrayIcon::ActivationReason reason) {
	if (reason == QSystemTrayIcon::Trigger) {
		if (parentGUI->isVisible() == true)
			parentGUI->hide();
		else
			parentGUI->show();
	}
}

void YouMainGUI::SystemTrayManager::createActions() {
	minimizeAction = new QAction(tr("Minimize"), parentGUI);
	connect(minimizeAction, SIGNAL(triggered()), parentGUI, SLOT(hide()));

	maximizeAction = new QAction(tr("Maximize"), parentGUI);
	connect(maximizeAction, SIGNAL(triggered()), parentGUI, SLOT(showMaximized()));

	restoreAction = new QAction(tr("Restore"), parentGUI);
	connect(restoreAction, SIGNAL(triggered()), parentGUI, SLOT(showNormal()));

	quitAction = new QAction(tr("Quit"), parentGUI);
	connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}