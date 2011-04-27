#ifndef LIBYM_CELL_DOTLIB_PTLIBRARY_H
#define LIBYM_CELL_DOTLIB_PTLIBRARY_H

/// @file libym_cell/dotlib/PtLibrary.h
/// @brief PtLibrary のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PtNode.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class PtLibrary PtLibrary.h "PtLibrary.h"
/// @brief library グループを表すパース木の要素
//////////////////////////////////////////////////////////////////////
class PtLibrary :
  public PtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] name ライブラリ名
  PtLibrary(const PtValue* name);

  /// @brief デストラクタ
  ~PtLibrary();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 子供を追加する．
  /// @param[in] attr_name 属性名
  /// @param[in] node 追加する子供のノード
  /// @note デフォルトの実装はなにもしない．
  virtual
  void
  add_child(const ShString& attr_name,
	    PtNode* node);

  /// @brief セルを追加する．
  /// @param[in] cell セル
  /// @retval true 成功した．
  /// @retval false エラーが起こった．
  bool
  add_cell(PtCell* cell);


public:

  /// @brief セル数を得る．
  ymuint
  cell_num() const;

  /// @brief セルを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < cell_num() )
  const PtCell*
  cell(ymuint pos) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を参照する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値の数を返す．
  /// @note simple attribute なら常に1
  virtual
  ymuint
  value_num() const;

  /// @brief 値を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < value_num() )
  virtual
  const PtValue*
  value(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  const PtValue* mName;

  // セルのリスト
  vector<PtCell*> mCellList;

  // セル名をキーにした連想配列
  hash_map<ShString, PtCell*> mCellMap;

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_PTLIBRARY_H
