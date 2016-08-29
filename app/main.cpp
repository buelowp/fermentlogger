/*
 * main.cpp
 *
 *  Created on: Sep 10, 2015
 *      Author: buelowp
 */

#include <QCoreApplication>
#include <QDebug>
#include "Monitor.h"

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);
	Monitor monitor;

	if (argc != 2) {
		qWarning() << "Arg count is" << argc;
		return -1;
	}

	monitor.setDevice("260046001647343337363432");
	monitor.setToken("71407113d77f7b586f2bde57910691ed777a4acf");
	monitor.setName(app.arguments().at(1));
	if (monitor.dbConnect())
		return app.exec();
	else
		qDebug() << "Did not connect";

	return -1;
}


