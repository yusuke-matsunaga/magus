#ifndef YMUTILS_HASHFUNC_H
#define YMUTILS_HASHFUNC_H

/// @file YmUtils/HashFunc.h
/// @brief HaseFunc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmTools.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class HashFunc HashFunc.h "YmUtils/HashFunc.h"
/// @brief ハッシュ関数を表すファンクタクラス
///
/// 下のコードを見ればわかるが Key_Type が直接 ymuint にキャスト
/// できない場合にはデフォルト実装ではエラーとなるので
/// 個別に特殊化する必要がある．
//////////////////////////////////////////////////////////////////////
template<typename Key_Type>
struct HashFunc
{
  ymuint
  operator()(const Key_Type& key)
  {
    // Key_Type が ymuint にキャストできなければエラーになる．
    return key;
  }
};


//////////////////////////////////////////////////////////////////////
// HashFunc<void*> の特殊化
//////////////////////////////////////////////////////////////////////
template<>
struct
HashFunc<void*>
{
  ymuint
  operator()(void* key) const
  {
    return reinterpret_cast<ympuint>(key) / sizeof(void*);
  }
};


//////////////////////////////////////////////////////////////////////
// HashFunc<int> の特殊化
//////////////////////////////////////////////////////////////////////
template<>
struct
HashFunc<int>
{
  ymuint
  operator()(int key) const
  {
    return static_cast<ymuint>(key);
  }
};


//////////////////////////////////////////////////////////////////////
// HashFunc<ymuint> の特殊化
//////////////////////////////////////////////////////////////////////
template<>
struct
HashFunc<ymuint>
{
  ymuint
  operator()(ymuint key) const
  {
    return key;
  }
};


//////////////////////////////////////////////////////////////////////
// HashFunc<string> の特殊化
//////////////////////////////////////////////////////////////////////
template<>
struct
HashFunc<string>
{
  ymuint
  operator()(const string& key) const
  {
    ymuint h = 0;
    for (ymuint i = 0; i < key.size(); ++ i) {
      h = h * 33 + key[i];
    }
    return h;
  }
};

END_NAMESPACE_YM

#endif // YMUTILS_HASHFUNC_H
