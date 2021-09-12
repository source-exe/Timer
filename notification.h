#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <QWidget>
#include <QScreen>
#include <QSize>
#include <QPropertyAnimation>
#include <QTimer>
#include <QSound>
#include <QPushButton>

namespace Ui {
	class notification;
}

class notification : public QWidget
{
	Q_OBJECT

public:
	explicit notification(QWidget* parent = nullptr);
	void show_notification(QPixmap icon, QString label, QString content_text, const long duration);
	~notification();
	QSound* alarm = new QSound(":/alarms/Drip.wav");
	QPushButton* repeat;
	std::string elapsed_time;

public slots:
	void hide_notification();

private:

	QTimer* timer = new QTimer();
	QPropertyAnimation* show_transparency_animation;
	QPropertyAnimation* hide_transparency_animation;
	QSize* resolution;
	Ui::notification* ui;
};

#endif // NOTIFICATION_H
