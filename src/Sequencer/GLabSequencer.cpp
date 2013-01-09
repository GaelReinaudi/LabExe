#include "GLabSequencer.h"
#include "Sequencer/G_old_Sequence.h"
#include "ModelView/GInstructionView.h"
#include "ModelView/GEventTreeView.h"
#include "ModelView/GSequenceView.h"
#include "ModelView/GEventTreeModel.h"
#include "ModelView/GSequenceModel.h"
#include "device.h"
#include "Instruction/GCurveInstruction.h"
#include "Curve/GCurveSegment.h"

#include <QDoubleSpinBox>

GLabSequencer::GLabSequencer(QWidget *parent /*= 0*/)
	: GWorkBench(parent)
{
	ui.setupUi(this);
	MakeTheSequence();
	show();
}

GLabSequencer::~GLabSequencer()
{

}

void GLabSequencer::MakeTheSequence()
{
// 	G_old_Sequence* pSeq = new G_old_Sequence(this);
// 
// 	GInstructionView* pInsView = new GInstructionView(this);
// 	pInsView->show();
// 	pInsView->resize(300, 60);
// 	GInstructionView* pInsView2 = new GInstructionView(this);
// 	pInsView2->show();
// 	pInsView2->resize(290, 60);
// 	pInsView2->move(310, 5);
// 	GEventTreeView* pInsTreeView = new GEventTreeView(this);
// 	pInsTreeView->show();
// 	pInsTreeView->resize(250, 433);
// 	pInsTreeView->move(775, 90);
// 	GSequenceView* pSeqView = new GSequenceView(this);
// 	pSeqView->show();
// 	pSeqView->resize(750, 433);
// 	pSeqView->move(5, 90);
// 	GSequenceModel* pSeqModel = new GSequenceModel(this);
// 	pSeqModel->SetSequence(pSeq);
// 	pSeqView->setModel(pSeqModel);
// 	QDoubleSpinBox* spin = new QDoubleSpinBox(this);
// 	spin->setSingleStep(0.02);
// 	spin->setMinimum(-1);
// 	spin->move(310, 50);
// 	QDoubleSpinBox* spin2 = new QDoubleSpinBox(this);
// 	spin2->setSingleStep(0.02);
// 	spin2->setMinimum(-1);
// 	spin2->move(350, 50);
// 
// 	GDevice* pDev = new GDummyDevice("rzrzzr", this);	pDev->ReName("pxiDo1");
// 	GDevice* pDev2 = new GDummyDevice("rzdrzzr", this);	pDev2->ReName("pxiDo2");
// 	GDevice* pDev3 = new GDummyDevice("rzrfsdzzr", this);	pDev3->ReName("pxiDo3");
// 	GDevice* pDev4 = new GDummyDevice("rzrzzrzrggzr", this);	pDev4->ReName("pxiDo4");
// 	GDevice* pDev5 = new GDummyDevice("rzrzéggfazgzazr", this);	pDev5->ReName("pxiDo5");
// 
// 	GCurveInstruction* pIns = new GCurveInstruction(0);
// 	pIns->SetSignalDevice(pDev);
// 	GCurveInstruction* pIns2 = new GCurveInstruction(0);
// 	pIns2->SetSignalDevice(pDev2);
// 	GCurveInstruction* pIns3 = new GCurveInstruction(0);
// 	pIns3->SetSignalDevice(pDev3);
// 	GCurveInstruction* pIns4 = new GCurveInstruction(0);
// 	pIns4->SetSignalDevice(pDev4);
// 	GCurveInstruction* pIns5 = new GCurveInstruction(0);
// 	pIns5->SetSignalDevice(pDev5);
// 	GCurveSegment* pSeg = new GCurveSegment(0);
// 	pSeg->SetDuration(0.25)->SetInitialValue(-2)->SetFinalValue(2);
// 	GCurveSegment* pSeg2 = new GCurveSegment(0);
// 	pSeg2->SetDuration(0.20)->SetInitialValue(-4)->SetFinalValue(4);
// 	GCurveSegment* pSeg3 = new GCurveSegment(0);
// 	pSeg3->SetDuration(0.15)->SetInitialValue(-6)->SetFinalValue(6);
// 	GCurveSegment* pSeg4 = new GCurveSegment(0);
// 	pSeg4->SetDuration(0.10)->SetInitialValue(-8)->SetFinalValue(8);
// 	GCurveSegment* pSeg5 = new GCurveSegment(0);
// 	pSeg5->SetDuration(0.05)->SetInitialValue(-10)->SetFinalValue(10);
// 
// 	pIns->setCurve(pSeg);
// 	pIns2->setCurve(pSeg2);
// 	pIns3->setCurve(pSeg3);
// 	pIns4->setCurve(pSeg4);
// 	pIns5->setCurve(pSeg5);
// 
// 	connect(spin, SIGNAL(valueChanged( double)), pIns2, SLOT(testSlot(double)));
// 	connect(spin2, SIGNAL(valueChanged( double)), pIns3, SLOT(testSlot(double)));
// 	//pIns->SetAnchorPointTime(0.015);
// 	pIns2->SetAnchorPointTime(-0.015);
// 	//pIns3->SetAnchorPointTime(-0.15);
// 	//pIns4->SetAnchorPointTime(-0.30);
// 	//pIns5->SetAnchorPointTime(-0.45);
// 
// 	pIns->SynchronizeWith(pSeq);
// 	pIns->DelayFromParent(0.5);
// 	pIns2->SynchronizeWith(pSeq);
// 	pIns2->DelayFromParent(0.025);
// 	pIns3->SynchronizeWith(pIns2);
// 	pIns3->DelayFromParent(0.005);
// 	pIns4->SynchronizeWith(pIns2);
// 	pIns4->DelayFromParent(0.005);
// 	pIns5->SynchronizeWith(pIns3);
// 	pIns5->DelayFromParent(0.005);
// 
// 	pInsView->SetInstruction(pIns3);
// 	pInsView2->SetInstruction(pIns2);
// 
// 	GEventTreeModel* pTreeModel = new GEventTreeModel(this);
// 	pTreeModel->SetRootEvent(pSeq);
// 	//	new ModelTest(pTreeModel, this);
// 	pInsTreeView->setModel(pTreeModel);
// 	pInsTreeView->expandAll();
// 	pSeq->RemoveDevice(pDev);
}