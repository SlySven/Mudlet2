#ifndef MUDLET_DLGTRIGGERSCOMMANDS_H
#define MUDLET_DLGTRIGGERSCOMMANDS_H

#include "TTreeWidget.h"
#include "dlgTriggerEditor.h"
#include <QListWidgetItem>
#include <QPointer>
#include <QUndoCommand>
class Host;
class LuaInterface;
class dlgTriggerEditor;
class dlgTriggerPatternEdit;
class dlgAliasMainArea;
class dlgScriptsMainArea;
class dlgKeysMainArea;
class dlgActionMainArea;
class VarUnit;
class XMLImport;

class AddTriggerCommand : public QUndoCommand
{
public:
    AddTriggerCommand(Host* pHost, TTreeWidget* treeWidgetTriggers, bool isFolder, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    TTrigger* mpItemTrigger = nullptr;
    QModelIndex mCurrentIndex;

private:
    QPointer<Host> mpHost;
    QTreeWidgetItem* mpItem = nullptr;
    QTreeWidgetItem* mpParent = nullptr;
    TTreeWidget* mpTreeWidgetTriggers;
    bool mIsFolder;
    int mSiblingRow;
};

class PasteTriggerCommand : public QUndoCommand
{
public:
    PasteTriggerCommand(Host* pHost, TTreeWidget* treeWidgetTriggers, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    QTreeWidgetItem* mpItem = nullptr;
    QTreeWidgetItem* mpParent = nullptr;
    QString xml;
    int mImportedItemID;

private:
    QPointer<Host> mpHost;
    TTreeWidget* mpTreeWidgetTriggers;
    TTrigger* mpItemTrigger;
    int mSiblingRow;
};

class DeleteTriggerCommand : public QUndoCommand
{
public:
    DeleteTriggerCommand(Host* pHost, QTreeWidgetItem* pItem, TTreeWidget* treeWidgetTriggers, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    QPointer<Host> mpHost;
    QModelIndex mCurrentIndex;

private:
    QTreeWidgetItem* mpItem;
    QTreeWidgetItem* mpParent = nullptr;
    TTreeWidget* mpTreeWidgetTriggers;
    TTrigger* mpItemTrigger;
    QList<TTrigger*> mpItemTriggerList;
    QString mCommand;
    bool mIsFolder;
    bool mIsActive;
    bool mIsMultiline;
    bool mPerlSlashGOption;
    bool mFilterTrigger;
    bool mSoundTrigger;
    QString mSoundFile;
    bool mIsColorizerTrigger;
    QColor mFgColor;
    QColor mBgColor;
    int mStayOpen;
    int mConditionLineDelta;
    int mSiblingRow;
    int mParentRow;
    int mParentID;
    void recurseTriggers(QTreeWidgetItem* mpItem, QPointer<Host> mpHost);
};

class MoveTriggerCommand : public QUndoCommand, QTreeWidget
{
public:
    MoveTriggerCommand(Host* pHost,
                       TTreeWidget* treeWidgetTriggers,
                       int childID,
                       int oldParentID,
                       int newParentID,
                       int parentPosition,
                       int childPosition,
                       int prevParentPosition,
                       int prevChildPosition,
                       QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    QPointer<Host> mpHost;
    QTreeWidgetItem* mpParentItem;
    QTreeWidgetItem* mpPrevParentItem;
    QTreeWidgetItem* mpItem;
    TTrigger* mpItemTrigger;
    TTreeWidget* mpTreeWidgetTriggers;
    int mChildID;
    int mOldParentID;
    int mNewParentID;
    int mParentPosition;
    int mChildPosition;
    int mPrevParentPosition;
    int mPrevChildPosition;
    QModelIndex mParent;
    int mStart;
    int mEnd;

private:
    bool mIsFolder;
};

class AddAliasCommand : public QUndoCommand
{
public:
    AddAliasCommand(Host* pHost, TTreeWidget* treeWidgetTriggers, bool isFolder, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    TAlias* mpItemAlias;
    QModelIndex mCurrentIndex;

private:
    QPointer<Host> mpHost;
    QTreeWidgetItem* mpItem = nullptr;
    QTreeWidgetItem* mpParent = nullptr;
    TTreeWidget* mpTreeWidgetAliases;
    bool mIsFolder;
    int mSiblingRow;
    int mItemID;
};

class PasteAliasCommand : public QUndoCommand
{
public:
    PasteAliasCommand(Host* pHost, TTreeWidget* treeWidgetAliases, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    QTreeWidgetItem* mpItem = nullptr;
    QTreeWidgetItem* mpParent = nullptr;
    TAlias* mpItemAlias;
    int mImportedItemID;

private:
    QPointer<Host> mpHost;
    TTreeWidget* mpTreeWidgetAliases;
    int mSiblingRow;
};

class DeleteAliasCommand : public QUndoCommand
{
public:
    DeleteAliasCommand(Host* pHost, QTreeWidgetItem* pItem, TTreeWidget* treeWidgetAliases, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    QPointer<Host> mpHost;
    TAlias* mpItemAlias;
    QList<TAlias*> mpItemAliasList;
    QModelIndex mCurrentIndex;

private:
    QTreeWidgetItem* mpItem;
    QTreeWidgetItem* mpParent = nullptr;
    TTreeWidget* mpTreeWidgetAliases;
    int mSiblingRow;
    bool mIsFolder;
    bool mIsActive;
    int mParentRow;
    int mParentID;
    QString mAliasName;
    QString mAliasCommand;
    QString mAliasPattern;
    void recurseAliases(QTreeWidgetItem* mpItem, QPointer<Host> mpHost);
};

class MoveAliasCommand : public QUndoCommand, QTreeWidget
{
public:
    MoveAliasCommand(Host* pHost,
                     TTreeWidget* treeWidgetAliases,
                     int childID,
                     int oldParentID,
                     int newParentID,
                     int parentPosition,
                     int childPosition,
                     int prevParentPosition,
                     int prevChildPosition,
                     QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    QPointer<Host> mpHost;
    QTreeWidgetItem* mpParentItem;
    QTreeWidgetItem* mpPrevParentItem;
    QTreeWidgetItem* mpItem;
    TAlias* mpItemAlias;
    TTreeWidget* mpTreeWidgetAliases;
    int mChildID;
    int mOldParentID;
    int mNewParentID;
    int mParentPosition;
    int mChildPosition;
    int mPrevParentPosition;
    int mPrevChildPosition;
    QModelIndex mParent;
    int mStart;
    int mEnd;

private:
    bool mIsFolder;
};

class AliasNameTextEditedCommand : public QUndoCommand
{
public:
    AliasNameTextEditedCommand(Host* pHost, dlgAliasMainArea* triggersMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    QPointer<Host> mpHost;
    dlgAliasMainArea* mpAliasMainArea = nullptr;
    TTreeWidget* mpTreeWidgetAliases;
    QTreeWidgetItem* mpItem;
    QString mPrevAliasName;
    QString mAliasName;
    int mItemID;
};

class AliasCommandTextEditedCommand : public QUndoCommand
{
public:
    AliasCommandTextEditedCommand(Host* pHost, dlgAliasMainArea* triggersMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    QPointer<Host> mpHost;
    dlgAliasMainArea* mpAliasMainArea = nullptr;
    TTreeWidget* mpTreeWidgetAliases;
    QTreeWidgetItem* mpItem;
    QString mPrevAliasCommand;
    QString mAliasCommand;
    int mItemID;
};

class AliasPatternTextEditedCommand : public QUndoCommand
{
public:
    AliasPatternTextEditedCommand(Host* pHost, dlgAliasMainArea* triggersMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    QPointer<Host> mpHost;
    dlgAliasMainArea* mpAliasMainArea = nullptr;
    TTreeWidget* mpTreeWidgetAliases;
    QTreeWidgetItem* mpItem;
    QString mPrevAliasPattern;
    QString mAliasPattern;
    int mItemID;
};

class AddTimerCommand : public QUndoCommand
{
public:
    AddTimerCommand(Host* pHost, TTreeWidget* treeWidgetTriggers, bool isFolder, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    TTimer* mpItemTimer;
    QModelIndex mCurrentIndex;
    int mItemID;

private:
    QTreeWidgetItem* mpItem = nullptr;
    QTreeWidgetItem* mpParent = nullptr;
    TTreeWidget* mpTreeWidgetTimers;
    QPointer<Host> mpHost;
    bool mIsFolder;
    int mSiblingRow;
};

class PasteTimerCommand : public QUndoCommand
{
public:
    PasteTimerCommand(Host* pHost, TTreeWidget* treeWidgetTimers, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    QTreeWidgetItem* mpItem = nullptr;
    QTreeWidgetItem* mpParent = nullptr;
    TTimer* mpItemTimer;
    int mImportedItemID;

private:
    QPointer<Host> mpHost;
    TTreeWidget* mpTreeWidgetTimers;
    int mSiblingRow;
};

class DeleteTimerCommand : public QUndoCommand
{
public:
    DeleteTimerCommand(Host* pHost, QTreeWidgetItem* pItem, TTreeWidget* treeWidgetTimers, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor;
    QPointer<Host> mpHost;
    TTimer* mpItemTimer;
    QModelIndex mCurrentIndex;
    QList<TTimer*> mpItemTimerList;

private:
    QTreeWidgetItem* mpItem;
    QTreeWidgetItem* mpParent = nullptr;
    TTreeWidget* mpTreeWidgetTimers;
    int mSiblingRow;
    bool mIsFolder;
    bool mIsActive;
    int mParentRow;
    int mParentID;
    QString mTimerName;
    QString mTimerCommand;
    QString mTimerPattern;
    void recurseTimers(QTreeWidgetItem* mpItem, QPointer<Host> mpHost);
};

class MoveTimerCommand : public QUndoCommand, QTreeWidget
{
public:
    MoveTimerCommand(Host* pHost,
                     TTreeWidget* treeWidgetTimers,
                     int childID,
                     int oldParentID,
                     int newParentID,
                     int parentPosition,
                     int childPosition,
                     int prevParentPosition,
                     int prevChildPosition,
                     QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    QPointer<Host> mpHost;
    QTreeWidgetItem* mpParentItem;
    QTreeWidgetItem* mpPrevParentItem;
    QTreeWidgetItem* mpItem;
    TTimer* mpItemTimer;
    TTreeWidget* mpTreeWidgetTimers;
    int mChildID;
    int mOldParentID;
    int mNewParentID;
    int mParentPosition;
    int mChildPosition;
    int mPrevParentPosition;
    int mPrevChildPosition;
    QModelIndex mParent;
    int mStart;
    int mEnd;

private:
    bool mIsFolder;
};

class TimerNameTextEditedCommand : public QUndoCommand
{
public:
    TimerNameTextEditedCommand(Host* pHost, dlgTimersMainArea* timersMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    QPointer<Host> mpHost;
    dlgTimersMainArea* mpTimersMainArea = nullptr;
    TTreeWidget* mpTreeWidgetTimers;
    QTreeWidgetItem* mpItem = nullptr;
    TTimer* mpItemTimer;
    QString mPrevTimerName;
    QString mTimerName;
    int mItemID;
};

class TimerCommandTextEditedCommand : public QUndoCommand
{
public:
    TimerCommandTextEditedCommand(Host* pHost, dlgTimersMainArea* timersMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    QPointer<Host> mpHost;
    dlgTimersMainArea* mpTimersMainArea = nullptr;
    TTreeWidget* mpTreeWidgetTimers;
    QTreeWidgetItem* mpItem = nullptr;
    TTimer* mpItemTimer;
    QString mPrevTimerCommand;
    QString mTimerCommand;
    int mItemID;
};

class TimerHoursTextEditedCommand : public QUndoCommand
{
public:
    TimerHoursTextEditedCommand(Host* pHost, dlgTimersMainArea* timersMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    QPointer<Host> mpHost;
    dlgTimersMainArea* mpTimersMainArea = nullptr;
    TTreeWidget* mpTreeWidgetTimers;
    QTreeWidgetItem* mpItem = nullptr;
    TTimer* mpItemTimer;
    int mPrevTimerHours;
    int mTimerHours;
    int mItemID;
};

class TimerMinutesTextEditedCommand : public QUndoCommand
{
public:
    TimerMinutesTextEditedCommand(Host* pHost, dlgTimersMainArea* timersMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    QPointer<Host> mpHost;
    dlgTimersMainArea* mpTimersMainArea = nullptr;
    TTreeWidget* mpTreeWidgetTimers;
    QTreeWidgetItem* mpItem = nullptr;
    TTimer* mpItemTimer;
    int mPrevTimerMinutes;
    int mTimerMinutes;
    int mItemID;
};

class TimerSecondsTextEditedCommand : public QUndoCommand
{
public:
    TimerSecondsTextEditedCommand(Host* pHost, dlgTimersMainArea* timersMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    QPointer<Host> mpHost;
    dlgTimersMainArea* mpTimersMainArea = nullptr;
    TTreeWidget* mpTreeWidgetTimers;
    QTreeWidgetItem* mpItem = nullptr;
    TTimer* mpItemTimer;
    int mPrevTimerSeconds;
    int mTimerSeconds;
    int mItemID;
};

class TimerMilliSecondsTextEditedCommand : public QUndoCommand
{
public:
    TimerMilliSecondsTextEditedCommand(Host* pHost, dlgTimersMainArea* timersMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    QPointer<Host> mpHost;
    dlgTimersMainArea* mpTimersMainArea = nullptr;
    TTreeWidget* mpTreeWidgetTimers;
    QTreeWidgetItem* mpItem = nullptr;
    TTimer* mpItemTimer;
    int mPrevTimerMsecs;
    int mTimerMsecs;
    int mItemID;
};

class AddScriptCommand : public QUndoCommand
{
public:
    AddScriptCommand(Host* pHost, TTreeWidget* treeWidgetScripts, bool isFolder, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    TScript* mpItemScript;
    QModelIndex mCurrentIndex;
    int mItemID;

private:
    QPointer<Host> mpHost;
    QTreeWidgetItem* mpItem = nullptr;
    QTreeWidgetItem* mpParent = nullptr;
    TTreeWidget* mpTreeWidgetScripts;
    bool mIsFolder;
    int mSiblingRow;
};

class PasteScriptCommand : public QUndoCommand
{
public:
    PasteScriptCommand(Host* pHost, TTreeWidget* treeWidgetScripts, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    QTreeWidgetItem* mpItem = nullptr;
    QTreeWidgetItem* mpParent = nullptr;
    int mImportedItemID;

private:
    QPointer<Host> mpHost;
    TTreeWidget* mpTreeWidgetScripts;
    TScript* mpItemScript;
    int mSiblingRow;
};

class DeleteScriptCommand : public QUndoCommand
{
public:
    DeleteScriptCommand(Host* pHost, QTreeWidgetItem* pItem, TTreeWidget* treeWidgetScripts, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    QPointer<Host> mpHost;
    QModelIndex mCurrentIndex;
    QList<TScript*> mpItemScriptList;

private:
    QTreeWidgetItem* mpItem;
    QTreeWidgetItem* mpParent = nullptr;
    TTreeWidget* mpTreeWidgetScripts;
    TScript* mpItemScript;
    int mSiblingRow;
    int mParentRow;
    int mParentID;
    bool mIsFolder;
    bool mIsActive;
    void recurseScripts(QTreeWidgetItem* mpItem, QPointer<Host> mpHost);
};

class MoveScriptCommand : public QUndoCommand, QTreeWidget
{
public:
    MoveScriptCommand(Host* pHost,
                      TTreeWidget* treeWidgetScripts,
                      int childID,
                      int oldParentID,
                      int newParentID,
                      int parentPosition,
                      int childPosition,
                      int prevParentPosition,
                      int prevChildPosition,
                      QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    QPointer<Host> mpHost;
    QTreeWidgetItem* mpParentItem;
    QTreeWidgetItem* mpPrevParentItem;
    QTreeWidgetItem* mpItem;
    TScript* mpItemScript;
    TTreeWidget* mpTreeWidgetScripts;
    int mChildID;
    int mOldParentID;
    int mNewParentID;
    int mParentPosition;
    int mChildPosition;
    int mPrevParentPosition;
    int mPrevChildPosition;
    QModelIndex mParent;
    int mStart;
    int mEnd;

private:
    bool mIsFolder;
};

class ScriptNameTextEditedCommand : public QUndoCommand
{
public:
    ScriptNameTextEditedCommand(Host* pHost, dlgScriptsMainArea* scriptsMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    dlgScriptsMainArea* mpScriptsMainArea = nullptr;
    QPointer<Host> mpHost;
    TTreeWidget* mpTreeWidgetScripts;
    QTreeWidgetItem* mpItem;
    TScript* mpItemScript;
    QString mPrevScriptName;
    QString mScriptName;
    int mItemID;
};

class ScriptAddHandlerCommand : public QUndoCommand
{
public:
    ScriptAddHandlerCommand(Host* pHost, dlgScriptsMainArea* scriptsMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    dlgScriptsMainArea* mpScriptsMainArea = nullptr;
    QPointer<Host> mpHost;
    TTreeWidget* mpTreeWidgetScripts;
    QTreeWidgetItem* mpItem;
    QString mPrevScriptEventhandler;
    QString mScriptEventhandler;
    QListWidgetItem* mpWidgetItem;
    TScript* mpItemScript;
    int mRow;
    int mItemID;
};

class ScriptRemoveHandlerCommand : public QUndoCommand
{
public:
    ScriptRemoveHandlerCommand(Host* pHost, dlgScriptsMainArea* scriptsMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    dlgScriptsMainArea* mpScriptsMainArea = nullptr;
    QPointer<Host> mpHost;
    TTreeWidget* mpTreeWidgetScripts;
    QTreeWidgetItem* mpItem;
    QString mPrevScriptEventhandler;
    QString mScriptEventhandler;
    QListWidgetItem* mpWidgetItem;
    TScript* mpItemScript;
    int mRow;
    int mItemID;
};

class AddKeyCommand : public QUndoCommand
{
public:
    AddKeyCommand(Host* pHost, TTreeWidget* treeWidgetKeys, bool isFolder, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    TKey* mpItemKey;
    QModelIndex mCurrentIndex;

private:
    QTreeWidgetItem* mpItem = nullptr;
    QTreeWidgetItem* mpParent = nullptr;
    TTreeWidget* mpTreeWidgetKeys;
    QPointer<Host> mpHost;
    bool mIsFolder;
    int mSiblingRow;
    int mItemID;
};

class PasteKeyCommand : public QUndoCommand
{
public:
    PasteKeyCommand(Host* pHost, TTreeWidget* treeWidgetKeys, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    QTreeWidgetItem* mpItem = nullptr;
    QTreeWidgetItem* mpParent = nullptr;
    TKey* mpItemKey;
    int mImportedItemID;

private:
    QPointer<Host> mpHost;
    TTreeWidget* mpTreeWidgetKeys;
    int mSiblingRow;
};

class DeleteKeyCommand : public QUndoCommand
{
public:
    DeleteKeyCommand(Host* pHost, QTreeWidgetItem* pItem, TTreeWidget* treeWidgetKeys, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    TKey* mpItemKey;
    QModelIndex mCurrentIndex;
    QList<TKey*> mpItemKeyList;
    int mParentRow;

private:
    QTreeWidgetItem* mpItem;
    QTreeWidgetItem* mpParent = nullptr;
    TTreeWidget* mpTreeWidgetKeys;
    QPointer<Host> mpHost;
    int mSiblingRow;
    bool mIsFolder;
    bool mIsActive;
    int mParentID;
    void recurseKeys(QTreeWidgetItem* mpItem, QPointer<Host> mpHost);
};

class MoveKeyCommand : public QUndoCommand, QTreeWidget
{
public:
    MoveKeyCommand(Host* pHost,
                   TTreeWidget* treeWidgetKeys,
                   int childID,
                   int oldParentID,
                   int newParentID,
                   int parentPosition,
                   int childPosition,
                   int prevParentPosition,
                   int prevChildPosition,
                   QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    QTreeWidgetItem* mpParentItem;
    QTreeWidgetItem* mpPrevParentItem;
    QTreeWidgetItem* mpItem;
    TKey* mpItemKey;
    TTreeWidget* mpTreeWidgetKeys;
    int mChildID;
    int mOldParentID;
    int mNewParentID;
    int mParentPosition;
    int mChildPosition;
    int mPrevParentPosition;
    int mPrevChildPosition;
    QModelIndex mParent;
    int mStart;
    int mEnd;

private:
    QPointer<Host> mpHost;
    bool mIsFolder;
};

class KeyNameTextEditedCommand : public QUndoCommand
{
public:
    KeyNameTextEditedCommand(Host* pHost, dlgKeysMainArea* keysMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    dlgKeysMainArea* mpKeysMainArea = nullptr;
    QPointer<Host> mpHost;
    TTreeWidget* mpTreeWidgetKeys;
    QTreeWidgetItem* mpItem;
    TKey* mpItemKey;
    QString mPrevKeyName;
    QString mKeyName;
    int mItemID;
};

class KeyCommandTextEditedCommand : public QUndoCommand
{
public:
    KeyCommandTextEditedCommand(Host* pHost, dlgKeysMainArea* keysMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    dlgKeysMainArea* mpKeysMainArea = nullptr;
    TTreeWidget* mpTreeWidgetKeys;
    QTreeWidgetItem* mpItem;
    QPointer<Host> mpHost;
    TKey* mpItemKey;
    QString mPrevKeyCommand;
    QString mKeyCommand;
    int mItemID;
};

class KeyGrabTextEditedCommand : public QUndoCommand
{
public:
    KeyGrabTextEditedCommand(Host* pHost, dlgKeysMainArea* keysMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    dlgKeysMainArea* mpKeysMainArea = nullptr;
    TTreeWidget* mpTreeWidgetKeys;
    QTreeWidgetItem* mpItem;
    QPointer<Host> mpHost;
    TKey* mpItemKey;
    Qt::Key mKey;
    Qt::KeyboardModifiers mModifier;
    Qt::Key mPrevKey;
    Qt::KeyboardModifiers mPrevModifier;
    QString mPrevKeyName;
    QString mKeyName;
    int mItemID;
};

class AddActionCommand : public QUndoCommand
{
public:
    AddActionCommand(Host* pHost, TTreeWidget* treeWidgetActions, bool isFolder, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    TAction* mpItemAction;
    QModelIndex mCurrentIndex;

private:
    QTreeWidgetItem* mpItem = nullptr;
    QTreeWidgetItem* mpParent = nullptr;
    TTreeWidget* mpTreeWidgetActions;
    QPointer<Host> mpHost;
    bool mIsFolder;
    int mSiblingRow;
    int mItemID;
};

class PasteActionCommand : public QUndoCommand
{
public:
    PasteActionCommand(Host* pHost, TTreeWidget* treeWidgetActions, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    QTreeWidgetItem* mpItem = nullptr;
    QTreeWidgetItem* mpParent = nullptr;
    TAction* mpItemAction;
    int mImportedItemID;

private:
    QPointer<Host> mpHost;
    TTreeWidget* mpTreeWidgetActions;
    int mSiblingRow;
};

class DeleteActionCommand : public QUndoCommand
{
public:
    DeleteActionCommand(Host* pHost, QTreeWidgetItem* pItem, TTreeWidget* treeWidgetActions, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    QModelIndex mCurrentIndex;
    QList<TAction*> mpItemActionList;

private:
    QTreeWidgetItem* mpItem;
    QTreeWidgetItem* mpParent = nullptr;
    TTreeWidget* mpTreeWidgetActions;
    QPointer<Host> mpHost;
    TAction* mpItemAction;
    int mSiblingRow;
    int mParentRow;
    bool mIsFolder;
    bool mIsActive;
    int mParentID;
    void recurseActions(QTreeWidgetItem* mpItem, QPointer<Host> mpHost);
};
class MoveActionCommand : public QUndoCommand, QTreeWidget
{
public:
    MoveActionCommand(Host* pHost,
                      TTreeWidget* treeWidgetActions,
                      int childID,
                      int oldParentID,
                      int newParentID,
                      int parentPosition,
                      int childPosition,
                      int prevParentPosition,
                      int prevChildPosition,
                      QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    QTreeWidgetItem* mpParentItem;
    QTreeWidgetItem* mpPrevParentItem;
    QTreeWidgetItem* mpItem;
    TAction* mpItemAction;
    TTreeWidget* mpTreeWidgetActions;
    int mChildID;
    int mOldParentID;
    int mNewParentID;
    int mParentPosition;
    int mChildPosition;
    int mPrevParentPosition;
    int mPrevChildPosition;
    QModelIndex mParent;
    int mStart;
    int mEnd;

private:
    QPointer<Host> mpHost;
    bool mIsFolder;
};

class ActionNameTextEditedCommand : public QUndoCommand
{
public:
    ActionNameTextEditedCommand(Host* pHost, dlgActionMainArea* actionsMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    dlgActionMainArea* mpActionsMainArea = nullptr;
    TTreeWidget* mpTreeWidgetActions;
    QTreeWidgetItem* mpItem = nullptr;
    QPointer<Host> mpHost;
    TAction* mpItemAction;
    QString mPrevActionName;
    QString mActionName;
    int mItemID;
};

class ActionButtonRotationEditedCommand : public QUndoCommand
{
public:
    ActionButtonRotationEditedCommand(Host* pHost, dlgActionMainArea* actionsMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    dlgActionMainArea* mpActionsMainArea = nullptr;
    TTreeWidget* mpTreeWidgetActions;
    QTreeWidgetItem* mpItem;
    QPointer<Host> mpHost;
    TAction* mpItemAction;
    int mPrevRotation;
    int mRotation;
    int mItemID;
};

class ActionButtonCheckboxEditedCommand : public QUndoCommand
{
public:
    ActionButtonCheckboxEditedCommand(Host* pHost, dlgActionMainArea* actionsMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    dlgActionMainArea* mpActionsMainArea = nullptr;
    TTreeWidget* mpTreeWidgetActions;
    QTreeWidgetItem* mpItem;
    QPointer<Host> mpHost;
    TAction* mpItemAction;
    bool mPrevIsPushDown;
    bool mIsPushDown;
    int mItemID;
};

class ActionCommandDownTextEditedCommand : public QUndoCommand
{
public:
    ActionCommandDownTextEditedCommand(Host* pHost, dlgActionMainArea* actionsMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    dlgActionMainArea* mpActionsMainArea = nullptr;
    TTreeWidget* mpTreeWidgetActions;
    QTreeWidgetItem* mpItem;
    QPointer<Host> mpHost;
    TAction* mpItemAction;
    QString mPrevCommandDown;
    QString mCommandDown;
    int mItemID;
};

class ActionCommandUpTextEditedCommand : public QUndoCommand
{
public:
    ActionCommandUpTextEditedCommand(Host* pHost, dlgActionMainArea* actionsMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    dlgActionMainArea* mpActionsMainArea = nullptr;
    TTreeWidget* mpTreeWidgetActions;
    QTreeWidgetItem* mpItem;
    QPointer<Host> mpHost;
    TAction* mpItemAction;
    QString mPrevCommandUp;
    QString mCommandUp;
    int mItemID;
};

class ActionCssTextEditedCommand : public QUndoCommand
{
public:
    ActionCssTextEditedCommand(Host* pHost, dlgActionMainArea* actionsMainArea, QUndoCommand* parent = nullptr);
    int id() const override;
    bool mergeWith(const QUndoCommand* command) override;
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    dlgActionMainArea* mpActionsMainArea = nullptr;
    TTreeWidget* mpTreeWidgetActions;
    QTreeWidgetItem* mpItem;
    QPointer<Host> mpHost;
    TAction* mpItemAction;
    QString mPrevActionCss;
    QString mActionCss;
    int mItemID;
};

class AddVarCommand : public QUndoCommand
{
public:
    AddVarCommand(Host* pHost, TTreeWidget* treeWidgetVariables, bool isFolder, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    TVar* mpItemVar;

private:
    QTreeWidgetItem* mpItem = nullptr;
    QTreeWidgetItem* mpParent;
    TTreeWidget* mpTreeWidgetVariables;
    QPointer<Host> mpHost;
    bool mIsFolder;
};

class DeleteVarCommand : public QUndoCommand
{
public:
    DeleteVarCommand(Host* pHost, QTreeWidgetItem* pItem, TTreeWidget* treeWidgetVariables, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;

private:
    QTreeWidgetItem* mpItem;
    QTreeWidgetItem* mpParent;
    TTreeWidget* mpTreeWidgetVariables;
    QPointer<Host> mpHost;
    TVar* mpItemVar = nullptr;
    TVar* mpTempVar = nullptr;
};
class MoveVariableCommand : public QUndoCommand, QTreeWidget
{
public:
    MoveVariableCommand(Host* pHost, TTreeWidget* treeWidgetVariables, QTreeWidgetItem* parentItem, QTreeWidgetItem* cItem, QTreeWidgetItem* prevParentItem, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    QTreeWidgetItem* mpParentItem;
    QTreeWidgetItem* mpPrevParentItem;
    QTreeWidgetItem* mpItem;
    TVar* mpItemVar;
    TTreeWidget* mpTreeWidgetVariables;
    QDropEvent* mpEvent;

private:
    QPointer<Host> mpHost;
    bool mIsFolder;
};

class TriggerNameTextEditedCommand : public QUndoCommand
{
public:
    TriggerNameTextEditedCommand(Host* pHost, dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggersMainArea* mpTriggersMainArea = nullptr;
    QPointer<Host> mpHost;
    dlgTriggerEditor* mpEditor = nullptr;
    QTreeWidgetItem* mpItem;
    TTreeWidget* mpTreeWidgetTriggers;
    TTrigger* mpItemTrigger;
    QString mPrevLineEditTriggerName;
    QString mLineEditTriggerName;
};

class TriggerCommandTextEditedCommand : public QUndoCommand
{
public:
    TriggerCommandTextEditedCommand(Host* pHost, dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggersMainArea* mpTriggersMainArea = nullptr;
    dlgTriggerEditor* mpEditor = nullptr;
    QTreeWidgetItem* mpItem;
    TTrigger* mpItemTrigger;
    TTreeWidget* mpTreeWidgetTriggers;
    QPointer<Host> mpHost;
    QString mPrevLineEditTriggerCommand;
    QString mLineEditTriggerCommand;
};

class TriggerFireLengthEditedCommand : public QUndoCommand
{
public:
    TriggerFireLengthEditedCommand(Host* pHost, dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggersMainArea* mpTriggersMainArea = nullptr;
    dlgTriggerEditor* mpEditor = nullptr;
    QTreeWidgetItem* mpItem;
    TTrigger* mpItemTrigger;
    TTreeWidget* mpTreeWidgetTriggers;
    QPointer<Host> mpHost;
    int mPrevFireLength;
    int mFireLength;
};

class TriggerPlaySoundEditedCommand : public QUndoCommand
{
public:
    TriggerPlaySoundEditedCommand(Host* pHost, dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggersMainArea* mpTriggersMainArea = nullptr;
    dlgTriggerEditor* mpEditor = nullptr;
    QTreeWidgetItem* mpItem;
    TTrigger* mpItemTrigger;
    TTreeWidget* mpTreeWidgetTriggers;
    QPointer<Host> mpHost;
    bool mPrevGroupBoxSoundTrigger;
    bool mGroupBoxSoundTrigger;
};

class TriggerPlaySoundFileEditedCommand : public QUndoCommand
{
public:
    TriggerPlaySoundFileEditedCommand(Host* pHost, dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggersMainArea* mpTriggersMainArea = nullptr;
    dlgTriggerEditor* mpEditor = nullptr;
    QTreeWidgetItem* mpItem;
    TTreeWidget* mpTreeWidgetTriggers;
    QPointer<Host> mpHost;
    TTrigger* mpItemTrigger;
    QString mPrevLineEditSoundFile;
    QString mLineEditSoundFile;
};

class TriggerColorizerEditedCommand : public QUndoCommand
{
public:
    TriggerColorizerEditedCommand(Host* pHost, dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggersMainArea* mpTriggersMainArea = nullptr;
    dlgTriggerEditor* mpEditor = nullptr;
    QTreeWidgetItem* mpItem;
    TTreeWidget* mpTreeWidgetTriggers;
    QPointer<Host> mpHost;
    TTrigger* mpItemTrigger;
    bool mPrevBoxTriggerColorizer;
    bool mBoxTriggerColorizer;
};

class TriggerColorizerBgColorEditedCommand : public QUndoCommand
{
public:
    TriggerColorizerBgColorEditedCommand(Host* pHost, dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggersMainArea* mpTriggersMainArea = nullptr;
    dlgTriggerEditor* mpEditor = nullptr;
    QTreeWidgetItem* mpItem;
    TTreeWidget* mpTreeWidgetTriggers;
    QPointer<Host> mpHost;
    TTrigger* mpItemTrigger;
    QString mPrevbgColor;
    QString mBgColor;
};

class TriggerColorizerFgColorEditedCommand : public QUndoCommand
{
public:
    TriggerColorizerFgColorEditedCommand(Host* pHost, dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggersMainArea* mpTriggersMainArea = nullptr;
    dlgTriggerEditor* mpEditor = nullptr;
    QTreeWidgetItem* mpItem;
    TTreeWidget* mpTreeWidgetTriggers;
    QPointer<Host> mpHost;
    TTrigger* mpItemTrigger;
    QString mPrevfgColor;
    QString mFgColor;
};

class TriggerPerlSlashGOptionEditedCommand : public QUndoCommand
{
public:
    TriggerPerlSlashGOptionEditedCommand(Host* pHost, dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggersMainArea* mpTriggersMainArea = nullptr;
    dlgTriggerEditor* mpEditor = nullptr;
    QTreeWidgetItem* mpItem;
    TTreeWidget* mpTreeWidgetTriggers;
    QPointer<Host> mpHost;
    TTrigger* mpItemTrigger;
    bool mPrevPerlSlashGOption;
    bool mPerlSlashGOption;
};

class TriggerGroupFilterEditedCommand : public QUndoCommand
{
public:
    TriggerGroupFilterEditedCommand(Host* pHost, dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggersMainArea* mpTriggersMainArea = nullptr;
    dlgTriggerEditor* mpEditor = nullptr;
    QTreeWidgetItem* mpItem;
    TTreeWidget* mpTreeWidgetTriggers;
    QPointer<Host> mpHost;
    TTrigger* mpItemTrigger;
    bool mPrevFilterTrigger;
    bool mFilterTrigger;
};

class TriggerMultiLineEditedCommand : public QUndoCommand
{
public:
    TriggerMultiLineEditedCommand(Host* pHost, dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggersMainArea* mpTriggersMainArea = nullptr;
    dlgTriggerEditor* mpEditor = nullptr;
    QTreeWidgetItem* mpItem;
    TTreeWidget* mpTreeWidgetTriggers;
    QPointer<Host> mpHost;
    TTrigger* mpItemTrigger;
    bool mPrevMultiLineTrigger;
    bool mMultiLineTrigger;
};

class TriggerLineMarginEditedCommand : public QUndoCommand
{
public:
    TriggerLineMarginEditedCommand(Host* pHost, dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggersMainArea* mpTriggersMainArea = nullptr;
    dlgTriggerEditor* mpEditor = nullptr;
    QTreeWidgetItem* mpItem;
    TTreeWidget* mpTreeWidgetTriggers;
    QPointer<Host> mpHost;
    TTrigger* mpItemTrigger;
    int mPrevLineMargin;
    int mLineMargin;
};

class TriggerLineEditPatternItemEditedCommand : public QUndoCommand
{
public:
    TriggerLineEditPatternItemEditedCommand(Host* pHost, dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggersMainArea* mpTriggersMainArea = nullptr;
    dlgTriggerPatternEdit* mpPatternItem;
    dlgTriggerEditor* mpEditor = nullptr;
    QTreeWidgetItem* mpItem = nullptr;
    QScrollArea* mpScrollArea = nullptr;
    QComboBox* pBox;
    QComboBox* pBoxTemp = nullptr;
    TTreeWidget* mpTreeWidgetTriggers;
    TTrigger* mpItemTrigger;
    int mPrevTriggerPatternEdit;
    int mTriggerPatternEdit;
    int mRow;

private:
    QPointer<Host> mpHost;
};

class TriggerLineEditPatternEditedCommand : public QUndoCommand
{
public:
    TriggerLineEditPatternEditedCommand(Host* pHost, dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggersMainArea* mpTriggersMainArea = nullptr;
    dlgTriggerEditor* mpEditor = nullptr;
    QPointer<Host> mpHost;
    dlgTriggerPatternEdit* mpTriggerPattern;
    QList<dlgTriggerPatternEdit*> mpTriggerPatternEdit;
    QTreeWidgetItem* mpItem;
    TTreeWidget* mpTreeWidgetTriggers;
    TTrigger* mpItemTrigger;
    QModelIndex mItemIndex;
    QString mPrevLineEditTriggerPattern;
    QString mLineEditTriggerPattern;
    int mRow;
};

class TriggerLineSpacerEditedCommand : public QUndoCommand
{
public:
    TriggerLineSpacerEditedCommand(Host* pHost, dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggersMainArea* mpTriggersMainArea = nullptr;
    dlgTriggerPatternEdit* mpPatternItem;
    dlgTriggerEditor* mpEditor = nullptr;
    QTreeWidgetItem* mpItem;
    TTrigger* mpItemTrigger;
    TTreeWidget* mpTreeWidgetTriggers;
    QPointer<Host> mpHost;
    int mPrevLineSpacer;
    int mLineSpacer;
};

class TriggerColorFGEditedCommand : public QUndoCommand
{
public:
    TriggerColorFGEditedCommand(Host* pHost, dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggersMainArea* mpTriggersMainArea = nullptr;
    QList<dlgTriggerPatternEdit*> mpTriggerPatternEdit;
    dlgTriggerPatternEdit* mpPatternItem;
    QScrollArea* mpScrollArea = nullptr;
    QPushButton* mpPushButton;
    dlgTriggerEditor* mpEditor = nullptr;
    QTreeWidgetItem* mpItem;
    TTrigger* mpItemTrigger;
    TTreeWidget* mpTreeWidgetTriggers;
    QColor mPrevColorTriggerFgColor;
    QColor mColorTriggerFgColor;
    int mPrevColorTriggerFgAnsi;
    int mColorTriggerFgAnsi;
    int mRow;

private:
    QPointer<Host> mpHost;
};

class TriggerColorBGEditedCommand : public QUndoCommand
{
public:
    TriggerColorBGEditedCommand(Host* pHost, dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggersMainArea* mpTriggersMainArea = nullptr;
    dlgTriggerPatternEdit* mpPatternItem;
    QScrollArea* mpScrollArea = nullptr;
    QPushButton* mpPushButton;
    dlgTriggerEditor* mpEditor = nullptr;
    QTreeWidgetItem* mpItem;
    TTrigger* mpItemTrigger;
    TTreeWidget* mpTreeWidgetTriggers;
    QColor mPrevColorTriggerBgColor;
    QColor mColorTriggerBgColor;
    int mPrevColorTriggerBgAnsi;
    int mColorTriggerBgAnsi;

private:
    QPointer<Host> mpHost;
};
#endif
