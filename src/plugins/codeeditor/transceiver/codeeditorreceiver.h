/*
 * Copyright (C) 2022 Uniontech Software Technology Co., Ltd.
 *
 * Author:     huangyu<huangyub@uniontech.com>
 *
 * Maintainer: huangyu<huangyub@uniontech.com>
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
#ifndef CODEEDITORRECEIVER_H
#define CODEEDITORRECEIVER_H

#include "common/common.h"
#include "framework.h"

class CodeEditorReceiver: public dpf::EventHandler, dpf::AutoEventHandlerRegister<CodeEditorReceiver>
{
    friend class dpf::AutoEventHandlerRegister<CodeEditorReceiver>;
public:
    explicit CodeEditorReceiver(QObject * parent = nullptr);

    static Type type();

    static QStringList topics();

    virtual void eventProcess(const dpf::Event& event) override;
};


class DpfEventMiddleware : public QObject
{
    Q_OBJECT
    DpfEventMiddleware(){}
    DpfEventMiddleware(const DpfEventMiddleware&) = delete;

public:
    static DpfEventMiddleware* instance();

signals:
    void toOpenFile(const QString &filePath);
    void toOpenFile(const lsp::Head &head, const QString &filePath);
    void toRunFileLine(const QString &filePath, int line);
    void toRunClean();
    void toDebugPointClean();
    void toSearchText(const QString &srcText, int operateType);
    void toReplaceText(const QString &srcText, const QString &destText, int operateType);
    void toJumpFileLine(const lsp::Head &head, const QString &filePath, int line);
    void toSetLineBackground(const QString &filePath, int line, const QColor &color);
    void toDelLineBackground(const QString &filePath, int line);
    void toCleanLineBackground(const QString &filePath);
    /* Note = 767 Warning = 766 Error = 765 Fatal = 764*/
    void toSetAnnotation(const QString &filePath, int line, const QString &text, int role = 767);
    void toCleanAnnotation(const QString &filePath);
};

#endif // CODEEDITORRECEIVER_H
