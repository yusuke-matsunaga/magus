#ifndef FUNCHASH_H
#define FUNCHASH_H

/// @file FuncHash.h
/// @brief FuncHash のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "YmLogic/TvFunc.h"
#include "YmUtils/UnitAlloc.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class FuncHash FuncHash.h "FuncHash.h"
/// @brief 論理関数(TvFunc)のハッシュ表を表すクラス
//////////////////////////////////////////////////////////////////////
class FuncHash
{
public:

  /// @brief コンストラクタ
  FuncHash();

  /// @brief デストラクタ
  ~FuncHash();


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

  /// @brief 関数のリストを取り出す．
  void
  func_list(vector<TvFunc>& func_list) const;

  /// @brief 指定された入力数の関数のリストを取り出す．
  void
  func_list(ymuint ni,
	    vector<TvFunc>& func_list) const;


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

  // メモリアロケータ
  UnitAlloc mAlloc;

  // ハッシュテーブルサイズ
  ymuint32 mTableSize;

  // 要素数
  ymuint32 mNum;

  // ハッシュ表を拡張する目安
  ymuint32 mNextLimit;

  // ハッシュ表
  FuncData** mTable;

};

END_NAMESPACE_YM

#endif // FUNCHASH_H
