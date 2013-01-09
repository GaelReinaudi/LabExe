#ifndef GTESTEVENTNODE_H
#define GTESTEVENTNODE_H

#include <QObject>

class GTestEventNode : public QObject
{
	Q_OBJECT

public:
	GTestEventNode();
	~GTestEventNode();


	private slots:
		void testGui();	
};

#endif // GTESTEVENTNODE_H
