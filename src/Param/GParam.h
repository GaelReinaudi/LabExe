#ifndef GPARAM_H
#define GPARAM_H
#include "labexe_global.h"

// #include "GParamManager.h"
#include "ToolBox/GSerializable.h"
#include <QObject>
#include <QVariant>
#include <QSettings>
#include <QMenu>
#include <QTabWidget>
#include <QCache>
#include <QMutex>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QElapsedTimer>
#include <QBasicTimer>

typedef QVector<double> GVectorDouble;

namespace LabelWidget {
enum Options {NoOption = 0x00};
}

class GParamSettingsWidget;
class GParam;
class GParamLabel;
class GParamManager;
//! Returns the param manager
LABEXE_EXPORT GParamManager* ParamManagerInstance(); 
//! Returns the param whose uniqueID matches in the param manager, 0 otherwise.
LABEXE_EXPORT GParam* GetParamFromParamManager(QString uniqueID); 
//! Adds the param to the param manager's list of params.
LABEXE_EXPORT void AddToParamManager(GParam* pParam); 

/////////////////////////////////////////////////////////////////////
//! \brief The GParam class defines a parameter of the program (e.g.: an output value, an input value, a program setting).
/*!
A GParam can be associated with a (or many) GParamLabel (inherits QLabel) that 
are provided by the function ProvideNewLabel(). The GParamLabel display a name and can be used 
e.g. for draging and droping the GParam in useful widgets (like a GParamPlotter or GFormula).

Each GParam has a unique identifier that is used for:
	- serializing/deserializing the GParam,
	- and also for making easy the drag and drop operation through the use of MIME data

	After a GParam has been asked to change value, it will emit a signal ParamUpdateCompletion(). 
	This signal can be customize by the user, i.e. emitted when an external 
	signal (like from a device or driver) estimates that the value has been updated successfully (or not). 
	For this use the SetExternalCompletionSignal() method.

At creation time, a set of option can be passed to the parameter in order to define its behavior 
and the behavior of the widget it will provide for representing itself.
*/
class LABEXE_EXPORT GParam : public QObject, /*protected QVariant,*/ public GSerializable
{
	Q_OBJECT

public:
	// options for the parameter
	enum Properties {NoOption = 0x00, ReadOnly = 0x01, NoButton = 0x02, HiddenText = 0x04};//, StringUpdateOnlyWhenFinishedEditing = 0x04};
	// options when requesting a widget from this param
	enum WidgetOptions {Default, Minimal};
	GParam::Properties Options() const { return m_Options; }
	void SetOptions(GParam::Properties val) { m_Options = val; }

	//! Creates a param with the given name. The name should be meaningful since it will be used to automatically serialize this param upon exit.
	GParam(QString theName, QObject *parent, GParam::Properties paramOptions = NoOption);
	virtual ~GParam();

	//! Returns the name of the param
	QString Name() const;
	//! Sets the description of the param that appears in tool tips or info bubbles.
	void SetDescription(QString strDescr) { m_Description = strDescr; }
	//! Sets the unit as it appears in the widget if ShowUnits() is true
	void SetUnits(QString strUnits);
	//! Returns the units
	QString Units() const { return m_Units; }
	//! Sets the param to show its unit in its widget(s)
	void ShowUnits(bool doShowUnits);
	//! Returns true if the param is showing its units
	bool ShowingUnits() const { return m_DisplayUnits; }
	//! Tries to convert whatever the content is to a string, with a similar signature as QString::number() for when it is dealing with numbers.
	virtual QString StringContent(char format = 'g', int precision = 6) const { return "StringContent() not implemented."; }
	//! if we need a QVariant out of the param
	virtual QVariant ToVariant() const { return QVariant("ToVariant() not implemented."); }
	//! Prevents the update on the gui if it happened less than a given number of milliseconds from the previous gui update.
	void SetLimitVisualUpdate(int ms) { m_MsLimitVisualUpdate = ms; }

	//! Sets the signal that is going to inform this GParam that the value has been actually successfully updated (like the voltage on an analog voltage output or the position of a motor), or not.
	bool SetExternalCompletionSignal(const QObject* sender, const char* signal, Qt::ConnectionType type = Qt::AutoConnection);

