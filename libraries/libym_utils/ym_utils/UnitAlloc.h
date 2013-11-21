#ifndef YM_UTILS_UNITALLOC_H
#define YM_UTILS_UNITALLOC_H

/// @file ym_utils/UnitAlloc.h
/// @brief UnitAlloc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/Alloc.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class UnitAlloc Alloc.h "ym_utils/Alloc.h"
/// @brief 単一サイズのメモリ領域の管理を行うクラス
//////////////////////////////////////////////////////////////////////
class UnitAlloc :
  public Alloc
{
public:

  /// @brief コンストラクタ
  /// @param[in] unit_size メモリ割り当ての単位となるサイズ
  /// @param[in] block_size 一度に確保する個数
  UnitAlloc(ymuint64 unit_size,
	    ymuint64 block_size);

  /// @brief デストラクタ
  virtual
  ~UnitAlloc();


private:
  //////////////////////////////////////////////////////////////////////
  // AllocBase の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief n バイトの領域を確保する．
  /// @param[in] n 確保するメモリ量(単位はバイト)
  virtual
  void*
  _get_memory(ymuint64 n);

  /// @brief n バイトの領域を開放する．
  /// @param[in] n 確保したメモリ量(単位はバイト)
  /// @param[in] blk 開放するメモリ領域の先頭番地
  virtual
  void
  _put_memory(ymuint64 n,
	      void* blk);

  /// @brief 今までに確保した全ての領域を破棄する．
  /// 個々のオブジェクトのデストラクタなどは起動されない
  /// ので使用には注意が必要
  virtual
  void
  _destroy();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief サイズ 2^p のブロックがあれば返す．
  /// @note なければ NULL を返す．
  char*
  get_block(ymuint64 p);

  /// @brief サイズ 2^p のブロックをリストに戻す．
  void
  put_block(ymuint64 p,
	    char* block);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  // 利用可能なメモリ領域を管理するための構造体
  struct Block
  {
    // 次の要素を指すポインタ
    Block* mLink;
  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 単位サイズ
  ymuint64 mUnitSize;

  // ブロックサイズ
  ymuint64 mBlockSize;

  // 利用可能なメモリ領域のリスト
  Block* mAvailTop;

  // 確保して使用中のメモリ領域のリスト
  list<void*> mAllocList;

};

END_NAMESPACE_YM

#endif // YM_UTILS_UNITALLOC_H
