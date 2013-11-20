#ifndef LCCLASS_H
#define LCCLASS_H

/// @file LcClass.h
/// @brief LcClass のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "libcomp_nsdef.h"
#include "ym_logic/TvFunc.h"
#include "ym_logic/TvFuncM.h"


BEGIN_NAMESPACE_YM_CELL_LIBCOMP

//////////////////////////////////////////////////////////////////////
/// @class LcClass LcClass.h "LcClass.h"
/// @brief テンポラリに使用するセルクラス用のクラス
//////////////////////////////////////////////////////////////////////
class LcClass
{
  friend class LcGroupMgr;

public:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] builtin 組み込みクラスの時 true にするフラグ
  /// @param[in] repfunc 代表関数
  LcClass(ymuint id,
	  bool builtin,
	  const TvFunc& repfunc);

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] builtin 組み込みクラスの時 true にするフラグ
  /// @param[in] repfunc 代表関数
  LcClass(ymuint id,
	  bool builtin,
	  const TvFuncM& repfunc);

  /// @brief デストラクタ
  ~LcClass();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  ymuint
  id() const;

  /// @brief 組み込みクラスの時 true を返す．
  bool
  builtin() const;

  /// @brief 代表関数を返す．
  TvFuncM
  repfunc() const;

  /// @brief 同位体変換のリストを返す．
  const vector<NpnMapM>&
  idmap_list() const;

  /// @brief 実体を返す．
  const CellClass*
  cell_class() const;

  /// @brief このクラスに属しているグループのリストを返す．
  const vector<LcGroup*>&
  group_list() const;

  /// @brief グループを追加する．
  void
  add_group(LcGroup* group,
	    const NpnMapM& map);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  ymuint32 mId;

  // 代表関数
  TvFuncM mRepFunc;

  // 同位体変換のリスト
  vector<NpnMapM> mIdmapList;

  // 実体のオブジェクト
  CellClass* mCellClass;

  // グループのリスト
  vector<LcGroup*> mGroupList;

};

END_NAMESPACE_YM_CELL_LIBCOMP

#endif // LCCLASS_H
