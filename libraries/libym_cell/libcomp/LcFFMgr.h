#ifndef LCFFMGR_H
#define LCFFMGR_H

/// @file LcFFMgr.h
/// @brief LcFFMgr.h
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "libdump_nsdef.h"
#include "ym_logic/TvFuncM.h"
#include "ym_utils/BioIO.h"


BEGIN_NAMESPACE_YM_CELL_LIBCOMP

//////////////////////////////////////////////////////////////////////
/// @class LcFFMgr LcFFMgr.h "LcFFMgr.h"
/// @brief FFセルを表すクラス
//////////////////////////////////////////////////////////////////////
class LcFFMgr
{
public:

  /// @brief コンストラクタ
  LcFFMgr();

  /// @brief デストラクタ
  ~LcFFMgr();


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
  find_ff_group(const vector<TvFunc>& f_array);

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

  /// @brief FFグループの数を返す．
  ymuint
  ff_group_num() const;

  /// @brief FFグループを返す．
  /// @param[in] id FFグループ番号 ( 0 <= id < ff_group_num() )
  const LcGroup*
  ff_group(ymuint id) const;

  /// @brief FFクラスの数を返す．
  ymuint
  ff_class_num() const;

  /// @brief FFクラスを返す．
  /// @param[in] id FFクラス番号 ( 0 <= id < ff_class_num() )
  const LcClass*
  ff_class(ymuint id) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // FFグループのリスト
  // この配列上の位置とグループ番号は一致している．
  vector<LcGroup*> mFFGroupList;

  // 論理関数をキーとしてFFグループ番号を格納するハッシュ表
  hash_map<TvFuncM, ymuint> mFFGroupMap;

  // FFクラスのリスト
  // この配列上の位置とクラス番号は一致している．
  vector<LcClass*> mFFClassList;

  // 論理関数をキーとしてFFクラス番号を格納するハッシュ表
  hash_map<TvFuncM, ymuint> mFFClassMap;

};

END_NAMESPACE_YM_CELL_LIBCOMP

#endif // LCFFMGR_H
