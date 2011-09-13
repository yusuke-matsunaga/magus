#ifndef LDMGR_H
#define LDMGR_H

/// @file LdMgr.h
/// @brief LdMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "libdump_nsdef.h"
#include "ym_logic/TvFunc.h"


BEGIN_NAMESPACE_YM_CELL_LIBDUMP

//////////////////////////////////////////////////////////////////////
/// @class LdMgr LdMgr.h "LdMgr.h"
/// @brief セルライブラリを表すクラス
//////////////////////////////////////////////////////////////////////
class LdMgr
{
public:

  /// @brief コンストラクタ
  LdMgr();

  /// @brief デストラクタ
  ~LdMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // このクラスのメインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化する．
  /// @note 定数関数，リテラル関数を登録しておく
  ///
  /// 常に定数0，定数1，肯定リテラル，否定リテラルのグループ番号が
  /// 0, 1, 2, 3 になるようにする．
  void
  init();

  /// @brief f に対応する LdLogicGroup を求める．
  /// @param[in] f 関数
  /// @note なければ新規に作る．
  LdLogicGroup*
  find_group(const TvFunc& f);

  /// @brief 内容をバイナリダンプする．
  /// @param[in] s 出力先のストリーム
  void
  dump(ostream& s) const;

  /// @brief 内容を出力する．(デバッグ用)
  /// @param[in] s 出力先のストリーム
  void
  display(ostream& s) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 内容情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 論理グループの数を返す．
  ymuint
  logic_group_num() const;

  /// @brief 論理グループを返す．
  /// @param[in] id グループ番号 ( 0 <= id < logic_group_num() )
  const LdLogicGroup*
  logic_group(ymuint id) const;

  /// @brief 論理クラスの数を返す．
  ymuint
  logic_class_num() const;

  /// @brief 論理クラスを返す．
  /// @param[in] id クラス番号 ( 0 <= id < logic_class_num() )
  const LdLogicClass*
  logic_class(ymuint id) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief f に対応する LdLogicClass を求める．
  /// @param[in] f 関数
  /// @note なければ新規に作る．
  /// @note f は NpnMgr によって正規化されている必要がある．
  LdLogicClass*
  find_class(const TvFunc& f);

  /// @brief 変換マップの内容をダンプする．
  /// @param[in] s 出力先のストリーム
  /// @param[in] map 変換マップ
  static
  void
  dump_map(ostream& s,
	   const NpnMap& map);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // グループのリスト
  // この配列上の位置とグループ番号は一致している．
  vector<LdLogicGroup*> mGroupList;

  // 論理関数をキーとしてグループを保持するハッシュ表
  hash_map<TvFunc, LdLogicGroup*> mGroupMap;

  // クラスのリスト
  // この配列上の位置とクラス番号は一致している．
  vector<LdLogicClass*> mClassList;

  // 代表関数をキーとしてクラスを保持するハッシュ表
  hash_map<TvFunc, LdLogicClass*> mClassMap;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 論理グループの数を返す．
inline
ymuint
LdMgr::logic_gorup_num() const
{
  return mGroupList.size();
}

// @brief 論理グループを返す．
// @param[in] id グループ番号 ( 0 <= id < logic_group_num() )
inline
const LdLogicGroup*
LdMgr::logic_group(ymuint id) const
{
  return mGroupList[id];
}

// @brief 論理クラスの数を返す．
inline
ymuint
LdMgr::logic_class_num() const
{
  return mClassList.size();
}

// @brief 論理クラスを返す．
// @param[in] id クラス番号 ( 0 <= id < logic_class_num() )
inline
const LdLogicClass*
LdMgr::logic_class(ymuint id) const
{
  return mClassList[id];
}

END_NAMESPACE_YM_CELL_LIBDUMP

#endif // LDMGR_H
