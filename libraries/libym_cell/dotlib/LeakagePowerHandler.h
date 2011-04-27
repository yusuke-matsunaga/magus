#ifndef LIBYM_CELL_DOTLIB_LEAKAGEPOWERHANDLER_H
#define LIBYM_CELL_DOTLIB_LEAKAGEPOWERHANDLER_H

/// @file libym_cell/dotlib/LeakagePowerHandler.h
/// @brief LeakagePowerHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "GroupHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class LeakagePowerHandler LeakagePowerHandler.h "LeakagePowerHandler.h"
/// @brief cell group 用のハンドラ
//////////////////////////////////////////////////////////////////////
class LeakagePowerHandler :
  public GroupHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のハンドラ
  LeakagePowerHandler(GroupHandler* parent);

  /// @brief デストラクタ
  virtual
  ~LeakagePowerHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // 他のクラスから用いられる関数
  //////////////////////////////////////////////////////////////////////


protected:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる仮想関数
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

  // 対応する PtLeakagePower;
  PtLeakagePower* mLeakagePower;

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_LEAKAGEPOWERHANDLER_H
