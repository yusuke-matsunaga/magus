
/// @file libym_verilog/tests/ptview/VerilogView.cc
/// @brief VerilogView の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VerilogView.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include "VerilogView.h"
#include <QTextEdit>
#include <QHBoxLayout>
#include <QFile>
#include <QTextStream>
#include <iostream>

  
// @brief コンストラクタ
// @param[in] parent 親のウィジェット
VerilogView::VerilogView(QWidget* parent) :
  QWidget(parent)
{
  mTextEdit = new QTextEdit;
  
  mTextEdit->setReadOnly(true);
  
  QHBoxLayout* layout = new QHBoxLayout;
  layout->addWidget(mTextEdit);
  setLayout(layout);
}

// @brief デストラクタ
VerilogView::~VerilogView()
{
}

// @brief ファイルと結びつける．
// @param[in] file_name ファイル名
// @return 読み込みに成功したら true を返す．
bool
VerilogView::open(const char* file_name)
{
  using namespace std;
  
  QFile file(file_name);
  if ( !file.open(QIODevice::ReadOnly) ) {
    cerr << file_name << " : Could not open: "
	 << qPrintable(file.errorString()) << endl;
    return false;
  }

  int pos = 0;
  QTextStream in(&file);
  while ( !in.atEnd() ) {
    QString line = in.readLine();
    mTextEdit->append(line);
    mStartPos.push_back(pos);
    pos += line.length() + 1;
  }
  // ちょっと美しくない Hack
  mTextEdit->append(" ");
  return true;
}

// @brief カーソルの位置を該当する箇所にセットする．
// @param[in] cursor 対象のカーソル
// @param[in] start_line 開始位置の行番号
// @param[in] start_column 開始位置のコラム番号
// @param[in] end_line 終了位置の行番号
// @param[in] end_column 終了位置のコラム番号
void
VerilogView::set_cursor_pos(QTextCursor& cursor,
			    int start_line,
			    int start_column,
			    int end_line,
			    int end_column)
{
  int start_pos = xy2pos(start_line, start_column) - 1;
  int end_pos = xy2pos(end_line, end_column);
  cursor.setPosition(start_pos, QTextCursor::MoveAnchor);
  cursor.setPosition(end_pos, QTextCursor::KeepAnchor);
}

#if 0
// @brief 指定された領域を強調表示する．
void
VerilogView::hilight(QTextCursor* cursor)
{
  QList<QTextEdit::ExtraSelection> selections;

  QTextEdit::ExtraSelection selection;
  selection.cursor = *cursor;
  selection.format.setForeground(QColor("red"));
  selection.format.setBackground(QColor("black"));
  selections.push_back(selection);
  mTextEdit->setExtraSelections(selections);
}
#endif

// @brief 指定された領域を強調表示する．
// @param[in] start_line 開始位置の行番号
// @param[in] start_column 開始位置のコラム番号
// @param[in] end_line 終了位置の行番号
// @param[in] end_column 終了位置のコラム番号
void
VerilogView::hilight(int start_line,
		     int start_column,
		     int end_line,
		     int end_column)
{
  QTextCursor cursor(mTextEdit->document());
  set_cursor_pos(cursor, start_line, start_column, end_line, end_column);
  mTextEdit->setTextCursor(cursor);
  mTextEdit->ensureCursorVisible();
}

// @brief 行番号とコラム番号から絶対位置を取り出す．
// @param[in] line 行番号
// @param[in] column コラム番号
int
VerilogView::xy2pos(int line,
		    int column) const
{
  return mStartPos[line - 1] + column - 1;
}
