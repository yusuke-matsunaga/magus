#ifndef FUNCMGR_H
#define FUNCMGR_H

/// @file FuncMgr.h
/// @brief FuncMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/bdn.h"
#include "ym_logic/TvFunc.h"
#include "ym_utils/IDO.h"
#include "ym_utils/ODO.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
/// @class FuncMgr FuncMgr.h "FuncMgr.h"
/// @brief 論理関数を管理するためのクラス
//////////////////////////////////////////////////////////////////////
class FuncMgr
{
public:

  /// @brief コンストラクタ
  FuncMgr();

  /// @brief デストラクタ
  ~FuncMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をクリアする．
  void
  clear();

  /// @brief 関数を登録する．
  /// @note すでに登録されていたらなにもしない．
  void
  reg_func(const TvFunc& f);

  /// @brief マージする．
  /// @param[in] src マージする他のマネージャ
  void
  merge(const FuncMgr& src);

  /// @brief 関数のリストを取り出す．
  void
  func_list(vector<TvFunc>& func_list) const;

  /// @brief 指定された入力数の関数のリストを取り出す．
  void
  func_list(ymuint ni,
	    vector<TvFunc>& func_list) const;

  /// @brief 代表関数のリストを取り出す．
  void
  rep_func_list(vector<TvFunc>& func_list) const;

  /// @brief 指定された入力数の代表関数のリストを取り出す．
  void
  rep_func_list(ymuint ni,
		vector<TvFunc>& func_list) const;


public:
  //////////////////////////////////////////////////////////////////////
  // バイナリダンプ
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をバイナリダンプする．
  /// @param[in] s 出力先のストリーム
  void
  dump(ODO& s) const;

  /// @brief 代表関数のみをバイナリダンプする．
  /// @param[in] s 出力先のストリーム
  void
  dump_rep(ODO& s) const;

  /// @brief バイナリダンプされたファイルを読み込む．
  void
  restore(IDO& s);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 新しい FuncData を作り登録する．
  void
  new_data(const TvFunc& f);

  /// @brief ハッシュ表を拡大する．
  void
  alloc_table(ymuint32 new_size);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  struct FuncData
  {
    // 関数
    TvFunc mFunc;

    // ハッシュ表で次の要素を指すリンク
    FuncData* mLink;

  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ハッシュテーブルサイズ
  ymuint32 mTableSize;

  // 要素数
  ymuint32 mNum;

  // ハッシュ表を拡張する目安
  ymuint32 mNextLimit;

  // ハッシュ表
  FuncData** mTable;

  // 代表関数のハッシュ
  hash_set<TvFunc> mRepHash;

};

END_NAMESPACE_YM_NETWORKS

#endif // FUNCMGR_H
