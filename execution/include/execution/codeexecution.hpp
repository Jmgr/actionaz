/*
	Actiona
	Copyright (C) 2005 Jonathan Mercier-Ganady

	Actiona is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Actiona is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.

	Contact: jmgr@jmgr.info
*/

#pragma once

#include "execution_global.hpp"

#include <QObject>
#include <QScriptValue>

class QScriptContext;
class QScriptEngine;

namespace Execution
{
	class ScriptAgent;
	
	class EXECUTIONSHARED_EXPORT CodeExecution : public QObject
	{
		Q_OBJECT
		
	public:
		static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);
		static void setScriptAgent(ScriptAgent *scriptAgent) { mScriptAgent = scriptAgent; }

		static QScriptValue pause(QScriptContext *context, QScriptEngine *engine);
		static QScriptValue sleep(QScriptContext *context, QScriptEngine *engine);
		static QScriptValue stop(QScriptContext *context, QScriptEngine *engine);
		
	private:
		static ScriptAgent *mScriptAgent;
	};
}

