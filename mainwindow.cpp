#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
	ui->setupUi(this);

	connect(notify->repeat, SIGNAL(clicked()), this, SLOT(on_start_button_clicked()));
	//======================Tray=Action=Setup==========================
	connect(tray->quit_timer_action, SIGNAL(triggered()), this, SLOT(on_exit_button_clicked()));
	connect(tray->start_timer_action, SIGNAL(triggered()), this, SLOT(on_start_button_clicked()));
	connect(tray->stop_timer_action, SIGNAL(triggered()), this, SLOT(on_stop_button_clicked()));
	connect(tray->resume_timer_action, SIGNAL(triggered()), this, SLOT(on_resume_button_clicked()));
	connect(tray->theme_timer_action, SIGNAL(triggered()), this, SLOT(on_theme_button_clicked()));
	connect(tray->show_window_action, SIGNAL(triggered()), this, SLOT(show_window_tray()));
	connect(tray->backgroundtray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(on_activatedSysTrayIcon()));

	//=========================Timer=Setup=============================
	timer = new QTimer();
	connect(timer, SIGNAL(timeout()), this, SLOT(timer_slot()));
	//=================================================================
	setWindowFlag(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);
	if (!QFile("settings.ini").exists()) {
        config->setValue("theme/dark", true);
        Dark_theme = true;
	}
	else {
        Dark_theme = !config->value("theme/dark").toBool();
	}
	config->sync();
	on_theme_button_clicked();
}

MainWindow::~MainWindow() {
	delete ui;
}
void MainWindow::print_Time() {
	time.str(std::string());
	if (hours < 10)
		time << "0";
	time << hours << ":";
	if (minutes < 10)
		time << "0";
	time << minutes << ":";
	if (seconds < 10)
		time << "0";
	time << seconds;
	ui->time->setText(time.str().c_str());
	tray->tray_time->setText(time.str().c_str());
}
void MainWindow::timer_slot() {
	seconds--;

	if (hours <= 0 && minutes <= 0 && seconds <= 0) {
		ui->time->setText("00:00:00");
		tray->tray_time->setText("00:00:00");
		ui->start_button->setEnabled(true);
		ui->stop_button->setEnabled(false);
		tray->start_timer_action->setEnabled(true);
		tray->stop_timer_action->setEnabled(false);
		timer->stop();
		notify->show_notification(QPixmap(":/images/icon800x800.png"), "Timer", "Time's up!\nElapsed time: " + QString::fromStdString(notify->elapsed_time), 7000);
	}
	if (seconds < 0) {
		minutes--;
		seconds = 59;
	}
	if (minutes < 0) {
		hours--;
		minutes = 59;
	}
	print_Time();
}

void MainWindow::on_start_button_clicked() {
	if (notify->isVisible()) {
		notify->hide();
	}
	if (!notify->alarm->isFinished())
		notify->alarm->stop();

	if (tray->backgroundtray->isVisible()) {
		hours = tray->time_edit_tray->time().hour();
		minutes = tray->time_edit_tray->time().minute();
		seconds = tray->time_edit_tray->time().second();
	}
	else {
		hours = ui->timeEdit->time().hour();
		minutes = ui->timeEdit->time().minute();
		seconds = ui->timeEdit->time().second();
	}
	print_Time();
	notify->elapsed_time = time.str().c_str();
	if (!(hours <= 0 && minutes <= 0 && seconds <= 0)) {
		ui->resume_button->setEnabled(false);
		ui->stop_button->setEnabled(true);
		ui->start_button->setEnabled(false);
		tray->resume_timer_action->setEnabled(false);
		tray->stop_timer_action->setEnabled(true);
		tray->start_timer_action->setEnabled(false);
		timer->start(1000);
	}
	else {
		ui->time->setText("00:00:00");
		tray->tray_time->setText("00:00:00");
		ui->resume_button->setEnabled(false);
		ui->stop_button->setEnabled(false);
		ui->start_button->setEnabled(true);
		tray->resume_timer_action->setEnabled(false);
		tray->stop_timer_action->setEnabled(false);
		tray->start_timer_action->setEnabled(true);
	}
}

void MainWindow::on_stop_button_clicked() {
	timer->stop();
	ui->resume_button->setEnabled(true);
	ui->stop_button->setEnabled(false);
	ui->start_button->setEnabled(true);
	tray->resume_timer_action->setEnabled(true);
	tray->stop_timer_action->setEnabled(false);
	tray->start_timer_action->setEnabled(true);
}

void MainWindow::on_resume_button_clicked() {
	timer->start(1000);
	ui->resume_button->setEnabled(false);
	ui->stop_button->setEnabled(true);
	ui->start_button->setEnabled(false);
	tray->resume_timer_action->setEnabled(false);
	tray->stop_timer_action->setEnabled(true);
	tray->start_timer_action->setEnabled(false);
}

