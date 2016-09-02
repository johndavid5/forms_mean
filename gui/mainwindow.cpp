#include <QtGui>
#include <QAction>
#include <QMessageBox>
#include <QFileDialog>
#include <QTableView>
#include <QStatusBar>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <time.h> /* clock_t, clock, CLOCKS_PER_SEC */
using namespace std;

#include "mainwindow.h"

#include "utils.h"
#include "logger.h"

/* static */ const char* MainWindow::VERSION = "0.70"; 

/* static */ const char* MainWindow::APP_NAME = "SEC Forms";

MainWindow::MainWindow(int argc, char** argv, map<string, string>& leConfigMap, JDA::Logger& leLogger ) :
 configMap( leConfigMap), logger( leLogger) {
	const char* sWho = "MainWindow::MainWindow";

	this->argc = argc;
	this->argv = argv;

	logger(JDA::Logger::INFO) << sWho << "(): Welcome to " << APP_NAME << ", version " << VERSION << ", " << JDA::Utils::getUserName() << "..." << endl;

	logger(JDA::Logger::INFO) << sWho << "(): Your local time is " << JDA::Utils::get_local_pretty_timestamp() << "..." << endl;

	setUpCentralWidget();
	setCentralWidget(this->centralWidget);

	//setWindowIcon(QIcon(":/images/vf2.ico"));
	//setWindowIcon(QIcon(":/images/vintage-logo-new.ico"));
	//setWindowIcon(QIcon(":/images/vlogo.ico"));


}/* MainWindow::MainWindow() */


MainWindow::~MainWindow()
{
	const char* sWho = "MainWindow::~MainWindow";
	(void)sWho;
}

void MainWindow::setUpCentralWidget(){

	const char* sWho = "MainWindow::setUpCentralWidget";

	centralWidget = new QWidget();

	tableView = new QTableView();
	//tableView->setSelectionBehavior( QAbstractItemView::SelectRows ); // Will select only entire rows...
	tableView->setAlternatingRowColors( false );
	tableView->setSortingEnabled( true );

	logger(JDA::Logger::INFO) << sWho << "(): Calling capturesModel = new CapturesModel(this)..." << endl;
	formsModel = new FormsModel( this );

	logger(JDA::Logger::INFO) << sWho << "(): Calling tableView->setModel(capturesModel)..." << endl;
	tableView->setModel( formsModel );
	
    QVBoxLayout* vLayout = new QVBoxLayout;

	vLayout->addWidget( tableView );

    centralWidget->setLayout(vLayout);

}/* void MainWindow::setUpCentralWidget() */




