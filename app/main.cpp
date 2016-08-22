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

	monitor.setDevice("260046001647343337363432");
	monitor.setToken("71407113d77f7b586f2bde57910691ed777a4acf");
	if (monitor.dbConnect())
		return app.exec();
	else
		qDebug() << "Did not connect";

	return -1;
}


