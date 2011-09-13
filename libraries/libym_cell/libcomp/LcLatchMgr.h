#ifndef LCLATCHMGR_H
#define LCLATCHMGR_H

/// @file LcLatchMgr.h
/// @brief LcLatchMgr.h
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "libdump_nsdef.h"
#include "ym_logic/TvFuncM.h"
#include "ym_utils/BioIO.h"


BEGIN_NAMESPACE_YM_CELL_LIBCOMP

//////////////////////////////////////////////////////////////////////
/// @class LcLatchMgr LcLatchMgr.h "LcLatchMgr.h"
/// @brief FFセルを表すクラス
//////////////////////////////////////////////////////////////////////
class LcLatchMgr
{
public:

  /// @brief コンストラクタ
  LcLatchMgr();

  /// @brief デストラクタ
  ~LcLatchMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // このクラスのメインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化する．
  void
  init();

  /// @brief セルを追加する．
  void
  add_cell(const Cell* cell);

  /// @brief 対応する LcGroup を求める．
  /// @param[in] f_array 関数の配列
  /// @note なければ新規に作る．
  LcGroup*
  find_latch_group(const vector<TvFunc>& f_array);

  /// @brief 内容をバイナリダンプする．
  /// @param[in] bos 出力先のストリーム
  void
  dump(BinIO& bos) const;

  /// @brief 内容を出力する．(デバッグ用)
  /// @param[in] s 出力先のストリーム
  void
  display(ostream& s) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 内容情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ラッチグループの数を返す．
  ymuint
  latch_group_num() const;

  /// @brief ラッチグループを返す．
  /// @param[in] id ラッチグループ番号 ( 0 <= id < latch_group_num() )
  const LcGroup*
  latch_group(ymuint id) const;

  /// @brief ラッチクラスの数を返す．
  ymuint
  latch_class_num() const;

  /// @brief ラッチクラスを返す．
  /// @param[in] id ラッチクラス番号 ( 0 <= id < latch_class_num() )
  const LcClass*
  latch_class(ymuint id) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ラッチグループのリスト
  // この配列上の位置とグループ番号は一致している．
  vector<LcGroup*> mLatchGroupList;

  // 論理関数をキーとしてラッチグループ番号を格納するハッシュ表
  hash_map<TvFuncM, ymuint> mLatchGroupMap;

  // ラッチクラスのリスト
  // この配列上の位置とクラス番号は一致している．
  vector<LcClass*> mLatchClassList;

  // 論理関数をキーとしてFFクラス番号を格納するハッシュ表
  hash_map<TvFuncM, ymuint> mLatchClassMap;

};

END_NAMESPACE_YM_CELL_LIBCOMP

#endif // LCLATCHMGR_H
