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

    for (int x = 0; x < theme_dir->entryList(QDir::AllDirs).length() - 2; x++) {
        QAction *action = new QAction(theme_dir->entryList(QDir::AllDirs).at(x + 2).toStdString().c_str());
        action->setData(theme_dir->entryList(QDir::AllDirs).at(x + 2).toStdString().c_str());
        theme_menu->addAction(action);
    }
    ui->theme_button->setMenu(theme_menu);
    connect(theme_menu, SIGNAL(triggered(QAction*)), this, SLOT(on_theme_action_clicked(QAction*)));

	if (!QFile("settings.ini").exists()) {
        config->setValue("theme", theme);
	}
	else {
        theme = config->value("theme").toString();
	}
    set_theme();
	config->sync();

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
    theme_menu->popup(QCursor::pos());
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
    tray->theme_timer_action->setMenu(theme_menu);
	MainWindow::hide();
}
void MainWindow::show_window_tray() {
	tray->backgroundtray->hide();
	ui->timeEdit->setTime(tray->time_edit_tray->time());
    ui->theme_button->setMenu(theme_menu);
	MainWindow::show();
}

void MainWindow::set_theme() {
    theme_dir->cd(theme);
    main = new QFile(theme_dir->filePath("main.qss"));
    title = new QFile(theme_dir->filePath("title.qss"));
    notifi = new QFile(theme_dir->filePath("notification.qss"));
    tray_menu = new QFile(theme_dir->filePath("tray_menu.qss"));
    main->open(QIODevice::ReadOnly | QIODevice::Text);
    title->open(QIODevice::ReadOnly | QIODevice::Text);
    notifi->open(QIODevice::ReadOnly | QIODevice::Text);
    tray_menu->open(QIODevice::ReadOnly | QIODevice::Text);
    ui->main->setStyleSheet(main->readAll());
    ui->title->setStyleSheet(title->readAll());
    notify->setStyleSheet(notifi->readAll());
    tray->tray_menu->setStyleSheet(tray_menu->readAll());
    main->close();
    title->close();
    notifi->close();
    tray_menu->close();
    theme_dir->cd("..");
    config->setValue("theme", theme);
}

void MainWindow::on_theme_action_clicked(QAction* action) {
    theme = action->data().toString();
    set_theme();
//light

}

