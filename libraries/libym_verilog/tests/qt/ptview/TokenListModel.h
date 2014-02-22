#ifndef TOKENLISTMODEL_H
#define TOKENLISTMODEL_H

/// @file TokenListModel.h
/// @brief TokenListModel のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2009, 2014 Yusuke Matsunaga
/// All rights reserved.


#include <QAbstractTableModel>
#include "utils/FileRegion.h"


//////////////////////////////////////////////////////////////////////
/// @class TokenListModel TokenListModel.h
/// @brief verilog のトークンリストを表すモデル
//////////////////////////////////////////////////////////////////////
class TokenListModel :
  public QAbstractTableModel
{

  typedef nsYm::FileRegion FileRegion;

public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のオブジェクト
  TokenListModel(QObject* parent = NULL);

  /// @brief デストラクタ
  ~TokenListModel();


public:
  //////////////////////////////////////////////////////////////////////
  // QAbstractTableModel の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief コラム数を返す．
  virtual
  int
  columnCount(const QModelIndex& parent = QModelIndex()) const;

  /// @brief 行数を返す．
  virtual
  int
  rowCount(const QModelIndex& parent = QModelIndex()) const;

  /// @brief 該当するデータを返す．
  virtual
  QVariant
  data(const QModelIndex& index,
       int role = Qt::DisplayRole) const;


public:

  /// @brief トークンを追加する．
  /// @param[in] id トークン番号
  /// @param[in] file_region トークンのファイル上の位置
  /// @param[in] str 文字列
  void
  add_token(int id,
	    const FileRegion& file_region,
	    const char* str);

  /// @brief トークンのファイル上の位置を返す．
  const FileRegion&
  loc(int row_pos);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いるデータ構造
  //////////////////////////////////////////////////////////////////////

  struct Token
  {
    Token(int id,
	  const FileRegion& file_region,
	  const char* str);

    // トークン番号
    int mId;

    // ファイル上の位置
    FileRegion mLoc;

    // 文字列
    QString mStr;

  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // トークンのリスト
  QList<Token> mList;

};

#endif // TOKENLISTMODEL_H
