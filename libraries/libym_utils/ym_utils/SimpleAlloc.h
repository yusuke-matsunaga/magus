#ifndef YM_UTILS_SIMPLEALLOC_H
#define YM_UTILS_SIMPLEALLOC_H

/// @file ym_utils/SimpleAlloc.h
/// @brief SimpleAlloc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/Alloc.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class SimpleAlloc Alloc.h "ym_utils/Alloc.h"
/// @brief 単純なメモリ管理
//////////////////////////////////////////////////////////////////////
class SimpleAlloc :
  public Alloc
{
public:

  /// @brief コンストラクタ
  /// @param[in] max_size このオブジェクトが管理する最大サイズ
  /// @note max_size 以上のメモリ領域はデフォルトのアロケーターを
  /// 使用する．
  SimpleAlloc(ymuint64 max_size);

  /// @brief デストラクタ
  virtual
  ~SimpleAlloc();


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
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief アラインメントを考慮してサイズを調節する．
  static
  ymuint64
  align(ymuint64 req_size);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // コンストラクタの引数
  ymuint64 mMaxSize;

  // mMaxSize を越えない2の巾乗の数．
  ymuint64 mMaxPowerSize;

  // mMaxPowerSize の log
  ymuint64 mMaxLogSize;

  // 現在利用可能なメモリ領域
  char* mCurBlock;

  // mCurBlock の次に利用可能な位置
  ymuint64 mNextPos;

  // 確保して使用中のメモリ領域のリスト
  list<char*> mAllocList;

};

END_NAMESPACE_YM

#endif // YM_UTILS_SIMPLEALLOC_H
