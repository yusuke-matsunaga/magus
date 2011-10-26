#ifndef LCGROUPMGR_H
#define LCGROUPMGR_H

/// @file LcGroupMgr.h
/// @brief LcGroupMgr.h
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "libcomp_nsdef.h"
#include "ym_logic/TvFuncM.h"
#include "ym_utils/BinIO.h"


BEGIN_NAMESPACE_YM_CELL_LIBCOMP

//////////////////////////////////////////////////////////////////////
/// @class LcGroupMgr LcGroupMgr.h "LcGroupMgr.h"
/// @brief セルのグループ分けを行う基底クラス
//////////////////////////////////////////////////////////////////////
class LcGroupMgr
{
public:

  /// @brief コンストラクタ
  /// @param[in] libcomp 親の LibComp
  LcGroupMgr(LibComp& libcomp);

  /// @brief デストラクタ
  ~LcGroupMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // このクラスのメインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を空にする．
  void
  clear();

  /// @brief セルを追加する．
  /// @param[in] cell セル
  void
  add_cell(const Cell* cell);

  /// @brief f に対応する LcGroup を求める．
  /// @param[in] f 関数
  /// @note なければ新規に作る．
  LcGroup*
  find_group(const TvFuncM& f);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief セルのシグネチャ関数を作る．
  /// @param[in] cell セル
  /// @param[out] f シグネチャ関数
  virtual
  void
  gen_signature(const Cell* cell,
		TvFuncM& f) = 0;

  /// @brief 代表関数を求める．
  /// @param[in] f 関数
  /// @param[out] repfunc 代表関数
  /// @param[out] xmap 変換
  virtual
  void
  find_repfunc(const TvFuncM& f,
	       TvFuncM& repfunc,
	       NpnMapM& xmap) = 0;

  /// @brief 同位体変換リストを求める．
  /// @param[in] func 対象の関数
  /// @param[out] idmap_list 同位体変換のリスト
  virtual
  void
  find_idmap_list(const TvFuncM& func,
		  vector<NpnMapM>& idmap_list) = 0;


protected:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるクラスメソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief 代表関数を求める．
  static
  void
  default_repfunc(const TvFuncM& f,
		  TvFuncM& repfunc,
		  NpnMapM& xmap);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親の LibComp
  LibComp& mLibComp;

  // 多出力論理関数をキーとしてグループ番号を保持するハッシュ表
  hash_map<TvFuncM, ymuint> mGroupMap;

  // 代表関数をキーとしてクラス番号を保持するハッシュ表
  hash_map<TvFuncM, ymuint> mClassMap;

};

END_NAMESPACE_YM_CELL_LIBCOMP

#endif // LCGROUPMGR_H
