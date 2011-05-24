#ifndef LIBYM_TECHMAP_CELLMAP_LIBDUMP_LDFFMGR_H
#define LIBYM_TECHMAP_CELLMAP_LIBDUMP_LDFFMGR_H

/// @file libym_techmap/cellmap/libdump/LdFFMgr.h
/// @brief LdFFMgr.h
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "libdump_nsdef.h"


BEGIN_NAMESPACE_YM_CELLMAP_LIBDUMP

class LdFFGroup;
class LdFFClass;

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

  /// @brief 対応する LdFFGroup を求める．
  /// @param[in] f 関数
  /// @note なければ新規に作る．
  LdFFGroup*
  find_group(ymuint clock_sense,
	     ymuint clear_sense,
	     ymuint preset_sense,
	     ymuint data_pos,
	     ymuint clock_pos,
	     ymuint clear_pos,
	     ymuint preset_pos,
	     ymuint q_pos,
	     ymuint iq_pos);

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
  vector<LdFFGroup*> mFFGroupList;

  // FFクラスのリスト
  // この配列上の位置とクラス番号は一致している．
  vector<LdFFClass*> mFFClassList;

};

END_NAMESPACE_YM_CELLMAP_LIBDUMP

#endif // LIBYM_TECHMAP_CELLMAP_LIBDUMP_LDFFMGR_H
