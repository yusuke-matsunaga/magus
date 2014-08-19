#ifndef VERILOGMODEL_H
#define VERILOGMODEL_H

/// @file VerilogModel.h
/// @brief VerilogModel のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2009, 2014 Yusuke Matsunaga
/// All rights reserved.


#include <QAbstractItemModel>
#include "YmVerilog/verilog.h"
#include "YmUtils/FileRegion.h"


BEGIN_NAMESPACE_YM_VERILOG

class VlMgr;
class VmNode;

//////////////////////////////////////////////////////////////////////
/// @class VerilogModel VerilogModel.h
/// @brief verilog の構造を表すモデル
//////////////////////////////////////////////////////////////////////
class VerilogModel :
  public QAbstractItemModel
{
public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のオブジェクト
  VerilogModel(QObject* parent = NULL);

  /// @brief デストラクタ
  ~VerilogModel();


public:
  //////////////////////////////////////////////////////////////////////
  // QAbstractTableModel の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 対応するインデックスを返す．
  /// @param[in] row 行番号
  /// @param[in] column 列番号
  /// @param[in] parent 親のインデックス
  virtual
  QModelIndex
  index(int row,
	int column,
	const QModelIndex& parent) const;

  /// @brief 親のインデックスを返す．
  /// @param[in] child 子のインデックス
  virtual
  QModelIndex
  parent(const QModelIndex& child) const;

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

  /// @brief ヘッダのデータを返す．
  virtual
  QVariant
  headerData(int section,
	     Qt::Orientation orientation,
	     int role) const;


public:

  /// @brief 関連するVerilog構造をセットする．
  void
  set_vlmgr(const VlMgr& vl_mgr);

  /// @brief トークンのファイル上の位置を返す．
  FileRegion
  loc(const QModelIndex& index);


private:

  /// @brief インデックスをノードに変換する．
  VmNode*
  index2node(const QModelIndex& index) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 根のノード
  VmNode* mRootNode;

};

END_NAMESPACE_YM_VERILOG

#endif // VERILOGMODEL_H
