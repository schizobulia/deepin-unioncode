/*
 * Copyright (C) 2022 Uniontech Software Technology Co., Ltd.
 *
 * Author:     zhouyi<zhouyi1@uniontech.com>
 *
 * Maintainer: zhouyi<zhouyi1@uniontech.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "ninjagenerator.h"

#include "ninjabuild.h"
#include "ninjadebug.h"
#include "services/project/projectservice.h"
#include "services/option/optionmanager.h"

#include <QFile>

using namespace dpfservice;

class NinjaGeneratorPrivate
{
    friend class NinjaGenerator;
    QSharedPointer<NinjaDebug> ninjaDebug;
};

NinjaGenerator::NinjaGenerator()
    : d(new NinjaGeneratorPrivate())
{
    d->ninjaDebug.reset(new NinjaDebug());
}

NinjaGenerator::~NinjaGenerator()
{
    if (d)
        delete d;
}

bool NinjaGenerator::prepareDebug(const QMap<QString, QVariant> &param, QString &retMsg)
{
    Q_UNUSED(param)
    Q_UNUSED(retMsg)
    return true;
}

bool NinjaGenerator::requestDAPPort(const QString &uuid, const QMap<QString, QVariant> &param, QString &retMsg)
{
    QString targetPath = param.value("targetPath").toString();
    QStringList arguments = param.value("arguments").toStringList();

    return d->ninjaDebug->requestDAPPort(uuid, toolKitName(), targetPath, arguments, retMsg);
}

bool NinjaGenerator::isNeedBuild()
{
    return true;
}

bool NinjaGenerator::isTargetReady()
{
    return true;
}

bool NinjaGenerator::isLaunchNotAttach()
{
    return true;
}

dap::LaunchRequest NinjaGenerator::launchDAP(const QMap<QString, QVariant> &param)
{
    QString targetPath = param.value("targetPath").toString();
    QStringList arguments = param.value("arguments").toStringList();

    return d->ninjaDebug->launchDAP(targetPath, arguments);
}

QString NinjaGenerator::build(const QString &projectPath)
{
    return NinjaBuild::build(toolKitName(), projectPath);
}

QMap<QString, QVariant> NinjaGenerator::getDebugArguments(const dpfservice::ProjectInfo &projectInfo,
                                                          const QString &currentFile)
{
    Q_UNUSED(currentFile)

    QMap<QString, QVariant> param;
    QString workspace = projectInfo.workspaceFolder();
    param.insert("workspace", workspace);
    QString targetPath = workspace + QDir::separator() + QFileInfo(workspace).fileName();
    param.insert("targetPath", targetPath);

    return param;
}

RunCommandInfo NinjaGenerator::getRunArguments(const ProjectInfo &projectInfo, const QString &currentFile)
{
    Q_UNUSED(currentFile)

    RunCommandInfo runCommandInfo;
    QString workspace = projectInfo.workspaceFolder();
    QString targetPath = workspace + QDir::separator() + QFileInfo(workspace).fileName();
    runCommandInfo.program = targetPath;
    runCommandInfo.arguments = projectInfo.runCustomArgs();
    runCommandInfo.workingDir = workspace;

    return runCommandInfo;
}
