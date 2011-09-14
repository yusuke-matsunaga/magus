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
  LcGroupMgr();

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


public:
  //////////////////////////////////////////////////////////////////////
  // 内容情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief セルグループの数を返す．
  ymuint
  group_num() const;

  /// @brief セルグループを返す．
  /// @param[in] id セルグループ番号 ( 0 <= id < group_num() )
  const LcGroup*
  group(ymuint id) const;

  /// @brief NPN同値クラスの数を返す．
  ymuint
  npn_class_num() const;

  /// @brief NPN同値クラスを返す．
  /// @param[in] id NPN同値クラス番号 ( 0 <= id < npn_class_num() )
  const LcClass*
  npn_class(ymuint id) const;


public:
  //////////////////////////////////////////////////////////////////////
  // ダンプ用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をバイナリダンプする．
  /// @param[in] bos 出力先のストリーム
  void
  dump(BinO& bos) const;

  /// @brief 内容を出力する．(デバッグ用)
  /// @param[in] s 出力先のストリーム
  void
  display(ostream& s) const;


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


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 新しいグループを作る．
  LcGroup*
  new_group();

  /// @brief 新しいクラスを作る．
  /// @param[in] repfunc 代表関数
  LcClass*
  new_class(const TvFuncM& repfunc);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // セルグループのリスト
  // この配列上の位置とグループ番号は一致している．
  vector<LcGroup*> mGroupList;

  // 多出力論理関数をキーとしてグループ番号を保持するハッシュ表
  hash_map<TvFuncM, ymuint> mGroupMap;

  // NPN同値クラスのリスト
  // この配列上の位置とクラス番号は一致している．
  vector<LcClass*> mClassList;

  // 代表関数をキーとしてクラス番号を保持するハッシュ表
  hash_map<TvFuncM, ymuint> mClassMap;

};

END_NAMESPACE_YM_CELL_LIBCOMP

#endif // LCGROUPMGR_H
