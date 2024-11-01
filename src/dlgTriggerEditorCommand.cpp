#include "dlgTriggerEditorCommand.h"
#include "Host.h"
#include "LuaInterface.h"
#include "dlgTriggerEditor.h"
#include "dlgTriggerPatternEdit.h"
#include "dlgAliasMainArea.h"
#include "dlgScriptsMainArea.h"
#include "dlgKeysMainArea.h"
#include "dlgActionMainArea.h"
#include "mudlet.h"
#include <QPointer>

AddTriggerCommand::AddTriggerCommand(QTreeWidgetItem* pItem, TriggerUnit* triggerUnit, TTreeWidget* treeWidgetTriggers, bool isFolder, QUndoCommand* parent) : QUndoCommand(parent)
{
    mpTriggerUnit = triggerUnit;
    mpTreeWidgetTriggers = treeWidgetTriggers;
    mIsFolder = isFolder;
    mpItem = pItem;
}

void AddTriggerCommand::undo()
{
    if (!mpItem) {
        return;
    }
    mpParent = mpItem->parent();

    if (mpParent) {
        mpParent->removeChild(mpItem);
    } else {
        qDebug() << "parent is null ";
    }
}

void AddTriggerCommand::redo()
{
    if (!mpEditor) {
        return;
    }
    if (!mpItem) {
        mpEditor->addTrigger(mIsFolder);
        mpItem = mpTreeWidgetTriggers->currentItem();
        mpParent = mpItem->parent();
    } else {
        int count = mpParent->childCount();
        if (mIsFolder) {
            mpParent->addChild(mpItem);
        } else {
            mpParent->insertChild(count <= 0 ? 0 : count, mpItem);
        }
    }

    setText(QObject::tr("Add Trigger"));
}

DeleteTriggerCommand::DeleteTriggerCommand(QTreeWidgetItem* pItem, TriggerUnit* triggerUnit, TTreeWidget* treeWidgetTriggers, QUndoCommand* parent) : QUndoCommand(parent)
{
    mpItem = pItem;
    mpParent = mpItem->parent();
    mpTriggerUnit = triggerUnit;
    mpTreeWidgetTriggers = treeWidgetTriggers;
}

void DeleteTriggerCommand::undo()
{
    if (!mpItem) {
        return;
    }

    TTrigger* pT = nullptr;
    if (mpParent) {
        const int childID = mpItemTrigger->getID();
        mpItem->setData(0, Qt::UserRole, childID);
        mpParent->insertChild(mpParent->childCount() <= 0 ? 0 : mpParent->childCount(), mpItem);
        mpTreeWidgetTriggers->setCurrentItem(mpItem);
    } else {
        qDebug() << "parent is null ";
    }
}
void DeleteTriggerCommand::redo()
{
    if (!mpEditor) {
        return;
    }
    if (!mpHost) {
        return;
    }
    if (mpItem) {
        const int ID = mpItem->data(0, Qt::UserRole).toInt();
        TTrigger* p = mpTriggerUnit->getTrigger(ID);
        TTrigger* ptr = new TTrigger(p->mName, p->mPatterns, p->getRegexCodePropertyList(), false, mpHost);
        ptr->registerTrigger();
        mpItemTrigger = ptr;
        mpEditor->delete_trigger();
    }
    setText(QObject::tr("Delete Trigger"));
}

MoveTriggerCommand::MoveTriggerCommand(TriggerUnit* triggerUnit,
                                       TTreeWidget* treeWidgetTriggers,
                                       int childID,
                                       int oldParentID,
                                       int newParentID,
                                       int parentPosition,
                                       int childPosition,
                                       int prevParentPosition,
                                       int prevChildPosition,
                                       QUndoCommand* parent)
: QUndoCommand(parent)
{
    mChildID = childID;
    mOldParentID = oldParentID;
    mNewParentID = newParentID;
    mParentPosition = parentPosition;
    mChildPosition = childPosition;
    mPrevParentPosition = prevParentPosition;
    mPrevChildPosition = prevChildPosition;
    mpTriggerUnit = triggerUnit;
    mpTreeWidgetTriggers = treeWidgetTriggers;
}

void MoveTriggerCommand::undo()
{
    if (!mpHost) {
        return;
    }

    mpHost->getTriggerUnit()->reParentTrigger(mChildID, mNewParentID, mOldParentID, mPrevParentPosition, mPrevChildPosition);
    mpParentItem->removeChild(mpItem);
    mpPrevParentItem->insertChild(mpPrevParentItem->childCount() <= 0 ? 0 : mpParentItem->childCount(), mpItem);
    mpTreeWidgetTriggers->setCurrentItem(mpItem);
}

void MoveTriggerCommand::redo()
{
    mpHost->getTriggerUnit()->reParentTrigger(mChildID, mOldParentID, mNewParentID, mParentPosition, mChildPosition);
    if (mpPrevParentItem) {
        mpPrevParentItem->removeChild(mpItem);
    }
    if (mpParentItem) {
        const int count = mpParentItem->childCount();
        mpParentItem->insertChild(count <= 0 ? 0 : count, mpItem);
    }

    setText(QObject::tr("Move Trigger"));
}

AddAliasCommand::AddAliasCommand(QTreeWidgetItem* pItem, AliasUnit* aliasUnit, TTreeWidget* treeWidget_aliases, bool isFolder, QUndoCommand* parent)
{
    mpAliasUnit = aliasUnit;
    mpTreeWidgetAliases = treeWidget_aliases;
    mIsFolder = isFolder;
    mpItem = pItem;
}

void AddAliasCommand::undo()
{
    if (!mpItem) {
        return;
    }
    mpParent = mpItem->parent();

    if (mpParent) {
        mpParent->removeChild(mpItem);
    } else {
        qDebug() << "parent is null ";
    }
}

void AddAliasCommand::redo()
{
    if (!mpEditor) {
        return;
    }
    if (!mpItem) {
        mpEditor->addAlias(mIsFolder);
        mpItem = mpTreeWidgetAliases->currentItem();
        mpParent = mpItem->parent();
    } else {
        int count = mpParent->childCount();
        if (mIsFolder) {
            mpParent->addChild(mpItem);
        } else {
            mpParent->insertChild(count <= 0 ? 0 : count, mpItem);
        }
    }

    setText(QObject::tr("Add Alias"));
}

DeleteAliasCommand::DeleteAliasCommand(QTreeWidgetItem* pItem, AliasUnit* aliasUnit, TTreeWidget* treeWidget_aliases, QUndoCommand* parent) : QUndoCommand(parent)
{
    mpItem = pItem;
    mpParent = mpItem->parent();
    mpAliasUnit = aliasUnit;
    mpTreeWidgetAliases = treeWidget_aliases;
}

void DeleteAliasCommand::undo()
{
    if (!mpItem) {
        return;
    }

    TTrigger* pT = nullptr;
    if (mpParent) {
        const int childID = mpItemAlias->getID();
        mpItem->setData(0, Qt::UserRole, childID);
        mpParent->insertChild(mpParent->childCount() <= 0 ? 0 : mpParent->childCount(), mpItem);
        mpTreeWidgetAliases->setCurrentItem(mpItem);
    } else {
        qDebug() << "parent is null ";
    }
}

void DeleteAliasCommand::redo()
{
    if (!mpEditor) {
        return;
    }
    if (!mpHost) {
        return;
    }
    if (mpItem) {
        const int ID = mpItem->data(0, Qt::UserRole).toInt();
        TAlias* p = mpAliasUnit->getAlias(ID);
        TAlias* ptr = new TAlias(p->mName, mpHost);
        ptr->registerAlias();
        mpItemAlias = ptr;
        mpEditor->delete_alias();
    }
    setText(QObject::tr("Delete Alias"));
}

MoveAliasCommand::MoveAliasCommand(AliasUnit* aliasUnit,
                                   TTreeWidget* treeWidget_aliases,
                                   int childID,
                                   int oldParentID,
                                   int newParentID,
                                   int parentPosition,
                                   int childPosition,
                                   int prevParentPosition,
                                   int prevChildPosition,
                                   QUndoCommand* parent)
: QUndoCommand(parent)
{
    mChildID = childID;
    mOldParentID = oldParentID;
    mNewParentID = newParentID;
    mParentPosition = parentPosition;
    mChildPosition = childPosition;
    mPrevParentPosition = prevParentPosition;
    mPrevChildPosition = prevChildPosition;
    mpAliasUnit = aliasUnit;
    mpTreeWidgetAliases = treeWidget_aliases;
}

void MoveAliasCommand::undo()
{
    if (!mpHost) {
        return;
    }

    mpHost->getAliasUnit()->reParentAlias(mChildID, mNewParentID, mOldParentID, mPrevParentPosition, mPrevChildPosition);
    mpParentItem->removeChild(mpItem);
    mpPrevParentItem->insertChild(mpPrevParentItem->childCount() <= 0 ? 0 : mpPrevParentItem->childCount(), mpItem);
    mpTreeWidgetAliases->setCurrentItem(mpItem);
}

void MoveAliasCommand::redo()
{
    mpHost->getAliasUnit()->reParentAlias(mChildID, mOldParentID, mNewParentID, mParentPosition, mChildPosition);
    if (mpPrevParentItem) {
        mpPrevParentItem->removeChild(mpItem);
    }
    if (mpParentItem) {
        const int count = mpParentItem->childCount();
        mpParentItem->insertChild(count <= 0 ? 0 : count, mpItem);
    }

    setText(QObject::tr("Move Alias"));
}

AliasNameTextEditedCommand::AliasNameTextEditedCommand(dlgAliasMainArea* mpAliasMainArea, QUndoCommand* parent)
{
    mpAliasMainArea = mpAliasMainArea;
}

void AliasNameTextEditedCommand::undo()
{
    if (!mpItem) {
        return;
    }
    mpTreeWidgetAliases->setCurrentItem(mpItem);
    mpAliasMainArea->lineEdit_alias_name->setText(mPrevAliasName);
}

void AliasNameTextEditedCommand::redo()
{
    if (!mpItem) {
        return;
    }
    mpTreeWidgetAliases->setCurrentItem(mpItem);
    mpAliasMainArea->lineEdit_alias_name->setText(mAliasName);
    setText(QObject::tr("Edit alias name"));
}

AliasCommandTextEditedCommand::AliasCommandTextEditedCommand(dlgAliasMainArea* mpAliasMainArea, QUndoCommand* parent)
{
    mpAliasMainArea = mpAliasMainArea;
}

