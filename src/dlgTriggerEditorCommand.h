#ifndef MUDLET_DLGTRIGGERSCOMMANDS_H
#define MUDLET_DLGTRIGGERSCOMMANDS_H

//#include "ActionUnit.h"
//#include "AliasUnit.h"
//#include "KeyUnit.h"
//#include "ScriptUnit.h"
#include "TTreeWidget.h"
//#include "TimerUnit.h"
//#include "TriggerUnit.h"
//#include "VarUnit.h"
#include "dlgTriggerEditor.h"
#include <QPointer>
#include <QListWidgetItem>
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

class AddTriggerCommand : public QUndoCommand
{
public:
    AddTriggerCommand(QTreeWidgetItem* pItem, TTreeWidget* treeWidgetTriggers, bool isFolder, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;

private:
    QPointer<Host> mpHost;
    QTreeWidgetItem* mpItem;
    QTreeWidgetItem* mpParent;
    TTreeWidget* mpTreeWidgetTriggers;
    TTrigger* mpItemTrigger;
    bool mIsFolder;
};

class DeleteTriggerCommand : public QUndoCommand
{
public:
    DeleteTriggerCommand(Host* pHost, QTreeWidgetItem* pItem, TTreeWidget* treeWidgetTriggers, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    QPointer<Host> mpHost;

private:
    QTreeWidgetItem* mpItem;
    QTreeWidgetItem* mpParent;
    TTreeWidget* mpTreeWidgetTriggers;
    TTrigger* mpItemTrigger;
    bool mIsFolder;
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
    AddAliasCommand(QTreeWidgetItem* pItem,TTreeWidget* treeWidgetTriggers, bool isFolder, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;

private:
    QTreeWidgetItem* mpItem;
    QTreeWidgetItem* mpParent;
    TTreeWidget* mpTreeWidgetAliases;
    TTrigger* mpItemTrigger;
    bool mIsFolder;
};

class DeleteAliasCommand : public QUndoCommand
{
public:
    DeleteAliasCommand(Host* pHost, QTreeWidgetItem* pItem, TTreeWidget* treeWidgetAliases, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    QPointer<Host> mpHost;

private:
    QTreeWidgetItem* mpItem;
    QTreeWidgetItem* mpParent;
    TTreeWidget* mpTreeWidgetAliases;
    TAlias* mpItemAlias;
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
    TTrigger* mpItemAlias;
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
    AliasNameTextEditedCommand(dlgAliasMainArea* triggersMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgAliasMainArea* mpAliasMainArea = nullptr;
    TTreeWidget* mpTreeWidgetAliases;
    QTreeWidgetItem* mpItem;
    QString mPrevAliasName;
    QString mAliasName;
};

class AliasCommandTextEditedCommand : public QUndoCommand
{
public:
    AliasCommandTextEditedCommand(dlgAliasMainArea* triggersMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgAliasMainArea* mpAliasMainArea = nullptr;
    TTreeWidget* mpTreeWidgetAliases;
    QTreeWidgetItem* mpItem;
    QString mPrevAliasCommand;
    QString mAliasCommand;
};

class AliasPatternTextEditedCommand : public QUndoCommand
{
public:
    AliasPatternTextEditedCommand(dlgAliasMainArea* triggersMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgAliasMainArea* mpAliasMainArea = nullptr;
    TTreeWidget* mpTreeWidgetAliases;
    QTreeWidgetItem* mpItem;
    QString mPrevAliasPattern;
    QString mAliasPattern;
};

class AddTimerCommand : public QUndoCommand
{
public:
    AddTimerCommand(QTreeWidgetItem* pItem, TTreeWidget* treeWidgetTriggers, bool isFolder, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;

private:
    QTreeWidgetItem* mpItem;
    QTreeWidgetItem* mpParent;
    TTreeWidget* mpTreeWidgetTimers;
    QPointer<Host> mpHost;
    TTrigger* mpItemTrigger;
    bool mIsFolder;
};

class DeleteTimerCommand : public QUndoCommand
{
public:
    DeleteTimerCommand(Host* pHost, QTreeWidgetItem* pItem, TTreeWidget* treeWidgetTimers, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor;
    QPointer<Host> mpHost;

private:
    QTreeWidgetItem* mpItem;
    QTreeWidgetItem* mpParent;
    TTreeWidget* mpTreeWidgetTimers;
    TTimer* mpItemTimer;
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
    TimerNameTextEditedCommand(dlgTimersMainArea* timersMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTimersMainArea* mpTimersMainArea = nullptr;
    TTreeWidget* mpTreeWidgetTimers;
    QTreeWidgetItem* mpItem;
    QString mPrevTimerName;
    QString mTimerName;
};

class TimerCommandTextEditedCommand : public QUndoCommand
{
public:
    TimerCommandTextEditedCommand(dlgTimersMainArea* timersMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTimersMainArea* mpTimersMainArea = nullptr;
    TTreeWidget* mpTreeWidgetTimers;
    QTreeWidgetItem* mpItem;
    QString mPrevTimerCommand;
    QString mTimerCommand;
};

class TimerHoursTextEditedCommand : public QUndoCommand
{
public:
    TimerHoursTextEditedCommand(dlgTimersMainArea* timersMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTimersMainArea* mpTimersMainArea = nullptr;
    TTreeWidget* mpTreeWidgetTimers;
    QTreeWidgetItem* mpItem;
    int mPrevTimerHours;
    int mTimerHours;
};

class TimerMinutesTextEditedCommand : public QUndoCommand
{
public:
    TimerMinutesTextEditedCommand(dlgTimersMainArea* timersMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTimersMainArea* mpTimersMainArea = nullptr;
    TTreeWidget* mpTreeWidgetTimers;
    QTreeWidgetItem* mpItem;
    int mPrevTimerMinutes;
    int mTimerMinutes;
};

class TimerSecondsTextEditedCommand : public QUndoCommand
{
public:
    TimerSecondsTextEditedCommand(dlgTimersMainArea* timersMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTimersMainArea* mpTimersMainArea = nullptr;
    TTreeWidget* mpTreeWidgetTimers;
    QTreeWidgetItem* mpItem;
    int mPrevTimerSeconds;
    int mTimerSeconds;
};

class TimerMilliSecondsTextEditedCommand : public QUndoCommand
{
public:
    TimerMilliSecondsTextEditedCommand(dlgTimersMainArea* timersMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTimersMainArea* mpTimersMainArea = nullptr;
    TTreeWidget* mpTreeWidgetTimers;
    QTreeWidgetItem* mpItem;
    int mPrevTimerMsecs;
    int mTimerMsecs;
};

class AddScriptCommand : public QUndoCommand
{
public:
    AddScriptCommand(QTreeWidgetItem* pItem, TTreeWidget* treeWidgetScripts, bool isFolder, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;

private:
    QTreeWidgetItem* mpItem;
    QTreeWidgetItem* mpParent;
    TTreeWidget* mpTreeWidgetScripts;
    TTrigger* mpItemTrigger;
    bool mIsFolder;
};

class DeleteScriptCommand : public QUndoCommand
{
public:
    DeleteScriptCommand(Host* pHost, QTreeWidgetItem* pItem, TTreeWidget* treeWidgetScripts, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    QPointer<Host> mpHost;

private:
    QTreeWidgetItem* mpItem;
    QTreeWidgetItem* mpParent;
    TTreeWidget* mpTreeWidgetScripts;
    TScript* mpItemScript;
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
    ScriptNameTextEditedCommand(dlgScriptsMainArea* scriptsMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgScriptsMainArea* mpScriptsMainArea = nullptr;
    TTreeWidget* mpTreeWidgetScripts;
    QTreeWidgetItem* mpItem;
    QString mPrevScriptName;
    QString mScriptName;
};

class ScriptAddHandlerCommand : public QUndoCommand
{
public:
    ScriptAddHandlerCommand(dlgScriptsMainArea* scriptsMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgScriptsMainArea* mpScriptsMainArea = nullptr;
    TTreeWidget* mpTreeWidgetScripts;
    QTreeWidgetItem* mpItem;
    QString mPrevScriptEventhandler;
    QString mScriptEventhandler;
    QListWidgetItem* mpWidgetItem;
    int mRow;
};

class ScriptRemoveHandlerCommand : public QUndoCommand
{
public:
    ScriptRemoveHandlerCommand(dlgScriptsMainArea* scriptsMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgScriptsMainArea* mpScriptsMainArea = nullptr;
    TTreeWidget* mpTreeWidgetScripts;
    QTreeWidgetItem* mpItem;
    QString mPrevScriptEventhandler;
    QString mScriptEventhandler;
    QListWidgetItem* mpWidgetItem;
    int mRow;
};

class AddKeyCommand : public QUndoCommand
{
public:
    AddKeyCommand(QTreeWidgetItem* pItem, TTreeWidget* treeWidgetKeys, bool isFolder, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;

private:
    QTreeWidgetItem* mpItem;
    QTreeWidgetItem* mpParent;
    TTreeWidget* mpTreeWidgetKeys;
    TTrigger* mpItemTrigger;
    bool mIsFolder;
};

class DeleteKeyCommand : public QUndoCommand
{
public:
    DeleteKeyCommand(Host* pHost, QTreeWidgetItem* pItem, TTreeWidget* treeWidgetKeys, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;

private:
    QTreeWidgetItem* mpItem;
    QTreeWidgetItem* mpParent;
    TTreeWidget* mpTreeWidgetKeys;
    QPointer<Host> mpHost;
    TKey* mpItemKey;
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
    KeyNameTextEditedCommand(dlgKeysMainArea* keysMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgKeysMainArea* mpKeysMainArea = nullptr;
    TTreeWidget* mpTreeWidgetKeys;
    QTreeWidgetItem* mpItem;
    QString mPrevKeyName;
    QString mKeyName;
};

class KeyCommandTextEditedCommand : public QUndoCommand
{
public:
    KeyCommandTextEditedCommand(dlgKeysMainArea* keysMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgKeysMainArea* mpKeysMainArea = nullptr;
    TTreeWidget* mpTreeWidgetKeys;
    QTreeWidgetItem* mpItem;
    QString mPrevKeyCommand;
    QString mKeyCommand;
};

class KeyGrabTextEditedCommand : public QUndoCommand
{
public:
    KeyGrabTextEditedCommand(Host* pHost, dlgKeysMainArea* keysMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgKeysMainArea* mpKeysMainArea = nullptr;
    TTreeWidget* mpTreeWidgetKeys;
    QTreeWidgetItem* mpItem;
    QPointer<Host> mpHost;
    Qt::Key mKey;
    Qt::KeyboardModifiers mModifier;
    Qt::Key mPrevKey;
    Qt::KeyboardModifiers mPrevModifier;
    QString mPrevKeyName;
    QString mKeyName;
};

class AddActionCommand : public QUndoCommand
{
public:
    AddActionCommand(QTreeWidgetItem* pItem, TTreeWidget* treeWidgetActions, bool isFolder, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;

private:
    QTreeWidgetItem* mpItem;
    QTreeWidgetItem* mpParent;
    TTreeWidget* mpTreeWidgetActions;
    TTrigger* mpItemTrigger;
    bool mIsFolder;
};

class DeleteActionCommand : public QUndoCommand
{
public:
    DeleteActionCommand(Host* pHost, QTreeWidgetItem* pItem, TTreeWidget* treeWidgetActions, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;

private:
    QTreeWidgetItem* mpItem;
    QTreeWidgetItem* mpParent;
    TTreeWidget* mpTreeWidgetActions;
    QPointer<Host> mpHost;
    TAction* mpItemAction;
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
    ActionNameTextEditedCommand(dlgActionMainArea* actionsMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgActionMainArea* mpActionsMainArea = nullptr;
    TTreeWidget* mpTreeWidgetActions;
    QTreeWidgetItem* mpItem;
    QString mPrevActionName;
    QString mActionName;
};

class ActionButtonRotationEditedCommand : public QUndoCommand
{
public:
    ActionButtonRotationEditedCommand(dlgActionMainArea* actionsMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgActionMainArea* mpActionsMainArea = nullptr;
    TTreeWidget* mpTreeWidgetActions;
    QTreeWidgetItem* mpItem;
    int mPrevRotation;
    int mRotation;
};

class ActionButtonCheckboxEditedCommand : public QUndoCommand
{
public:
    ActionButtonCheckboxEditedCommand(dlgActionMainArea* actionsMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    dlgActionMainArea* mpActionsMainArea = nullptr;
    TTreeWidget* mpTreeWidgetActions;
    QTreeWidgetItem* mpItem;
    bool mPrevIsPushDown;
    bool mIsPushDown;
};

class ActionCommandDownTextEditedCommand : public QUndoCommand
{
public:
    ActionCommandDownTextEditedCommand(dlgActionMainArea* actionsMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgActionMainArea* mpActionsMainArea = nullptr;
    TTreeWidget* mpTreeWidgetActions;
    QTreeWidgetItem* mpItem;
    QString mPrevCommandDown;
    QString mCommandDown;
};

class ActionCommandUpTextEditedCommand : public QUndoCommand
{
public:
    ActionCommandUpTextEditedCommand(dlgActionMainArea* actionsMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgActionMainArea* mpActionsMainArea = nullptr;
    TTreeWidget* mpTreeWidgetActions;
    QTreeWidgetItem* mpItem;
    QString mPrevCommandUp;
    QString mCommandUp;
};

class ActionCssTextEditedCommand : public QUndoCommand
{
public:
    ActionCssTextEditedCommand(dlgActionMainArea* actionsMainArea, QUndoCommand* parent = nullptr);
    int id() const override;
    bool mergeWith(const QUndoCommand *command) override;
    void undo() override;
    void redo() override;
    dlgActionMainArea* mpActionsMainArea = nullptr;
    TTreeWidget* mpTreeWidgetActions;
    QTreeWidgetItem* mpItem;
    QString mPrevActionCss;
    QString mActionCss;
};

class AddVarCommand : public QUndoCommand
{
public:
    AddVarCommand(QTreeWidgetItem* pItem, TTreeWidget* treeWidgetVariables, bool isFolder, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;

private:
    QTreeWidgetItem* mpItem;
    QTreeWidgetItem* mpParent;
    TTreeWidget* mpTreeWidgetVariables;
    TTrigger* mpItemTrigger;
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
    TriggerNameTextEditedCommand(dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggersMainArea* mpTriggersMainArea = nullptr;
    dlgTriggerEditor* mpEditor = nullptr;
    QTreeWidgetItem* mpItem;
    TTreeWidget* mpTreeWidgetTriggers;
    QString mPrevLineEditTriggerName;
    QString mLineEditTriggerName;
};

class TriggerCommandTextEditedCommand : public QUndoCommand
{
public:
    TriggerCommandTextEditedCommand(dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggersMainArea* mpTriggersMainArea = nullptr;
    dlgTriggerEditor* mpEditor = nullptr;
    QTreeWidgetItem* mpItem;
    TTreeWidget* mpTreeWidgetTriggers;
    QString mPrevLineEditTriggerCommand;
    QString mLineEditTriggerCommand;
};

class TriggerFireLengthEditedCommand : public QUndoCommand
{
public:
    TriggerFireLengthEditedCommand(dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggersMainArea* mpTriggersMainArea = nullptr;
    dlgTriggerEditor* mpEditor = nullptr;
    QTreeWidgetItem* mpItem;
    TTreeWidget* mpTreeWidgetTriggers;
    int mPrevFireLength;
    int mFireLength;
};

class TriggerPlaySoundEditedCommand : public QUndoCommand
{
public:
    TriggerPlaySoundEditedCommand(dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggersMainArea* mpTriggersMainArea = nullptr;
    dlgTriggerEditor* mpEditor = nullptr;
    QTreeWidgetItem* mpItem;
    TTreeWidget* mpTreeWidgetTriggers;
    bool mPrevGroupBoxSoundTrigger;
    bool mGroupBoxSoundTrigger;
};

class TriggerPlaySoundFileEditedCommand : public QUndoCommand
{
public:
    TriggerPlaySoundFileEditedCommand(dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggersMainArea* mpTriggersMainArea = nullptr;
    dlgTriggerEditor* mpEditor = nullptr;
    QTreeWidgetItem* mpItem;
    TTreeWidget* mpTreeWidgetTriggers;
    QString mPrevLineEditSoundFile;
    QString mLineEditSoundFile;
};

class TriggerColorizerEditedCommand : public QUndoCommand
{
public:
    TriggerColorizerEditedCommand(dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggersMainArea* mpTriggersMainArea = nullptr;
    dlgTriggerEditor* mpEditor = nullptr;
    QTreeWidgetItem* mpItem;
    TTreeWidget* mpTreeWidgetTriggers;
    bool mPrevBoxTriggerColorizer;
    bool mBoxTriggerColorizer;
};

class TriggerColorizerBgColorEditedCommand : public QUndoCommand
{
public:
    TriggerColorizerBgColorEditedCommand(dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggersMainArea* mpTriggersMainArea = nullptr;
    dlgTriggerEditor* mpEditor = nullptr;
    QTreeWidgetItem* mpItem;
    TTreeWidget* mpTreeWidgetTriggers;
    QString mPrevbgColor;
    QString mBgColor;
};

class TriggerColorizerFgColorEditedCommand : public QUndoCommand
{
public:
    TriggerColorizerFgColorEditedCommand(dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggersMainArea* mpTriggersMainArea = nullptr;
    dlgTriggerEditor* mpEditor = nullptr;
    QTreeWidgetItem* mpItem;
    TTreeWidget* mpTreeWidgetTriggers;
    QString mPrevfgColor;
    QString mFgColor;
};

class TriggerPerlSlashGOptionEditedCommand : public QUndoCommand
{
public:
    TriggerPerlSlashGOptionEditedCommand(dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggersMainArea* mpTriggersMainArea = nullptr;
    dlgTriggerEditor* mpEditor = nullptr;
    QTreeWidgetItem* mpItem;
    TTreeWidget* mpTreeWidgetTriggers;
    bool mPrevPerlSlashGOption;
    bool mPerlSlashGOption;
};

class TriggerGroupFilterEditedCommand : public QUndoCommand
{
public:
    TriggerGroupFilterEditedCommand(dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggersMainArea* mpTriggersMainArea = nullptr;
    dlgTriggerEditor* mpEditor = nullptr;
    QTreeWidgetItem* mpItem;
    TTreeWidget* mpTreeWidgetTriggers;
    bool mPrevFilterTrigger;
    bool mFilterTrigger;
};

class TriggerMultiLineEditedCommand : public QUndoCommand
{
public:
    TriggerMultiLineEditedCommand(dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggersMainArea* mpTriggersMainArea = nullptr;
    dlgTriggerEditor* mpEditor = nullptr;
    QTreeWidgetItem* mpItem;
    TTreeWidget* mpTreeWidgetTriggers;
    bool mPrevMultiLineTrigger;
    bool mMultiLineTrigger;
};

class TriggerLineMarginEditedCommand : public QUndoCommand
{
public:
    TriggerLineMarginEditedCommand(dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggersMainArea* mpTriggersMainArea = nullptr;
    dlgTriggerEditor* mpEditor = nullptr;
    QTreeWidgetItem* mpItem;
    TTreeWidget* mpTreeWidgetTriggers;
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
    QTreeWidgetItem* mpItem;
    QComboBox* pBox;
    QComboBox* pBoxTemp = nullptr;
    TTreeWidget* mpTreeWidgetTriggers;
    int mPrevTriggerPatternEdit;
    int mTriggerPatternEdit;
    int mRow;
private:
    QPointer<Host> mpHost;
};

class TriggerLineEditPatternEditedCommand : public QUndoCommand
{
public:
    TriggerLineEditPatternEditedCommand(dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggersMainArea* mpTriggersMainArea = nullptr;
    dlgTriggerEditor* mpEditor = nullptr;
    dlgTriggerPatternEdit* mpTriggerPattern;
    QList<dlgTriggerPatternEdit*> mpTriggerPatternEdit;
    QTreeWidgetItem* mpItem;
    TTreeWidget* mpTreeWidgetTriggers;
    QString mPrevLineEditTriggerPattern;
    QString mLineEditTriggerPattern;
    int mRow;
};

class TriggerLineSpacerEditedCommand : public QUndoCommand
{
public:
    TriggerLineSpacerEditedCommand(dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggersMainArea* mpTriggersMainArea = nullptr;
    dlgTriggerPatternEdit* mpPatternItem;
    dlgTriggerEditor* mpEditor = nullptr;
    QTreeWidgetItem* mpItem;
    TTreeWidget* mpTreeWidgetTriggers;
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
    QPushButton* mpPushButton;
    dlgTriggerEditor* mpEditor = nullptr;
    QTreeWidgetItem* mpItem;
    TTreeWidget* mpTreeWidgetTriggers;
    QColor mPrevColorTriggerFgColor;
    QColor mColorTriggerFgColor;
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
    QPushButton* mpPushButton;
    dlgTriggerEditor* mpEditor = nullptr;
    QTreeWidgetItem* mpItem;
    TTreeWidget* mpTreeWidgetTriggers;
    QColor mPrevColorTriggerBgColor;
    QColor mColorTriggerBgColor;
private:
    QPointer<Host> mpHost;
};
#endif
