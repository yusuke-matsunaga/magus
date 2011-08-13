#ifndef LDFUNCMGR_H
#define LDFUNCMGR_H

/// @file LdFuncMgr.h
/// @brief LdFuncMgr.h
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "libdump_nsdef.h"
#include "ym_logic/TvFunc.h"


BEGIN_NAMESPACE_YM_CELL_LIBDUMP

class LdFunc;
class LdFuncClass;

//////////////////////////////////////////////////////////////////////
/// @class LdFuncMgr LdFuncMgr.h "LdFuncMgr.h"
/// @brief セルライブラリを表すクラス
//////////////////////////////////////////////////////////////////////
class LdFuncMgr
{
public:

  /// @brief コンストラクタ
  LdFuncMgr();

  /// @brief デストラクタ
  ~LdFuncMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // このクラスのメインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化する．
  /// @note 定数関数，リテラル関数を登録しておく
  ///
  /// 常に定数0，定数1，肯定リテラル，否定リテラルの関数番号が
  /// 0, 1, 2, 3 になるようにする．
  void
  init();

  /// @brief f に対応する LdFunc を求める．
  /// @param[in] f 関数
  /// @note なければ新規に作る．
  LdFunc*
  find_func(const TvFunc& f);

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

  /// @brief 論理関数の数を返す．
  ymuint
  func_num() const;

  /// @brief 論理関数情報のオブジェクトを返す．
  /// @param[in] id 関数番号 ( 0 <= id < func_num() )
  const LdFunc*
  func(ymuint id) const;

  /// @brief 代表関数の数を返す．
  ymuint
  rep_num() const;

  /// @brief 代表関数を返す．
  /// @param[in] id 代表関数番号 ( 0 <= id < rep_num() )
  const LdFuncClass*
  rep(ymuint id) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief f に対応する LdFuncClass を求める．
  /// @param[in] f 関数
  /// @note なければ新規に作る．
  /// @note f は NpnMgr によって正規化されている必要がある．
  LdFuncClass*
  find_repfunc(const TvFunc& f);

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

  // 論理関数のリスト
  // この配列上の位置と関数番号は一致している．
  vector<LdFunc*> mFuncList;

  // 論理関数のハッシュ表
  hash_map<TvFunc, LdFunc*> mFuncMap;

  // 代表関数のリスト
  // この配列上の位置と代表関数番号は一致している．
  vector<LdFuncClass*> mRepList;

  // 代表関数のハッシュ表
  hash_map<TvFunc, LdFuncClass*> mRepMap;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 論理関数の数を返す．
inline
ymuint
LdFuncMgr::func_num() const
{
  return mFuncList.size();
}

// @brief 論理関数を返す．
// @param[in] id 関数番号 ( 0 <= id < func_num() )
inline
const LdFunc*
LdFuncMgr::func(ymuint id) const
{
  return mFuncList[id];
}

// @brief 代表関数の数を返す．
inline
ymuint
LdFuncMgr::rep_num() const
{
  return mRepList.size();
}

// @brief 代表関数を返す．
// @param[in] id 代表関数番号 ( 0 <= id < rep_num() )
inline
const LdFuncClass*
LdFuncMgr::rep(ymuint id) const
{
  return mRepList[id];
}

END_NAMESPACE_YM_CELL_LIBDUMP

#endif // LDFUNCMGR_H
