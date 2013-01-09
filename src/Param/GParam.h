#ifndef GPARAM_H
#define GPARAM_H
#include "labexe_global.h"

// #include "GParamManager.h"
#include "ToolBox/GSerializable.h"
// #include "GParamLabel.h"
#include <QObject>
#include <QVariant>
#include <QSettings>
#include <QMenu>
#include <QTabWidget>
#include <QCache>
#include <QMutex>
#include <QLabel>
#include <QDoubleSpinBox>

class GParamSettingsWidget;

typedef QWidget GParamControlWidget;

typedef QVector<double> GVectorDouble;

// namespace ControlWidget {
// 	enum Options {InheritedFromParam = 0x1000, NoOption = 0x00, ReadOnly = 0x01};
// }

namespace LabelWidget {
enum Options {NoOption = 0x00};
}

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
e.g. for draging and droping the GParam in useful widgets (like GVariatorWidgetContainer).

Each GParam has a unique identifier that is used for:
	- serializing the GParam,
	- and also for making easy the drag and drop operation through the MIME data

	After a GParam has been asked to change value, it will emit a signal ParamUpdateCompletion(). 
	This signal can be customize by the user, i.e. emitted when an external 
	signal (like from a device or driver) estimates that the value has been updated successfully (or not). 
	For this use the SetExternalCompletionSignal() method.

At creation time, a set of option can be passed to the parameter in order to define its behavior 
and the behavior of the widget it will provide for representing itself.
*/
class LABEXE_EXPORT GParam : public QObject, protected QVariant, public GSerializable
{
	Q_OBJECT

public:
	// options for the parameter
	enum Properties {NoOption = 0x00, ReadOnly = 0x01, HiddenText = 0x02};//, StringUpdateOnlyWhenFinishedEditing = 0x04};
	// options when requesting a widget
	enum WidgetOptions {Default, Minimal, Blabla};
	GParam::Properties Options() const { return m_Options; }
	void SetOptions(GParam::Properties val) { m_Options = val; }

	//! Creates a param with the given name. The name should be meaningful.
	GParam(QString theName, QObject *parent, GParam::Properties paramOptions = NoOption);

	~GParam();

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
	QString StringContent(char format = 'g', int precision = 6);

	//! Sets the signal that is going to inform this GParam that the value has been actually successfully updated (like the voltage on an analog voltage output or the position of a motor), or not.
	bool SetExternalCompletionSignal(const QObject* sender, const char* signal, Qt::ConnectionType type = Qt::AutoConnection);

	//! Displays the widget containing the settings for this param. Returns the widget so that re-implementations can add more content to the layout.
	virtual GParamSettingsWidget* PopupSettingWidget();

protected:
	//! Reimplemented from GSerializable. Writes the settings to permanent storage using the provided QSettings. 
	virtual void PopulateSettings(QSettings& inQsettings);
	//! Reimplemented from GSerializable. Reads the settings from permanent storage using the provided QSettings. 
	virtual void InterpretSettings(QSettings& fromQsettings);

public slots:
	//! Renames the param
	void SetName(QString theName);

protected slots:
	//! Displays a context menu that will be populated by the reimplemented PopulateContextMenu(). pos is the position of the context menu event that the widget received.  
	void ProvideParamMenu( const QPoint & pos ) const;

protected:
	//! Populates the context menu that was provided by ProvideParamMenu().
	virtual void PopulateParamMenu(QMenu* pTheMenu, GParamLabel* pSenderLabel = 0) const;

public:
	//! Provides a widget that will be used to control (read or modify) the param.
	virtual GParamControlWidget* ProvideNewParamWidget(QWidget* forWhichParent, GParam::WidgetOptions optionWid = Default) = 0; 
	//! Provides a label that will be used to display the name, or Drag&Drop the param (e.g. to a GVariator).
	virtual QLabel* ProvideNewLabel(QWidget* forWhichParent, LabelWidget::Options someOptions = LabelWidget::NoOption);
	//! Appends the packed-up label and widget in a layout. If it is a form-layout, it will put the label in the appropriate zone
//	void ProvideNewWidgetLabeledInLayout(QLayout* pLayout);

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

signals:
	//! emitted when one of the label of the param is being hovered (entered actually) by the mouse cursor
	void MouseEnteredParamLabel();
	//! emitted when the mouse cursor left the param label
	void MouseExitedParamLabel();

private:
	Properties m_Options;
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