void AliasCommandTextEditedCommand::undo()
{
    if (!mpItem) {
        return;
    }
    mpTreeWidgetAliases->setCurrentItem(mpItem);
    mpAliasMainArea->lineEdit_alias_command->setText(mPrevAliasCommand);
}

void AliasCommandTextEditedCommand::redo()
{
    if (!mpItem) {
        return;
    }
    mpTreeWidgetAliases->setCurrentItem(mpItem);
    mpAliasMainArea->lineEdit_alias_command->setText(mAliasCommand);
    setText(QObject::tr("Edit alias command"));
}

AliasPatternTextEditedCommand::AliasPatternTextEditedCommand(dlgAliasMainArea* mpAliasMainArea, QUndoCommand* parent)
{
    mpAliasMainArea = mpAliasMainArea;
}

void AliasPatternTextEditedCommand::undo()
{
    if (!mpItem) {
        return;
    }
    mpTreeWidgetAliases->setCurrentItem(mpItem);
    mpAliasMainArea->lineEdit_alias_pattern->setText(mPrevAliasPattern);
}

void AliasPatternTextEditedCommand::redo()
{
    if (!mpItem) {
        return;
    }
    mpTreeWidgetAliases->setCurrentItem(mpItem);
    mpAliasMainArea->lineEdit_alias_pattern->setText(mAliasPattern);
    setText(QObject::tr("Edit alias pattern"));
}

AddTimerCommand::AddTimerCommand(QTreeWidgetItem* pItem, TimerUnit* timerUnit, TTreeWidget* treeWidgetTimers, bool isFolder, QUndoCommand* parent)
{
    mpTimerUnit = timerUnit;
    mpTreeWidgetTimers = treeWidgetTimers;
    mIsFolder = isFolder;
    mpItem = pItem;
}

void AddTimerCommand::undo()
{
    if (!mpItem) {
        return;
    }
    mpParent = mpItem->parent();

    if (mpParent) {
        mpParent->removeChild(mpItem);
    } else {
        qDebug() << "parent is null ";
    }
}

void AddTimerCommand::redo()
{
    if (!mpEditor) {
        return;
    }
    if (!mpItem) {
        mpEditor->addTimer(mIsFolder);
        mpItem = mpTreeWidgetTimers->currentItem();
        mpParent = mpItem->parent();
    } else {
        int count = mpParent->childCount();
        if (mIsFolder) {
            mpParent->addChild(mpItem);
        } else {
            mpParent->insertChild(count <= 0 ? 0 : count, mpItem);
        }
    }

    setText(QObject::tr("Add Timer"));
}

DeleteTimerCommand::DeleteTimerCommand(QTreeWidgetItem* pItem, TimerUnit* timerUnit, TTreeWidget* treeWidgetTimers, QUndoCommand* parent) : QUndoCommand(parent)
{
    mpItem = pItem;
    mpParent = mpItem->parent();
    mpTimerUnit = timerUnit;
    mpTreeWidgetTimers = treeWidgetTimers;
}

void DeleteTimerCommand::undo()
{
    if (!mpItem) {
        return;
    }

    TTrigger* pT = nullptr;
    if (mpParent) {
        const int childID = mpItemTimer->getID();
        mpItem->setData(0, Qt::UserRole, childID);
        mpParent->insertChild(mpParent->childCount() <= 0 ? 0 : mpParent->childCount(), mpItem);
        mpTreeWidgetTimers->setCurrentItem(mpItem);
    } else {
        qDebug() << "parent is null ";
    }
}

void DeleteTimerCommand::redo()
{
    if (!mpEditor) {
        return;
    }
    if (!mpHost) {
        return;
    }
    if (mpItem) {
        const int ID = mpItem->data(0, Qt::UserRole).toInt();
        TTimer* p = mpTimerUnit->getTimer(ID);
        TTimer* ptr = new TTimer(p->getName(), p->getTime(), mpHost);
        mpHost->getTimerUnit()->registerTimer(ptr);
        mpItemTimer = ptr;
        mpEditor->delete_timer();
    }
    setText(QObject::tr("Delete Timer"));
}

MoveTimerCommand::MoveTimerCommand(TimerUnit* timerUnit,
                                   TTreeWidget* treeWidgetTimers,
                                   int childID,
                                   int oldParentID,
                                   int newParentID,
                                   int parentPosition,
                                   int childPosition,
                                   int prevParentPosition,
                                   int prevChildPosition,
                                   QUndoCommand* parent)
: QUndoCommand(parent)
{
    mChildID = childID;
    mOldParentID = oldParentID;
    mNewParentID = newParentID;
    mParentPosition = parentPosition;
    mChildPosition = childPosition;
    mPrevParentPosition = prevParentPosition;
    mPrevChildPosition = prevChildPosition;
    mpTimerUnit = timerUnit;
    mpTreeWidgetTimers = treeWidgetTimers;
}

void MoveTimerCommand::undo()
{
    if (!mpHost) {
        return;
    }

    mpHost->getTimerUnit()->reParentTimer(mChildID, mNewParentID, mOldParentID, mPrevParentPosition, mPrevChildPosition);
    mpParentItem->removeChild(mpItem);
    mpPrevParentItem->insertChild(mpPrevParentItem->childCount() <= 0 ? 0 : mpPrevParentItem->childCount(), mpItem);
    mpTreeWidgetTimers->setCurrentItem(mpItem);
}

void MoveTimerCommand::redo()
{
    mpHost->getTimerUnit()->reParentTimer(mChildID, mOldParentID, mNewParentID, mParentPosition, mChildPosition);
    if (mpPrevParentItem) {
        mpPrevParentItem->removeChild(mpItem);
    }
    if (mpParentItem) {
        const int count = mpParentItem->childCount();
        mpParentItem->insertChild(count <= 0 ? 0 : count, mpItem);
    }

    setText(QObject::tr("Move Timer"));
}

TimerNameTextEditedCommand::TimerNameTextEditedCommand(dlgTimersMainArea* timersMainArea, QUndoCommand* parent)
{
    mpTimersMainArea = timersMainArea;
}

void TimerNameTextEditedCommand::undo()
{
    if (!mpItem) {
        return;
    }
    mpTreeWidgetTimers->setCurrentItem(mpItem);
    mpTimersMainArea->lineEdit_timer_name->setText(mPrevTimerName);
}

void TimerNameTextEditedCommand::redo()
{
    if (!mpItem) {
        return;
    }
    mpTreeWidgetTimers->setCurrentItem(mpItem);
    mpTimersMainArea->lineEdit_timer_name->setText(mTimerName);
    setText(QObject::tr("Edit timer name"));
}

TimerCommandTextEditedCommand::TimerCommandTextEditedCommand(dlgTimersMainArea* timersMainArea, QUndoCommand* parent)
{
    mpTimersMainArea = timersMainArea;
}

void TimerCommandTextEditedCommand::undo()
{
    if (!mpItem) {
        return;
    }
    mpTreeWidgetTimers->setCurrentItem(mpItem);
    mpTimersMainArea->lineEdit_timer_command->setText(mPrevTimerCommand);
}

void TimerCommandTextEditedCommand::redo()
{
    if (!mpItem) {
        return;
    }
    mpTreeWidgetTimers->setCurrentItem(mpItem);
    mpTimersMainArea->lineEdit_timer_command->setText(mTimerCommand);
    setText(QObject::tr("Edit timer command"));
}

TimerHoursTextEditedCommand::TimerHoursTextEditedCommand(dlgTimersMainArea* timersMainArea, QUndoCommand* parent)
{
    mpTimersMainArea = timersMainArea;
}

void TimerHoursTextEditedCommand::undo()
{
    if (!mpItem) {
        return;
    }
    mpTreeWidgetTimers->setCurrentItem(mpItem);
    mpTimersMainArea->timeEdit_timer_hours->blockSignals(true);
    mpTimersMainArea->timeEdit_timer_hours->setTime(QTime(mPrevTimerHours, 0, 0, 0));
    mpTimersMainArea->timeEdit_timer_hours->blockSignals(false);
}

void TimerHoursTextEditedCommand::redo()
{
    if (!mpItem) {
        return;
    }
    mpTreeWidgetTimers->setCurrentItem(mpItem);
    mpTimersMainArea->timeEdit_timer_hours->blockSignals(true);
    mpTimersMainArea->timeEdit_timer_hours->setTime(QTime(mTimerHours, 0, 0, 0));
    mpTimersMainArea->timeEdit_timer_hours->blockSignals(false);
    setText(QObject::tr("Edit timer hours"));
}

TimerMinutesTextEditedCommand::TimerMinutesTextEditedCommand(dlgTimersMainArea* timersMainArea, QUndoCommand* parent)
{
    mpTimersMainArea = timersMainArea;
}

void TimerMinutesTextEditedCommand::undo()
{
    if (!mpItem) {
        return;
    }
    mpTreeWidgetTimers->setCurrentItem(mpItem);
    mpTimersMainArea->timeEdit_timer_minutes->blockSignals(true);
    mpTimersMainArea->timeEdit_timer_minutes->setTime(QTime(0, mPrevTimerMinutes, 0, 0));
    mpTimersMainArea->timeEdit_timer_minutes->blockSignals(false);
}

void TimerMinutesTextEditedCommand::redo()
{
    if (!mpItem) {
        return;
    }
    mpTreeWidgetTimers->setCurrentItem(mpItem);
    mpTimersMainArea->timeEdit_timer_minutes->blockSignals(true);
    mpTimersMainArea->timeEdit_timer_minutes->setTime(QTime(0, mTimerMinutes, 0, 0));
    mpTimersMainArea->timeEdit_timer_minutes->blockSignals(false);
    setText(QObject::tr("Edit timer minutes"));
}

TimerSecondsTextEditedCommand::TimerSecondsTextEditedCommand(dlgTimersMainArea* timersMainArea, QUndoCommand* parent)
{
    mpTimersMainArea = timersMainArea;
}

void TimerSecondsTextEditedCommand::undo()
{
    if (!mpItem) {
        return;
    }
    mpTreeWidgetTimers->setCurrentItem(mpItem);
    mpTimersMainArea->timeEdit_timer_seconds->blockSignals(true);
    mpTimersMainArea->timeEdit_timer_seconds->setTime(QTime(0, 0, mPrevTimerSeconds, 0));
    mpTimersMainArea->timeEdit_timer_seconds->blockSignals(false);
}

