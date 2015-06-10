#ifndef NLMAILWINDOW_H
#define NLMAILWINDOW_H

/// @file NlMailWindow.h
/// @brief NlMailWindow のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "nlink_nsdef.h"
#include <QMainWindow>


class QAction;

namespace nsYm { namespace nsNlink {

class NlView;

//////////////////////////////////////////////////////////////////////
/// @class NlMainWindow NlMainWindow.h "NlMainWindow.h"
/// @brief nlview 用のメインウィンドウのクラス
//////////////////////////////////////////////////////////////////////
class NlMainWindow :
  public QMainWindow
{
  Q_OBJECT

public:

  NlMainWindow();


protected:

  void
  closeEvent(QCloseEvent* event);


private slots:
  //////////////////////////////////////////////////////////////////////
  // 内部スロット
  //////////////////////////////////////////////////////////////////////

  /// @brief 'open' アクション
  void
  open();

  /// @brief 'open recent file' アクション
  void
  open_recent_file();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  void
  create_actions();

  void
  create_menus();

  void
  create_context_menu();

  void
  create_toolbars();

  void
  create_statusbar();

  void
  load_file(const QString& file_name);

  void
  set_current_file(const QString& file_name);

  void
  update_recent_file_actions();

  static
  QString
  stripped_name(const QString& full_file_name);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  NlView* mNlView;

  QStringList mRecentFiles;

  QString mCurFile;

  static
  const int kMaxRecentFiles = 5;

  QAction* mRecentFileActions[kMaxRecentFiles];

  QAction* mSeparatorAction;

  QMenu* mFileMenu;

  QMenu* mEditMenu;

  QMenu* mToolMenu;

  QMenu* mOptionsMenu;

  QMenu* mHelpMenu;

  QAction* mAboutQtAction;

  QToolBar* mFileToolBar;

  QToolBar* mEditToolBar;

  QAction* mOpenAction;

  QAction* mExitAction;

};

} }

#endif // NLMAILWINDOW_H
