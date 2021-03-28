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

#include <QString>

#include <stdexcept>
#include <memory>
#include <set>

namespace Backend
{
    class Capabilities;
    class Mouse;
    class Keyboard;
    class Process;
    class Windowing;
    class System;

    class BACKENDSHARED_EXPORT BackendError
    {
    public:
        BackendError(const QString &message): mMessage(message) {}
        BackendError(): mMessage(QStringLiteral("call failed")) {} // TODO: remove

        const QString &what() const noexcept { return mMessage; }

    private:
        QString mMessage;
    };

    class BACKENDSHARED_EXPORT Instance final
    {
        Q_DISABLE_COPY(Instance)

    private:
        Instance();

    public:
        ~Instance();

        static void initialize() { get(); }
        // Release all buttons and keys.
        static void releaseAll() { get().instReleaseAll(); }
        static const Mouse &mouse() { return *get().mMouse.get(); }
        static const Keyboard &keyboard() { return *get().mKeyboard.get(); }
        static const Process &process() { return *get().mProcess.get(); }
        static const Windowing &windowing() { return *get().mWindowing.get(); }
        static const System &system() { return *get().mSystem.get(); }

    private:
        static Instance &get();

        void instReleaseAll();

        std::unique_ptr<Capabilities> mCapabilities;
        std::unique_ptr<Mouse> mMouse;
        std::unique_ptr<Keyboard> mKeyboard;
        std::unique_ptr<Process> mProcess;
        std::unique_ptr<Windowing> mWindowing;
        std::unique_ptr<System> mSystem;
        std::set<int> mPressedButtons;
        std::set<std::pair<QString, bool>> mPressedKeys;
    };
}