void TimerSecondsTextEditedCommand::redo()
{
    if (!mpItem) {
        return;
    }
    mpTreeWidgetTimers->setCurrentItem(mpItem);
    mpTimersMainArea->timeEdit_timer_seconds->blockSignals(true);
    mpTimersMainArea->timeEdit_timer_seconds->setTime(QTime(0, 0, mTimerSeconds, 0));
    mpTimersMainArea->timeEdit_timer_seconds->blockSignals(false);
    setText(QObject::tr("Edit timer seconds"));
}

TimerMilliSecondsTextEditedCommand::TimerMilliSecondsTextEditedCommand(dlgTimersMainArea* timersMainArea, QUndoCommand* parent)
{
    mpTimersMainArea = timersMainArea;
}

void TimerMilliSecondsTextEditedCommand::undo()
{
    if (!mpItem) {
        return;
    }
    mpTreeWidgetTimers->setCurrentItem(mpItem);
    mpTimersMainArea->timeEdit_timer_msecs->blockSignals(true);
    mpTimersMainArea->timeEdit_timer_msecs->setTime(QTime(0, 0, 0, mPrevTimerMsecs));
    mpTimersMainArea->timeEdit_timer_msecs->blockSignals(false);
}

void TimerMilliSecondsTextEditedCommand::redo()
{
    if (!mpItem) {
        return;
    }
    mpTreeWidgetTimers->setCurrentItem(mpItem);
    mpTimersMainArea->timeEdit_timer_msecs->blockSignals(true);
    mpTimersMainArea->timeEdit_timer_msecs->setTime(QTime(0, 0, 0, mTimerMsecs));
    mpTimersMainArea->timeEdit_timer_msecs->blockSignals(false);
    setText(QObject::tr("Edit timer msecs"));
}

AddScriptCommand::AddScriptCommand(QTreeWidgetItem* pItem, ScriptUnit* scriptUnit, TTreeWidget* treeWidget_scripts, bool isFolder, QUndoCommand* parent)
{
    mpScriptUnit = scriptUnit;
    mpTreeWidgetScripts = treeWidget_scripts;
    mIsFolder = isFolder;
    mpItem = pItem;
}

void AddScriptCommand::undo()
{
    if (!mpItem) {
        return;
    }
    mpParent = mpItem->parent();

    if (mpParent) {
        mpParent->removeChild(mpItem);
    } else {
        qDebug() << "parent is null ";
    }
}

void AddScriptCommand::redo()
{
    if (!mpEditor) {
        return;
    }
    if (!mpItem) {
        mpEditor->addScript(mIsFolder);
        mpItem = mpTreeWidgetScripts->currentItem();
        mpParent = mpItem->parent();
    } else {
        int count = mpParent->childCount();
        if (mIsFolder) {
            mpParent->addChild(mpItem);
        } else {
            mpParent->insertChild(count <= 0 ? 0 : count, mpItem);
        }
    }

    setText(QObject::tr("Add Script"));
}

DeleteScriptCommand::DeleteScriptCommand(QTreeWidgetItem* pItem, ScriptUnit* scriptUnit, TTreeWidget* treeWidget_scripts, QUndoCommand* parent) : QUndoCommand(parent)
{
    mpItem = pItem;
    mpParent = mpItem->parent();
    mpScriptUnit = scriptUnit;
    mpTreeWidgetScripts = treeWidget_scripts;
}

void DeleteScriptCommand::undo()
{
    if (!mpItem) {
        return;
    }

    TTrigger* pT = nullptr;
    if (mpParent) {
        const int childID = mpItemScript->getID();
        mpItem->setData(0, Qt::UserRole, childID);
        mpParent->insertChild(mpParent->childCount() <= 0 ? 0 : mpParent->childCount(), mpItem);
        mpTreeWidgetScripts->setCurrentItem(mpItem);
    } else {
        qDebug() << "parent is null ";
    }
}

void DeleteScriptCommand::redo()
{
    if (!mpEditor) {
        return;
    }
    if (!mpHost) {
        return;
    }
    if (mpItem) {
        const int ID = mpItem->data(0, Qt::UserRole).toInt();
        TScript* p = mpScriptUnit->getScript(ID);
        TScript* ptr = new TScript(p->getName(), mpHost);
        ptr->registerScript();
        mpItemScript = ptr;
        mpEditor->delete_script();
    }
    setText(QObject::tr("Delete Script"));
}

MoveScriptCommand::MoveScriptCommand(ScriptUnit* scriptUnit,
                                     TTreeWidget* treeWidget_scripts,
                                     int childID,
                                     int oldParentID,
                                     int newParentID,
                                     int parentPosition,
                                     int childPosition,
                                     int prevParentPosition,
                                     int prevChildPosition,
                                     QUndoCommand* parent)
: QUndoCommand(parent)
{
    mChildID = childID;
    mOldParentID = oldParentID;
    mNewParentID = newParentID;
    mParentPosition = parentPosition;
    mChildPosition = childPosition;
    mPrevParentPosition = prevParentPosition;
    mPrevChildPosition = prevChildPosition;
    mpScriptUnit = scriptUnit;
    mpTreeWidgetScripts = treeWidget_scripts;
}

void MoveScriptCommand::undo()
{
    if (!mpHost) {
        return;
    }

    mpHost->getScriptUnit()->reParentScript(mChildID, mNewParentID, mOldParentID, mPrevParentPosition, mPrevChildPosition);
    mpParentItem->removeChild(mpItem);
    mpPrevParentItem->insertChild(mpPrevParentItem->childCount() <= 0 ? 0 : mpPrevParentItem->childCount(), mpItem);
    mpTreeWidgetScripts->setCurrentItem(mpItem);
}

void MoveScriptCommand::redo()
{
    mpHost->getScriptUnit()->reParentScript(mChildID, mOldParentID, mNewParentID, mParentPosition, mChildPosition);
    if (mpPrevParentItem) {
        mpPrevParentItem->removeChild(mpItem);
    }
    if (mpParentItem) {
        const int count = mpParentItem->childCount();
        mpParentItem->insertChild(count <= 0 ? 0 : count, mpItem);
    }

    setText(QObject::tr("Move Script"));
}

ScriptNameTextEditedCommand::ScriptNameTextEditedCommand(dlgScriptsMainArea* scriptsMainArea, QUndoCommand* parent)
{
    mpScriptsMainArea = scriptsMainArea;
}

void ScriptNameTextEditedCommand::undo()
{
    if (!mpItem) {
        return;
    }
    mpTreeWidgetScripts->setCurrentItem(mpItem);
    mpScriptsMainArea->lineEdit_script_name->setText(mPrevScriptName);
}

void ScriptNameTextEditedCommand::redo()
{
    if (!mpItem) {
        return;
    }
    mpTreeWidgetScripts->setCurrentItem(mpItem);
    mpScriptsMainArea->lineEdit_script_name->setText(mScriptName);
    setText(QObject::tr("Edit script name"));
}

ScriptAddHandlerCommand::ScriptAddHandlerCommand(dlgScriptsMainArea* scriptsMainArea, QUndoCommand* parent)
{
    mpScriptsMainArea = scriptsMainArea;
}

void ScriptAddHandlerCommand::undo()
{
    if (!mpItem) {
        return;
    }
    mpTreeWidgetScripts->setCurrentItem(mpItem);
    auto pItem = new QListWidgetItem;
    pItem->setText(mPrevScriptEventhandler);
    mpScriptsMainArea->listWidget_script_registered_event_handlers->takeItem(mRow);
}

void ScriptAddHandlerCommand::redo()
{
    if (!mpItem) {
        return;
    }
    mpTreeWidgetScripts->setCurrentItem(mpItem);
    auto pItem = new QListWidgetItem;
    pItem->setText(mScriptEventhandler);
    mpScriptsMainArea->listWidget_script_registered_event_handlers->addItem(pItem);
    mpWidgetItem = pItem;
    mRow = mpScriptsMainArea->listWidget_script_registered_event_handlers->indexFromItem(pItem).row();
    mpScriptsMainArea->listWidget_script_registered_event_handlers->setCurrentRow(mRow);
    setText(QObject::tr("Add script handler"));
}

ScriptRemoveHandlerCommand::ScriptRemoveHandlerCommand(dlgScriptsMainArea* scriptsMainArea, QUndoCommand* parent)
{
    mpScriptsMainArea = scriptsMainArea;
}

void ScriptRemoveHandlerCommand::undo()
{
    if (!mpItem) {
        return;
    }
    mpTreeWidgetScripts->setCurrentItem(mpItem);
    mpScriptsMainArea->listWidget_script_registered_event_handlers->addItem(mpWidgetItem);
    mRow = mpScriptsMainArea->listWidget_script_registered_event_handlers->indexFromItem(mpWidgetItem).row();
    mpScriptsMainArea->listWidget_script_registered_event_handlers->setCurrentRow(mRow);
}

void ScriptRemoveHandlerCommand::redo()
{
    if (!mpItem) {
        return;
    }
    mpTreeWidgetScripts->setCurrentItem(mpItem);
    mRow = mpScriptsMainArea->listWidget_script_registered_event_handlers->currentRow();
    if(mRow < 0)
    {
        return;
    }
    mpWidgetItem = mpScriptsMainArea->listWidget_script_registered_event_handlers->takeItem(mRow);
    setText(QObject::tr("Remove script handler"));
}

AddKeyCommand::AddKeyCommand(QTreeWidgetItem* pItem, KeyUnit* keyUnit, TTreeWidget* treeWidget_keys, bool isFolder, QUndoCommand* parent)
{
    mpKeyUnit = keyUnit;
    mpTreeWidgetKeys = treeWidget_keys;
    mIsFolder = isFolder;
    mpItem = pItem;
}

void AddKeyCommand::undo()
{
    if (!mpItem) {
        return;
    }
    mpParent = mpItem->parent();

    if (mpParent) {
        mpParent->removeChild(mpItem);
    } else {
        qDebug() << "parent is null ";
    }
}

void AddKeyCommand::redo()
{
    if (!mpEditor) {
        return;
    }
    if (!mpItem) {
        mpEditor->addKey(mIsFolder);
        mpItem = mpTreeWidgetKeys->currentItem();
        mpParent = mpItem->parent();
    } else {
        int count = mpParent->childCount();
        if (mIsFolder) {
            mpParent->addChild(mpItem);
        } else {
            mpParent->insertChild(count <= 0 ? 0 : count, mpItem);
        }
    }

    setText(QObject::tr("Add Key"));
}

