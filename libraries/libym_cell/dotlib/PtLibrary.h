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
  PtLibrary(const ShString& name);

  /// @brief デストラクタ
  ~PtLibrary();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief セルを追加する．
  /// @param[in] cell セル
  /// @retval true 成功した．
  /// @retval false エラーが起こった．
  bool
  add_cell(PtCell* cell);


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を設定する PtNode の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief simple attribute を設定する．
  /// @param[in] attr_name 属性名
  /// @param[in] value 値
  /// @return 設定が失敗したら false を返す．
  virtual
  bool
  add_simple_attr(const ShString& attr_name,
		  const PtValue* value);

  /// @brief complex attribute を設定する．
  /// @param[in] attr_name 属性名
  /// @param[in] value_list 値のリスト
  /// @return 設定が失敗したら false を返す．
  virtual
  bool
  add_complex_attr(const ShString& attr_name,
		   const vector<const PtValue*>& value_list);


public:
  //////////////////////////////////////////////////////////////////////
  // PtLibrary の情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ライブラリ名を得る．
  ShString
  name() const;

  /// @brief セル数を得る．
  ymuint
  cell_num() const;

  /// @brief セルを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < cell_num() )
  const PtCell*
  cell(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  ShString mName;

  // セルのリスト
  vector<PtCell*> mCellList;

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_PTLIBRARY_H
