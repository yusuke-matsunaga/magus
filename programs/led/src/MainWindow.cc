
/// @file MainWindow.cc
/// @brief MainWindow の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include <QtGui>
#include "led_nsdef.h"
#include "MainWindow.h"


BEGIN_NAMESPACE_YM_LED

//////////////////////////////////////////////////////////////////////
// クラス MainWindow
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MainWindow::MainWindow()
{
  create_actions();
  create_menus();
  create_context_menu();
  create_toolbars();
  create_statusbar();

  read_settings();

  set_currentfile("");
}

// @brief アクションを作る．
void
MainWindow::create_actions()
{
  // 'new' アクション
  mNewAction = new QAction(tr("&New"), this);
  mNewAction->setIcon(QIcon(":/images/new.png"));
  mNewAction->setShortcut(tr("Ctrl+N"));
  mNewAction->setStatusTip(tr("Create a new file"));

  // 'open' アクション
  mOpenAction = new QAction(tr("&Open"), this);
  mOpenAction->setIcon(QIcon(":/images/open.png"));
  mOpenAction->setShortcut(tr("Ctrl+F"));
  mOpenAction->setStatusTip(tr("Open file"));

  // 'save' アクション
  mSaveAction = new QAction(tr("&Save"), this);
  mSaveAction->setIcon(QIcon(":/images/save.png"));
  mSaveAction->setShortcut(tr("Ctrl+W"));
  mSaveAction->setStatusTip(tr("Save file"));

  // 'save as' アクション
  mSaveAsAction = new QAction(tr("Save &As"), this);
  mSaveAsAction->setIcon(QIcon(":/images/saveas.png"));
  mSaveAsAction->setShortcut(tr("Ctrl+A"));
  mSaveAsAction->setStatusTip(tr("Save file As ..."));

  // 'exit' アクション
  mExitAction = new QAction(tr("E&xit"), this);
  mExitAction->setIcon(QIcon(":/images/exit.png"));
  mExitAction->setShortcut(tr("Ctrl+Q"));
  mExitAction->setStatusTip(tr("Exit"));

  // 最近開いたファイルアクション
  for (ymuint i = 0; i < kMaxRecentFiles; ++ i) {
    mRecentFileActions[i] = new QAction(this);
    mRecentFileActions[i]->setVisible(false);
    connect(mRectnFileActions[i], SIGNAL(triggered()),
	    this, SLOT(open_recent_file()));
  }

  // 'about' アクション
  mAboutAction = new QAction(tr("About &Qt"), this);
  mAboutAction->setStatusTip(tf("Show the Qt library's About box"));
  connect(mAboutAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

// @brief メニューを作る．
void
MainWindow::create_menus()
{
  mFileMenu = menuBar()->addMenu(tr("&File"));
  mFileMenu->addAction(mNewAction);
  mFileMenu->addAction(mOpenAction);
  mFileMenu->addAction(mSaveAction);
  mFileMenu->addAction(mSaveAsAction);

  mSeparatorAction = mFileMenu->addSeparator();

  for (ymuint i = 0; i < kMaxRecentFiles; ++ i) {
    mFileMenu->addAction(mRecentFileActions[i]);
  }

  mFileMenu->addSeparator();
  mFileMenu->addAction(mExitAction);


}

// @brief コンテキストメニューを作る．
void
MainWindow::create_context_menu()
{
}

// @brief ツールバーを作る．
void
MainWindow::create_toolbars()
{
  mFileToolBar = addToolBar(tr("&File"));
  mFileToolBar->addAction(mNewAction);
  mFileToolBar->addAction(mOpenAction);
  mFileToolBar->addAction(mSaveAction);

  mEditToolBar = addToolBar(tr("&Edit"));
}

// @brief ステータスバーを作る．
void
MainWindow::create_statusbar()
{
}

// @brief 設定を読み込む．
void
MainWindow::read_settings()
{
}

// @brief 設定を書き出す．
void
MainWindow::write_settings()
{
}

// @brief ファイルを読み込む．
void
MainWindow::load_file(const QString& filename)
{
}

// @brief ファイルを書き出す．
bool
MainWindow::save_file(cosnt QString& filename)
{
  return true;
}

// @brief カレントファイルを設定する．
void
MainWindow::set_current_file(const QString& filename)
{
  mCurFile = filename;
  setWindowModified(false);

  QString shown_name = "Untitled";

  if ( !mCurFile.isEmpty() ) {
    shown_name = stripped_name(mCurFile);
    mRecentFiles.removeAll(mCurFile);
    mRecentFiles.prepend(mCurFile);
    update_recent_file_actions();
  }

  setWindowTitle(tr("%1[*] - %2").arg(shown_name).arg(tr("Logic circuit")));

}

// @brief 最近のファイルリストを更新する．
void
MainWindow::update_recent_file_actions()
{
}

// @brief ファイル名の本体を返す．
QString
MainWindow::stripped_name(const QString& fullFileName)
{
  return QFileInfo(fullFileName).fileName();
}

// @brief 'new file' スロット
void
MainWindow::new_file()
{
}

// @brief 'open' スロット
void
MainWindow::open()
{
}

// @brief 'save' スロット
bool
MainWindow::save()
{
  if ( mCurFile.isEmpty() ) {
    return save_as();
  }
  else {
    return save_file(mCurFile);
  }
}

// @brief 'save as' スロット
void
MainWindow::save_as()
{
  QString filename = QFileDialog::getSaveFileName(this,
						  tr("Save logic circuit"), ".",
						  tf("Logic circuit files (*.lcf)"));
  if ( filename.is_Empty() ) {
    return false;
  }

  return save_file(filename);
}

// @brief 'about' スロット
void
MainWindow::about()
{
}

// @brief close イベント
// @param[in] event イベント
void
MainWindow::close_event(QCloseEvent* event)
{
  if ( okToContinue() ) {
    write_settings();
    event->accept();
  }
  else {
    event->ignore();
  }
}

END_NAMESPACE_YM_LED
