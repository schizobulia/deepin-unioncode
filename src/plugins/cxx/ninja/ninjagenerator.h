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
#ifndef NINJAGENERATOR_H
#define NINJAGENERATOR_H

#include "services/language/languagegenerator.h"
#include "dap/protocol.h"

class NinjaGeneratorPrivate;
class NinjaGenerator : public dpfservice::LanguageGenerator
{
    Q_OBJECT
public:
    explicit NinjaGenerator();
    virtual ~NinjaGenerator() override;

    static QString toolKitName() { return "ninja"; }

    bool prepareDebug(const QMap<QString, QVariant> &param, QString &retMsg) override;
    bool requestDAPPort(const QString &uuid, const QMap<QString, QVariant> &param, QString &retMsg) override;
    bool isNeedBuild() override;
    bool isTargetReady() override;
    bool isLaunchNotAttach() override;
    dap::LaunchRequest launchDAP(const QMap<QString, QVariant> &param) override;
    QString build(const QString &projectPath) override;
    QMap<QString, QVariant> getDebugArguments(const dpfservice::ProjectInfo &projectInfo,
                                              const QString &currentFile) override;
    dpfservice::RunCommandInfo getRunArguments(const dpfservice::ProjectInfo &projectInfo,
                                               const QString &currentFile) override;

signals:

private slots:

private:
    NinjaGeneratorPrivate *const d;
};

#endif // NINJAGENERATOR_H
