/*
 * Monitor.h
 *
 * Copyright 2015 by Peter Buelow <goballstate@gmail.com>
 *
 * This file is part of some open source application.
 *
 * Some open source application is free software: you can redistribute
 * it and/or modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * Some open source application is distributed in the hope that it will
 * be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @license GPL-3.0+ <http://spdx.org/licenses/GPL-3.0+>
 */

#ifndef MONITOR_H_
#define MONITOR_H_

#include <QtCore>
#include <QtSql>
#include <QtNetwork>
#include <QDebug>

class Monitor : QObject {
	Q_OBJECT
public:
	Monitor(QObject *parent = 0);
	virtual ~Monitor();

	bool dbConnect();
	void setDevice(QString d) { strDevice = d; }
	void setToken(QString t) { strToken = t; }

protected slots:
	void myRequestFinished(QNetworkReply*);
	void timerUpdate();

private:
	QSqlDatabase db;
	QNetworkAccessManager *pManager;
	QString strDevice;
	QString strToken;
};

#endif /* MONITOR_H_ */
