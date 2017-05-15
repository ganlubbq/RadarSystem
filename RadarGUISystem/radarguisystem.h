#ifndef RADARGUISYSTEM_H
#define RADARGUISYSTEM_H

#include <QtWidgets/QMainWindow>
#include "ui_radarguisystem.h"


class RadarGUISystem : public QMainWindow
{
	Q_OBJECT

public:
	RadarGUISystem(QWidget *parent = 0);
	~RadarGUISystem();
signals:
	void sendMessage(QString msg);
public slots:
	void receiveMessage(QString msg);

private:
	Ui::RadarGUISystemClass ui;
};

#endif // RADARGUISYSTEM_H