DeleteKeyCommand::DeleteKeyCommand(QTreeWidgetItem* pItem, KeyUnit* keyUnit, TTreeWidget* treeWidget_keys, QUndoCommand* parent) : QUndoCommand(parent)
{
    mpItem = pItem;
    mpParent = mpItem->parent();
    mpKeyUnit = keyUnit;
    mpTreeWidgetKeys = treeWidget_keys;
}

void DeleteKeyCommand::undo()
{
    if (!mpItem) {
        return;
    }

    TTrigger* pT = nullptr;
    if (mpParent) {
        const int childID = mpItemKey->getID();
        mpItem->setData(0, Qt::UserRole, childID);
        mpParent->insertChild(mpParent->childCount() <= 0 ? 0 : mpParent->childCount(), mpParent);
        mpTreeWidgetKeys->setCurrentItem(mpItem);
    } else {
        qDebug() << "parent is null ";
    }
}

void DeleteKeyCommand::redo()
{
    if (!mpEditor) {
        return;
    }
    if (!mpHost) {
        return;
    }
    if (mpItem) {
        const int ID = mpItem->data(0, Qt::UserRole).toInt();
        TKey* p = mpKeyUnit->getKey(ID);
        TKey* ptr = new TKey(p->getName(), mpHost);
        ptr->registerKey();
        mpItemKey = ptr;
        mpEditor->delete_key();
    }
    setText(QObject::tr("Delete Key"));
}
MoveKeyCommand::MoveKeyCommand(KeyUnit* keyUnit,
                               TTreeWidget* treeWidget_keys,
                               int childID,
                               int oldParentID,
                               int newParentID,
                               int parentPosition,
                               int childPosition,
                               int prevParentPosition,
                               int prevChildPosition,
                               QUndoCommand* parent)
: QUndoCommand(parent)
{
    mChildID = childID;
    mOldParentID = oldParentID;
    mNewParentID = newParentID;
    mParentPosition = parentPosition;
    mChildPosition = childPosition;
    mPrevParentPosition = prevParentPosition;
    mPrevChildPosition = prevChildPosition;
    mpKeyUnit = keyUnit;
    mpTreeWidgetKeys = treeWidget_keys;
}

void MoveKeyCommand::undo()
{
    if (!mpHost) {
        return;
    }

    mpHost->getKeyUnit()->reParentKey(mChildID, mNewParentID, mOldParentID, mPrevParentPosition, mPrevChildPosition);
    mpParentItem->removeChild(mpItem);
    mpPrevParentItem->insertChild(mpPrevParentItem->childCount() <= 0 ? 0 : mpPrevParentItem->childCount(), mpItem);
    mpTreeWidgetKeys->setCurrentItem(mpItem);
}

void MoveKeyCommand::redo()
{
    mpHost->getKeyUnit()->reParentKey(mChildID, mOldParentID, mNewParentID, mParentPosition, mChildPosition);
    if (mpPrevParentItem) {
        mpPrevParentItem->removeChild(mpItem);
    }
    if (mpParentItem) {
        const int count = mpParentItem->childCount();
        mpParentItem->insertChild(count <= 0 ? 0 : count, mpItem);
    }

    setText(QObject::tr("Move Key"));
}

KeyNameTextEditedCommand::KeyNameTextEditedCommand(dlgKeysMainArea* keysMainArea, QUndoCommand* parent)
{
    mpKeysMainArea = keysMainArea;
}

void KeyNameTextEditedCommand::undo()
{
    if (!mpItem) {
        return;
    }
    mpTreeWidgetKeys->setCurrentItem(mpItem);
    mpKeysMainArea->lineEdit_key_name->blockSignals(true);
    mpKeysMainArea->lineEdit_key_name->setText(mPrevKeyName);
    mpKeysMainArea->lineEdit_key_name->blockSignals(false);
}

void KeyNameTextEditedCommand::redo()
{
    if (!mpItem) {
        return;
    }
    mpTreeWidgetKeys->setCurrentItem(mpItem);
    mpKeysMainArea->lineEdit_key_name->blockSignals(true);
    mpKeysMainArea->lineEdit_key_name->setText(mKeyName);
    mpKeysMainArea->lineEdit_key_name->blockSignals(false);
    setText(QObject::tr("Edit key name"));
}

KeyCommandTextEditedCommand::KeyCommandTextEditedCommand(dlgKeysMainArea* keysMainArea, QUndoCommand* parent)
{
    mpKeysMainArea = keysMainArea;
}

void KeyCommandTextEditedCommand::undo()
{
    if (!mpItem) {
        return;
    }
    mpTreeWidgetKeys->setCurrentItem(mpItem);
    mpKeysMainArea->lineEdit_key_command->blockSignals(true);
    mpKeysMainArea->lineEdit_key_command->setText(mPrevKeyCommand);
    mpKeysMainArea->lineEdit_key_command->blockSignals(false);
}

void KeyCommandTextEditedCommand::redo()
{
    if (!mpItem) {
        return;
    }
    mpTreeWidgetKeys->setCurrentItem(mpItem);
    mpKeysMainArea->lineEdit_key_command->blockSignals(true);
    mpKeysMainArea->lineEdit_key_command->setText(mKeyCommand);
    mpKeysMainArea->lineEdit_key_command->blockSignals(false);
    setText(QObject::tr("Edit key command"));
}

KeyGrabTextEditedCommand::KeyGrabTextEditedCommand(dlgKeysMainArea* keysMainArea, QUndoCommand* parent)
{
    mpKeysMainArea = keysMainArea;
}

void KeyGrabTextEditedCommand::undo()
{
    if (!mpItem) {
        return;
    }
    mpTreeWidgetKeys->setCurrentItem(mpItem);
    const int triggerID = mpItem->data(0, Qt::UserRole).toInt();
    TKey* pT = mpKeyUnit->getKey(triggerID);
    if (pT) {
        pT->setKeyCode(mPrevKey);
        pT->setKeyModifiers(mPrevModifier);
    }
    mpKeysMainArea->lineEdit_key_binding->blockSignals(true);
    mpKeysMainArea->lineEdit_key_binding->setText(mPrevKeyName);
    mpKeysMainArea->lineEdit_key_binding->blockSignals(false);
}

void KeyGrabTextEditedCommand::redo()
{
    if (!mpItem) {
        return;
    }
    mpTreeWidgetKeys->setCurrentItem(mpItem);
    const int triggerID = mpItem->data(0, Qt::UserRole).toInt();
    TKey* pT = mpKeyUnit->getKey(triggerID);
    if (pT) {
        mPrevKey = pT->getKeyCode();
        mPrevModifier = pT->getKeyModifiers();
        pT->setKeyCode(mKey);
        pT->setKeyModifiers(mModifier);
    }
    mpKeysMainArea->lineEdit_key_binding->blockSignals(true);
    mpKeysMainArea->lineEdit_key_binding->setText(mKeyName);
    mpKeysMainArea->lineEdit_key_binding->blockSignals(false);
    setText(QObject::tr("Edit key modifer"));
}

AddActionCommand::AddActionCommand(QTreeWidgetItem* pItem, ActionUnit* actionUnit, TTreeWidget* treeWidget_actions, bool isFolder, QUndoCommand* parent)
{
    mpActionUnit = actionUnit;
    mpTreeWidgetActions = treeWidget_actions;
    mIsFolder = isFolder;
    mpItem = pItem;
}

void AddActionCommand::undo()
{
    if (!mpItem) {
        return;
    }
    mpParent = mpItem->parent();

    if (mpParent) {
        mpParent->removeChild(mpItem);
    } else {
        qDebug() << "parent is null ";
    }
}

void AddActionCommand::redo()
{
    if (!mpEditor) {
        return;
    }
    if (!mpItem) {
        mpEditor->addAction(mpItem);
        mpItem = mpTreeWidgetActions->currentItem();
        mpParent = mpItem->parent();
    } else {
        int count = mpParent->childCount();
        if (mIsFolder) {
            mpParent->addChild(mpItem);
        } else {
            mpParent->insertChild(count <= 0 ? 0 : count, mpItem);
        }
    }

    setText(QObject::tr("Add Action"));
}
DeleteActionCommand::DeleteActionCommand(QTreeWidgetItem* pItem, ActionUnit* actionUnit, TTreeWidget* treeWidget_actions, QUndoCommand* parent) : QUndoCommand(parent)
{
    mpItem = pItem;
    mpParent = mpItem->parent();
    mpActionUnit = actionUnit;
    mpTreeWidgetActions = treeWidget_actions;
}

void DeleteActionCommand::undo()
{
    if (!mpItem) {
        return;
    }

    TTrigger* pT = nullptr;
    if (mpParent) {
        const int childID = mpItemAction->getID();
        mpItem->setData(0, Qt::UserRole, childID);
        mpParent->insertChild(mpParent->childCount() <= 0 ? 0 : mpParent->childCount(), mpItem);
        mpTreeWidgetActions->setCurrentItem(mpItem);
    } else {
        qDebug() << "parent is null ";
    }
}

void DeleteActionCommand::redo()
{
    if (!mpEditor) {
        return;
    }
    if (!mpHost) {
        return;
    }
    if (mpItem) {
        const int ID = mpItem->data(0, Qt::UserRole).toInt();
        TAction* p = mpActionUnit->getAction(ID);
        TAction* ptr = new TAction(p->getName(), mpHost);
        ptr->registerAction();
        mpItemAction = ptr;
        mpEditor->delete_action();
    }
    setText(QObject::tr("Delete Action"));
}
MoveActionCommand::MoveActionCommand(ActionUnit* actionUnit,
                                     TTreeWidget* treeWidget_actions,
                                     int childID,
                                     int oldParentID,
                                     int newParentID,
                                     int parentPosition,
                                     int childPosition,
                                     int prevParentPosition,
                                     int prevChildPosition,
                                     QUndoCommand* parent)
: QUndoCommand(parent)
{
    mChildID = childID;
    mOldParentID = oldParentID;
    mNewParentID = newParentID;
    mParentPosition = parentPosition;
    mChildPosition = childPosition;
    mPrevParentPosition = prevParentPosition;
    mPrevChildPosition = prevChildPosition;
    mpActionUnit = actionUnit;
    mpTreeWidgetActions = treeWidget_actions;
}

