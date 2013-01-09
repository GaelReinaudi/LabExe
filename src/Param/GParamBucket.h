#ifndef GPARAMBUCKET_H
#define GPARAMBUCKET_H
#include "labexe_global.h"

#include "GParam.h"
#include <QStandardItemModel>

class GParamBucketTreeWidget;
class GParamBucketWidget;
class GParamBool;
class GParamNum;
class GParamDouble;
class GParamString;
class GParamInt;

/////////////////////////////////////////////////////////////////////
//! \brief The GParamBucket class defines an object that is made to receive a param
/*!
It allows the user to drag and drop a param into a provided widget
and would handle it by forwarding some signals and slots to some other use. Sounds a bit abstract? 
well here are some examples of classes that could benefit of using the GParamBucket:
- an optimizer could have a GParamBucket so that the user can drop a param to be optimized and displayed.
- it would also have a GParamBucket (or many) for the variable parameters.
- a formula widget could have several GParamBucket in order to associate a name to theme like "x", or "y"
- a feedback loop, just like the optimizer.

In order to extend the functionalities of the bucket, one can use the function 
AddExtraFieldBool() or AddExtraFieldString(). 
This will append a new field associated with each param present in the bucket. This effectively create e new 
GParam object that can be obtained latter by using the function ExtraParam() in which you must 
specify the parameter it is associated with as well as the given field.
the convenience function ExtraParamBool(), ExtraParamNum() and ExtraParamString() return 
a conversion to the right param class if it makes sens.
*/
class LABEXE_EXPORT GParamBucket : public GParam
{
	Q_OBJECT

public:
	GParamBucket(QString theName, QObject *parent, GParam::Properties paramOptions = NoOption);

	~GParamBucket();

	//! test to make GParamBucket inherited from GParam
	QWidget* ProvideNewParamWidget(QWidget* forWhichParent, GParam::WidgetOptions optionWid = Default);

	//! Provides a widget that will accept drops of GParam.
	virtual GParamBucketWidget* ProvideParamBucketWidget(QWidget* parentWidget, Qt::Orientation orientation = Qt::Vertical);
	//! Provides a tree widget that will accept drops of GParam.
	virtual GParamBucketTreeWidget* ProvideParamBucketTreeWidget(QWidget* parentWidget);
	//! returns a list of the pointer to the held params. It might be only one for a single param bucket.
	QList<GParam*> Params() const { return m_Params; }
	//! returns a list of the pointer to the held numerical params. None GParamNum are pointed to 0 in the list. If the accessMask is given, only the Param correpsonding to a true will be put in the list.
	QVector<GParamNum*> ParamNums(const QVector<bool> & accessMask = QVector<bool>()) const;
	//! returns a pointer to the indexParam numerical param in the bucket. A none GParamNum is pointed to 0.
	GParamNum* ParamNum(int indexParam) const;
	//! returns a list of the values of the held numerical params. 0 if not a GParamNum. If boolMaskField corresponds to a bool field, only the one corresponding to trues are returned.
	GVectorDouble const DoubleValues(const QString & boolMaskField = QString()) const;
	//! Sets new values to the parameters in the bucket. Values assigned to a none GParamNum will do nothing and be discarded.
	void SetValues(const GVectorDouble & theValuesToSet, const QVector<bool> & accessMask = QVector<bool>());

	//! Returns true if the bucket is empty.
	bool IsEmpty() const { return Params().isEmpty(); }
	//! Returns true if the bucket contains the param pointed by pParam.
	bool Contains(GParam* pParam) const { return Params().contains(pParam); }
	//! Returns the number of params in the .
	int ParamCount() const { return Params().count(); }
	//! Freezes the bucket, making it an editable by dropping/removing params. Useful when you want the bucket to be seen as a fixed bunch of parameters.
	void Freeze(bool doFreeze = true);
	//! Returns true if the bucket is freezed.
	bool IsFreezed() const { return m_IsFreezed; }
	//! returns the 0 based index of the pParam in the bucket.
	int IndexParam(GParam* pParam) {return Params().indexOf(pParam);}
	//! Returns the number of times the bucket has sent the BucketUpdatedValues() signal.
	unsigned int NumUpdates() const { return m_NumUpdates; }
	//! Makes this bucket to be triggered by the update of the param 
	void SetParamTriggeringConnection(GParam* pParam, bool doUseAsTrigger);

protected:
	//! Re-implemented
	virtual void PopulateSettings(QSettings& inQsettings);
	//! Re-implemented
	virtual void InterpretSettings(QSettings& fromQsettings);

