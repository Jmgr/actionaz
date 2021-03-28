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

#include "backend/backend_global.hpp"
#include "backend/backend.hpp"
#include "backend/feature.hpp"

#include <QList>

namespace Backend
{
    class BACKENDSHARED_EXPORT Process final
    {
        Q_DISABLE_COPY(Process)

    public:
        enum class KillMode
        {
            Graceful,
            Forceful,
            GracefulThenForceful
        };
        enum class ProcessStatus
        {
            Running,
            Stopped
        };
        enum class Priority
        {
            AboveNormal,
            BelowNormal,
            High,
            Idle,
            Normal,
            Realtime
        };

        Process();

        Feature<void(int id, KillMode killMode, int timeout)> killProcess
        {
            QStringLiteral("killProcess"),
            [](int, Process::KillMode, int){}
        };
        Feature<ProcessStatus(int id)> processStatus
        {
            QStringLiteral("processStatus"),
            [](int){ return Process::ProcessStatus::Running; }
        };
        Feature<QList<int>()> runningProcesses
        {
            QStringLiteral("runningProcesses"),
            [](){ return QList<int>{}; }
        };
        Feature<int(int id)> parentProcess
        {
            QStringLiteral("parentProcess"),
            [](int){ return 0; }
        };
        Feature<QString(int id)> processCommand
        {
            QStringLiteral("processCommand"),
            [](int){ return QString{}; }
        };
        Feature<Priority(int id)> processPriority
        {
            QStringLiteral("processPriority"),
            [](int){ return Process::Priority{}; }
        };
    };
}