void MoveActionCommand::undo()
{
    if (!mpHost) {
        return;
    }

    mpHost->getActionUnit()->reParentAction(mChildID, mNewParentID, mOldParentID, mPrevParentPosition, mPrevChildPosition);
    mpParentItem->removeChild(mpItem);
    mpPrevParentItem->insertChild(mpPrevParentItem->childCount() <= 0 ? 0 : mpPrevParentItem->childCount(), mpItem);
    mpTreeWidgetActions->setCurrentItem(mpItem);
    mpHost->getActionUnit()->updateToolbar();
}

void MoveActionCommand::redo()
{
    mpHost->getActionUnit()->reParentAction(mChildID, mOldParentID, mNewParentID, mParentPosition, mChildPosition);
    if (mpPrevParentItem) {
        mpPrevParentItem->removeChild(mpItem);
    }
    if (mpParentItem) {
        const int count = mpParentItem->childCount();
        mpParentItem->insertChild(count <= 0 ? 0 : count, mpItem);
    }
    mpHost->getActionUnit()->updateToolbar();

    setText(QObject::tr("Move Action"));
}

ActionNameTextEditedCommand::ActionNameTextEditedCommand(dlgActionMainArea* actionsMainArea, QUndoCommand* parent)
{
    mpActionsMainArea = actionsMainArea;
}

void ActionNameTextEditedCommand::undo()
{
    if (!mpItem) {
        return;
    }
    mpTreeWidgetActions->setCurrentItem(mpItem);
    mpActionsMainArea->lineEdit_action_name->blockSignals(true);
    mpActionsMainArea->lineEdit_action_name->setText(mPrevActionName);
    mpActionsMainArea->lineEdit_action_name->blockSignals(false);
}

void ActionNameTextEditedCommand::redo()
{
    if (!mpItem) {
        return;
    }
    mpTreeWidgetActions->setCurrentItem(mpItem);
    mpActionsMainArea->lineEdit_action_name->blockSignals(true);
    mpActionsMainArea->lineEdit_action_name->setText(mActionName);
    mpActionsMainArea->lineEdit_action_name->blockSignals(false);
    setText(QObject::tr("Edit action name"));
}

ActionButtonRotationEditedCommand::ActionButtonRotationEditedCommand(dlgActionMainArea* actionsMainArea, QUndoCommand* parent)
{
    mpActionsMainArea = actionsMainArea;
}

void ActionButtonRotationEditedCommand::undo()
{
    if (!mpItem) {
        return;
    }
    mpTreeWidgetActions->setCurrentItem(mpItem);
    mpActionsMainArea->comboBox_action_button_rotation->blockSignals(true);
    mpActionsMainArea->comboBox_action_button_rotation->setCurrentIndex(mPrevRotation);
    mpActionsMainArea->comboBox_action_button_rotation->blockSignals(false);
}

void ActionButtonRotationEditedCommand::redo()
{
    if (!mpItem) {
        return;
    }
    mpTreeWidgetActions->setCurrentItem(mpItem);
    mpActionsMainArea->comboBox_action_button_rotation->blockSignals(true);
    mpActionsMainArea->comboBox_action_button_rotation->setCurrentIndex(mRotation);
    mpActionsMainArea->comboBox_action_button_rotation->blockSignals(false);
    setText(QObject::tr("Edit action rotation"));
}

ActionButtonCheckboxEditedCommand::ActionButtonCheckboxEditedCommand(dlgActionMainArea* actionsMainArea, QUndoCommand* parent)
{
    mpActionsMainArea = actionsMainArea;
}

void ActionButtonCheckboxEditedCommand::undo()
{
    if (!mpItem) {
        return;
    }
    mpTreeWidgetActions->setCurrentItem(mpItem);
    mpActionsMainArea->checkBox_action_button_isPushDown->blockSignals(true);
    mpActionsMainArea->checkBox_action_button_isPushDown->setChecked(mPrevIsPushDown);
    mpActionsMainArea->checkBox_action_button_isPushDown->blockSignals(false);
    if (mPrevIsPushDown) {
        mpActionsMainArea->lineEdit_action_button_command_up->show();
        mpActionsMainArea->label_action_button_command_up->show();
        mpActionsMainArea->label_action_button_command_down->setText(mpEditor->tr("Command (down):"));
    } else {
        mpActionsMainArea->lineEdit_action_button_command_up->hide();
        mpActionsMainArea->label_action_button_command_up->hide();
        mpActionsMainArea->label_action_button_command_down->setText(mpEditor->tr("Command:"));
    }
}

void ActionButtonCheckboxEditedCommand::redo()
{
    if (!mpItem) {
        return;
    }
    mpTreeWidgetActions->setCurrentItem(mpItem);
    mpActionsMainArea->checkBox_action_button_isPushDown->blockSignals(true);
    mpActionsMainArea->checkBox_action_button_isPushDown->setChecked(mIsPushDown);
    mpActionsMainArea->checkBox_action_button_isPushDown->blockSignals(false);
    if (mIsPushDown) {
        mpActionsMainArea->lineEdit_action_button_command_up->show();
        mpActionsMainArea->label_action_button_command_up->show();
        mpActionsMainArea->label_action_button_command_down->setText(mpEditor->tr("Command (down):"));
    } else {
        mpActionsMainArea->lineEdit_action_button_command_up->hide();
        mpActionsMainArea->label_action_button_command_up->hide();
        mpActionsMainArea->label_action_button_command_down->setText(mpEditor->tr("Command:"));
    }
    setText(QObject::tr("Edit action pushdown"));
}

ActionCommandDownTextEditedCommand::ActionCommandDownTextEditedCommand(dlgActionMainArea* actionsMainArea, QUndoCommand* parent)
{
    mpActionsMainArea = actionsMainArea;
}

void ActionCommandDownTextEditedCommand::undo()
{
    if (!mpItem) {
        return;
    }
    mpTreeWidgetActions->setCurrentItem(mpItem);
    mpActionsMainArea->lineEdit_action_button_command_down->blockSignals(true);
    mpActionsMainArea->lineEdit_action_button_command_down->setText(mPrevCommandDown);
    mpActionsMainArea->lineEdit_action_button_command_down->blockSignals(false);
}

void ActionCommandDownTextEditedCommand::redo()
{
    if (!mpItem) {
        return;
    }
    mpTreeWidgetActions->setCurrentItem(mpItem);
    mpActionsMainArea->lineEdit_action_button_command_down->blockSignals(true);
    mpActionsMainArea->lineEdit_action_button_command_down->setText(mCommandDown);
    mpActionsMainArea->lineEdit_action_button_command_down->blockSignals(false);
    setText(QObject::tr("Edit action down"));
}

ActionCommandUpTextEditedCommand::ActionCommandUpTextEditedCommand(dlgActionMainArea* actionsMainArea, QUndoCommand* parent)
{
    mpActionsMainArea = actionsMainArea;
}

void ActionCommandUpTextEditedCommand::undo()
{
    if (!mpItem) {
        return;
    }
    mpTreeWidgetActions->setCurrentItem(mpItem);
    mpActionsMainArea->lineEdit_action_button_command_up->blockSignals(true);
    mpActionsMainArea->lineEdit_action_button_command_up->setText(mPrevCommandUp);
    mpActionsMainArea->lineEdit_action_button_command_up->blockSignals(false);
}

void ActionCommandUpTextEditedCommand::redo()
{
    if (!mpItem) {
        return;
    }
    mpTreeWidgetActions->setCurrentItem(mpItem);
    mpActionsMainArea->lineEdit_action_button_command_up->blockSignals(true);
    mpActionsMainArea->lineEdit_action_button_command_up->setText(mCommandUp);
    mpActionsMainArea->lineEdit_action_button_command_up->blockSignals(false);
    setText(QObject::tr("Edit action up"));
}

ActionCssTextEditedCommand::ActionCssTextEditedCommand(dlgActionMainArea* actionsMainArea, QUndoCommand* parent)
{
    mpActionsMainArea = actionsMainArea;
}

int ActionCssTextEditedCommand::id() const
{
    const short id = reinterpret_cast<uintptr_t>(typeid(this).name());
    return id;
}

bool ActionCssTextEditedCommand::mergeWith(const QUndoCommand *other)
{
    QString text = static_cast<const ActionCssTextEditedCommand*>(other)->mActionCss;
    if(text.length() < mActionCss.length())
    {
        mActionCss = text;
        return false;
    }
    QTextCursor cursor = mpActionsMainArea->plainTextEdit_action_css->textCursor();
    if(cursor.movePosition(QTextCursor::PreviousCharacter,QTextCursor::KeepAnchor))
    {
        QString selectedChar = cursor.selectedText();
        bool flag = text == mActionCss;
        if(selectedChar == QChar::ParagraphSeparator || selectedChar == QChar::Space || selectedChar == QChar::CarriageReturn || selectedChar == QChar::LineFeed)
        {
            return false;
        }
    }
    mActionCss = text;
    return true;
}

void ActionCssTextEditedCommand::undo()
{
    if (!mpItem) {
        return;
    }
    mpTreeWidgetActions->setCurrentItem(mpItem);
    mpActionsMainArea->plainTextEdit_action_css->blockSignals(true);
    QTextCursor cssCursor(mpActionsMainArea->plainTextEdit_action_css->textCursor());
    int curPosition = cssCursor.position();
    mpActionsMainArea->plainTextEdit_action_css->setPlainText(mPrevActionCss);
    cssCursor.setPosition(curPosition);
    mpActionsMainArea->plainTextEdit_action_css->setTextCursor(cssCursor);
    mpActionsMainArea->plainTextEdit_action_css->blockSignals(false);
}

void ActionCssTextEditedCommand::redo()
{
    if (!mpItem) {
        return;
    }
    mpTreeWidgetActions->setCurrentItem(mpItem);
    mpActionsMainArea->plainTextEdit_action_css->blockSignals(true);
    QTextCursor cssCursor(mpActionsMainArea->plainTextEdit_action_css->textCursor());
    int curPosition = cssCursor.position();
    mpActionsMainArea->plainTextEdit_action_css->setPlainText(mActionCss);
    cssCursor.setPosition(curPosition);
    mpActionsMainArea->plainTextEdit_action_css->setTextCursor(cssCursor);
    mpActionsMainArea->plainTextEdit_action_css->blockSignals(false);
    setText(QObject::tr("Edit action css"));
}

