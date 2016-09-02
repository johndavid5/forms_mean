#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <map>
using namespace std;

#include <QMainWindow>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <QCheckBox>
#include <QTableView>
#include <QTableWidget>

#include "logger.h"
#include "FormsModel.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

	public:
		static const char* VERSION;
		static const char* APP_NAME;

		MainWindow(int argc, char** argv, map<string, string>& leConfigMap, JDA::Logger& leLogger ); 
		virtual ~MainWindow();

	protected:
		void setUpCentralWidget();

	public slots:

	protected slots:

	private:
		map<string, string>& configMap;
		JDA::Logger& logger;

		int argc;
		char** argv;

		QWidget* centralWidget;

		QTableView* tableView;

		FormsModel* formsModel;
};

#endif /* #ifndef MAINWINDOW_H */
