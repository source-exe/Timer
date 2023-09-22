#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::transform_main() {
    if (notify->isVisible())
        notify->hide_notification();

    if (!notify->alarm->isFinished())
        notify->alarm->stop();

    if (timer->isActive()) {
        timer->stop();
    }
    else if (stopwatch->isActive()) {
        stopwatch->stop();
    }
    ui->time->setText("00:00:00");
    tray->tray_time->setText("00:00:00");
    ui->resume_button->setEnabled(false);
    ui->stop_button->setEnabled(false);
    ui->start_button->setEnabled(true);
    tray->resume_timer_action->setEnabled(false);
    tray->stop_timer_action->setEnabled(false);
    tray->start_timer_action->setEnabled(true);
    ui->timeEdit->setTime(QTime(0, 0, 0));
    tray->time_edit_tray->setTime(QTime(0, 0, 0));
    seconds = 0;
    minutes = 0;
    hours = 0;

    disconnect(ui->start_button);
    QFont font = ui->time->font();
    if (m == mode::TIMER) {
        tray->tray_menu->insertAction(tray->resume_timer_action, tray->tray_time_select);
        tray->tray_menu->insertSeparator(tray->resume_timer_action);
        ui->timeEdit->show();
        ui->label->show();
        ui->resume_button->setGeometry(330, 10, 101, 21);
        ui->stop_button->setGeometry(330, 40, 101, 21);
        ui->start_button->setGeometry(330, 70, 101, 21);
        ui->theme_button->setGeometry(330, 100, 101, 21);
        ui->mode_button->setGeometry(330, 130, 101, 21);
        ui->time->setGeometry(0, 36, 331, 121);
        font.setPointSize(60);
    }
    else if (m == mode::STOPWATCH) {
        tray->tray_menu->removeAction(tray->tray_time_select);
        ui->timeEdit->hide();
        ui->label->hide();
        ui->resume_button->setGeometry(6, 130, 81, 21);
        ui->stop_button->setGeometry(94, 130, 81, 21);
        ui->start_button->setGeometry(180, 130, 81, 21);
        ui->theme_button->setGeometry(266, 130, 81, 21);
        ui->mode_button->setGeometry(352, 130, 81, 21);
        ui->time->setGeometry(0, 0, 441, 121);
        font.setPointSize(80);
    }
    ui->time->setFont(font);
    if (!initial_startup) {

        switch_anim_show->start();
    }
}

void MainWindow::mode_switch(QAction *action) {

    if ((mode)action->data().toInt() == m) {
        return;
    }

    switch_anim_hide->start();

    m = (mode)action->data().toInt();
    config->setValue("mode", m);
    config->sync();
}

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    tray->mode_switch_action->setMenu(mode_menu);

    ui->main->setGraphicsEffect(switch_effect);
    switch_effect->setOpacity(1);
    switch_anim_hide->setDuration(500);
    switch_anim_hide->setStartValue(1);
    switch_anim_hide->setEndValue(0);
    switch_anim_hide->setEasingCurve(QEasingCurve::OutBack);

    switch_anim_show->setDuration(500);
    switch_anim_show->setStartValue(0);
    switch_anim_show->setEndValue(1);
    switch_anim_show->setEasingCurve(QEasingCurve::InBack);

    mode_menu->addAction(timer_switch);
    mode_menu->addAction(stopwatch_switch);
    timer_switch->setData(mode::TIMER);
    stopwatch_switch->setData(mode::STOPWATCH);

    connect(switch_anim_hide,SIGNAL(finished()),this,SLOT(transform_main()));

    connect(mode_menu, SIGNAL(triggered(QAction*)), this, SLOT(mode_switch(QAction*)));

    ui->mode_button->setMenu(mode_menu);


    ui->exit_button->setText(QString::fromWCharArray(L"\x2715"));

    connect(notify->repeat, SIGNAL(clicked()), this, SLOT(on_start_button_clicked()));

	//======================Tray=Action=Setup==========================
	connect(tray->quit_timer_action, SIGNAL(triggered()), this, SLOT(on_exit_button_clicked()));
	connect(tray->start_timer_action, SIGNAL(triggered()), this, SLOT(on_start_button_clicked()));
	connect(tray->stop_timer_action, SIGNAL(triggered()), this, SLOT(on_stop_button_clicked()));
	connect(tray->resume_timer_action, SIGNAL(triggered()), this, SLOT(on_resume_button_clicked()));
	connect(tray->show_window_action, SIGNAL(triggered()), this, SLOT(show_window_tray()));
	connect(tray->backgroundtray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(on_activatedSysTrayIcon()));

	//=========================Timer=Setup=============================
	timer = new QTimer();
	connect(timer, SIGNAL(timeout()), this, SLOT(timer_slot()));
    stopwatch = new QTimer();
    connect(stopwatch, SIGNAL(timeout()), this, SLOT(stopwatch_slot()));
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
        config->setValue("mode", m);
	}
	else {
        theme = config->value("theme").toString();
        m = (mode)config->value("mode").toInt();
	}
    transform_main();

    set_theme();
    config->sync();
    initial_startup = false;
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

void MainWindow::stopwatch_slot() {
    seconds++;

    if (seconds > 59) {
        minutes++;
        seconds = 0;
    }
    if (minutes > 59) {
        hours++;
        minutes = 0;
    }
    print_Time();

}

void MainWindow::on_start_button_clicked() {
    if (m == mode::TIMER) {
        if (notify->isVisible())
            notify->hide_notification();

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
    else {
        hours = 0;
        minutes = 0;
        seconds = 0;
        ui->time->setText("00:00:00");
        tray->tray_time->setText("00:00:00");
        ui->resume_button->setEnabled(false);
        ui->stop_button->setEnabled(true);
        ui->start_button->setEnabled(false);
        tray->resume_timer_action->setEnabled(false);
        tray->stop_timer_action->setEnabled(true);
        tray->start_timer_action->setEnabled(false);
        stopwatch->start(1000);
    }
}

void MainWindow::on_stop_button_clicked() {
    if (m == mode::TIMER)
        timer->stop();
    else {
        stopwatch->stop();
    }
    ui->resume_button->setEnabled(true);
	ui->stop_button->setEnabled(false);
	ui->start_button->setEnabled(true);
	tray->resume_timer_action->setEnabled(true);
	tray->stop_timer_action->setEnabled(false);
	tray->start_timer_action->setEnabled(true);
}

void MainWindow::on_resume_button_clicked() {
    if (m == mode::TIMER)
        timer->start();
    else {
        stopwatch->start();
    }
	ui->resume_button->setEnabled(false);
	ui->stop_button->setEnabled(true);
	ui->start_button->setEnabled(false);
	tray->resume_timer_action->setEnabled(false);
	tray->stop_timer_action->setEnabled(true);
	tray->start_timer_action->setEnabled(false);
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
    notifi = new QFile(theme_dir->filePath("notification.qss"));
    tray_menu = new QFile(theme_dir->filePath("tray_menu.qss"));
    main->open(QIODevice::ReadOnly | QIODevice::Text);
    notifi->open(QIODevice::ReadOnly | QIODevice::Text);
    tray_menu->open(QIODevice::ReadOnly | QIODevice::Text);
    ui->background->setStyleSheet(main->readAll());
    notify->setStyleSheet(notifi->readAll());
    tray->tray_menu->setStyleSheet(tray_menu->readAll());
    main->close();
    notifi->close();
    tray_menu->close();
    theme_dir->cd("..");
    config->setValue("theme", theme);
}

void MainWindow::on_theme_action_clicked(QAction* action) {
    theme = action->data().toString();
    set_theme();
}

