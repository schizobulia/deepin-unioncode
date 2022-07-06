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
#ifndef GRADLEGENERATOR_H
#define GRADLEGENERATOR_H

#include "services/builder/buildergenerator.h"

#include <QObject>

class GradleGeneratorPrivate;
class GradleGenerator : public dpfservice::BuilderGenerator
{
    Q_OBJECT
    GradleGeneratorPrivate *const d;
public:
    explicit GradleGenerator();
    virtual ~GradleGenerator() override;

    static QString toolKitName() { return "gradle"; }

signals:

private slots:

private:
};

#endif // GRADLEGENERATOR_H
