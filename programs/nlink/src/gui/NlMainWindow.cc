
/// @file NlMainWindow.cc
/// @brief NlMainWindow の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "NlMainWindow.h"
#include "NlView.h"
#include "NlProblem.h"
#include <QtGui>


BEGIN_NAMESPACE_YM_NLINK

//////////////////////////////////////////////////////////////////////
// クラス NlMainWindow
//////////////////////////////////////////////////////////////////////

NlMainWindow::NlMainWindow()
{
  mNlView = new NlView();
  setCentralWidget(mNlView);

  create_actions();
  create_menus();
  create_context_menu();
  create_toolbars();
  create_statusbar();
}

void
NlMainWindow::closeEvent(QCloseEvent* event)
{
}

// 'open' アクション
void
NlMainWindow::open()
{
  QString file_name = QFileDialog::getOpenFileName(this,
						   tr("Open problem"), ".",
						   tr("Problem files (*.txt)"));
  if ( !file_name.isEmpty() ) {
    load_file(file_name);
  }
}

// @brief 'open recent file' アクション
void
NlMainWindow::open_recent_file()
{
}

void
NlMainWindow::create_actions()
{
  mOpenAction = new QAction(tr("&Open"), this);
  mOpenAction->setShortcut(tr("Ctrl+F"));
  mOpenAction->setStatusTip(tr("Open a problem file"));
  connect(mOpenAction, SIGNAL(triggered()), this, SLOT(open()));

  for (int i = 0; i < kMaxRecentFiles; ++ i) {
    mRecentFileActions[i] = new QAction(this);
    mRecentFileActions[i]->setVisible(false);
    connect(mRecentFileActions[i], SIGNAL(triggered()),
	    this, SLOT(open_recent_file()));
  }

  mAboutQtAction = new QAction(tr("About &Qt"), this);
  mAboutQtAction->setStatusTip(tr("Show the Qt library 's About box"));
  connect(mAboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

  mExitAction = new QAction(tr("E&xit"), this);
  mExitAction->setShortcut(tr("Ctrl+Q"));
  mExitAction->setStatusTip(tr("Exit application"));
  connect(mExitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void
NlMainWindow::create_menus()
{
  mFileMenu = menuBar()->addMenu(tr("&File"));
  mFileMenu->addAction(mOpenAction);
  mSeparatorAction = mFileMenu->addSeparator();
  for (int i = 0; i < kMaxRecentFiles; ++ i) {
    mFileMenu->addAction(mRecentFileActions[i]);
  }
  mFileMenu->addSeparator();
  mFileMenu->addAction(mExitAction);

  mEditMenu = menuBar()->addMenu(tr("&Edit"));

  mToolMenu = menuBar()->addMenu(tr("&Tools"));

  mOptionsMenu = menuBar()->addMenu(tr("&Options"));

  menuBar()->addSeparator();

  mHelpMenu = menuBar()->addMenu(tr("&Help"));
  mHelpMenu->addAction(mAboutQtAction);
}

void
NlMainWindow::create_context_menu()
{
}

void
NlMainWindow::create_toolbars()
{
  mFileToolBar = addToolBar(tr("&File"));
  mFileToolBar->addAction(mOpenAction);
}

void
NlMainWindow::create_statusbar()
{
}

void
NlMainWindow::load_file(QString& file_name)
{
  NlProblem problem = read_problem(file_name.toStdString());
  mNlView->set_problem(problem);
}

END_NAMESPACE_YM_NLINK
