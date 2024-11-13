#include "dlgTriggerEditorCommand.h"
#include "Host.h"
#include "LuaInterface.h"
#include "VarUnit.h"
#include "XMLimport.h"
#include "dlgActionMainArea.h"
#include "dlgAliasMainArea.h"
#include "dlgKeysMainArea.h"
#include "dlgScriptsMainArea.h"
#include "dlgTriggerEditor.h"
#include "dlgTriggerPatternEdit.h"
#include "mudlet.h"
#include <QPointer>

AddTriggerCommand::AddTriggerCommand(Host* pHost, TTreeWidget* treeWidgetTriggers, bool isFolder, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
{
    mpTreeWidgetTriggers = treeWidgetTriggers;
    mIsFolder = isFolder;
}

void AddTriggerCommand::undo()
{
    if (!mpItem) {
        return;
    }

    int tID = mpItemTrigger->getID();
    mpEditor->selectTriggerByID(tID);
    mpItem = mpTreeWidgetTriggers->currentItem();

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
        const int id = mpTreeWidgetTriggers->currentItem()->data(0, Qt::UserRole).toInt();
        TTrigger* pT = mpHost->getTriggerUnit()->getTrigger(id);
        mpItemTrigger = pT;
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

PasteTriggerCommand::PasteTriggerCommand(Host* pHost, TTreeWidget* treeWidgetTriggers, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
{
    mpTreeWidgetTriggers = treeWidgetTriggers;
    mpItemTrigger = nullptr;
}

void PasteTriggerCommand::undo()
{
    if (!mpEditor) {
        return;
    }

    if(!mImportedItemID)
    {
        return;
    }
    mpEditor->selectTriggerByID(mImportedItemID);
    mpItem = mpTreeWidgetTriggers->currentItem();

    mpParent = mpItem->parent();
    mpParent->removeChild(mpItem);

}

void PasteTriggerCommand::redo()
{
    if(!mpItem)
    {
        auto parent = mpTreeWidgetTriggers->currentIndex().parent();
        auto parentRow = parent.row();
        auto parentId = parent.data(Qt::UserRole).toInt();
        const int siblingRow = mpTreeWidgetTriggers->currentIndex().row() + 1;
        mpHost->getTriggerUnit()->reParentTrigger(mImportedItemID, 0, parentId, parentRow, siblingRow);
    }
    else
    {
        int count = mpParent->childCount();
        mpParent->insertChild(count <= 0 ? 0 : count, mpItem);
    }

    mpEditor->selectTriggerByID(mImportedItemID);
    mpItem = mpTreeWidgetTriggers->currentItem();

    setText(QObject::tr("Paste trigger"));
}

DeleteTriggerCommand::DeleteTriggerCommand(Host* pHost, QTreeWidgetItem* pItem, TTreeWidget* treeWidgetTriggers, QUndoCommand* parent)
    : QUndoCommand(parent), mpHost(pHost)
{
    mpItem = pItem;
    mpParent = mpItem->parent();
    mpTreeWidgetTriggers = treeWidgetTriggers;
}

void DeleteTriggerCommand::undo()
{
    if (!mpItem) {
        return;
    }

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
        TTrigger* p = mpHost->getTriggerUnit()->getTrigger(ID);
        TTrigger* ptr = new TTrigger(p->mName, p->mPatterns, p->getRegexCodePropertyList(), mpHost);
        ptr->registerTrigger();
        mpItemTrigger = ptr;
        mpEditor->delete_trigger();
    }
    setText(QObject::tr("Delete trigger"));
}

MoveTriggerCommand::MoveTriggerCommand(Host* pHost,
                                       TTreeWidget* treeWidgetTriggers,
                                       int childID,
                                       int oldParentID,
                                       int newParentID,
                                       int parentPosition,
                                       int childPosition,
                                       int prevParentPosition,
                                       int prevChildPosition,
                                       QUndoCommand* parent)
: QUndoCommand(parent), mpHost(pHost)
{
    mChildID = childID;
    mOldParentID = oldParentID;
    mNewParentID = newParentID;
    mParentPosition = parentPosition;
    mChildPosition = childPosition;
    mPrevParentPosition = prevParentPosition;
    mPrevChildPosition = prevChildPosition;
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

    setText(QObject::tr("Move trigger"));
}

AddAliasCommand::AddAliasCommand(Host* pHost, TTreeWidget* treeWidget_aliases, bool isFolder, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
{
    mpTreeWidgetAliases = treeWidget_aliases;
    mIsFolder = isFolder;
}

void AddAliasCommand::undo()
{
    if (!mpItem) {
        return;
    }
    int id = mpItemAlias->getID();
    mpEditor->selectAliasByID(id);
    mpItem = mpTreeWidgetAliases->currentItem();

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
        const int id = mpTreeWidgetAliases->currentItem()->data(0, Qt::UserRole).toInt();
        TAlias* pT = mpHost->getAliasUnit()->getAlias(id);
        mpItemAlias = pT;
    } else {
        int count = mpParent->childCount();
        if (mIsFolder) {
            mpParent->addChild(mpItem);
        } else {
            mpParent->insertChild(count <= 0 ? 0 : count, mpItem);
        }
    }

    setText(QObject::tr("Add alias"));
}

PasteAliasCommand::PasteAliasCommand(Host* pHost, TTreeWidget* treeWidgetAliases, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
{
    mpTreeWidgetAliases = treeWidgetAliases;
}

void PasteAliasCommand::undo()
{
    if (!mpEditor) {
        return;
    }

    if(!mImportedItemID)
    {
        return;
    }
    mpEditor->selectAliasByID(mImportedItemID);
    mpItem = mpTreeWidgetAliases->currentItem();
    mpParent = mpItem->parent();
    mpParent->removeChild(mpItem);
}

void PasteAliasCommand::redo()
{
    if (!mpItem) {
        auto parent = mpTreeWidgetAliases->currentIndex().parent();
        auto parentRow = parent.row();
        auto parentId = parent.data(Qt::UserRole).toInt();

        const int siblingRow = mpTreeWidgetAliases->currentIndex().row() + 1;
        mpHost->getAliasUnit()->reParentAlias(mImportedItemID, 0, parentId, parentRow, siblingRow);
    }
    else
    {
        int count = mpParent->childCount();
        mpParent->insertChild(count <= 0 ? 0 : count, mpItem);
    }

    mpEditor->selectAliasByID(mImportedItemID);
    mpItem = mpTreeWidgetAliases->currentItem();
    setText(QObject::tr("Paste alias"));
}

DeleteAliasCommand::DeleteAliasCommand(Host* pHost, QTreeWidgetItem* pItem, TTreeWidget* treeWidget_aliases, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
{
    mpItem = pItem;
    mpParent = mpItem->parent();
    mpTreeWidgetAliases = treeWidget_aliases;
}

void DeleteAliasCommand::undo()
{
    if (!mpItem) {
        return;
    }

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
        TAlias* p = mpHost->getAliasUnit()->getAlias(ID);
        TAlias* ptr = new TAlias(p->mName, mpHost);
        ptr->registerAlias();
        mpItemAlias = ptr;
        mpEditor->delete_alias();
    }
    setText(QObject::tr("Delete alias"));
}

MoveAliasCommand::MoveAliasCommand(Host* pHost,
                                   TTreeWidget* treeWidget_aliases,
                                   int childID,
                                   int oldParentID,
                                   int newParentID,
                                   int parentPosition,
                                   int childPosition,
                                   int prevParentPosition,
                                   int prevChildPosition,
                                   QUndoCommand* parent)
: QUndoCommand(parent), mpHost(pHost)
{
    mChildID = childID;
    mOldParentID = oldParentID;
    mNewParentID = newParentID;
    mParentPosition = parentPosition;
    mChildPosition = childPosition;
    mPrevParentPosition = prevParentPosition;
    mPrevChildPosition = prevChildPosition;
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

    setText(QObject::tr("Move alias"));
}

AliasNameTextEditedCommand::AliasNameTextEditedCommand(Host* pHost, dlgAliasMainArea* aliasMainArea, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
{
    mpAliasMainArea = aliasMainArea;
}

void AliasNameTextEditedCommand::undo()
{
    if (!mpItem) {
        return;
    }
    int id = mpItemAlias->getID();
    mpEditor->selectAliasByID(id);
    mpItem = mpTreeWidgetAliases->currentItem();

    mpAliasMainArea->lineEdit_alias_name->setText(mPrevAliasName);
    mpEditor->slot_saveSelectedItem(mpItem);
}

void AliasNameTextEditedCommand::redo()
{
    if (!mpItem) {
        return;
    }
    int id = mpItemAlias->getID();
    mpEditor->selectAliasByID(id);
    mpItem = mpTreeWidgetAliases->currentItem();

    mpAliasMainArea->lineEdit_alias_name->setText(mAliasName);
    mpEditor->slot_saveSelectedItem(mpItem);
    setText(QObject::tr("Edit alias name"));
}

AliasCommandTextEditedCommand::AliasCommandTextEditedCommand(Host* pHost, dlgAliasMainArea* aliasMainArea, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
{
    mpAliasMainArea = aliasMainArea;
}

void AliasCommandTextEditedCommand::undo()
{
    if (!mpItem) {
        return;
    }
    int id = mpItemAlias->getID();
    mpEditor->selectAliasByID(id);
    mpItem = mpTreeWidgetAliases->currentItem();

    mpAliasMainArea->lineEdit_alias_command->setText(mPrevAliasCommand);
    mpEditor->slot_saveSelectedItem(mpItem);
}

void AliasCommandTextEditedCommand::redo()
{
    if (!mpItem) {
        return;
    }
    int id = mpItemAlias->getID();
    mpEditor->selectAliasByID(id);
    mpItem = mpTreeWidgetAliases->currentItem();

    mpAliasMainArea->lineEdit_alias_command->setText(mAliasCommand);
    mpEditor->slot_saveSelectedItem(mpItem);
    setText(QObject::tr("Edit alias command"));
}

AliasPatternTextEditedCommand::AliasPatternTextEditedCommand(Host* pHost, dlgAliasMainArea* aliasMainArea, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
{
    mpAliasMainArea = aliasMainArea;
}

void AliasPatternTextEditedCommand::undo()
{
    if (!mpItem) {
        return;
    }
    int id = mpItemAlias->getID();
    mpEditor->selectAliasByID(id);
    mpItem = mpTreeWidgetAliases->currentItem();

    mpAliasMainArea->lineEdit_alias_pattern->setText(mPrevAliasPattern);
    mpEditor->slot_saveSelectedItem(mpItem);
}

void AliasPatternTextEditedCommand::redo()
{
    if (!mpItem) {
        return;
    }

    int id = mpItemAlias->getID();
    mpEditor->selectAliasByID(id);
    mpItem = mpTreeWidgetAliases->currentItem();

    mpAliasMainArea->lineEdit_alias_pattern->setText(mAliasPattern);
    setText(QObject::tr("Edit alias pattern"));
    mpEditor->slot_saveSelectedItem(mpItem);
}

AddTimerCommand::AddTimerCommand(Host* pHost, TTreeWidget* treeWidgetTimers, bool isFolder, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
{
    mpTreeWidgetTimers = treeWidgetTimers;
    mIsFolder = isFolder;
}

void AddTimerCommand::undo()
{
    if (!mpItem) {
        return;
    }

    int id = mpItemTimer->getID();
    mpEditor->selectTimerByID(id);
    mpItem = mpTreeWidgetTimers->currentItem();

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
        const int id = mpTreeWidgetTimers->currentItem()->data(0, Qt::UserRole).toInt();
        TTimer* pT = mpHost->getTimerUnit()->getTimer(id);
        mpItemTimer = pT;
    } else {
        int count = mpParent->childCount();
        if (mIsFolder) {
            mpParent->addChild(mpItem);
        } else {
            mpParent->insertChild(count <= 0 ? 0 : count, mpItem);
        }
    }
    setText(QObject::tr("Add timer"));
}

PasteTimerCommand::PasteTimerCommand(Host* pHost, TTreeWidget* treeWidgetTimers, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
{
    mpTreeWidgetTimers = treeWidgetTimers;
}

void PasteTimerCommand::undo()
{
    if (!mpEditor) {
        return;
    }

    if(!mImportedItemID)
    {
        return;
    }

    mpEditor->selectTimerByID(mImportedItemID);
    mpItem = mpTreeWidgetTimers->currentItem();

    mpParent = mpItem->parent();
    mpParent->removeChild(mpItem);
}

void PasteTimerCommand::redo()
{
    if(!mpItem)
    {
        auto parent = mpTreeWidgetTimers->currentIndex().parent();
        auto parentRow = parent.row();
        auto parentId = parent.data(Qt::UserRole).toInt();
        const int siblingRow = mpTreeWidgetTimers->currentIndex().row() + 1;
        mpHost->getTimerUnit()->reParentTimer(mImportedItemID, 0, parentId, parentRow, siblingRow);
    }
    else
    {
        int count = mpParent->childCount();
        mpParent->insertChild(count <= 0 ? 0 : count, mpItem);
    }

    mpEditor->selectTimerByID(mImportedItemID);
    mpItem = mpTreeWidgetTimers->currentItem();
    setText(QObject::tr("Paste timer"));
}

DeleteTimerCommand::DeleteTimerCommand(Host* pHost, QTreeWidgetItem* pItem, TTreeWidget* treeWidgetTimers, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
{
    mpItem = pItem;
    mpParent = mpItem->parent();
    mpTreeWidgetTimers = treeWidgetTimers;
}

void DeleteTimerCommand::undo()
{
    if (!mpItem) {
        return;
    }

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
        TTimer* p = mpHost->getTimerUnit()->getTimer(ID);
        TTimer* ptr = new TTimer(p->getName(), p->getTime(), mpHost);
        mpHost->getTimerUnit()->registerTimer(ptr);
        mpItemTimer = ptr;
        mpEditor->delete_timer();
    }
    setText(QObject::tr("Delete timer"));
}

MoveTimerCommand::MoveTimerCommand(Host* pHost,
                                   TTreeWidget* treeWidgetTimers,
                                   int childID,
                                   int oldParentID,
                                   int newParentID,
                                   int parentPosition,
                                   int childPosition,
                                   int prevParentPosition,
                                   int prevChildPosition,
                                   QUndoCommand* parent)
: QUndoCommand(parent), mpHost(pHost)
{
    mChildID = childID;
    mOldParentID = oldParentID;
    mNewParentID = newParentID;
    mParentPosition = parentPosition;
    mChildPosition = childPosition;
    mPrevParentPosition = prevParentPosition;
    mPrevChildPosition = prevChildPosition;
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

    setText(QObject::tr("Move timer"));
}

TimerNameTextEditedCommand::TimerNameTextEditedCommand(Host* pHost, dlgTimersMainArea* timersMainArea, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
{
    mpTimersMainArea = timersMainArea;
}

void TimerNameTextEditedCommand::undo()
{
    if (!mpItem) {
        return;
    }
    int id = mpItemTimer->getID();
    mpEditor->selectTimerByID(id);
    mpItem = mpTreeWidgetTimers->currentItem();

    mpTreeWidgetTimers->setCurrentItem(mpItem);
    mpTimersMainArea->lineEdit_timer_name->setText(mPrevTimerName);
}

void TimerNameTextEditedCommand::redo()
{
    if (!mpItem) {
        return;
    }
    int id = mpItemTimer->getID();
    mpEditor->selectTimerByID(id);
    mpItem = mpTreeWidgetTimers->currentItem();
    mpTimersMainArea->lineEdit_timer_name->blockSignals(true);
    mpTimersMainArea->lineEdit_timer_name->setText(mTimerName);
    mpTimersMainArea->lineEdit_timer_name->blockSignals(false);
    setText(QObject::tr("Edit timer name"));
}

TimerCommandTextEditedCommand::TimerCommandTextEditedCommand(Host* pHost, dlgTimersMainArea* timersMainArea, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
{
    mpTimersMainArea = timersMainArea;
}

void TimerCommandTextEditedCommand::undo()
{
    if (!mpItem) {
        return;
    }
    int id = mpItemTimer->getID();
    mpEditor->selectTimerByID(id);
    mpItem = mpTreeWidgetTimers->currentItem();
    mpTimersMainArea->lineEdit_timer_command->blockSignals(true);
    mpTimersMainArea->lineEdit_timer_command->setText(mPrevTimerCommand);
    mpTimersMainArea->lineEdit_timer_command->blockSignals(false);
}

void TimerCommandTextEditedCommand::redo()
{
    if (!mpItem) {
        return;
    }
    int id = mpItemTimer->getID();
    mpEditor->selectTimerByID(id);
    mpItem = mpTreeWidgetTimers->currentItem();
    mpTimersMainArea->lineEdit_timer_command->blockSignals(true);
    mpTimersMainArea->lineEdit_timer_command->setText(mTimerCommand);
    mpTimersMainArea->lineEdit_timer_command->blockSignals(false);
    setText(QObject::tr("Edit timer command"));
}

TimerHoursTextEditedCommand::TimerHoursTextEditedCommand(Host* pHost, dlgTimersMainArea* timersMainArea, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
{
    mpTimersMainArea = timersMainArea;
}

void TimerHoursTextEditedCommand::undo()
{
    if (!mpItem) {
        return;
    }
    int id = mpItemTimer->getID();
    mpEditor->selectTimerByID(id);
    mpItem = mpTreeWidgetTimers->currentItem();

    mpTimersMainArea->timeEdit_timer_hours->blockSignals(true);
    mpTimersMainArea->timeEdit_timer_hours->setTime(QTime(mPrevTimerHours, 0, 0, 0));
    mpTimersMainArea->timeEdit_timer_hours->blockSignals(false);
}

void TimerHoursTextEditedCommand::redo()
{
    if (!mpItem) {
        return;
    }
    int id = mpItemTimer->getID();
    mpEditor->selectTimerByID(id);
    mpItem = mpTreeWidgetTimers->currentItem();

    mpTimersMainArea->timeEdit_timer_hours->blockSignals(true);
    mpTimersMainArea->timeEdit_timer_hours->setTime(QTime(mTimerHours, 0, 0, 0));
    mpTimersMainArea->timeEdit_timer_hours->blockSignals(false);
    setText(QObject::tr("Edit timer hours"));
}

TimerMinutesTextEditedCommand::TimerMinutesTextEditedCommand(Host* pHost, dlgTimersMainArea* timersMainArea, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
{
    mpTimersMainArea = timersMainArea;
}

void TimerMinutesTextEditedCommand::undo()
{
    if (!mpItem) {
        return;
    }
    int id = mpItemTimer->getID();
    mpEditor->selectTimerByID(id);
    mpItem = mpTreeWidgetTimers->currentItem();

    mpEditor->slot_timerSelected(mpItem);
    mpTimersMainArea->timeEdit_timer_minutes->blockSignals(true);
    mpTimersMainArea->timeEdit_timer_minutes->setTime(QTime(0, mPrevTimerMinutes, 0, 0));
    mpTimersMainArea->timeEdit_timer_minutes->blockSignals(false);
}

void TimerMinutesTextEditedCommand::redo()
{
    if (!mpItem) {
        return;
    }
    int id = mpItemTimer->getID();
    mpEditor->selectTimerByID(id);
    mpItem = mpTreeWidgetTimers->currentItem();

    mpTimersMainArea->timeEdit_timer_minutes->blockSignals(true);
    mpTimersMainArea->timeEdit_timer_minutes->setTime(QTime(0, mTimerMinutes, 0, 0));
    mpTimersMainArea->timeEdit_timer_minutes->blockSignals(false);
    setText(QObject::tr("Edit timer minutes"));
}

TimerSecondsTextEditedCommand::TimerSecondsTextEditedCommand(Host* pHost, dlgTimersMainArea* timersMainArea, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
{
    mpTimersMainArea = timersMainArea;
}

void TimerSecondsTextEditedCommand::undo()
{
    if (!mpItem) {
        return;
    }
    int id = mpItemTimer->getID();
    mpEditor->selectTimerByID(id);
    mpItem = mpTreeWidgetTimers->currentItem();

    mpTimersMainArea->timeEdit_timer_seconds->blockSignals(true);
    mpTimersMainArea->timeEdit_timer_seconds->setTime(QTime(0, 0, mPrevTimerSeconds, 0));
    mpTimersMainArea->timeEdit_timer_seconds->blockSignals(false);
}

void TimerSecondsTextEditedCommand::redo()
{
    if (!mpItem) {
        return;
    }
    int id = mpItemTimer->getID();
    mpEditor->selectTimerByID(id);
    mpItem = mpTreeWidgetTimers->currentItem();

    mpTimersMainArea->timeEdit_timer_seconds->blockSignals(true);
    mpTimersMainArea->timeEdit_timer_seconds->setTime(QTime(0, 0, mTimerSeconds, 0));
    mpTimersMainArea->timeEdit_timer_seconds->blockSignals(false);
    setText(QObject::tr("Edit timer seconds"));
}

TimerMilliSecondsTextEditedCommand::TimerMilliSecondsTextEditedCommand(Host* pHost, dlgTimersMainArea* timersMainArea, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
{
    mpTimersMainArea = timersMainArea;
}

void TimerMilliSecondsTextEditedCommand::undo()
{
    if (!mpItem) {
        return;
    }
    int id = mpItemTimer->getID();
    mpEditor->selectTimerByID(id);
    mpItem = mpTreeWidgetTimers->currentItem();

    mpTimersMainArea->timeEdit_timer_msecs->blockSignals(true);
    mpTimersMainArea->timeEdit_timer_msecs->setTime(QTime(0, 0, 0, mPrevTimerMsecs));
    mpTimersMainArea->timeEdit_timer_msecs->blockSignals(false);
}

void TimerMilliSecondsTextEditedCommand::redo()
{
    if (!mpItem) {
        return;
    }
    int id = mpItemTimer->getID();
    mpEditor->selectTimerByID(id);
    mpItem = mpTreeWidgetTimers->currentItem();

    mpTimersMainArea->timeEdit_timer_msecs->blockSignals(true);
    mpTimersMainArea->timeEdit_timer_msecs->setTime(QTime(0, 0, 0, mTimerMsecs));
    mpTimersMainArea->timeEdit_timer_msecs->blockSignals(false);
    setText(QObject::tr("Edit timer msecs"));
}

AddScriptCommand::AddScriptCommand(Host* pHost, TTreeWidget* treeWidget_scripts, bool isFolder, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
{
    mpTreeWidgetScripts = treeWidget_scripts;
    mIsFolder = isFolder;
}

void AddScriptCommand::undo()
{
    if (!mpItem) {
        return;
    }
    int id = mpItemScript->getID();
    mpEditor->selectScriptByID(id);
    mpItem = mpTreeWidgetScripts->currentItem();

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
        const int id = mpTreeWidgetScripts->currentItem()->data(0, Qt::UserRole).toInt();
        TScript* pT = mpHost->getScriptUnit()->getScript(id);
        mpItemScript = pT;
    } else {
        int count = mpParent->childCount();
        if (mIsFolder) {
            mpParent->addChild(mpItem);
        } else {
            mpParent->insertChild(count <= 0 ? 0 : count, mpItem);
        }
    }

    setText(QObject::tr("Add script"));
}

PasteScriptCommand::PasteScriptCommand(Host* pHost, TTreeWidget* treeWidgetScripts, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
{
    mpTreeWidgetScripts = treeWidgetScripts;
}

void PasteScriptCommand::undo()
{
    if (!mpEditor) {
        return;
    }

    if(!mImportedItemID)
    {
        return;
    }

    mpEditor->selectScriptByID(mImportedItemID);
    mpItem = mpTreeWidgetScripts->currentItem();

    mpParent = mpItem->parent();
    mpParent->removeChild(mpItem);
}

void PasteScriptCommand::redo()
{
    if(!mpItem)
    {
        auto parent = mpTreeWidgetScripts->currentIndex().parent();
        auto parentRow = parent.row();
        auto parentId = parent.data(Qt::UserRole).toInt();
        const int siblingRow = mpTreeWidgetScripts->currentIndex().row() + 1;
        mpHost->getTriggerUnit()->reParentTrigger(mImportedItemID, 0, parentId, parentRow, siblingRow);
    }
    else
    {
        int count = mpParent->childCount();
        mpParent->insertChild(count <= 0 ? 0 : count, mpItem);
    }

    mpEditor->selectScriptByID(mImportedItemID);
    mpItem = mpTreeWidgetScripts->currentItem();
    setText(QObject::tr("Paste timer"));
}

DeleteScriptCommand::DeleteScriptCommand(Host* pHost, QTreeWidgetItem* pItem, TTreeWidget* treeWidgetScripts, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
{
    mpItem = pItem;
    mpParent = mpItem->parent();
    mpTreeWidgetScripts = treeWidgetScripts;
}

void DeleteScriptCommand::undo()
{
    if (!mpItem) {
        return;
    }

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
        TScript* p = mpHost->getScriptUnit()->getScript(ID);
        TScript* ptr = new TScript(p->getName(), mpHost);
        ptr->registerScript();
        mpItemScript = ptr;
        mpEditor->delete_script();
    }
    setText(QObject::tr("Delete script"));
}

MoveScriptCommand::MoveScriptCommand(Host* pHost,
                                     TTreeWidget* treeWidget_scripts,
                                     int childID,
                                     int oldParentID,
                                     int newParentID,
                                     int parentPosition,
                                     int childPosition,
                                     int prevParentPosition,
                                     int prevChildPosition,
                                     QUndoCommand* parent)
: QUndoCommand(parent), mpHost(pHost)
{
    mChildID = childID;
    mOldParentID = oldParentID;
    mNewParentID = newParentID;
    mParentPosition = parentPosition;
    mChildPosition = childPosition;
    mPrevParentPosition = prevParentPosition;
    mPrevChildPosition = prevChildPosition;
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

    setText(QObject::tr("Move script"));
}

ScriptNameTextEditedCommand::ScriptNameTextEditedCommand(Host* pHost, dlgScriptsMainArea* scriptsMainArea, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
{
    mpScriptsMainArea = scriptsMainArea;
}

void ScriptNameTextEditedCommand::undo()
{
    if (!mpItem) {
        return;
    }
    int id = mpItemScript->getID();
    mpEditor->selectScriptByID(id);
    mpItem = mpTreeWidgetScripts->currentItem();

    mpScriptsMainArea->lineEdit_script_name->setText(mPrevScriptName);
    mpEditor->slot_saveSelectedItem(mpItem);
}

void ScriptNameTextEditedCommand::redo()
{
    if (!mpItem) {
        return;
    }
    int id = mpItemScript->getID();
    mpEditor->selectScriptByID(id);
    mpItem = mpTreeWidgetScripts->currentItem();

    mpScriptsMainArea->lineEdit_script_name->setText(mScriptName);
    mpEditor->slot_saveSelectedItem(mpItem);
    setText(QObject::tr("Edit script name"));
}

ScriptAddHandlerCommand::ScriptAddHandlerCommand(Host* pHost, dlgScriptsMainArea* scriptsMainArea, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
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
    mRow = mpScriptsMainArea->listWidget_script_registered_event_handlers->row(pItem);
    mpScriptsMainArea->listWidget_script_registered_event_handlers->setCurrentRow(mRow);
    setText(QObject::tr("Add script handler"));
}

ScriptRemoveHandlerCommand::ScriptRemoveHandlerCommand(Host* pHost, dlgScriptsMainArea* scriptsMainArea, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
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
    mRow = mpScriptsMainArea->listWidget_script_registered_event_handlers->row(mpWidgetItem);
    mpScriptsMainArea->listWidget_script_registered_event_handlers->setCurrentRow(mRow);
}

void ScriptRemoveHandlerCommand::redo()
{
    if (!mpItem) {
        return;
    }
    mpTreeWidgetScripts->setCurrentItem(mpItem);
    mRow = mpScriptsMainArea->listWidget_script_registered_event_handlers->currentRow();
    if (mRow < 0) {
        return;
    }
    mpWidgetItem = mpScriptsMainArea->listWidget_script_registered_event_handlers->takeItem(mRow);
    setText(QObject::tr("Remove script handler"));
}

AddKeyCommand::AddKeyCommand(Host* pHost, TTreeWidget* treeWidget_keys, bool isFolder, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
{
    mpTreeWidgetKeys = treeWidget_keys;
    mIsFolder = isFolder;
}

void AddKeyCommand::undo()
{
    if (!mpItem) {
        return;
    }

    int id = mpItemKey->getID();
    mpEditor->selectKeyByID(id);
    mpItem = mpTreeWidgetKeys->currentItem();

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
        const int id = mpTreeWidgetKeys->currentItem()->data(0, Qt::UserRole).toInt();
        TKey* pT = mpHost->getKeyUnit()->getKey(id);
        mpItemKey = pT;
    } else {
        int count = mpParent->childCount();
        if (mIsFolder) {
            mpParent->addChild(mpItem);
        } else {
            mpParent->insertChild(count <= 0 ? 0 : count, mpItem);
        }
    }

    setText(QObject::tr("Add key"));
}

PasteKeyCommand::PasteKeyCommand(Host* pHost, TTreeWidget* treeWidgetKeys, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
{
    mpTreeWidgetKeys = treeWidgetKeys;
}

void PasteKeyCommand::undo()
{
    if (!mpEditor) {
        return;
    }

    if(!mImportedItemID)
    {
        return;
    }
    mpEditor->selectKeyByID(mImportedItemID);
    mpItem = mpTreeWidgetKeys->currentItem();

    mpParent = mpItem->parent();
    mpParent->removeChild(mpItem);
}

void PasteKeyCommand::redo()
{
    if(!mpItem)
    {
        auto parent = mpTreeWidgetKeys->currentIndex().parent();
        auto parentRow = parent.row();
        auto parentId = parent.data(Qt::UserRole).toInt();
        const int siblingRow = mpTreeWidgetKeys->currentIndex().row() + 1;
        mpHost->getKeyUnit()->reParentKey(mImportedItemID, 0, parentId, parentRow, siblingRow);
    }
    else
    {
        int count = mpParent->childCount();
        mpParent->insertChild(count <= 0 ? 0 : count, mpItem);
    }

    mpEditor->selectKeyByID(mImportedItemID);
    mpItem = mpTreeWidgetKeys->currentItem();
    setText(QObject::tr("Paste key"));
}

DeleteKeyCommand::DeleteKeyCommand(Host* pHost, QTreeWidgetItem* pItem, TTreeWidget* treeWidget_keys, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
{
    mpItem = pItem;
    mpParent = mpItem->parent();
    mpTreeWidgetKeys = treeWidget_keys;
}

void DeleteKeyCommand::undo()
{
    if (!mpItem) {
        return;
    }

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
        TKey* p = mpHost->getKeyUnit()->getKey(ID);
        TKey* ptr = new TKey(p->getName(), mpHost);
        ptr->registerKey();
        mpItemKey = ptr;
        mpEditor->delete_key();
    }
    setText(QObject::tr("Delete key"));
}
MoveKeyCommand::MoveKeyCommand(Host* pHost,
                               TTreeWidget* treeWidget_keys,
                               int childID,
                               int oldParentID,
                               int newParentID,
                               int parentPosition,
                               int childPosition,
                               int prevParentPosition,
                               int prevChildPosition,
                               QUndoCommand* parent)
: QUndoCommand(parent), mpHost(pHost)
{
    mChildID = childID;
    mOldParentID = oldParentID;
    mNewParentID = newParentID;
    mParentPosition = parentPosition;
    mChildPosition = childPosition;
    mPrevParentPosition = prevParentPosition;
    mPrevChildPosition = prevChildPosition;
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

    setText(QObject::tr("Move key"));
}

KeyNameTextEditedCommand::KeyNameTextEditedCommand(Host* pHost, dlgKeysMainArea* keysMainArea, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
{
    mpKeysMainArea = keysMainArea;
}

void KeyNameTextEditedCommand::undo()
{
    if (!mpItem) {
        return;
    }
    int id = mpItemKey->getID();
    mpEditor->selectKeyByID(id);
    mpItem = mpTreeWidgetKeys->currentItem();

    mpKeysMainArea->lineEdit_key_name->blockSignals(true);
    mpKeysMainArea->lineEdit_key_name->setText(mPrevKeyName);
    mpKeysMainArea->lineEdit_key_name->blockSignals(false);
}

void KeyNameTextEditedCommand::redo()
{
    if (!mpItem) {
        return;
    }
    int id = mpItemKey->getID();
    mpEditor->selectKeyByID(id);
    mpItem = mpTreeWidgetKeys->currentItem();

    mpKeysMainArea->lineEdit_key_name->blockSignals(true);
    mpKeysMainArea->lineEdit_key_name->setText(mKeyName);
    mpKeysMainArea->lineEdit_key_name->blockSignals(false);
    setText(QObject::tr("Edit key name"));
}

KeyCommandTextEditedCommand::KeyCommandTextEditedCommand(Host* pHost, dlgKeysMainArea* keysMainArea, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
{
    mpKeysMainArea = keysMainArea;
}

void KeyCommandTextEditedCommand::undo()
{
    if (!mpItem) {
        return;
    }
    int id = mpItemKey->getID();
    mpEditor->selectKeyByID(id);
    mpItem = mpTreeWidgetKeys->currentItem();

    mpKeysMainArea->lineEdit_key_command->blockSignals(true);
    mpKeysMainArea->lineEdit_key_command->setText(mPrevKeyCommand);
    mpKeysMainArea->lineEdit_key_command->blockSignals(false);
}

void KeyCommandTextEditedCommand::redo()
{
    if (!mpItem) {
        return;
    }
    int id = mpItemKey->getID();
    mpEditor->selectKeyByID(id);
    mpItem = mpTreeWidgetKeys->currentItem();

    mpKeysMainArea->lineEdit_key_command->blockSignals(true);
    mpKeysMainArea->lineEdit_key_command->setText(mKeyCommand);
    mpKeysMainArea->lineEdit_key_command->blockSignals(false);
    setText(QObject::tr("Edit key command"));
}

KeyGrabTextEditedCommand::KeyGrabTextEditedCommand(Host* pHost, dlgKeysMainArea* keysMainArea, QUndoCommand* parent)
    : QUndoCommand(parent), mpHost(pHost)
{
    mpKeysMainArea = keysMainArea;
}

void KeyGrabTextEditedCommand::undo()
{
    if (!mpItem) {
        return;
    }
    int id = mpItemKey->getID();
    mpEditor->selectKeyByID(id);
    mpItem = mpTreeWidgetKeys->currentItem();

    const int triggerID = mpItem->data(0, Qt::UserRole).toInt();
    TKey* pT = mpHost->getKeyUnit()->getKey(triggerID);
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
    int id = mpItemKey->getID();
    mpEditor->selectKeyByID(id);
    mpItem = mpTreeWidgetKeys->currentItem();

    const int triggerID = mpItem->data(0, Qt::UserRole).toInt();
    TKey* pT = mpHost->getKeyUnit()->getKey(triggerID);
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

AddActionCommand::AddActionCommand(Host* pHost, TTreeWidget* treeWidget_actions, bool isFolder, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
{
    mpTreeWidgetActions = treeWidget_actions;
    mIsFolder = isFolder;
}

void AddActionCommand::undo()
{
    if (!mpItem) {
        return;
    }
    int id = mpItemAction->getID();
    mpEditor->selectActionByID(id);
    mpItem = mpTreeWidgetActions->currentItem();

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
        const int id = mpTreeWidgetActions->currentItem()->data(0, Qt::UserRole).toInt();
        TAction* pT = mpHost->getActionUnit()->getAction(id);
        mpItemAction = pT;
    } else {
        int count = mpParent->childCount();
        if (mIsFolder) {
            mpParent->addChild(mpItem);
        } else {
            mpParent->insertChild(count <= 0 ? 0 : count, mpItem);
        }
    }

    setText(QObject::tr("Add action"));
}

PasteActionCommand::PasteActionCommand(Host* pHost, TTreeWidget* treeWidgetActions, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
{
    mpTreeWidgetActions = treeWidgetActions;
}

void PasteActionCommand::undo()
{
    if (!mpEditor) {
        return;
    }

    if(!mImportedItemID)
    {
        return;
    }
    mpEditor->selectActionByID(mImportedItemID);
    mpItem = mpTreeWidgetActions->currentItem();

    mpParent = mpItem->parent();
    mpParent->removeChild(mpItem);
}

void PasteActionCommand::redo()
{
    if(!mpItem)
    {
        auto parent = mpTreeWidgetActions->currentIndex().parent();
        auto parentRow = parent.row();
        auto parentId = parent.data(Qt::UserRole).toInt();
        const int siblingRow = mpTreeWidgetActions->currentIndex().row() + 1;
        mpHost->getActionUnit()->reParentAction(mImportedItemID, 0, parentId, parentRow, siblingRow);
    }
    else
    {
        int count = mpParent->childCount();
        mpParent->insertChild(count <= 0 ? 0 : count, mpItem);
    }

    mpEditor->selectActionByID(mImportedItemID);
    mpItem = mpTreeWidgetActions->currentItem();

    setText(QObject::tr("Paste action"));
}

DeleteActionCommand::DeleteActionCommand(Host* pHost, QTreeWidgetItem* pItem, TTreeWidget* treeWidget_actions, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
{
    mpItem = pItem;
    mpParent = mpItem->parent();
    mpTreeWidgetActions = treeWidget_actions;
}

void DeleteActionCommand::undo()
{
    if (!mpItem) {
        return;
    }

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
        TAction* p = mpHost->getActionUnit()->getAction(ID);
        TAction* ptr = new TAction(p->getName(), mpHost);
        ptr->registerAction();
        mpItemAction = ptr;
        mpEditor->delete_action();
    }
    setText(QObject::tr("Delete action"));
}
MoveActionCommand::MoveActionCommand(Host* pHost,
                                     TTreeWidget* treeWidget_actions,
                                     int childID,
                                     int oldParentID,
                                     int newParentID,
                                     int parentPosition,
                                     int childPosition,
                                     int prevParentPosition,
                                     int prevChildPosition,
                                     QUndoCommand* parent)
: QUndoCommand(parent), mpHost(pHost)
{
    mChildID = childID;
    mOldParentID = oldParentID;
    mNewParentID = newParentID;
    mParentPosition = parentPosition;
    mChildPosition = childPosition;
    mPrevParentPosition = prevParentPosition;
    mPrevChildPosition = prevChildPosition;
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

    setText(QObject::tr("Move action"));
}

ActionNameTextEditedCommand::ActionNameTextEditedCommand(Host* pHost, dlgActionMainArea* actionsMainArea, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
{
    mpActionsMainArea = actionsMainArea;
}

void ActionNameTextEditedCommand::undo()
{
    if (!mpItem) {
        return;
    }
    int id = mpItemAction->getID();
    mpEditor->selectActionByID(id);
    mpItem = mpTreeWidgetActions->currentItem();

    mpActionsMainArea->lineEdit_action_name->blockSignals(true);
    mpActionsMainArea->lineEdit_action_name->setText(mPrevActionName);
    mpActionsMainArea->lineEdit_action_name->blockSignals(false);
}

void ActionNameTextEditedCommand::redo()
{
    if (!mpItem) {
        return;
    }
    int id = mpItemAction->getID();
    mpEditor->selectActionByID(id);
    mpItem = mpTreeWidgetActions->currentItem();

    mpActionsMainArea->lineEdit_action_name->blockSignals(true);
    mpActionsMainArea->lineEdit_action_name->setText(mActionName);
    mpActionsMainArea->lineEdit_action_name->blockSignals(false);
    setText(QObject::tr("Edit action name"));
}

ActionButtonRotationEditedCommand::ActionButtonRotationEditedCommand(Host* pHost, dlgActionMainArea* actionsMainArea, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
{
    mpActionsMainArea = actionsMainArea;
}

void ActionButtonRotationEditedCommand::undo()
{
    if (!mpItem) {
        return;
    }
    int id = mpItemAction->getID();
    mpEditor->selectActionByID(id);
    mpItem = mpTreeWidgetActions->currentItem();

    mpActionsMainArea->comboBox_action_button_rotation->blockSignals(true);
    mpActionsMainArea->comboBox_action_button_rotation->setCurrentIndex(mPrevRotation);
    mpActionsMainArea->comboBox_action_button_rotation->blockSignals(false);
}

void ActionButtonRotationEditedCommand::redo()
{
    if (!mpItem) {
        return;
    }
    int id = mpItemAction->getID();
    mpEditor->selectActionByID(id);
    mpItem = mpTreeWidgetActions->currentItem();

    mpActionsMainArea->comboBox_action_button_rotation->blockSignals(true);
    mpActionsMainArea->comboBox_action_button_rotation->setCurrentIndex(mRotation);
    mpActionsMainArea->comboBox_action_button_rotation->blockSignals(false);
    setText(QObject::tr("Edit action rotation"));
}

ActionButtonCheckboxEditedCommand::ActionButtonCheckboxEditedCommand(Host* pHost, dlgActionMainArea* actionsMainArea, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
{
    mpActionsMainArea = actionsMainArea;
}

void ActionButtonCheckboxEditedCommand::undo()
{
    if (!mpItem) {
        return;
    }
    int id = mpItemAction->getID();
    mpEditor->selectActionByID(id);
    mpItem = mpTreeWidgetActions->currentItem();

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
    int id = mpItemAction->getID();
    mpEditor->selectActionByID(id);
    mpItem = mpTreeWidgetActions->currentItem();

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

ActionCommandDownTextEditedCommand::ActionCommandDownTextEditedCommand(Host* pHost, dlgActionMainArea* actionsMainArea, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
{
    mpActionsMainArea = actionsMainArea;
}

void ActionCommandDownTextEditedCommand::undo()
{
    if (!mpItem) {
        return;
    }
    int id = mpItemAction->getID();
    mpEditor->selectActionByID(id);
    mpItem = mpTreeWidgetActions->currentItem();

    mpActionsMainArea->lineEdit_action_button_command_down->blockSignals(true);
    mpActionsMainArea->lineEdit_action_button_command_down->setText(mPrevCommandDown);
    mpActionsMainArea->lineEdit_action_button_command_down->blockSignals(false);
}

void ActionCommandDownTextEditedCommand::redo()
{
    if (!mpItem) {
        return;
    }
    int id = mpItemAction->getID();
    mpEditor->selectActionByID(id);
    mpItem = mpTreeWidgetActions->currentItem();

    mpActionsMainArea->lineEdit_action_button_command_down->blockSignals(true);
    mpActionsMainArea->lineEdit_action_button_command_down->setText(mCommandDown);
    mpActionsMainArea->lineEdit_action_button_command_down->blockSignals(false);
    setText(QObject::tr("Edit action down"));
}

ActionCommandUpTextEditedCommand::ActionCommandUpTextEditedCommand(Host* pHost, dlgActionMainArea* actionsMainArea, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
{
    mpActionsMainArea = actionsMainArea;
}

void ActionCommandUpTextEditedCommand::undo()
{
    if (!mpItem) {
        return;
    }
    int id = mpItemAction->getID();
    mpEditor->selectActionByID(id);
    mpItem = mpTreeWidgetActions->currentItem();

    mpActionsMainArea->lineEdit_action_button_command_up->blockSignals(true);
    mpActionsMainArea->lineEdit_action_button_command_up->setText(mPrevCommandUp);
    mpActionsMainArea->lineEdit_action_button_command_up->blockSignals(false);
}

void ActionCommandUpTextEditedCommand::redo()
{
    if (!mpItem) {
        return;
    }
    int id = mpItemAction->getID();
    mpEditor->selectActionByID(id);
    mpItem = mpTreeWidgetActions->currentItem();

    mpActionsMainArea->lineEdit_action_button_command_up->blockSignals(true);
    mpActionsMainArea->lineEdit_action_button_command_up->setText(mCommandUp);
    mpActionsMainArea->lineEdit_action_button_command_up->blockSignals(false);
    setText(QObject::tr("Edit action up"));
}

ActionCssTextEditedCommand::ActionCssTextEditedCommand(Host* pHost, dlgActionMainArea* actionsMainArea, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
{
    mpActionsMainArea = actionsMainArea;
}

int ActionCssTextEditedCommand::id() const
{
    const short id = reinterpret_cast<uintptr_t>(typeid(this).name());
    return id;
}

bool ActionCssTextEditedCommand::mergeWith(const QUndoCommand* other)
{
    QString text = static_cast<const ActionCssTextEditedCommand*>(other)->mActionCss;
    if (text.length() < mActionCss.length()) {
        mActionCss = text;
        return false;
    }
    QTextCursor cursor = mpActionsMainArea->plainTextEdit_action_css->textCursor();
    if (cursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor)) {
        QString selectedChar = cursor.selectedText();
        if (selectedChar == QChar::ParagraphSeparator || selectedChar == QChar::Space || selectedChar == QChar::CarriageReturn || selectedChar == QChar::LineFeed) {
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
    QTextCursor cssCursor(mpActionsMainArea->plainTextEdit_action_css->textCursor());
    int curPosition = cssCursor.position();

    auto curItem = mpTreeWidgetActions->currentItem();
    const int actionID = curItem->data(0, Qt::UserRole).toInt();
    int id = mpItemAction->getID();
    if(actionID != id)
    {
        mpEditor->selectActionByID(id);
        mpItem = mpTreeWidgetActions->currentItem();
    }

    mpActionsMainArea->plainTextEdit_action_css->blockSignals(true);
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
    QTextCursor cssCursor(mpActionsMainArea->plainTextEdit_action_css->textCursor());
    int curPosition = cssCursor.position();

    auto curItem = mpTreeWidgetActions->currentItem();
    const int actionID = curItem->data(0, Qt::UserRole).toInt();
    int id = mpItemAction->getID();
    if(actionID != id)
    {
        mpEditor->selectActionByID(id);
        mpItem = mpTreeWidgetActions->currentItem();
    }

    mpActionsMainArea->plainTextEdit_action_css->blockSignals(true);
    mpActionsMainArea->plainTextEdit_action_css->setPlainText(mActionCss);
    cssCursor.setPosition(curPosition);
    mpActionsMainArea->plainTextEdit_action_css->setTextCursor(cssCursor);
    mpActionsMainArea->plainTextEdit_action_css->blockSignals(false);
    setText(QObject::tr("Edit action css"));
}

AddVarCommand::AddVarCommand(Host* pHost, TTreeWidget* treeWidget_variables, bool isFolder, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
{
    mpTreeWidgetVariables = treeWidget_variables;
    mIsFolder = isFolder;
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

    setText(QObject::tr("Add variable"));
}

DeleteVarCommand::DeleteVarCommand(Host* pHost, QTreeWidgetItem* pItem, TTreeWidget* treeWidget_variables, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
{
    mpItem = pItem;
    mpParent = mpItem->parent();
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
        LuaInterface* lI = mpHost->getLuaInterface();
        lI->getVarUnit()->addTreeItem(mpItem, mpTempVar);

        mpParent->addChild(mpItem);
        QList<QTreeWidgetItem*> list;
        mpEditor->recurseVariablesDown(mpItem, list);
        for (auto& treeWidgetItem : list) {
            TVar* v = lI->getVarUnit()->getWVar(treeWidgetItem);
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
            LuaInterface* lI = mpHost->getLuaInterface();
            TVar* p = lI->getVarUnit()->getWVar(mpItem);
            *mpItemVar = *p;
        }
        mpEditor->delete_variable();
        mpTempVar = nullptr;
    }
    setText(QObject::tr("Delete variable"));
}

MoveVariableCommand::MoveVariableCommand(
        Host* pHost, TTreeWidget* treeWidget_variables, QTreeWidgetItem* parentItem, QTreeWidgetItem* cItem, QTreeWidgetItem* prevParentItem, QUndoCommand* parent)
: QUndoCommand(parent), mpHost(pHost)
{
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
    setText(QObject::tr("Move variable"));
}

TriggerNameTextEditedCommand::TriggerNameTextEditedCommand(Host* pHost, dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
{
    mpTriggersMainArea = triggersMainArea;
}

void TriggerNameTextEditedCommand::undo()
{
    if (!mpItem || !mpEditor) {
        return;
    }

    int id = mpItemTrigger->getID();
    mpEditor->selectTriggerByID(id);
    mpItem = mpTreeWidgetTriggers->currentItem();

    mpEditor->slot_triggerSelected(mpItem);
    mpTriggersMainArea->lineEdit_trigger_name->setText(mPrevLineEditTriggerName);
    mpEditor->slot_saveSelectedItem(mpItem);
}

void TriggerNameTextEditedCommand::redo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    int id = mpItemTrigger->getID();
    mpEditor->selectTriggerByID(id);
    mpItem = mpTreeWidgetTriggers->currentItem();

    mpTriggersMainArea->lineEdit_trigger_name->setText(mLineEditTriggerName);
    mpEditor->slot_saveSelectedItem(mpItem);
    setText(QObject::tr("Edit trigger name"));
}

TriggerCommandTextEditedCommand::TriggerCommandTextEditedCommand(Host* pHost, dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
{
    mpTriggersMainArea = triggersMainArea;
}

void TriggerCommandTextEditedCommand::undo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    int id = mpItemTrigger->getID();
    mpEditor->selectTriggerByID(id);
    mpItem = mpTreeWidgetTriggers->currentItem();

    mpTriggersMainArea->lineEdit_trigger_command->setText(mPrevLineEditTriggerCommand);
}

void TriggerCommandTextEditedCommand::redo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    int id = mpItemTrigger->getID();
    mpEditor->selectTriggerByID(id);
    mpItem = mpTreeWidgetTriggers->currentItem();

    mpEditor->slot_triggerSelected(mpItem);
    mpTriggersMainArea->lineEdit_trigger_command->setText(mLineEditTriggerCommand);
    setText(QObject::tr("Edit trigger command"));
}

TriggerFireLengthEditedCommand::TriggerFireLengthEditedCommand(Host* pHost, dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
{
    mpTriggersMainArea = triggersMainArea;
}

void TriggerFireLengthEditedCommand::undo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    int id = mpItemTrigger->getID();
    mpEditor->selectTriggerByID(id);
    mpItem = mpTreeWidgetTriggers->currentItem();

    mpTriggersMainArea->spinBox_stayOpen->blockSignals(true);
    mpTriggersMainArea->spinBox_stayOpen->setValue(mPrevFireLength);
    mpTriggersMainArea->spinBox_stayOpen->blockSignals(false);
}

void TriggerFireLengthEditedCommand::redo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    int id = mpItemTrigger->getID();
    mpEditor->selectTriggerByID(id);
    mpItem = mpTreeWidgetTriggers->currentItem();

    mpTriggersMainArea->spinBox_stayOpen->blockSignals(true);
    mpTriggersMainArea->spinBox_stayOpen->setValue(mFireLength);
    mpTriggersMainArea->spinBox_stayOpen->blockSignals(false);
    setText(QObject::tr("Edit fire length"));
}

TriggerPlaySoundEditedCommand::TriggerPlaySoundEditedCommand(Host* pHost, dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
{
    mpTriggersMainArea = triggersMainArea;
}

void TriggerPlaySoundEditedCommand::undo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    int id = mpItemTrigger->getID();
    mpEditor->selectTriggerByID(id);
    mpItem = mpTreeWidgetTriggers->currentItem();

    mpTriggersMainArea->groupBox_soundTrigger->blockSignals(true);
    mpTriggersMainArea->groupBox_soundTrigger->setChecked(mPrevGroupBoxSoundTrigger);
    mpTriggersMainArea->groupBox_soundTrigger->blockSignals(false);
}

void TriggerPlaySoundEditedCommand::redo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    int id = mpItemTrigger->getID();
    mpEditor->selectTriggerByID(id);
    mpItem = mpTreeWidgetTriggers->currentItem();

    mpTriggersMainArea->groupBox_soundTrigger->blockSignals(true);
    mpTriggersMainArea->groupBox_soundTrigger->setChecked(mGroupBoxSoundTrigger);
    mpTriggersMainArea->groupBox_soundTrigger->blockSignals(false);
    setText(QObject::tr("Edit play sound"));
}

TriggerPlaySoundFileEditedCommand::TriggerPlaySoundFileEditedCommand(Host* pHost, dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
{
    mpTriggersMainArea = triggersMainArea;
}

void TriggerPlaySoundFileEditedCommand::undo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    int id = mpItemTrigger->getID();
    mpEditor->selectTriggerByID(id);
    mpItem = mpTreeWidgetTriggers->currentItem();

    mpTriggersMainArea->lineEdit_soundFile->blockSignals(true);
    mpTriggersMainArea->lineEdit_soundFile->setText(mPrevLineEditSoundFile);
    mpTriggersMainArea->lineEdit_soundFile->blockSignals(false);
}

void TriggerPlaySoundFileEditedCommand::redo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    int id = mpItemTrigger->getID();
    mpEditor->selectTriggerByID(id);
    mpItem = mpTreeWidgetTriggers->currentItem();

    mpTriggersMainArea->lineEdit_soundFile->blockSignals(true);
    mpTriggersMainArea->lineEdit_soundFile->setText(mLineEditSoundFile);
    mpTriggersMainArea->lineEdit_soundFile->blockSignals(false);
    setText(QObject::tr("Edit play sound file"));
}

TriggerColorizerEditedCommand::TriggerColorizerEditedCommand(Host* pHost, dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
{
    mpTriggersMainArea = triggersMainArea;
}

void TriggerColorizerEditedCommand::undo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    int id = mpItemTrigger->getID();
    mpEditor->selectTriggerByID(id);
    mpItem = mpTreeWidgetTriggers->currentItem();

    mpTriggersMainArea->groupBox_triggerColorizer->blockSignals(true);
    mpTriggersMainArea->groupBox_triggerColorizer->setChecked(mPrevBoxTriggerColorizer);
    mpTriggersMainArea->groupBox_triggerColorizer->blockSignals(false);
}

void TriggerColorizerEditedCommand::redo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    int id = mpItemTrigger->getID();
    mpEditor->selectTriggerByID(id);
    mpItem = mpTreeWidgetTriggers->currentItem();

    mpTriggersMainArea->groupBox_triggerColorizer->blockSignals(true);
    mpTriggersMainArea->groupBox_triggerColorizer->setChecked(mBoxTriggerColorizer);
    mpTriggersMainArea->groupBox_triggerColorizer->blockSignals(false);
    setText(QObject::tr("Edit trigger colorizer"));
}

TriggerColorizerBgColorEditedCommand::TriggerColorizerBgColorEditedCommand(Host* pHost, dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
{
    mpTriggersMainArea = triggersMainArea;
}

void TriggerColorizerBgColorEditedCommand::undo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    int id = mpItemTrigger->getID();
    mpEditor->selectTriggerByID(id);
    mpItem = mpTreeWidgetTriggers->currentItem();

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
    int id = mpItemTrigger->getID();
    mpEditor->selectTriggerByID(id);
    mpItem = mpTreeWidgetTriggers->currentItem();

    mpTriggersMainArea->pushButtonBgColor->blockSignals(true);
    mpTriggersMainArea->pushButtonBgColor->setProperty("baseColor", mBgColor);
    const bool keepColor = mBgColor == QColorConstants::Transparent.name();
    mpTriggersMainArea->pushButtonBgColor->setText(keepColor ? mpEditor->tr("keep") : QString());
    mpTriggersMainArea->pushButtonBgColor->setStyleSheet(mpEditor->generateButtonStyleSheet(mBgColor));
    mpTriggersMainArea->pushButtonBgColor->blockSignals(false);
    setText(QObject::tr("Edit trigger bg color"));
}

TriggerColorizerFgColorEditedCommand::TriggerColorizerFgColorEditedCommand(Host* pHost, dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
{
    mpTriggersMainArea = triggersMainArea;
}

void TriggerColorizerFgColorEditedCommand::undo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    int id = mpItemTrigger->getID();
    mpEditor->selectTriggerByID(id);
    mpItem = mpTreeWidgetTriggers->currentItem();

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
    int id = mpItemTrigger->getID();
    mpEditor->selectTriggerByID(id);
    mpItem = mpTreeWidgetTriggers->currentItem();

    mpTriggersMainArea->pushButtonFgColor->blockSignals(true);
    mpTriggersMainArea->pushButtonFgColor->setProperty("baseColor", mFgColor);
    const bool keepColor = mFgColor == QColorConstants::Transparent.name();
    mpTriggersMainArea->pushButtonFgColor->setText(keepColor ? mpEditor->tr("keep") : QString());
    mpTriggersMainArea->pushButtonFgColor->setStyleSheet(mpEditor->generateButtonStyleSheet(mFgColor));
    mpTriggersMainArea->pushButtonFgColor->blockSignals(false);
    setText(QObject::tr("Edit trigger fg color"));
}

TriggerPerlSlashGOptionEditedCommand::TriggerPerlSlashGOptionEditedCommand(Host* pHost, dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
{
    mpTriggersMainArea = triggersMainArea;
}

void TriggerPerlSlashGOptionEditedCommand::undo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    int id = mpItemTrigger->getID();
    mpEditor->selectTriggerByID(id);
    mpItem = mpTreeWidgetTriggers->currentItem();

    mpTriggersMainArea->checkBox_perlSlashGOption->blockSignals(true);
    mpTriggersMainArea->checkBox_perlSlashGOption->setChecked(mPrevPerlSlashGOption);
    mpTriggersMainArea->checkBox_perlSlashGOption->blockSignals(false);
}

void TriggerPerlSlashGOptionEditedCommand::redo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    int id = mpItemTrigger->getID();
    mpEditor->selectTriggerByID(id);
    mpItem = mpTreeWidgetTriggers->currentItem();

    mpTriggersMainArea->checkBox_perlSlashGOption->blockSignals(true);
    mpTriggersMainArea->checkBox_perlSlashGOption->setChecked(mPerlSlashGOption);
    mpTriggersMainArea->checkBox_perlSlashGOption->blockSignals(false);
    setText(QObject::tr("Edit Perl Option"));
}

TriggerGroupFilterEditedCommand::TriggerGroupFilterEditedCommand(Host* pHost, dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
{
    mpTriggersMainArea = triggersMainArea;
}

void TriggerGroupFilterEditedCommand::undo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    int id = mpItemTrigger->getID();
    mpEditor->selectTriggerByID(id);
    mpItem = mpTreeWidgetTriggers->currentItem();

    mpTriggersMainArea->checkBox_filterTrigger->blockSignals(true);
    mpTriggersMainArea->checkBox_filterTrigger->setChecked(mPrevFilterTrigger);
    mpTriggersMainArea->checkBox_filterTrigger->blockSignals(false);
}

void TriggerGroupFilterEditedCommand::redo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    int id = mpItemTrigger->getID();
    mpEditor->selectTriggerByID(id);
    mpItem = mpTreeWidgetTriggers->currentItem();

    mpTriggersMainArea->checkBox_filterTrigger->blockSignals(true);
    mpTriggersMainArea->checkBox_filterTrigger->setChecked(mFilterTrigger);
    mpTriggersMainArea->checkBox_filterTrigger->blockSignals(false);
    setText(QObject::tr("Edit filter trigger"));
}

TriggerMultiLineEditedCommand::TriggerMultiLineEditedCommand(Host* pHost, dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
{
    mpTriggersMainArea = triggersMainArea;
}

void TriggerMultiLineEditedCommand::undo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    int id = mpItemTrigger->getID();
    mpEditor->selectTriggerByID(id);
    mpItem = mpTreeWidgetTriggers->currentItem();

    mpTriggersMainArea->groupBox_multiLineTrigger->blockSignals(true);
    mpTriggersMainArea->groupBox_multiLineTrigger->setChecked(mPrevMultiLineTrigger);
    mpTriggersMainArea->groupBox_multiLineTrigger->blockSignals(false);
}

void TriggerMultiLineEditedCommand::redo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    int id = mpItemTrigger->getID();
    mpEditor->selectTriggerByID(id);
    mpItem = mpTreeWidgetTriggers->currentItem();

    mpTriggersMainArea->groupBox_multiLineTrigger->blockSignals(true);
    mpTriggersMainArea->groupBox_multiLineTrigger->setChecked(mMultiLineTrigger);
    mpTriggersMainArea->groupBox_multiLineTrigger->blockSignals(false);
    setText(QObject::tr("Edit multiline trigger"));
}

TriggerLineMarginEditedCommand::TriggerLineMarginEditedCommand(Host* pHost, dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
{
    mpTriggersMainArea = triggersMainArea;
}

void TriggerLineMarginEditedCommand::undo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    int id = mpItemTrigger->getID();
    mpEditor->selectTriggerByID(id);
    mpItem = mpTreeWidgetTriggers->currentItem();

    mpTriggersMainArea->spinBox_lineMargin->blockSignals(true);
    mpTriggersMainArea->spinBox_lineMargin->setValue(mPrevLineMargin);
    mpTriggersMainArea->spinBox_lineMargin->blockSignals(false);
}

void TriggerLineMarginEditedCommand::redo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    int id = mpItemTrigger->getID();
    mpEditor->selectTriggerByID(id);
    mpItem = mpTreeWidgetTriggers->currentItem();

    mpTriggersMainArea->spinBox_lineMargin->blockSignals(true);
    mpTriggersMainArea->spinBox_lineMargin->setValue(mLineMargin);
    mpTriggersMainArea->spinBox_lineMargin->blockSignals(false);
    setText(QObject::tr("Edit line margin"));
}

TriggerLineEditPatternItemEditedCommand::TriggerLineEditPatternItemEditedCommand(Host* pHost, dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
{
    mpTriggersMainArea = triggersMainArea;
}

void TriggerLineEditPatternItemEditedCommand::undo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    int id = mpItemTrigger->getID();
    mpEditor->selectTriggerByID(id);
    mpItem = mpTreeWidgetTriggers->currentItem();
    mpScrollArea->ensureWidgetVisible(mpPatternItem);

    pBox->blockSignals(true);
    pBox->setCurrentIndex(mPrevTriggerPatternEdit);
    pBox->blockSignals(false);
    pBox->currentIndexChanged(mPrevTriggerPatternEdit);
}

void TriggerLineEditPatternItemEditedCommand::redo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    int id = mpItemTrigger->getID();
    mpEditor->selectTriggerByID(id);
    mpItem = mpTreeWidgetTriggers->currentItem();
    mpScrollArea->ensureWidgetVisible(mpPatternItem);

    pBox->blockSignals(true);
    pBox->setCurrentIndex(mTriggerPatternEdit);
    pBox->blockSignals(false);
    pBox->currentIndexChanged(mTriggerPatternEdit);
    setText(QObject::tr("Edit line pattern"));
}

TriggerLineEditPatternEditedCommand::TriggerLineEditPatternEditedCommand(Host* pHost, dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
{
    mpTriggersMainArea = triggersMainArea;
}

void TriggerLineEditPatternEditedCommand::undo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    int tID = mpItemTrigger->getID();
    mpEditor->selectTriggerByID(tID);
    mpItem = mpTreeWidgetTriggers->currentItem();

    mpTriggerPattern->lineEdit_pattern->blockSignals(true);
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
    mpEditor->slot_saveSelectedItem(mpItem);
}

void TriggerLineEditPatternEditedCommand::redo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    int id = mpItemTrigger->getID();
    mpEditor->selectTriggerByID(id);
    mpItem = mpTreeWidgetTriggers->currentItem();

    mpTriggerPattern->lineEdit_pattern->blockSignals(true);
    mpTriggerPattern->lineEdit_pattern->setText(mLineEditTriggerPattern);

    if (!mLineEditTriggerPattern.isEmpty()) {
        dlgTriggerPatternEdit* pPatternItem = mpTriggerPatternEdit[mRow];
        pPatternItem->lineEdit_pattern->setEnabled(true);
    }
    dlgTriggerPatternEdit* pPatternItem = mpTriggerPatternEdit[mRow + 1];
    pPatternItem->lineEdit_pattern->blockSignals(true);
    pPatternItem->lineEdit_pattern->setEnabled(true);
    pPatternItem->lineEdit_pattern->blockSignals(false);
    mpEditor->slot_saveSelectedItem(mpItem);
    setText(QObject::tr("Edit trigger pattern"));
}

TriggerLineSpacerEditedCommand::TriggerLineSpacerEditedCommand(Host* pHost, dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
{
    mpTriggersMainArea = triggersMainArea;
}

void TriggerLineSpacerEditedCommand::undo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    int id = mpItemTrigger->getID();
    mpEditor->selectTriggerByID(id);
    mpItem = mpTreeWidgetTriggers->currentItem();

    mpPatternItem->spinBox_lineSpacer->blockSignals(true);
    mpPatternItem->spinBox_lineSpacer->setValue(mPrevLineSpacer);
    mpPatternItem->spinBox_lineSpacer->blockSignals(false);
}

void TriggerLineSpacerEditedCommand::redo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    int id = mpItemTrigger->getID();
    mpEditor->selectTriggerByID(id);
    mpItem = mpTreeWidgetTriggers->currentItem();

    mpPatternItem->spinBox_lineSpacer->blockSignals(true);
    mpPatternItem->spinBox_lineSpacer->setValue(mLineSpacer);
    mpPatternItem->spinBox_lineSpacer->blockSignals(false);
    setText(QObject::tr("Edit line spacer"));
}

TriggerColorFGEditedCommand::TriggerColorFGEditedCommand(Host* pHost, dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
{
    mpTriggersMainArea = triggersMainArea;
}

void TriggerColorFGEditedCommand::undo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    int id = mpItemTrigger->getID();
    mpEditor->selectTriggerByID(id);
    mpItem = mpTreeWidgetTriggers->currentItem();

    QString styleSheet;
    if (mPrevColorTriggerFgColor.isValid()) {
        styleSheet = mpEditor->generateButtonStyleSheet(mPrevColorTriggerFgColor);
    }
    mpItemTrigger->mColorTriggerFgAnsi = mPrevColorTriggerFgAnsi;
    mpPushButton->setStyleSheet(styleSheet);
    mpPatternItem->lineEdit_pattern->setText(TTrigger::createColorPatternText(mpItemTrigger->mColorTriggerFgAnsi, mpItemTrigger->mColorTriggerBgAnsi));

    if (mpItemTrigger->mColorTriggerFgAnsi == TTrigger::scmIgnored) {
        //: Color trigger ignored foreground color button, ensure all three instances have the same text
        mpPushButton->setText(mpEditor->tr("Foreground color ignored"));
    } else if (mpItemTrigger->mColorTriggerFgAnsi == TTrigger::scmDefault) {
        //: Color trigger default foreground color button, ensure all three instances have the same text
        mpPushButton->setText(mpEditor->tr("Default foreground color"));
    } else {
        //: Color trigger ANSI foreground color button, ensure all three instances have the same text
        mpPushButton->setText(mpEditor->tr("Foreground color [ANSI %1]").arg(QString::number(mpItemTrigger->mColorTriggerFgAnsi)));
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
    int id = mpItemTrigger->getID();
    mpEditor->selectTriggerByID(id);
    mpItem = mpTreeWidgetTriggers->currentItem();

    QString styleSheet;
    if (mColorTriggerFgColor.isValid()) {
        styleSheet = mpEditor->generateButtonStyleSheet(mColorTriggerFgColor);
    }
    mpItemTrigger->mColorTriggerFgAnsi = mColorTriggerFgAnsi;
    mpPushButton->setStyleSheet(styleSheet);
    mpPatternItem->lineEdit_pattern->setText(TTrigger::createColorPatternText(mpItemTrigger->mColorTriggerFgAnsi, mpItemTrigger->mColorTriggerBgAnsi));

    if (mpItemTrigger->mColorTriggerFgAnsi == TTrigger::scmIgnored) {
        //: Color trigger ignored foreground color button, ensure all three instances have the same text
        mpPushButton->setText(mpEditor->tr("Foreground color ignored"));
    } else if (mpItemTrigger->mColorTriggerFgAnsi == TTrigger::scmDefault) {
        //: Color trigger default foreground color button, ensure all three instances have the same text
        mpPushButton->setText(mpEditor->tr("Default foreground color"));
    } else {
        //: Color trigger ANSI foreground color button, ensure all three instances have the same text
        mpPushButton->setText(mpEditor->tr("Foreground color [ANSI %1]").arg(QString::number(mpItemTrigger->mColorTriggerFgAnsi)));
    }
    dlgTriggerPatternEdit* pPatternItem = mpTriggerPatternEdit[mRow + 1];
    pPatternItem->lineEdit_pattern->blockSignals(true);
    pPatternItem->lineEdit_pattern->setEnabled(true);
    pPatternItem->lineEdit_pattern->blockSignals(false);
    setText(QObject::tr("Edit FG Color"));
}

TriggerColorBGEditedCommand::TriggerColorBGEditedCommand(Host* pHost, dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent) : QUndoCommand(parent), mpHost(pHost)
{
    mpTriggersMainArea = triggersMainArea;
}

void TriggerColorBGEditedCommand::undo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    int id = mpItemTrigger->getID();
    mpEditor->selectTriggerByID(id);
    mpItem = mpTreeWidgetTriggers->currentItem();
    mpScrollArea->ensureWidgetVisible(mpPatternItem);
    QString styleSheet;
    if (mPrevColorTriggerBgColor.isValid()) {
        styleSheet = mpEditor->generateButtonStyleSheet(mPrevColorTriggerBgColor);
    }
    mpItemTrigger->mColorTriggerBgAnsi = mPrevColorTriggerBgAnsi;
    mpPushButton->setStyleSheet(styleSheet);
    mpPatternItem->lineEdit_pattern->setText(TTrigger::createColorPatternText(mpItemTrigger->mColorTriggerFgAnsi, mpItemTrigger->mColorTriggerBgAnsi));

    if (mpItemTrigger->mColorTriggerBgAnsi == TTrigger::scmIgnored) {
        //: Color trigger ignored background color button, ensure all three instances have the same text
        mpPushButton->setText(mpEditor->tr("Background color ignored"));
    } else if (mpItemTrigger->mColorTriggerBgAnsi == TTrigger::scmDefault) {
        //: Color trigger default background color button, ensure all three instances have the same text
        mpPushButton->setText(mpEditor->tr("Default background color"));
    } else {
        //: Color trigger ANSI background color button, ensure all three instances have the same text
        mpPushButton->setText(mpEditor->tr("Background color [ANSI %1]").arg(QString::number(mpItemTrigger->mColorTriggerBgAnsi)));
    }
}

void TriggerColorBGEditedCommand::redo()
{
    if (!mpItem || !mpEditor) {
        return;
    }
    int id = mpItemTrigger->getID();
    mpEditor->selectTriggerByID(id);
    mpItem = mpTreeWidgetTriggers->currentItem();
    mpScrollArea->ensureWidgetVisible(mpPatternItem);
    QString styleSheet;
    if (mColorTriggerBgColor.isValid()) {
        styleSheet = mpEditor->generateButtonStyleSheet(mColorTriggerBgColor);
    }
    mpItemTrigger->mColorTriggerBgAnsi = mColorTriggerBgAnsi;
    mpPushButton->setStyleSheet(styleSheet);
    mpPatternItem->lineEdit_pattern->setText(TTrigger::createColorPatternText(mpItemTrigger->mColorTriggerFgAnsi, mpItemTrigger->mColorTriggerBgAnsi));

    if (mpItemTrigger->mColorTriggerBgAnsi == TTrigger::scmIgnored) {
        //: Color trigger ignored background color button, ensure all three instances have the same text
        mpPushButton->setText(mpEditor->tr("Background color ignored"));
    } else if (mpItemTrigger->mColorTriggerBgAnsi == TTrigger::scmDefault) {
        //: Color trigger default background color button, ensure all three instances have the same text
        mpPushButton->setText(mpEditor->tr("Default background color"));
    } else {
        //: Color trigger ANSI background color button, ensure all three instances have the same text
        mpPushButton->setText(mpEditor->tr("Background color [ANSI %1]").arg(QString::number(mpItemTrigger->mColorTriggerBgAnsi)));
    }
    setText(QObject::tr("Edit BG Color"));
}
