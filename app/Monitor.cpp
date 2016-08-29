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
	if (db.open())
		timerUpdate();
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
	double filt = 0.0;

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
		if (filter(temp, &filt)) {
			QSqlQuery query("INSERT INTO FermentTemps (id, date, device, value, filter, name) "
					"VALUES (0, NOW(), ?, ?, ?, ?)");
			query.bindValue(0, strDevice);
			query.bindValue(1, temp);
			query.bindValue(2, filt);
			query.bindValue(3, m_name);
			if (!query.exec()) {
				qWarning() << "QSqlQuery exec() failed with error " << query.lastError().number();
				qWarning() << "Failed Query was";
				qWarning() << query.executedQuery();
				qWarning() << "Failed value on insert was " << value;
				qWarning() << "SQl error was" << query.lastError().text();
			}
		}
	}
	QTimer::singleShot(60000, this, SLOT(timerUpdate()));
}

/**
 * Should be a simple linear regression to find a working short term mean
 * The idea is to ignore anything that varies by more than a few percent
 * as an invalid entry and not store it in the database. So, we calculate
 * the percent error for this value from the last one, and if > 5%, just
 * return false;
 */
bool Monitor::filter(double temp, double *rval)
{
	double last = (double)m_past.back();
	double deviation = std::abs(temp - last);
	double pcnterr = 0.0;
	double mean = 0.0;

	if (deviation > 0)
		pcnterr = (deviation / temp) * 100.0;

	if (pcnterr > 5.0) {
		qWarning() << "Deviation is large:" << pcnterr << "%";
		qWarning() << "Temp is" << temp;
		qWarning() << "Last temp is" << last;
		return false;
	}

	if (m_past.size() > 10)
		m_past.dequeue();

	m_past.enqueue(temp);

	QListIterator<double> i(m_past);
	while (i.hasNext()) {
		mean += i.next();
	}
	mean = mean / m_past.size();
	*rval = mean;
	return true;
}