AddVarCommand::AddVarCommand(QTreeWidgetItem* pItem, VarUnit* varUnit, TTreeWidget* treeWidget_variables, bool isFolder, QUndoCommand* parent)
{
    mpVarUnit = varUnit;
    mpTreeWidgetVariables = treeWidget_variables;
    mIsFolder = isFolder;
    mpItem = pItem;
}

void AddVarCommand::undo()
{
    if (!mpItem) {
        return;
    }

    mpParent = mpItem->parent();

    if (mpParent) {
        mpParent->removeChild(mpItem);
    } else {
        qDebug() << "parent is null ";
    }
}

void AddVarCommand::redo()
{
    if (!mpEditor) {
        return;
    }
    if (!mpItem) {
        mpEditor->addVar(mIsFolder);
        mpItem = mpTreeWidgetVariables->currentItem();
        mpParent = mpItem->parent();
    } else {
        if (!mpParent) {
            if (mpItem->parent()) {
                mpParent = mpItem->parent();
            }
        }
        int count = mpParent->childCount();
        if (mIsFolder) {
            mpParent->addChild(mpItem);
        } else {
            mpParent->insertChild(count <= 0 ? 0 : count, mpItem);
        }
    }

    setText(QObject::tr("Add Variable"));
}
DeleteVarCommand::DeleteVarCommand(QTreeWidgetItem* pItem, VarUnit* varUnit, TTreeWidget* treeWidget_variables, QUndoCommand* parent) : QUndoCommand(parent)
{
    mpItem = pItem;
    mpParent = mpItem->parent();
    mpVarUnit = varUnit;
    mpTreeWidgetVariables = treeWidget_variables;
}

void DeleteVarCommand::undo()
{
    if (!mpItem) {
        return;
    }

    if (mpParent) {
        if (!mpTempVar) {
            mpTempVar = new TVar();
            *mpTempVar = *mpItemVar;
        }
        TVar* parent = mpTempVar->getParent();
        parent->addChild(mpTempVar);
        mpVarUnit->addTreeItem(mpItem, mpTempVar);

        mpParent->addChild(mpItem);
        QList<QTreeWidgetItem*> list;
        mpEditor->recurseVariablesDown(mpItem, list);
        for (auto& treeWidgetItem : list) {
            TVar* v = mpVarUnit->getWVar(treeWidgetItem);
            TVar* vparent = v->getParent();
            const void* pval = vparent->pValue;
            if (v->getParent()->hidden) {
                v->setParent(mpTempVar);
            }
        }
        mpTreeWidgetVariables->setCurrentItem(mpItem);
    } else {
        qDebug() << "parent is null ";
    }
}

void DeleteVarCommand::redo()
{
    if (!mpEditor) {
        return;
    }
    if (!mpHost) {
        return;
    }
    if (mpItem) {
        mpTreeWidgetVariables->setCurrentItem(mpItem);
        if (!mpItemVar) {
            mpItemVar = new TVar();
            TVar* p = mpVarUnit->getWVar(mpItem);
            *mpItemVar = *p;
        }
        mpEditor->delete_variable();
        mpTempVar = nullptr;
    }
    setText(QObject::tr("Delete Variable"));
}
MoveVariableCommand::MoveVariableCommand(
        VarUnit* varUnit, TTreeWidget* treeWidget_variables, QTreeWidgetItem* parentItem, QTreeWidgetItem* cItem, QTreeWidgetItem* prevParentItem, QUndoCommand* parent)
: QUndoCommand(parent)
{
    mpVarUnit = varUnit;
    mpTreeWidgetVariables = treeWidget_variables;
    mpParentItem = parentItem;
    mpItem = cItem;
    mpPrevParentItem = prevParentItem;
}

void MoveVariableCommand::undo()
{
    if (!mpHost) {
        return;
    }
    LuaInterface* lI = mpHost->getLuaInterface();
    VarUnit* varUnit = lI->getVarUnit();
    if (!lI->reparentVariable(mpPrevParentItem, mpItem, mpParentItem)) {
        mpEvent->setDropAction(Qt::IgnoreAction);
        mpEvent->ignore();
    }
    mpParentItem->removeChild(mpItem);
    mpPrevParentItem->insertChild(mpPrevParentItem->childCount() <= 0 ? 0 : mpPrevParentItem->childCount(), mpItem);
    mpTreeWidgetVariables->setCurrentItem(mpItem);
}

void MoveVariableCommand::redo()
{
    LuaInterface* lI = mpHost->getLuaInterface();
    VarUnit* varUnit = lI->getVarUnit();
    if (!lI->reparentVariable(mpParentItem, mpItem, mpPrevParentItem)) {
        mpEvent->setDropAction(Qt::IgnoreAction);
        mpEvent->ignore();
    }
    if (mpPrevParentItem) {
        mpPrevParentItem->removeChild(mpItem);
    }
    if (mpParentItem) {
        const int count = mpParentItem->childCount();
        mpParentItem->insertChild(count <= 0 ? 0 : count, mpItem);
    }
    setText(QObject::tr("Move Variable"));
}

TriggerNameTextEditedCommand::TriggerNameTextEditedCommand(dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent): QUndoCommand(parent)
{
    mpTriggersMainArea = triggersMainArea;
}

void TriggerNameTextEditedCommand::undo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    mpTreeWidgetTriggers->setCurrentItem(mpItem);
    // mpEditor->slot_triggerSelected(mpItem);
    mpTriggersMainArea->lineEdit_trigger_name->setText(mPrevLineEditTriggerName);
}

void TriggerNameTextEditedCommand::redo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    mpTreeWidgetTriggers->setCurrentItem(mpItem);
    // mpEditor->slot_triggerSelected(mpItem);
    mpTriggersMainArea->lineEdit_trigger_name->setText(mLineEditTriggerName);
    setText(QObject::tr("Edit trigger name"));
}

TriggerCommandTextEditedCommand::TriggerCommandTextEditedCommand(dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent): QUndoCommand(parent)
{
    mpTriggersMainArea = triggersMainArea;
}

void TriggerCommandTextEditedCommand::undo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    mpTreeWidgetTriggers->setCurrentItem(mpItem);
    mpEditor->slot_triggerSelected(mpItem);
    mpTriggersMainArea->lineEdit_trigger_command->setText(mPrevLineEditTriggerCommand);
}

void TriggerCommandTextEditedCommand::redo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    mpTreeWidgetTriggers->setCurrentItem(mpItem);
    mpEditor->slot_triggerSelected(mpItem);
    mpTriggersMainArea->lineEdit_trigger_command->setText(mLineEditTriggerCommand);
    setText(QObject::tr("Edit trigger command"));
}

TriggerFireLengthEditedCommand::TriggerFireLengthEditedCommand(dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent): QUndoCommand(parent)
{
    mpTriggersMainArea = triggersMainArea;
}

void TriggerFireLengthEditedCommand::undo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    mpTreeWidgetTriggers->setCurrentItem(mpItem);
    mpEditor->slot_triggerSelected(mpItem);
    mpTriggersMainArea->spinBox_stayOpen->blockSignals(true);
    mpTriggersMainArea->spinBox_stayOpen->setValue(mPrevFireLength);
    mpTriggersMainArea->spinBox_stayOpen->blockSignals(false);
}

void TriggerFireLengthEditedCommand::redo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    mpTreeWidgetTriggers->setCurrentItem(mpItem);
    mpEditor->slot_triggerSelected(mpItem);
    mpTriggersMainArea->spinBox_stayOpen->blockSignals(true);
    mpTriggersMainArea->spinBox_stayOpen->setValue(mFireLength);
    mpTriggersMainArea->spinBox_stayOpen->blockSignals(false);
    setText(QObject::tr("Edit fire length"));
}

TriggerPlaySoundEditedCommand::TriggerPlaySoundEditedCommand(dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent): QUndoCommand(parent)
{
    mpTriggersMainArea = triggersMainArea;
}

void TriggerPlaySoundEditedCommand::undo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    mpTreeWidgetTriggers->setCurrentItem(mpItem);
    mpEditor->slot_triggerSelected(mpItem);
    mpTriggersMainArea->groupBox_soundTrigger->blockSignals(true);
    mpTriggersMainArea->groupBox_soundTrigger->setChecked(mPrevGroupBoxSoundTrigger);
    mpTriggersMainArea->groupBox_soundTrigger->blockSignals(false);
}

void TriggerPlaySoundEditedCommand::redo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    mpTreeWidgetTriggers->setCurrentItem(mpItem);
    mpEditor->slot_triggerSelected(mpItem);
    mpTriggersMainArea->groupBox_soundTrigger->blockSignals(true);
    mpTriggersMainArea->groupBox_soundTrigger->setChecked(mGroupBoxSoundTrigger);
    mpTriggersMainArea->groupBox_soundTrigger->blockSignals(false);
    setText(QObject::tr("Edit play sound"));
}

TriggerPlaySoundFileEditedCommand::TriggerPlaySoundFileEditedCommand(dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent): QUndoCommand(parent)
{
    mpTriggersMainArea = triggersMainArea;
}

void TriggerPlaySoundFileEditedCommand::undo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    mpTreeWidgetTriggers->setCurrentItem(mpItem);
    mpEditor->slot_triggerSelected(mpItem);
    mpTriggersMainArea->lineEdit_soundFile->blockSignals(true);
    mpTriggersMainArea->lineEdit_soundFile->setText(mPrevLineEditSoundFile);
    mpTriggersMainArea->lineEdit_soundFile->blockSignals(false);
}

void TriggerPlaySoundFileEditedCommand::redo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    mpTreeWidgetTriggers->setCurrentItem(mpItem);
    mpEditor->slot_triggerSelected(mpItem);
    mpTriggersMainArea->lineEdit_soundFile->blockSignals(true);
    mpTriggersMainArea->lineEdit_soundFile->setText(mLineEditSoundFile);
    mpTriggersMainArea->lineEdit_soundFile->blockSignals(false);
    setText(QObject::tr("Edit play sound file"));
}

TriggerColorizerEditedCommand::TriggerColorizerEditedCommand(dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent): QUndoCommand(parent)
{
    mpTriggersMainArea = triggersMainArea;
}

void TriggerColorizerEditedCommand::undo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    mpTreeWidgetTriggers->setCurrentItem(mpItem);
    mpEditor->slot_triggerSelected(mpItem);
    mpTriggersMainArea->groupBox_triggerColorizer->blockSignals(true);
    mpTriggersMainArea->groupBox_triggerColorizer->setChecked(mPrevBoxTriggerColorizer);
    mpTriggersMainArea->groupBox_triggerColorizer->blockSignals(false);
}

