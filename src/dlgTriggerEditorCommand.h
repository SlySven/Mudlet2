#ifndef MUDLET_DLGTRIGGERSCOMMANDS_H
#define MUDLET_DLGTRIGGERSCOMMANDS_H

#include "ActionUnit.h"
#include "AliasUnit.h"
#include "KeyUnit.h"
#include "ScriptUnit.h"
#include "TTreeWidget.h"
#include "TimerUnit.h"
#include "TriggerUnit.h"
#include "VarUnit.h"
#include "dlgTriggerEditor.h"
#include <QPointer>
#include <QListWidgetItem>
#include <QUndoCommand>

class LuaInterface;
class dlgTriggerEditor;
class dlgTriggerPatternEdit;
class dlgAliasMainArea;
class dlgScriptsMainArea;
class dlgKeysMainArea;
class dlgActionMainArea;

class AddTriggerCommand : public QUndoCommand
{
public:
    AddTriggerCommand(QTreeWidgetItem* pItem, TriggerUnit* triggerUnit, TTreeWidget* treeWidgetTriggers, bool isFolder, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;

private:
    QTreeWidgetItem* mpItem;
    QTreeWidgetItem* mpParent;
    TTreeWidget* mpTreeWidgetTriggers;
    TriggerUnit* mpTriggerUnit;
    TTrigger* mpItemTrigger;
    bool mIsFolder;
};

class DeleteTriggerCommand : public QUndoCommand
{
public:
    DeleteTriggerCommand(QTreeWidgetItem* pItem, TriggerUnit* triggerUnit, TTreeWidget* treeWidgetTriggers, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    QPointer<Host> mpHost = nullptr;

private:
    QTreeWidgetItem* mpItem;
    QTreeWidgetItem* mpParent;
    TTreeWidget* mpTreeWidgetTriggers;
    TriggerUnit* mpTriggerUnit;
    TTrigger* mpItemTrigger;
    bool mIsFolder;
};

class MoveTriggerCommand : public QUndoCommand, QTreeWidget
{
public:
    MoveTriggerCommand(TriggerUnit* triggerUnit,
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
    QPointer<Host> mpHost = nullptr;
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
    TriggerUnit* mpTriggerUnit;
    bool mIsFolder;
};

class AddAliasCommand : public QUndoCommand
{
public:
    AddAliasCommand(QTreeWidgetItem* pItem, AliasUnit* aliasUnit, TTreeWidget* treeWidgetTriggers, bool isFolder, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;

private:
    QTreeWidgetItem* mpItem;
    QTreeWidgetItem* mpParent;
    TTreeWidget* mpTreeWidgetAliases;
    AliasUnit* mpAliasUnit;
    TTrigger* mpItemTrigger;
    bool mIsFolder;
};

class DeleteAliasCommand : public QUndoCommand
{
public:
    DeleteAliasCommand(QTreeWidgetItem* pItem, AliasUnit* aliasUnit, TTreeWidget* treeWidgetAliases, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    QPointer<Host> mpHost = nullptr;

private:
    QTreeWidgetItem* mpItem;
    QTreeWidgetItem* mpParent;
    TTreeWidget* mpTreeWidgetAliases;
    AliasUnit* mpAliasUnit;
    TAlias* mpItemAlias;
};

class MoveAliasCommand : public QUndoCommand, QTreeWidget
{
public:
    MoveAliasCommand(AliasUnit* aliasUnit,
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
    QPointer<Host> mpHost = nullptr;
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
    AliasUnit* mpAliasUnit;
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
    AddTimerCommand(QTreeWidgetItem* pItem, TimerUnit* timerUnit, TTreeWidget* treeWidgetTriggers, bool isFolder, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;

private:
    QTreeWidgetItem* mpItem;
    QTreeWidgetItem* mpParent;
    TTreeWidget* mpTreeWidgetTimers;
    TimerUnit* mpTimerUnit;
    TTrigger* mpItemTrigger;
    bool mIsFolder;
};

class DeleteTimerCommand : public QUndoCommand
{
public:
    DeleteTimerCommand(QTreeWidgetItem* pItem, TimerUnit* timerUnit, TTreeWidget* treeWidgetTimers, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    QPointer<Host> mpHost = nullptr;

private:
    QTreeWidgetItem* mpItem;
    QTreeWidgetItem* mpParent;
    TTreeWidget* mpTreeWidgetTimers;
    TimerUnit* mpTimerUnit;
    TTimer* mpItemTimer;
};

class MoveTimerCommand : public QUndoCommand, QTreeWidget
{
public:
    MoveTimerCommand(TimerUnit* timerUnit,
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
    QPointer<Host> mpHost = nullptr;
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
    TimerUnit* mpTimerUnit;
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
    AddScriptCommand(QTreeWidgetItem* pItem, ScriptUnit* scriptUnit, TTreeWidget* treeWidgetScripts, bool isFolder, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;

private:
    QTreeWidgetItem* mpItem;
    QTreeWidgetItem* mpParent;
    TTreeWidget* mpTreeWidgetScripts;
    ScriptUnit* mpScriptUnit;
    TTrigger* mpItemTrigger;
    bool mIsFolder;
};

class DeleteScriptCommand : public QUndoCommand
{
public:
    DeleteScriptCommand(QTreeWidgetItem* pItem, ScriptUnit* scriptUnit, TTreeWidget* treeWidgetScripts, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    QPointer<Host> mpHost = nullptr;

private:
    QTreeWidgetItem* mpItem;
    QTreeWidgetItem* mpParent;
    TTreeWidget* mpTreeWidgetScripts;
    ScriptUnit* mpScriptUnit;
    TScript* mpItemScript;
};

class MoveScriptCommand : public QUndoCommand, QTreeWidget
{
public:
    MoveScriptCommand(ScriptUnit* scriptUnit,
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
    QPointer<Host> mpHost = nullptr;
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
    ScriptUnit* mpScriptUnit;
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
    AddKeyCommand(QTreeWidgetItem* pItem, KeyUnit* keyUnit, TTreeWidget* treeWidgetKeys, bool isFolder, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;

private:
    QTreeWidgetItem* mpItem;
    QTreeWidgetItem* mpParent;
    TTreeWidget* mpTreeWidgetKeys;
    KeyUnit* mpKeyUnit;
    TTrigger* mpItemTrigger;
    bool mIsFolder;
};

class DeleteKeyCommand : public QUndoCommand
{
public:
    DeleteKeyCommand(QTreeWidgetItem* pItem, KeyUnit* keyUnit, TTreeWidget* treeWidgetKeys, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    QPointer<Host> mpHost = nullptr;

private:
    QTreeWidgetItem* mpItem;
    QTreeWidgetItem* mpParent;
    TTreeWidget* mpTreeWidgetKeys;
    KeyUnit* mpKeyUnit;
    TKey* mpItemKey;
};

class MoveKeyCommand : public QUndoCommand, QTreeWidget
{
public:
    MoveKeyCommand(KeyUnit* keyUnit,
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
    QPointer<Host> mpHost = nullptr;
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
    KeyUnit* mpKeyUnit;
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
    KeyGrabTextEditedCommand(dlgKeysMainArea* keysMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgKeysMainArea* mpKeysMainArea = nullptr;
    TTreeWidget* mpTreeWidgetKeys;
    QTreeWidgetItem* mpItem;
    KeyUnit* mpKeyUnit;
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
    AddActionCommand(QTreeWidgetItem* pItem, ActionUnit* actionUnit, TTreeWidget* treeWidgetActions, bool isFolder, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;

private:
    QTreeWidgetItem* mpItem;
    QTreeWidgetItem* mpParent;
    TTreeWidget* mpTreeWidgetActions;
    ActionUnit* mpActionUnit;
    TTrigger* mpItemTrigger;
    bool mIsFolder;
};

class DeleteActionCommand : public QUndoCommand
{
public:
    DeleteActionCommand(QTreeWidgetItem* pItem, ActionUnit* actionUnit, TTreeWidget* treeWidgetActions, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    QPointer<Host> mpHost = nullptr;

private:
    QTreeWidgetItem* mpItem;
    QTreeWidgetItem* mpParent;
    TTreeWidget* mpTreeWidgetActions;
    ActionUnit* mpActionUnit;
    TAction* mpItemAction;
};
class MoveActionCommand : public QUndoCommand, QTreeWidget
{
public:
    MoveActionCommand(ActionUnit* actionUnit,
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
    QPointer<Host> mpHost = nullptr;
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
    ActionUnit* mpActionUnit;
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
    AddVarCommand(QTreeWidgetItem* pItem, VarUnit* varUnit, TTreeWidget* treeWidgetVariables, bool isFolder, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;

private:
    QTreeWidgetItem* mpItem;
    QTreeWidgetItem* mpParent;
    TTreeWidget* mpTreeWidgetVariables;
    VarUnit* mpVarUnit;
    TTrigger* mpItemTrigger;
    bool mIsFolder;
};

class DeleteVarCommand : public QUndoCommand
{
public:
    DeleteVarCommand(QTreeWidgetItem* pItem, VarUnit* varUnit, TTreeWidget* treeWidgetVariables, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    QPointer<Host> mpHost = nullptr;

private:
    QTreeWidgetItem* mpItem;
    QTreeWidgetItem* mpParent;
    TTreeWidget* mpTreeWidgetVariables;
    VarUnit* mpVarUnit;
    TVar* mpItemVar = nullptr;
    TVar* mpTempVar = nullptr;
};
class MoveVariableCommand : public QUndoCommand, QTreeWidget
{
public:
    MoveVariableCommand(VarUnit* varUnit, TTreeWidget* treeWidgetVariables, QTreeWidgetItem* parentItem, QTreeWidgetItem* cItem, QTreeWidgetItem* prevParentItem, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggerEditor* mpEditor = nullptr;
    QPointer<Host> mpHost = nullptr;
    QTreeWidgetItem* mpParentItem;
    QTreeWidgetItem* mpPrevParentItem;
    QTreeWidgetItem* mpItem;
    TVar* mpItemVar;
    TTreeWidget* mpTreeWidgetVariables;
    QDropEvent* mpEvent;

private:
    VarUnit* mpVarUnit;
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
    TriggerLineEditPatternItemEditedCommand(dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggersMainArea* mpTriggersMainArea = nullptr;
    dlgTriggerPatternEdit* mpPatternItem;
    dlgTriggerEditor* mpEditor = nullptr;
    QTreeWidgetItem* mpItem;
    QComboBox* pBox;
    QComboBox* pBoxTemp = nullptr;
    TTreeWidget* mpTreeWidgetTriggers;
    TriggerUnit* mpTriggerUnit;
    int mPrevTriggerPatternEdit;
    int mTriggerPatternEdit;
    int mRow;
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
    TriggerColorFGEditedCommand(dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggersMainArea* mpTriggersMainArea = nullptr;
    QList<dlgTriggerPatternEdit*> mpTriggerPatternEdit;
    dlgTriggerPatternEdit* mpPatternItem;
    QPushButton* mpPushButton;
    TriggerUnit* mpTriggerUnit;
    dlgTriggerEditor* mpEditor = nullptr;
    QTreeWidgetItem* mpItem;
    TTreeWidget* mpTreeWidgetTriggers;
    QColor mPrevColorTriggerFgColor;
    QColor mColorTriggerFgColor;
    int mRow;
};

class TriggerColorBGEditedCommand : public QUndoCommand
{
public:
    TriggerColorBGEditedCommand(dlgTriggersMainArea* triggersMainArea, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    dlgTriggersMainArea* mpTriggersMainArea = nullptr;
    dlgTriggerPatternEdit* mpPatternItem;
    QPushButton* mpPushButton;
    TriggerUnit* mpTriggerUnit;
    dlgTriggerEditor* mpEditor = nullptr;
    QTreeWidgetItem* mpItem;
    TTreeWidget* mpTreeWidgetTriggers;
    QColor mPrevColorTriggerBgColor;
    QColor mColorTriggerBgColor;
};
#endif
