#include "winlabexe.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	WinLabexe w;
	w.show();
	return a.exec();
}