void MainWindow::on_theme_button_clicked() {
	if (!Dark_theme) {
        ui->main->setStyleSheet("background-color: #535353; border-bottom-left-radius: 9px; border-bottom-right-radius: 9px;");
        ui->timeEdit->setStyleSheet("background-color: #9D9D9D; border-radius: 7px;");
        ui->hide_button->setStyleSheet("QPushButton{background-color: #9D9D9D; border-radius: 0px;} QPushButton:hover{background-color: #2C2C2C; color: #FFFFFF;}");
        ui->start_button->setStyleSheet("QPushButton{background-color: #9D9D9D; border-radius: 7px;} QPushButton:hover{background-color: #2C2C2C; color: #FFFFFF;}");
        ui->stop_button->setStyleSheet("QPushButton{background-color: #9D9D9D; border-radius: 7px;} QPushButton:hover{background-color: #2C2C2C; color: #FFFFFF;}");
        ui->resume_button->setStyleSheet("QPushButton{background-color: #9D9D9D; border-radius: 7px;} QPushButton:hover{background-color: #2C2C2C; color: #FFFFFF;}");
        ui->theme_button->setStyleSheet("QPushButton{background-color: #9D9D9D; border-radius: 7px;} QPushButton:hover{background-color: #2C2C2C; color: #FFFFFF;}");
        ui->exit_button->setStyleSheet("QPushButton{background-color: #9D9D9D; border-radius: 7px;} QPushButton:hover{background-color: #2C2C2C; color: #FFFFFF;}");
        tray->tray_menu->setStyleSheet("QMenu { background-color: #535353;} QMenu::item:selected {background-color: #2C2C2C; color: #FFFFFF;} QMenu::item { background-color: #9D9D9D; border-radius: 5px;} QTimeEdit {background-color: #9D9D9D; border-radius: 5px;}  QMenu::item:disabled {color: #2F2F2F;} QMenu::item::disabled:selected { background-color: #9D9D9D; border-radius: 5px;}");
        notify->setStyleSheet("QWidget {background-color: #535353; border-bottom-left-radius: 9px; border-bottom-right-radius: 9px; border-top-left-radius: 7px; border-top-right-radius: 7px;} QPushButton{background-color: #9D9D9D; border-radius: 7px;} QPushButton:hover{background-color: #2C2C2C; color: #FFFFFF;}");
        ui->title->setStyleSheet("background-color: #9D9D9D; border-top-left-radius: 7px; border-top-right-radius: 7px;");
		ui->theme_button->setText("Theme: Dark");
		tray->theme_timer_action->setText("     Theme: Dark");
		Dark_theme = true;
        config->setValue("theme/dark", Dark_theme);
	}
	else {
        ui->main->setStyleSheet("background-color: #F0F0F0; border-bottom-left-radius: 9px; border-bottom-right-radius: 9px;");
        ui->timeEdit->setStyleSheet("background-color: #C4C4C4; border-radius: 7px;");
        ui->hide_button->setStyleSheet("QPushButton{background-color: #C4C4C4; border-radius: 0px;} QPushButton:hover{background-color: #565656; color: #FFFFFF;}");
        ui->start_button->setStyleSheet("QPushButton{background-color: #C4C4C4; border-radius: 7px;} QPushButton:hover{background-color: #565656; color: #FFFFFF;}");
        ui->stop_button->setStyleSheet("QPushButton{background-color: #C4C4C4; border-radius: 7px;} QPushButton:hover{background-color: #565656; color: #FFFFFF;}");
        ui->resume_button->setStyleSheet("QPushButton{background-color: #C4C4C4; border-radius: 7px;} QPushButton:hover{background-color: #565656; color: #FFFFFF;}");
        ui->theme_button->setStyleSheet("QPushButton{background-color: #C4C4C4; border-radius: 7px;} QPushButton:hover{background-color: #565656; color: #FFFFFF;}");
        ui->exit_button->setStyleSheet("QPushButton{background-color: #C4C4C4; border-radius: 7px;} QPushButton:hover{background-color: #565656; color: #FFFFFF;}");
        tray->tray_menu->setStyleSheet("QMenu { background-color: #F0F0F0;} QMenu::item:selected {background-color: #565656; color: #FFFFFF} QMenu::item { background-color: #C4C4C4; border-radius: 5px;} QTimeEdit {background-color: #C4C4C4; border-radius: 5px;}  QMenu::item:disabled {color: #595959;} QMenu::item::disabled:selected { background-color: #C4C4C4; border-radius: 5px;}");
        notify->setStyleSheet("QWidget {background-color: #F0F0F0; border-bottom-left-radius: 9px; border-bottom-right-radius: 9px; border-top-left-radius: 7px; border-top-right-radius: 7px;} QPushButton{background-color: #C4C4C4; border-radius: 7px;} QPushButton:hover{background-color: #565656; color: #FFFFFF;}");
        ui->title->setStyleSheet("background-color: #C4C4C4; border-top-left-radius: 7px; border-top-right-radius: 7px;");
		ui->theme_button->setText("Theme: Light");
		tray->theme_timer_action->setText("    Theme: Light");
		Dark_theme = false;
        config->setValue("theme/dark", Dark_theme);
	}
	//light
}

void MainWindow::on_exit_button_clicked() {
	if (tray->backgroundtray->isVisible()) {
		tray->backgroundtray->hide();
	}
	MainWindow::close();
	config->sync();
}

void MainWindow::on_hide_button_clicked() {
	tray->backgroundtray->show();
	tray->time_edit_tray->setTime(ui->timeEdit->time());
	MainWindow::hide();
}
void MainWindow::show_window_tray() {
	tray->backgroundtray->hide();
	ui->timeEdit->setTime(tray->time_edit_tray->time());
	MainWindow::show();
};
