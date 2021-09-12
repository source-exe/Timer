#include "notification.h"
#include "ui_notification.h"

notification::notification(QWidget* parent) :
	QWidget(parent),
	ui(new Ui::notification)
{
	ui->setupUi(this);
	repeat = new QPushButton("Restart", ui->widget);
	notification::setFixedSize(340, 83);
	setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
	setAttribute(Qt::WA_ShowWithoutActivating);
	setAttribute(Qt::WA_TranslucentBackground);
	resolution = new QSize(notification::screen()->size());
	move(resolution->width(), resolution->height() - 139);
	repeat->setGeometry(250, 30, 80, 21);

	//========================Animation=Setup======================================
	show_transparency_animation = new QPropertyAnimation(this, "windowOpacity");
	show_transparency_animation->setStartValue(0);
	show_transparency_animation->setEndValue(1);

	hide_transparency_animation = new QPropertyAnimation(this, "windowOpacity");
	hide_transparency_animation->setStartValue(1);
	hide_transparency_animation->setEndValue(0);
	//=============================================================================
	connect(timer, SIGNAL(timeout()), this, SLOT(hide_notification()));
}

notification::~notification()
{
	delete ui;
}

void notification::show_notification(QPixmap icon, QString label, QString content_text, const long duration) {
	ui->label->setText(label);
	ui->context_text->setText(content_text);
	ui->icon->setPixmap(icon);
	delete resolution;
	resolution = new QSize(notification::screen()->size());
	move(resolution->width() - 350, resolution->height() - 139);
	setWindowOpacity(0);
	show();
	show_transparency_animation->setDuration(300);
	show_transparency_animation->start();
	activateWindow();
	raise();
	alarm->play();
	timer->start(duration);
}

void notification::hide_notification() {
	delete resolution;
	resolution = new QSize(notification::screen()->size());
	move(resolution->width() - 350, resolution->height() - 139);
	setWindowOpacity(1);
	hide_transparency_animation->setDuration(300);
	hide_transparency_animation->start();
	if (timer->isActive())
		timer->stop();
}