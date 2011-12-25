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
  /// @param[in] page_size このオブジェクトが管理するメモリ量の単位
  /// @note page_size 以上のメモリ領域はデフォルトのアロケーターを
  /// 使用する．
  SimpleAlloc(ymuint64 page_size);

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
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  struct Page
  {
    // コンストラクタ
    Page(void* p = NULL) :
      mNextPos(0),
      mChunk(static_cast<char*>(p))
    {
    }

    // 利用可能な先頭番地
    ymuint64 mNextPos;

    // メモリチャンク
    char* mChunk;

  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 一度に確保するメモリの単位
  ymuint64 mPageSize;

  // 使用可能なメモリ領域のリスト
  list<Page> mAvailList;

  // 使用中のメモリ領域のリスト
  list<Page> mUsedList;

};

END_NAMESPACE_YM

#endif // YM_UTILS_SIMPLEALLOC_H