void TriggerColorizerEditedCommand::redo()
{
    mpTriggersMainArea->groupBox_triggerColorizer->blockSignals(true);
    mpTriggersMainArea->groupBox_triggerColorizer->setChecked(mBoxTriggerColorizer);
    mpTriggersMainArea->groupBox_triggerColorizer->blockSignals(false);
    setText(QObject::tr("Edit trigger colorizer"));
}

TriggerColorizerBgColorEditedCommand::TriggerColorizerBgColorEditedCommand(dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent): QUndoCommand(parent)
{
    mpTriggersMainArea = triggersMainArea;
}

void TriggerColorizerBgColorEditedCommand::undo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    mpTreeWidgetTriggers->setCurrentItem(mpItem);
    mpEditor->slot_triggerSelected(mpItem);
    mpTriggersMainArea->pushButtonBgColor->blockSignals(true);
    mpTriggersMainArea->pushButtonBgColor->setProperty("baseColor", mPrevbgColor);
    const bool keepColor = mPrevbgColor == QColorConstants::Transparent.name();
    mpTriggersMainArea->pushButtonBgColor->setText(keepColor ? mpEditor->tr("keep") : QString());
    mpTriggersMainArea->pushButtonBgColor->setStyleSheet(mpEditor->generateButtonStyleSheet(mPrevbgColor));
    mpTriggersMainArea->pushButtonBgColor->blockSignals(false);
}

void TriggerColorizerBgColorEditedCommand::redo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    mpTreeWidgetTriggers->setCurrentItem(mpItem);
    mpEditor->slot_triggerSelected(mpItem);
    mpTriggersMainArea->pushButtonBgColor->blockSignals(true);
    mpTriggersMainArea->pushButtonBgColor->setProperty("baseColor", mBgColor);
    const bool keepColor = mBgColor == QColorConstants::Transparent.name();
    mpTriggersMainArea->pushButtonBgColor->setText(keepColor ? mpEditor->tr("keep") : QString());
    mpTriggersMainArea->pushButtonBgColor->setStyleSheet(mpEditor->generateButtonStyleSheet(mBgColor));
    mpTriggersMainArea->pushButtonBgColor->blockSignals(false);
    setText(QObject::tr("Edit trigger bg color"));
}

TriggerColorizerFgColorEditedCommand::TriggerColorizerFgColorEditedCommand(dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent): QUndoCommand(parent)
{
    mpTriggersMainArea = triggersMainArea;
}

void TriggerColorizerFgColorEditedCommand::undo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    mpTreeWidgetTriggers->setCurrentItem(mpItem);
    mpEditor->slot_triggerSelected(mpItem);
    mpTriggersMainArea->pushButtonFgColor->blockSignals(true);
    mpTriggersMainArea->pushButtonFgColor->setProperty("baseColor", mPrevfgColor);
    const bool keepColor = mPrevfgColor == QColorConstants::Transparent.name();
    mpTriggersMainArea->pushButtonFgColor->setText(keepColor ? mpEditor->tr("keep") : QString());
    mpTriggersMainArea->pushButtonFgColor->setStyleSheet(mpEditor->generateButtonStyleSheet(mPrevfgColor));
    mpTriggersMainArea->pushButtonFgColor->blockSignals(false);
}

void TriggerColorizerFgColorEditedCommand::redo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    mpTreeWidgetTriggers->setCurrentItem(mpItem);
    mpEditor->slot_triggerSelected(mpItem);
    mpTriggersMainArea->pushButtonFgColor->blockSignals(true);
    mpTriggersMainArea->pushButtonFgColor->setProperty("baseColor", mFgColor);
    const bool keepColor = mFgColor == QColorConstants::Transparent.name();
    mpTriggersMainArea->pushButtonFgColor->setText(keepColor ? mpEditor->tr("keep") : QString());
    mpTriggersMainArea->pushButtonFgColor->setStyleSheet(mpEditor->generateButtonStyleSheet(mFgColor));
    mpTriggersMainArea->pushButtonFgColor->blockSignals(false);
    setText(QObject::tr("Edit trigger fg color"));
}

TriggerPerlSlashGOptionEditedCommand::TriggerPerlSlashGOptionEditedCommand(dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent): QUndoCommand(parent)
{
    mpTriggersMainArea = triggersMainArea;
}

void TriggerPerlSlashGOptionEditedCommand::undo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    mpTreeWidgetTriggers->setCurrentItem(mpItem);
    mpEditor->slot_triggerSelected(mpItem);
    mpTriggersMainArea->groupBox_perlSlashGOption->blockSignals(true);
    mpTriggersMainArea->groupBox_perlSlashGOption->setChecked(mPrevPerlSlashGOption);
    mpTriggersMainArea->groupBox_perlSlashGOption->blockSignals(false);
}

void TriggerPerlSlashGOptionEditedCommand::redo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    mpTreeWidgetTriggers->setCurrentItem(mpItem);
    mpEditor->slot_triggerSelected(mpItem);
    mpTriggersMainArea->groupBox_perlSlashGOption->blockSignals(true);
    mpTriggersMainArea->groupBox_perlSlashGOption->setChecked(mPerlSlashGOption);
    mpTriggersMainArea->groupBox_perlSlashGOption->blockSignals(false);
    setText(QObject::tr("Edit Perl Option"));
}

TriggerGroupFilterEditedCommand::TriggerGroupFilterEditedCommand(dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent): QUndoCommand(parent)
{
    mpTriggersMainArea = triggersMainArea;
}

void TriggerGroupFilterEditedCommand::undo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    mpTreeWidgetTriggers->setCurrentItem(mpItem);
    mpEditor->slot_triggerSelected(mpItem);
    mpTriggersMainArea->groupBox_filterTrigger->blockSignals(true);
    mpTriggersMainArea->groupBox_filterTrigger->setChecked(mPrevFilterTrigger);
    mpTriggersMainArea->groupBox_filterTrigger->blockSignals(false);
}

void TriggerGroupFilterEditedCommand::redo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    mpTreeWidgetTriggers->setCurrentItem(mpItem);
    mpEditor->slot_triggerSelected(mpItem);
    mpTriggersMainArea->groupBox_filterTrigger->blockSignals(true);
    mpTriggersMainArea->groupBox_filterTrigger->setChecked(mFilterTrigger);
    mpTriggersMainArea->groupBox_filterTrigger->blockSignals(false);
    setText(QObject::tr("Edit filter trigger"));
}

TriggerMultiLineEditedCommand::TriggerMultiLineEditedCommand(dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent): QUndoCommand(parent)
{
    mpTriggersMainArea = triggersMainArea;
}

void TriggerMultiLineEditedCommand::undo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    mpTreeWidgetTriggers->setCurrentItem(mpItem);
    mpEditor->slot_triggerSelected(mpItem);
    mpTriggersMainArea->groupBox_multiLineTrigger->blockSignals(true);
    mpTriggersMainArea->groupBox_multiLineTrigger->setChecked(mPrevMultiLineTrigger);
    mpTriggersMainArea->groupBox_multiLineTrigger->blockSignals(false);
}

void TriggerMultiLineEditedCommand::redo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    mpTreeWidgetTriggers->setCurrentItem(mpItem);
    mpEditor->slot_triggerSelected(mpItem);
    mpTriggersMainArea->groupBox_multiLineTrigger->blockSignals(true);
    mpTriggersMainArea->groupBox_multiLineTrigger->setChecked(mMultiLineTrigger);
    mpTriggersMainArea->groupBox_multiLineTrigger->blockSignals(false);
    setText(QObject::tr("Edit multiline trigger"));
}

TriggerLineMarginEditedCommand::TriggerLineMarginEditedCommand(dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent): QUndoCommand(parent)
{
    mpTriggersMainArea = triggersMainArea;
}

void TriggerLineMarginEditedCommand::undo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    mpTreeWidgetTriggers->setCurrentItem(mpItem);
    mpEditor->slot_triggerSelected(mpItem);
    mpTriggersMainArea->spinBox_lineMargin->blockSignals(true);
    mpTriggersMainArea->spinBox_lineMargin->setValue(mPrevLineMargin);
    mpTriggersMainArea->spinBox_lineMargin->blockSignals(false);
}

void TriggerLineMarginEditedCommand::redo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    mpTreeWidgetTriggers->setCurrentItem(mpItem);
    mpEditor->slot_triggerSelected(mpItem);
    mpTriggersMainArea->spinBox_lineMargin->blockSignals(true);
    mpTriggersMainArea->spinBox_lineMargin->setValue(mLineMargin);
    mpTriggersMainArea->spinBox_lineMargin->blockSignals(false);
    setText(QObject::tr("Edit line margin"));
}

TriggerLineEditPatternItemEditedCommand::TriggerLineEditPatternItemEditedCommand(dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent): QUndoCommand(parent)
{
    mpTriggersMainArea = triggersMainArea;
}

void TriggerLineEditPatternItemEditedCommand::undo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    mpTreeWidgetTriggers->setCurrentItem(mpItem);
    // mpEditor->slot_triggerSelected(mpItem);
    pBox->blockSignals(true);
    pBox->setCurrentIndex(mPrevTriggerPatternEdit);
    pBox->blockSignals(false);
    pBox->currentIndexChanged(mPrevTriggerPatternEdit);
    mpEditor->slot_saveSelectedItem(mpItem);
}

void TriggerLineEditPatternItemEditedCommand::redo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    mpTreeWidgetTriggers->setCurrentItem(mpItem);
    // mpEditor->slot_triggerSelected(mpItem);
    const int ID = mpItem->data(0, Qt::UserRole).toInt();
    TTrigger* pT = mpTriggerUnit->getTrigger(ID);
    pBox->blockSignals(true);
    pBox->setCurrentIndex(mTriggerPatternEdit);
    pBox->blockSignals(false);
    pBox->currentIndexChanged(mTriggerPatternEdit);
    mpEditor->slot_saveSelectedItem(mpItem);
    setText(QObject::tr("Edit line pattern"));
}

TriggerLineEditPatternEditedCommand::TriggerLineEditPatternEditedCommand(dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent): QUndoCommand(parent)
{
    mpTriggersMainArea = triggersMainArea;
}

