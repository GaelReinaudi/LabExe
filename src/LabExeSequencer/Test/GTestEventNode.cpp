#include "GTestEventNode.h"
#include <QtGui>
#include <QtTest/QtTest>
#include "Model/GEventNode.h"

GTestEventNode::GTestEventNode()
	: QObject()
{

}

GTestEventNode::~GTestEventNode()
{

}

void GTestEventNode::testGui()
{
	QLineEdit lineEdit;
	QTest::keyClicks(&lineEdit, "hello world");
	QCOMPARE(lineEdit.text(), QString("hello world"));
}

QTEST_MAIN(GTestEventNode)
#include "moc_GTestEventNode.cpp"