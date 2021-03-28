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
#include "backend/system.hpp"

namespace Backend
{
    void BACKENDSHARED_EXPORT logoutUnix(bool force);
    void BACKENDSHARED_EXPORT restartUnix(bool force);
    void BACKENDSHARED_EXPORT shutdownUnix(bool force);
    void BACKENDSHARED_EXPORT suspendUnix(bool force);
    void BACKENDSHARED_EXPORT hibernateUnix(bool force);
    void BACKENDSHARED_EXPORT lockScreenUnix();
    void BACKENDSHARED_EXPORT startScreenSaverUnix();
    QString BACKENDSHARED_EXPORT getUsernameUnix();
    void BACKENDSHARED_EXPORT copyFilesUnix(const QString &sourceFilepath, const QString &destinationFilepath, const System::FileOperationParameters &parameters);
    void BACKENDSHARED_EXPORT moveFilesUnix(const QString &sourceFilepath, const QString &destinationFilepath, const System::FileOperationParameters &parameters);
    void BACKENDSHARED_EXPORT renameFilesUnix(const QString &sourceFilepath, const QString &destinationFilepath, const System::FileOperationParameters &parameters);
    void BACKENDSHARED_EXPORT removeFilesUnix(const QString &filepath, const System::FileOperationParameters &parameters);
}