void TriggerLineEditPatternEditedCommand::undo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    mpTreeWidgetTriggers->setCurrentItem(mpItem);
    mpTriggerPattern->lineEdit_pattern->blockSignals(true);
    // mpEditor->slot_triggerSelected(mpItem);
    mpTriggerPattern->lineEdit_pattern->setText(mPrevLineEditTriggerPattern);
    mpTriggerPattern->lineEdit_pattern->blockSignals(false);

    if (!mPrevLineEditTriggerPattern.isEmpty()) {
        dlgTriggerPatternEdit* pPatternItem = mpTriggerPatternEdit[mRow];
        pPatternItem->lineEdit_pattern->setEnabled(true);
    }
    dlgTriggerPatternEdit* pPatternItem = mpTriggerPatternEdit[mRow + 1];
    pPatternItem->lineEdit_pattern->blockSignals(true);
    pPatternItem->lineEdit_pattern->setEnabled(false);
    pPatternItem->lineEdit_pattern->blockSignals(false);
}

void TriggerLineEditPatternEditedCommand::redo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    mpTreeWidgetTriggers->setCurrentItem(mpItem);
    mpTriggerPattern->lineEdit_pattern->blockSignals(true);
    // mpEditor->slot_triggerSelected(mpItem);
    mpTriggerPattern->lineEdit_pattern->setText(mLineEditTriggerPattern);

    if (!mLineEditTriggerPattern.isEmpty()) {
        dlgTriggerPatternEdit* pPatternItem = mpTriggerPatternEdit[mRow];
        pPatternItem->lineEdit_pattern->setEnabled(true);
    }
    dlgTriggerPatternEdit* pPatternItem = mpTriggerPatternEdit[mRow + 1];
    pPatternItem->lineEdit_pattern->blockSignals(true);
    pPatternItem->lineEdit_pattern->setEnabled(true);
    pPatternItem->lineEdit_pattern->blockSignals(false);
    setText(QObject::tr("Edit trigger pattern"));
}

TriggerLineSpacerEditedCommand::TriggerLineSpacerEditedCommand(dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent): QUndoCommand(parent)
{
    mpTriggersMainArea = triggersMainArea;
}

void TriggerLineSpacerEditedCommand::undo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    mpTreeWidgetTriggers->setCurrentItem(mpItem);
    mpEditor->slot_triggerSelected(mpItem);
    mpPatternItem->spinBox_lineSpacer->blockSignals(true);
    mpPatternItem->spinBox_lineSpacer->setValue(mPrevLineSpacer);
    mpPatternItem->spinBox_lineSpacer->blockSignals(false);
}

void TriggerLineSpacerEditedCommand::redo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    mpTreeWidgetTriggers->setCurrentItem(mpItem);
    mpEditor->slot_triggerSelected(mpItem);
    mpPatternItem->spinBox_lineSpacer->blockSignals(true);
    mpPatternItem->spinBox_lineSpacer->setValue(mLineSpacer);
    mpPatternItem->spinBox_lineSpacer->blockSignals(false);
    setText(QObject::tr("Edit line spacer"));
}

TriggerColorFGEditedCommand::TriggerColorFGEditedCommand(dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent): QUndoCommand(parent)
{
    mpTriggersMainArea = triggersMainArea;
}

void TriggerColorFGEditedCommand::undo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    mpTreeWidgetTriggers->setCurrentItem(mpItem);
    // mpEditor->slot_triggerSelected(mpItem);
    const int triggerID = mpItem->data(0, Qt::UserRole).toInt();
    TTrigger* pT = mpTriggerUnit->getTrigger(triggerID);
    if (!pT) {
        return;
    }

    QString styleSheet;
    if (mPrevColorTriggerFgColor.isValid()) {
        styleSheet = mpEditor->generateButtonStyleSheet(mPrevColorTriggerFgColor);
    }
    mpPushButton->setStyleSheet(styleSheet);
    qDebug() << mpPatternItem->mRow;
    qDebug() << mpPatternItem->lineEdit_pattern->text();

    mpPatternItem->lineEdit_pattern->setText(TTrigger::createColorPatternText(pT->mColorTriggerFgAnsi, pT->mColorTriggerBgAnsi));

    if (pT->mColorTriggerFgAnsi == TTrigger::scmIgnored) {
        //: Color trigger ignored foreground color button, ensure all three instances have the same text
        mpPushButton->setText(mpEditor->tr("Foreground color ignored"));
    } else if (pT->mColorTriggerFgAnsi == TTrigger::scmDefault) {
        //: Color trigger default foreground color button, ensure all three instances have the same text
        mpPushButton->setText(mpEditor->tr("Default foreground color"));
    } else {
        //: Color trigger ANSI foreground color button, ensure all three instances have the same text
        mpPushButton->setText(mpEditor->tr("Foreground color [ANSI %1]").arg(QString::number(pT->mColorTriggerFgAnsi)));
    }
    dlgTriggerPatternEdit* pPatternItem = mpTriggerPatternEdit[mRow + 1];
    pPatternItem->lineEdit_pattern->blockSignals(true);
    pPatternItem->lineEdit_pattern->setEnabled(false);
    pPatternItem->lineEdit_pattern->blockSignals(false);
}

void TriggerColorFGEditedCommand::redo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    mpTreeWidgetTriggers->setCurrentItem(mpItem);
    // mpEditor->slot_triggerSelected(mpItem);
    const int triggerID = mpItem->data(0, Qt::UserRole).toInt();
    TTrigger* pT = mpTriggerUnit->getTrigger(triggerID);
    if (!pT) {
        return;
    }

    QString styleSheet;
    if (mColorTriggerFgColor.isValid()) {
        styleSheet = mpEditor->generateButtonStyleSheet(mColorTriggerFgColor);
    }
    mpPushButton->setStyleSheet(styleSheet);
    qDebug() << mpPatternItem->mRow;
    mpPatternItem->lineEdit_pattern->setText(TTrigger::createColorPatternText(pT->mColorTriggerFgAnsi, pT->mColorTriggerBgAnsi));

    if (pT->mColorTriggerFgAnsi == TTrigger::scmIgnored) {
        //: Color trigger ignored foreground color button, ensure all three instances have the same text
        mpPushButton->setText(mpEditor->tr("Foreground color ignored"));
    } else if (pT->mColorTriggerFgAnsi == TTrigger::scmDefault) {
        //: Color trigger default foreground color button, ensure all three instances have the same text
        mpPushButton->setText(mpEditor->tr("Default foreground color"));
    } else {
        //: Color trigger ANSI foreground color button, ensure all three instances have the same text
        mpPushButton->setText(mpEditor->tr("Foreground color [ANSI %1]").arg(QString::number(pT->mColorTriggerFgAnsi)));
    }
    // if (!mLineEdit_trigger_pattern.isEmpty()) {
    //     dlgTriggerPatternEdit* pPatternItem = mpTriggerPatternEdit[mRow];
    //     pPatternItem->lineEdit_pattern->setEnabled(true);
    // }
    dlgTriggerPatternEdit* pPatternItem = mpTriggerPatternEdit[mRow + 1];
    pPatternItem->lineEdit_pattern->blockSignals(true);
    pPatternItem->lineEdit_pattern->setEnabled(true);
    pPatternItem->lineEdit_pattern->blockSignals(false);
    setText(QObject::tr("Edit FG Color"));
}

TriggerColorBGEditedCommand::TriggerColorBGEditedCommand(dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent): QUndoCommand(parent)
{
    mpTriggersMainArea = triggersMainArea;
}

void TriggerColorBGEditedCommand::undo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    mpTreeWidgetTriggers->setCurrentItem(mpItem);
    mpEditor->slot_triggerSelected(mpItem);
    const int triggerID = mpItem->data(0, Qt::UserRole).toInt();
    TTrigger* pT = mpTriggerUnit->getTrigger(triggerID);
    if (!pT) {
        return;
    }

    QString styleSheet;
    if (mPrevColorTriggerBgColor.isValid()) {
        styleSheet = mpEditor->generateButtonStyleSheet(mPrevColorTriggerBgColor);
    }
    mpPushButton->setStyleSheet(styleSheet);

    mpPatternItem->lineEdit_pattern->setText(TTrigger::createColorPatternText(pT->mColorTriggerFgAnsi, pT->mColorTriggerBgAnsi));

    if (pT->mColorTriggerBgAnsi == TTrigger::scmIgnored) {
        //: Color trigger ignored background color button, ensure all three instances have the same text
        mpPushButton->setText(mpEditor->tr("Background color ignored"));
    } else if (pT->mColorTriggerBgAnsi == TTrigger::scmDefault) {
        //: Color trigger default background color button, ensure all three instances have the same text
        mpPushButton->setText(mpEditor->tr("Default background color"));
    } else {
        //: Color trigger ANSI background color button, ensure all three instances have the same text
        mpPushButton->setText(mpEditor->tr("Background color [ANSI %1]").arg(QString::number(pT->mColorTriggerBgAnsi)));
    }
}

void TriggerColorBGEditedCommand::redo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    mpTreeWidgetTriggers->setCurrentItem(mpItem);
    mpEditor->slot_triggerSelected(mpItem);
    const int triggerID = mpItem->data(0, Qt::UserRole).toInt();
    TTrigger* pT = mpTriggerUnit->getTrigger(triggerID);
    if (!pT) {
        return;
    }

    QString styleSheet;
    if (mColorTriggerBgColor.isValid()) {
        styleSheet = mpEditor->generateButtonStyleSheet(mColorTriggerBgColor);
    }
    mpPushButton->setStyleSheet(styleSheet);

    mpPatternItem->lineEdit_pattern->setText(TTrigger::createColorPatternText(pT->mColorTriggerFgAnsi, pT->mColorTriggerBgAnsi));

    if (pT->mColorTriggerBgAnsi == TTrigger::scmIgnored) {
        //: Color trigger ignored background color button, ensure all three instances have the same text
        mpPushButton->setText(mpEditor->tr("Background color ignored"));
    } else if (pT->mColorTriggerBgAnsi == TTrigger::scmDefault) {
        //: Color trigger default background color button, ensure all three instances have the same text
        mpPushButton->setText(mpEditor->tr("Default background color"));
    } else {
        //: Color trigger ANSI background color button, ensure all three instances have the same text
        mpPushButton->setText(mpEditor->tr("Background color [ANSI %1]").arg(QString::number(pT->mColorTriggerBgAnsi)));
    }
    setText(QObject::tr("Edit BG Color"));
}
