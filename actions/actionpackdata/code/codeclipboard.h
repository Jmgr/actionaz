/*
	Actionaz
	Copyright (C) 2008-2010 Jonathan Mercier-Ganady

	Actionaz is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Actionaz is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.

	Contact : jmgr@jmgr.info
*/

#ifndef CODECLIPBOARD_H
#define CODECLIPBOARD_H

#include <QObject>
#include <QScriptable>
#include <QScriptValue>
#include <QScriptEngine>
#include <QClipboard>

class CodeClipboard : public QObject, public QScriptable
{
    Q_OBJECT
	Q_ENUMS(Mode)

public:
	enum Mode
	{
        Clipboard =		QClipboard::Clipboard,
		Selection =		QClipboard::Selection,
		FindBuffer =	QClipboard::FindBuffer
	};
	enum DataType
	{
		Text,
		Image
	};
	
	static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);

	CodeClipboard();

public slots:
	QScriptValue setMode(Mode mode);
	QScriptValue writeText(const QString &value) const;
	QScriptValue writeImage(const QScriptValue &data) const;
	QString readText() const;
	QScriptValue readImage() const;
	DataType dataType() const;
	
private:
	bool isModeValid(Mode mode) const;

	Mode mMode;
};

#endif // CODECLIPBOARD_H