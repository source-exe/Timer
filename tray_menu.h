#ifndef TRAY_MENU_H
#define TRAY_MENU_H

#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QWidgetAction>
#include <QLabel>
#include <QTimeEdit>

class Tray_menu
{
public:

	Tray_menu();
    QWidgetAction* tray_time_select = new QWidgetAction(NULL);
    QWidgetAction* tray_timer = new QWidgetAction(NULL);
	QSystemTrayIcon* backgroundtray = new QSystemTrayIcon();
	QMenu* tray_menu = new QMenu();
    QAction* show_window_action = new QAction("    Show Window");
    QAction* resume_timer_action = new QAction("         Resume");
    QAction* stop_timer_action = new QAction("            Stop");
    QAction* start_timer_action = new QAction("            Start");
    QAction* theme_timer_action = new QAction("          Theme");
    QAction* mode_switch_action = new QAction("           Mode");
    QAction* quit_timer_action = new QAction("            Quit");
    QAction* separator = new QAction();

	QLabel* tray_time = new QLabel("00:00:00");
	QTimeEdit* time_edit_tray = new QTimeEdit();

private:

};

#endif // TRAY_MENU_H
