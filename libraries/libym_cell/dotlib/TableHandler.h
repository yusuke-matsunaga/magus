#ifndef LIBYM_CELL_DOTLIB_TABLEHANDLER_H
#define LIBYM_CELL_DOTLIB_TABLEHANDLER_H

/// @file libym_cell/dotlib/TableHandler.h
/// @brief TableHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "GroupHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

class TimingHandler;

//////////////////////////////////////////////////////////////////////
/// @class TableHandler TableHandler.h "TableHandler.h"
//////////////////////////////////////////////////////////////////////
class TableHandler :
  public GroupHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のハンドラ
  TableHandler(GroupHandler* parent);

  /// @brief デストラクタ
  ~TableHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部から用いられる GroupHandler の仮想関数
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

  /// @brief index_x 属性をセットする．
  /// @param[in] attr_name 属性名
  /// @param[in] value_list 値のリスト
  virtual
  bool
  add_index_x(const ShString& attr_name,
	      const vector<const PtValue*>& value_list);

  /// @brief values 属性をセットする．
  /// @param[in] attr_name 属性名
  /// @param[in] value_list 値のリスト
  virtual
  bool
  add_values(const vector<const PtValue*>& value_list);

  /// @brief domain グループをセットする．
  /// @param[in] domain ドメイン
  virtual
  bool
  add_domain(PtDomain* domain);


protected:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる GroupHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief group statement の最初に呼ばれる関数
  /// @param[in] attr_name 属性名
  /// @param[in] attr_loc ファイル上の位置
  /// @param[in] value_list 値を表すトークンのリスト
  virtual
  bool
  begin_group(const ShString& attr_name,
	      const FileRegion& attr_loc,
	      const vector<const PtValue*>& value_list);

  /// @brief group statement の最後に呼ばれる関数
  virtual
  bool
  end_group();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対応する PtTable
  PtTable* mTable;

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_TABLEHANDLER_H
