#ifndef VLTOKENLIST_H
#define VLTOKENLIST_H

/// @file libym_verilog/tests/vlview/VlTokenList.h
/// @brief VlTokenList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlTokenList.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include <QAbstractTableModel>
#include <ym_utils/FileRegion.h>


//////////////////////////////////////////////////////////////////////
/// @class VlTokenList VlTokenList.h
/// @brief verilog のトークンリストを表すモデル
//////////////////////////////////////////////////////////////////////
class VlTokenList :
  public QAbstractTableModel
{
  
  typedef nsYm::FileRegion FileRegion;
  
public:
  
  /// @brief コンストラクタ
  /// @param[in] parent 親のオブジェクト
  VlTokenList(QObject* parent = NULL);

  /// @brief デストラクタ
  ~VlTokenList();
  

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

#endif // VLTOKENLIST_H
