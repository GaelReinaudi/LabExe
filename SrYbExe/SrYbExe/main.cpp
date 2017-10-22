#include "srybexe.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QString folderPath;
	if (argc >= 2) {
		folderPath = argv[1];
	}

	SrYbExe w(folderPath);
	w.show();

	return a.exec();
}

