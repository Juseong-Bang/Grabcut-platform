#ifdef Q_COMPILER_INITIALIZER_LISTS
#undef Q_COMPILER_INITIALIZER_LISTS
#endif

#include "platform.h"
#include <QtWidgets/QApplication>
//#include <vld.h>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	QFile qss("stylesheet.qss");
	qss.open(QFile::ReadOnly);
	app.setStyleSheet(qss.readAll());
	qss.close();
	
	/*
	CPlatform p;
	p.show();
	*/
	
	BorderlessMainWindow w;
	w.show();
	
	return app.exec();
}