	//! Displays the widget containing the settings for this param. Returns the widget so that re-implementations can add more content to the layout.
	virtual GParamSettingsWidget* PopupSettingWidget();
	//! Provides a widget that will be used to control (read or modify) the param.
	virtual QWidget* ProvideNewParamWidget(QWidget* forWhichParent, GParam::WidgetOptions optionWid = Default) = 0; 
	//! Provides a label that will be used to display the name, or Drag&Drop the param (e.g. to a GVariator).
	virtual QLabel* ProvideNewLabel(QWidget* forWhichParent, LabelWidget::Options someOptions = LabelWidget::NoOption);

protected:
	//! Re-implemented from GSerializable. Writes the settings to permanent storage using the provided QSettings. 
	virtual void PopulateSettings(QSettings& inQsettings);
	//! Re-implemented from GSerializable. Reads the settings from permanent storage using the provided QSettings. 
	virtual void InterpretSettings(QSettings& fromQsettings);

public slots:
	//! Renames the param
	void SetName(QString theName);
	//! generic way to set the value using a QVariant
	virtual void SetFromVariant( QVariant varVal ) = 0;

protected slots:
	//! Displays a context menu that will be populated by the reimplemented PopulateContextMenu(). pos is the position of the context menu event that the widget received.  
	void ProvideParamMenu( const QPoint & pos ) const;
    //! Slot intended to start the timer in this param's thread.
    void StartGuiUpdateTimer(bool doStart);

protected:
	//! Populates the context menu that was provided by ProvideParamMenu().
	virtual void PopulateParamMenu(QMenu* pTheMenu, GParamLabel* pSenderLabel = 0) const;
	//! Re-implemented to add the new ID in the ParamManagerInstance(). It doesn't remove the previous one.
	void Event_UniqueSystemIDChanged();
	//! Tells if it is ok to emit a signal to update the gui based on the m_MsLimitVisualUpdate. It restarts the timer if it hasExpired().
	bool canUpdateGui();

    //! implemented to push a gui update of the param
	void timerEvent(QTimerEvent * event);

signals:
	//! Emitted when the name of the param changed.
	void NameChanged(QString newname);
	//! Emitted when the value was updated. It is called by ValueUpdated() of the derived classes. It acts as an intermediate between the various ValueUpdated(someType) in derived classes and the ParamUpdateCompletion(bool).
	void ParamValueWasUpdated(bool success = true);
	//! Should be emitted when the value changed and was updated successfully and that the desired effect was achieved (or failed). For example it would inform the program that a servomotor did finish its moving. Usually, it should be connected by using SetExternalCompletionSignal().
	void ParamUpdateCompletion(bool success /*= true*/);
	//! Emitted when the units of the param changed.
	void UnitsChanged(QString newUnits);
	//! Emitted when the units displaying of the param changed.
	void UnitDisplayChanged(bool newDisplayUnits);
	//! emitted when one of the label of the param is being hovered (entered actually) by the mouse cursor
	void MouseEnteredParamLabel();
	//! Emitted when the mouse cursor left the param label
	void MouseExitedParamLabel();
	//! Emitted when the param has updates at least once without a gui update (to save cpu) in order to have an up to date value on the screen.
	void RequestUpdateDisplay();
    //! Emitted when the param needs (maybe from another thread) to start the timer for gui update
    void RequestTimerStartForUpdate(bool doStart);

private:
	Properties m_Options;
	//! prevents the update on the gui if it happened less than a given number of milliseconds from the previous gui update.
	int m_MsLimitVisualUpdate;
	//! the timer
	QElapsedTimer m_GuiUpdateTimer;
	QBasicTimer m_GuiLastUpdater;
	unsigned int m_CouldUpdateGui;
	void ParamInit();

protected:
	//! mutex for protecting the value manipulation
	mutable QMutex m_MutexVariant;
	//! Description of the param for tool tips
	QString m_Description;
	//! Units of the parameter if relevant
	QString m_Units;
	//! Tells the param to display its units in its widgets
	bool m_DisplayUnits;
	friend class GParamManager;
	friend class GParamLabel;
};

/*! \def AUTOSET_UNIQUE_IDENTIFIER(aParam)
\brief This macro allows the automatic setting of the Unique ID of the GParam that is part of a GDevice. 

This way, the name is fixed at compile time and will stay always the same over different instance of the program. 
- It creates a Unique Id like this: QString("%1{%2}").arg(UniqueSystemID() of the device).arg(# aParam). 
- It then calls ParamManagerInstance()->Add(&aParam); in oder to register the GParam
- As a consequence, the GParam is also registered for its previous IDs.

You have to call it from a GDevice object, to any object that has a UniqueSystemID() function.
*/
#define AUTOSET_UNIQUE_IDENTIFIER(aParam)																\
	if(!qobject_cast<GDevice*>(this))																	\
		qWarning("AUTOSET_UNIQUE_IDENTIFIER has to be used from within a GDevice object");				\
	else {																								\
		(aParam.ChangeUniqueSystemID(QString("%1{%2}").arg(UniqueSystemID()).arg(# aParam)));	\
		AddToParamManager(&aParam);															\
	}


#endif // GPARAM_H