#include "tray_menu.h"

Tray_menu::Tray_menu()
{
	resume_timer_action->setEnabled(false);
	stop_timer_action->setEnabled(false);

	tray_time->setAlignment(Qt::AlignCenter);
	tray_time->setFont(QFont("", 19));

	time_edit_tray->setAlignment(Qt::AlignCenter);
	time_edit_tray->setDisplayFormat("HH:mm:ss");
	time_edit_tray->setFocusPolicy(Qt::FocusPolicy::NoFocus);
	QPalette* p = new QPalette(time_edit_tray->palette());
	p->setColor(QPalette::Highlight, QColor(255, 255, 255, 0));
	p->setColor(QPalette::HighlightedText, QColor(0, 0, 0));
	time_edit_tray->setPalette(*p);
	delete p;
	time_edit_tray->setButtonSymbols(QTimeEdit::NoButtons);

	backgroundtray->setIcon(QIcon(":/images/icon800x800.png"));

	tray_menu->addAction(show_window_action);
	tray_menu->addSeparator();

	tray_timer->setDefaultWidget(tray_time);
	tray_menu->addAction(tray_timer);
	tray_menu->addSeparator()->setVisible(true);

	tray_time_select->setDefaultWidget(time_edit_tray);
	tray_menu->addAction(tray_time_select);

	tray_menu->addSeparator();
	tray_menu->addAction(resume_timer_action);
	tray_menu->addSeparator()->setVisible(true);
	tray_menu->addAction(stop_timer_action);
	tray_menu->addSeparator()->setVisible(true);
	tray_menu->addAction(start_timer_action);
	tray_menu->addSeparator();

	tray_menu->addAction(theme_timer_action);
	tray_menu->addSeparator()->setVisible(true);
	tray_menu->addAction(quit_timer_action);

	backgroundtray->setContextMenu(tray_menu);
}