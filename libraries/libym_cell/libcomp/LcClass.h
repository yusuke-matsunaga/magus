#ifndef LCCLASS_H
#define LCCLASS_H

/// @file LcClass.h
/// @brief LcClass のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "libdump_nsdef.h"
#include "ym_logic/TvFunc.h"
#include "ym_logic/TvFuncM.h"


BEGIN_NAMESPACE_YM_CELL_LIBCOMP

//////////////////////////////////////////////////////////////////////
/// @class LcClass LcClass.h "LcClass.h"
/// @brief テンポラリに使用するセルクラス用のクラス
//////////////////////////////////////////////////////////////////////
class LcClass
{
public:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] repfunc 代表関数
  LcClass(ymuint id,
	  const TvFunc& repfunc);

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] repfunc 代表関数
  LcClass(ymuint id,
	  const TvFuncM& repfunc);

  /// @brief デストラクタ
  ~LcClass();


public:

  /// @brief ID番号を返す．
  ymuint
  id() const;

  /// @brief 代表関数を返す．
  TvFuncM
  repfunc() const;

  /// @brief 実体を返す．
  const CellClass*
  cell_class() const;

  /// @brief このクラスに属しているグループのリストを返す．
  const vector<LdGroup*>&
  group_list() const;

  /// @brief グループを追加する．
  void
  add_group(LdGroup* group,
	    const NpnMapM& map);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  ymuint32 mId;

  // 代表関数
  TvFuncM mRepFunc;

  // 実体のオブジェクト
  CellClass* mCellClass;

  // グループのリスト
  vector<LdGroup*> mGroupList;

};

END_NAMESPACE_YM_CELL_LIBCOMP

#endif // LCCLASS_H
