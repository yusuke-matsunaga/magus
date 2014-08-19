#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/// @file MainWindow.h
/// @brief MainWindow のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include <QMainWindow>


namespace nsYm {
namespace nsLed {

//////////////////////////////////////////////////////////////////////
/// @class MainWindow MainWindow.h "MainWindow.h"
/// @brief メインウィンドウのクラス
//////////////////////////////////////////////////////////////////////
class MainWindow :
  public QMainWindow
{
  Q_OBJECT

public:

  /// @brief コンストラクタ
  MainWindow();


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから呼ばれる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief close イベント
  /// @param[in] event イベント
  void
  close_event(QCloseEvent* event);


private slots:
  //////////////////////////////////////////////////////////////////////
  // 内部で呼び出されるスロット
  //////////////////////////////////////////////////////////////////////

  /// @brief 'new file' スロット
  void
  new_file();

  /// @brief 'open' スロット
  void
  open();

  /// @brief 'save' スロット
  bool
  save();

  /// @brief 'save as' スロット
  bool
  save_as();

  /// @brief 'about' スロット
  void
  about();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で呼ばれる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief アクションを作る．
  void
  create_actions();

  /// @brief メニューを作る．
  void
  create_menus();

  /// @brief コンテキストメニューを作る．
  void
  create_context_menu();

  /// @brief ツールバーを作る．
  void
  create_toolbars();

  /// @brief ステータスバーを作る．
  void
  create_statusbar();

  /// @brief 設定を読み込む．
  void
  read_settings();

  /// @brief 設定を書き出す．
  void
  write_settings();

  /// @brief ファイルを読み込む．
  void
  load_file(const QString& filename);

  /// @brief ファイルを書き出す．
  bool
  save_file(cosnt QString& filename);

  /// @brief カレントファイルを設定する．
  void
  set_current_file(const QString& filename);

  /// @brief 最近のファイルリストを更新する．
  void
  update_recent_file_actions();

  /// @brief ファイル名の本体を返す．
  QString
  stripped_name(const QString& fullFileName);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 最近のファイルリスト
  QStringList mRecentFiles;

  // 現在のファイル
  QString mCurFile;

  // アクションリストの最近のファイルの最大数
  static
  const ymuint kMaxRecentFiles;

  // 最近のファイルのアクションリスト
  QAction* mRecentFileActions[kMaxRecentFiles];

  // アクションリスト中のセパレータ
  QAction* mSeparatorAction;

  // 'file' メニュー
  QMenu* mFileMenu;

  // 'edit' メニュー
  QMenu* mEditMenu;

  // 'file' ツールバー
  QToolBar* mFileToolBar;

  // 'edit' ツールバー
  QToolBar* mEditToolBar;

  // 'new' アクション
  QAction* mNewAction;

  // 'open' アクション
  QAction* mOpenAction;

  // 'save' アクション
  QAction* mSaveAction;

  // 'save as' アクション
  QAction* mSaveAsAction;

  // 'exit' アクション
  QAction* mExitAction;

  // 'about' アクション
  QAction* mAboutAction;

};

} // nsLed
} // nsYm

#endif // MAINWINDOW_H
