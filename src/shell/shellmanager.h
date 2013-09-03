/*
 *   Copyright (C) 2013 Ivan Cukic <ivan.cukic(at)kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2,
 *   or (at your option) any later version, as published by the Free
 *   Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef SHELLMANAGER_H
#define SHELLMANAGER_H

#include <QObject>

class ShellManager: public QObject {
    Q_OBJECT
public:
    static ShellManager * instance();
    ~ShellManager();

    void loadHandlers();

protected Q_SLOTS:
    void registerHandler(QObject * handler);
    void deregisterHandler(QObject * handler);

public Q_SLOTS:
    void requestShellUpdate();
    void updateShell();

Q_SIGNALS:
    void shellChanged(const QString & shell);

private:
    ShellManager();

    class Private;
    const QScopedPointer<Private> d;
};

#endif /* SHELLMANAGER_H */
