#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>

#include <QMainWindow>
#include <sstream>
#include <QTimer>
#include <QMouseEvent>
#include "tray_menu.h"
#include <QDir>
#include <QSettings>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include "notification.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
    std::stringstream time;
    enum mode {TIMER = 0, STOPWATCH = 1};
	~MainWindow();

private slots:

	void on_activatedSysTrayIcon() {
		tray->tray_menu->popup(QCursor::pos());
	}

	void show_window_tray();

	void on_start_button_clicked();

	void timer_slot();

    void stopwatch_slot();

	void on_stop_button_clicked();

    //void on_start_button_clicked_stopwatch();

	void on_resume_button_clicked();

	void on_exit_button_clicked();

	void on_hide_button_clicked();

    void mode_switch(QAction *action);

    void on_theme_action_clicked(QAction *action);

    void transform_main();

protected:
    void mousePressEvent(QMouseEvent* event) override {
        if (event->button() == Qt::LeftButton) {
            _p = event->pos();
        }
    }
    void mouseMoveEvent(QMouseEvent* event) override {
        if (!_p.isNull()) {
            move(mapToGlobal(event->pos() - _p));
        }
    }
private:
    QGraphicsOpacityEffect *switch_effect = new QGraphicsOpacityEffect(this);
    QPropertyAnimation *switch_anim_hide = new QPropertyAnimation(switch_effect,"opacity");
    QPropertyAnimation *switch_anim_show = new QPropertyAnimation(switch_effect, "opacity");
    bool initial_startup = true;
    QAction *timer_switch = new QAction("Timer");
    QAction *stopwatch_switch = new QAction("Stopwatch");
    QMenu *mode_menu = new QMenu();
    mode m = mode::TIMER;
    void set_theme();
    QMenu *theme_menu = new QMenu();
    QFile *main, *tray_menu, *notifi;
    QString theme = "Light";
    bool dark = false;
    QDir *theme_dir = new QDir("themes");
    QStringList *theme_list;
    QPoint _p;
    Tray_menu* tray = new Tray_menu();
	void print_Time();
    QSettings* config = new QSettings("settings.ini", QSettings::IniFormat);
    QTimer *stopwatch;
    QTimer *timer;
    unsigned short int hours = 0, minutes = 0, seconds = 0;
    Ui::MainWindow* ui;
    notification* notify = new notification();
};
#endif // MAINWINDOW_H