	// those are related to extra field functionalities
public:
	//! Names of the extra fields
	QList<QString> ExtraFieldsName() const { return m_ExtraFieldsName; }
	//! Param associated with the field of the pParam. 0 if nonsense.
	GParam* ExtraParam(GParam* pParam, const QString & field) const;
	//! Param T* associated with the field of the pParam. 0 if nonsense.
	template<class T>
	T* ExtraParam(GParam* pOfParam, const QString & field) const {
		return qobject_cast<T*>(m_Param_Field_ExtraParams.value(pOfParam).value(field, 0));
	}
	//! ParamNum associated with the field of the pParam. 0 if nonsense.
	GParamNum* ExtraParamNum(GParam* pParam, const QString & field) const;
	//! All the ParamNum associated with the field. Empty if no such field. If boolMaskField corresponds to a bool field, only the one corresponding to trues are returned.
	QVector<GParamNum*> ExtraParamsNum(QString field, const QString & boolMaskField = QString()) const;
	//! All the Param<T*> associated with the field. Empty if no such field. If boolMask is provided, only the one corresponding to trues are returned.
	template<class T>
	QVector<T*> ExtraParams(QString field, const QVector<bool> & boolMask = QVector<bool>()) const {
		QVector<T*> vecNums;
		int nPar = Params().count();
		for(int i = 0; i < nPar ; i++) {
			GParam* pPar = Params()[i];
			T* pExtraParT = ExtraParam<T>(pPar, field);
			// if there is a mask given
			if(!boolMask.isEmpty()) {
				// if it is not true, bye for this one. If missing value, then true.
				if(!boolMask.value(i, true)) {
					continue;
				}
			}
			vecNums.append(pExtraParT);
		}
		return vecNums;
	}

	//! ParamString associated with the field of the pParam. 0 if nonsense.
	GParamString* ExtraParamString(GParam* pParam, const QString & field) const;
	//! ParamBool associated with the field of the pParam. 0 if nonsense.
	GParamBool* ExtraParamBool(GParam* pParam, const QString & field) const;
	//! Adds a boolean field to the bucket. Always use this function before inserting the first parameter.
	void AddExtraFieldBool(const QString & fieldName);
	//! Adds a string field to the bucket. Always use this function before inserting the first parameter.
	void AddExtraFieldString(const QString & fieldName);
	//! Adds a double field to the bucket. Always use this function before inserting the first parameter.
	void AddExtraFieldDouble(const QString & fieldName);
	//! Removes a field that was there
	void RemoveExtraField(const QString & fieldName);
	//! Returns a list of the Extra Param representing forThatField
	QList<GParam*> ExtraParamList(const QString & forThatField);
	//! Convenient function that returns a list of the bool representing the logical state forThatField. List of false if not boolean field.
	const QVector<bool> ExtraParamBoolMask(const QString & forThatField) const;

public slots:
	//! Called when a param was dropped by the user in the bucket. It emits ParamAdded() and returns true if successful. false if the param was not added.
	virtual bool AddParam(GParam* pParam);
	//! Does the same as AddParam(), but through a Qt::QueuedConnection
	virtual void AddParamLater(GParam* pParam);
	//! Called when a param was asked to be removed from the bucket. It emit ParamRemoved() and return true if the param was in the bucket and was removed successfully.
	virtual bool RemoveParam(GParam* pParam);

private slots:
	//! used by RequestDropParamLater() to drop a param using its ID. Returns true if the param was found and dropped.
	bool DropParamByID(QString uniqueParamID);
	//! called by a param in the bucket that emitted ValueUpdated()
	void EventSomeValueUpdated();
	//! called from EventSomeValueUpdated() when one param changed, through a queued connection, so that other parameter could be given a chance to update too before we send the BucketUpdatedValues().
	void EventAboutToSendUpdateSignal();
	//! updates the triggering scheme of the parameters, i.e. which parameter is triggering the bucket for the update.
	void UpdateTriggeringScheme();

signals:
	//! Emitted to signal that a param was added.
	void ParamAdded(GParam* pParam);
	//! Emitted to signal that a param was removed, with the index of the param before it was removed.
	void ParamRemoved(int indexBeforeRemoval, GParam* pParam);
	//! You can use (emit) this signal to drop a param (using its ID) at a later time (when the program comes back to the main event loop). That might be useful during a initialization time, so that you can wait that the actual param exists.
	void RequestDropParamLater(QString uniqueParamID);

	//! Emitted when some of the, or all, params were updated. The argument should be the number of parameters that changed.
	void BucketUpdatedValues(int nParamUpdated = 1);
	//! Emitted when one of the param is set/unset as a triggering mechanism
	void ParamTringerringChanged(GParam* pParam, bool isTriggering);

private:
	//! makes some connections
	void Init();

protected:
	//! number of times this bucket sent the BucketUpdatedValues() signal
	unsigned int m_NumUpdates;
	//! Re-implement
	void DelayedInterpretSettings( QSettings& fromQsettings );

private:
	//! The list of pointers to the params. It might be only one for a single param bucket.
	QList<GParam*> m_Params;
	//! Makes the bucket un-editable by dropping/removing params if set to true.
	bool m_IsFreezed;
	//! This will be incremented in EventSomeValueUpdated(), when parameters send the ValueUpdated() signal. Later on, a queued signal will be emitted. Hopefully only one even if several vavlue changed at once.
	int m_SomeUpdateSignalCount;
	//! When an BucketUpdatedValues() is going to be sent through queued connection, the time that we should delay it, so that other parameter could be given a chance to update too.
	int m_DelayUpdateSignal;

	//! TEST in order to add field to the bucket for each parameters inside it. Should be something like ["GParamBool", "GParamString", "GParamString", etc...]
	QList<QString> m_ExtraFieldsClass;
	//! Names associated with the extra fields
	QList<QString> m_ExtraFieldsName;
	//! For each param, a list of extra param is associated
	QMap<GParam*, QMap<QString, GParam*> > m_Param_Field_ExtraParams;

};

#endif // GPARAMBUCKET_H
