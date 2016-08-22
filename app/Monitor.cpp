/*
 * Monitor.cpp
 *
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

#include "Monitor.h"

Monitor::Monitor(QObject *parent) : QObject(parent) {
	QTimer::singleShot(60000, this, SLOT(timerUpdate()));

	pManager = new QNetworkAccessManager(this);
	connect(pManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(myRequestFinished(QNetworkReply*)));
}

Monitor::~Monitor() {
}

bool Monitor::dbConnect()
{
	db = QSqlDatabase::addDatabase("QMYSQL");
	db.setHostName("localhost");
	db.setDatabaseName("Ferment");
	db.setUserName("fermenter");
	db.setPassword("Scrub12");
	return db.open();
}

void Monitor::timerUpdate()
{
	QNetworkRequest request;
	QString strUrl();
	request.setUrl(QUrl("https://api.particle.io/v1/devices/"+strDevice+"/fermTemp?access_token="+strToken));
	pManager->get(request);
}

void Monitor::myRequestFinished(QNetworkReply* reply)
{
	if (reply->error() > 0) {
		qWarning() << "QNetworkReply error " << reply->error();
	}
	else {
		QByteArray value = reply->readAll();
		QJsonDocument doc = QJsonDocument::fromJson(value);
		if (doc.isNull())
			qWarning() << "Doc is null";
		QJsonObject object = doc.object();

		double temp = object.value("result").toDouble();

		QSqlQuery query("INSERT INTO FermentTemps (id, date, device, value)"
				"VALUES (0, NOW(), ?, ?)");
		query.bindValue(0, strDevice);
		query.bindValue(1, temp);
		if (!query.exec()) {
			qWarning() << "QSqlQuery exec() failed with error " << query.lastError().number();
			qWarning() << "Failed Query was";
			qWarning() << query.lastQuery();
			qWarning() << "Failed value on insert was " << value;
		}
	}
	QTimer::singleShot(60000, this, SLOT(timerUpdate()));
}
