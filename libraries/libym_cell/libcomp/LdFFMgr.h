#ifndef LDFFMGR_H
#define LDFFMGR_H

/// @file LdFFMgr.h
/// @brief LdFFMgr.h
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "libdump_nsdef.h"
#include "ym_logic/TvFunc.h"
#include "ym_logic/TvFuncM.h"


BEGIN_NAMESPACE_YM_CELL_LIBDUMP

//////////////////////////////////////////////////////////////////////
/// @class LdFFMgr LdFFMgr.h "LdFFMgr.h"
/// @brief FFセルを表すクラス
//////////////////////////////////////////////////////////////////////
class LdFFMgr
{
public:

  /// @brief コンストラクタ
  LdFFMgr();

  /// @brief デストラクタ
  ~LdFFMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // このクラスのメインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化する．
  void
  init();

  /// @brief 対応する LdGroup を求める．
  /// @param[in] f_array 関数の配列
  /// @note なければ新規に作る．
  LdGroup*
  find_ff_group(const vector<TvFunc>& f_array);

  /// @brief 内容をバイナリダンプする．
  /// @param[in] s 出力先のストリーム
  void
  dump(ostream& s) const;

  /// @brief 内容を出力する．(デバッグ用)
  /// @param[in] s 出力先のストリーム
  void
  display(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // FFグループのリスト
  // この配列上の位置とグループ番号は一致している．
  vector<LdGroup*> mFFGroupList;

  // 論理関数をキーとしてFFグループ番号を格納するハッシュ表
  hash_map<TvFuncM, ymuint> mFFGroupMap;

  // FFクラスのリスト
  // この配列上の位置とクラス番号は一致している．
  vector<LdClass*> mFFClassList;

  // 論理関数をキーとしてFFクラス番号を格納するハッシュ表
  hash_map<TvFuncM, ymuint> mFFClassMap;

};

END_NAMESPACE_YM_CELL_LIBDUMP

#endif // LDFFMGR_H
