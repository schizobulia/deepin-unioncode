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

#ifndef PROJECTSERVICE_H
#define PROJECTSERVICE_H

#include "common/common.h"

#include <framework/framework.h>

#include <QStyledItemDelegate>
#include <QStandardItem>
#include <QMenu>

namespace dpfservice {

/*!
 * \brief The ProjectGenerator class 工程生成器对象
 * 该类主要功能为生成各类工程提供接口, 该类与ProjectService接口分开
 * 达到接口隔离和适应插件组合变化
 * 模式1. 仅仅包含ProjectGenerator的插件（独立功能的插件）
 * 模式2. 包含ProjectGenerator、主界面框架组件发布、流程执行（倾向于业务流程）
 * ProjectService 面向工程核心(ProjectCore) 提供对外接口
 * ProjectGenerator 面向工程扩展实现其他插件对外的接口
 * ProjectCore <-> ProjectService <-> [outside plugin]
 */
class ProjectGenerator : public Generator
{
    Q_OBJECT
public:
    /*!
     * \brief The ItemDataRole enum
     *  获得QModelIndex 与Item数据互通支持(Qt规范)
     */
    enum ItemDataRole
    {
        ToolKitProperty = Qt::ItemDataRole::UserRole,
    };

    // 自定义枚举，工具套件的唯一键
    enum_def(ToolKitPropertyKey, QString)
    {
        enum_exp ToolKitName = "ToolKitName";
    };

    ProjectGenerator(){}

    /*!
     * \brief configure 生成器配置界面，子类需要重载实现
     *  按照一般的做法，当前界面应当生成配置文件与工程路径绑定，
     *  在界面上选中、输入信息存入配置文件
     * \param projectPath 工程路径
     * \return 返回工程配置界面
     */
    virtual QWidget* configureWidget(const QString &projectPath) {
        Q_UNUSED(projectPath)
        return nullptr;
    }

    /*!
     * \brief configure 执行生成器的过程
     *  按照生成器配置界面约定规则，实现执行过程，
     *  例如在cmake中，需要执行configure指令生成cache等相关文件
     *  执行该函数，应当首先确保前置条件满足(配置文件已生成)。
     * \param projectPath 工程路径
     * \return 过程执行结果
     */
    virtual bool configure(const QString &projectPath) {
        Q_UNUSED(projectPath)
        return false;
    }

    /*!
     * \brief createRootItem 创建文件树路径，子类需要重载实现
     *  执行该函数应当首先确定前置条件的满足，比如已经执行了生成器的过程。
     * \param projectPath 工程文件路径
     * \return
     */
    virtual QStandardItem *createRootItem(const QString &projectPath) {
        Q_UNUSED(projectPath);
        return nullptr;
    }

    /*!
     * \brief createIndexMenu 文件树index点击时触发，子类需要重载实现
     *  文件树中右键触发的创建Menu，传入树中QModelIndex，它与Item数据相通(Qt规范)
     *  QMenu对象在插件使用方会被释放
     * \param index 索引数据
     * \return
     */
    virtual QMenu* createItemMenu(const QStandardItem *item) {
        Q_UNUSED(item);
        return nullptr;
    }

    static void setToolKitName(QStandardItem *item, const QString &name);
    static QString toolKitName(const QStandardItem *item);
    static void setToolKitProperty(QStandardItem *item, const QString &key, const QVariant &value);
    static QVariant toolKitProperty(const QStandardItem *item, const QString &key);
    static QVariantMap toolKitPropertyMap(const QStandardItem *item);
    static const QStandardItem *top(const QStandardItem *item);

Q_SIGNALS:
    /*!
     * \brief targetExecute 工程目标执行指令.
     *  使用该信号, 由于当前插件不隶属于构建系统,需要构建生成并且统计构建错误，则需要与builder插件通信
     * \param program 执行程序
     * \param arguments 参数
     */
    void targetExecute(const QString &program, const QStringList &arguments);
};

class ProjectService final : public dpf::PluginService,
        dpf::AutoServiceRegister<ProjectService>,
        dpf::QtClassFactory<ProjectGenerator>,
        dpf::QtClassManager<ProjectGenerator>

{
    Q_OBJECT
    Q_DISABLE_COPY(ProjectService)
    typedef dpf::QtClassManager<ProjectGenerator> GeneratorManager;
    typedef dpf::QtClassFactory<ProjectGenerator> GeneratorFactory;
public:
    static QString name();
    explicit ProjectService(QObject *parent = nullptr);

    /*!
     * \brief supportGenerator 獲取支持的工程名稱
     * \return
     */
    QStringList supportGeneratorName()
    {
        return GeneratorFactory::createKeys();
    }

    /*!
     * \brief implGenerator 導入工程文件生成器對象 T = Project::Generator類泛化對象
     * \param name 生成器對象名稱(唯一鍵)
     * \param errorString 錯誤信息
     */
    template<class T = ProjectGenerator>
    bool implGenerator(const QString &name, QString *errorString = nullptr)
    {
        return GeneratorFactory::regClass<T>(name, errorString);
    }

    /*!
     * \brief createGenerator
     * \param name 名稱
     * \param errorString 錯誤信息
     * \return 生成器對象實例
     */
    ProjectGenerator *createGenerator(const QString &name, QString *errorString = nullptr)
    {
        auto generator = GeneratorManager::value(name);
        if (!generator) {
            generator = GeneratorFactory::create(name, errorString);
            if (generator)
                GeneratorManager::append(name, generator);
        }
        return generator;
    }

    /*!
     * \brief addProjectRootItem 添加根數據到TreeView
     * \param aitem
     */
    DPF_INTERFACE(void, addProjectRootItem, QStandardItem *aitem);

    /*!
     * \brief expandedProjectDepth 展开工程子项根据深度
     * \param aitem root节点
     * \param depth 深度
     */
    DPF_INTERFACE(void, expandedProjectDepth, QStandardItem *aitem, int depth);

    /*!
     * \brief expandedProjectAll 展开工程所有子项
     * \param aitem root节点
     */
    DPF_INTERFACE(void, expandedProjectAll, QStandardItem *aitem);

Q_SIGNALS:
    /*!
     * \brief targetExecute 工程目标执行指令, 调用构建系统的入口
     *  当前projectservice不属于构建系统的一部分，需要构建系统支持
     * \param program 程序
     * \param arguments 程序参数
     */
    void targetExecute(const QString &program, const QStringList &arguments);
};

/* MainWindow codeediter workspace title,
 * use in window service swtich workspace
 */
extern const QString MWCWT_PROJECTS;

} //namespace dpfservice

#endif // PROJECTSERVICE_H