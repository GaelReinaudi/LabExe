#ifndef GPARAMDOUBLE_H
#define GPARAMDOUBLE_H
#include "labexe_global.h"

#include "GParamNum.h"

/////////////////////////////////////////////////////////////////////
//! \brief The GParamDouble class defines a GParamNum that holds a double.
/*!
*/
class LABEXE_EXPORT GParamDouble : public GParamNum
{
	Q_OBJECT

public:
	GParamDouble(QString theName, QObject* parent, GParam::Properties paramOptions = NoOption);
	~GParamDouble();

	virtual GParamDouble& operator=(double theValue) {SetParamValue(theValue); return *this;}
	virtual GParamDouble& operator=(int theValue) {SetParamValue(theValue); return *this;}
	virtual operator double() const {return DoubleValue();}

	//! Re-implemented
	GParamSettingsWidget* PopupSettingWidget();

public:
	//! For convenience. Just like ProvideNewParamWidget(), but it actually returns a GDoubleSpinBox
	GDoubleSpinBox* ProvideNewParamSpinBox(QWidget* forWhichParent);

private:
	
};

#endif // GPARAMDOUBLE_H
