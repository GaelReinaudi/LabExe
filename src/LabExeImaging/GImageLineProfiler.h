#ifndef GIMAGELINEPROFILER_H
#define GIMAGELINEPROFILER_H

#include "GImageProcessor.h"

/////////////////////////////////////////////////////////////////////
//! \brief The GImageLineProfiler class implements an image processor that reads the values of an image along lines.
/*!
*/ 
class GImageLineProfiler : public GImageProcessor
{
	Q_OBJECT

public:
	GImageLineProfiler(QObject *parent, QString uniqueIdentifierName = "");
	~GImageLineProfiler();

protected:
	//! Reimplemented
	void PopulateDeviceWidget(GDeviceWidget* theDeviceWidget);
	//! Re-implemented
	void ProcessImageAOIed(const GImageDouble & aoiImage);

private:
	QGraphicsLineItem* m_pCrossHair_H;
	QGraphicsLineItem* m_pCrossHair_V;
	
};

#endif // GIMAGELINEPROFILER_H
