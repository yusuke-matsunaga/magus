#ifndef YM_GBM_GBMNODEHANDLE_H
#define YM_GBM_GBMNODEHANDLE_H

/// @file ym_gbm/GbmNodeHandle.h
/// @brief GbmNodeHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_gbm/gbm_nsdef.h"


BEGIN_NAMESPACE_YM_GBM

//////////////////////////////////////////////////////////////////////
/// @class GbmNodeHandle GbmNodeHandle.h "ym_gbm/GbmNodeHandle.h"
/// @brief GbmNode のID番号と反転属性のペアを表すクラス
//////////////////////////////////////////////////////////////////////
class GbmNodeHandle
{
public:

  /// @brief 空のコンストラクタ
  GbmNodeHandle();

  /// @brief 内容を指定したコンストラクタ
  GbmNodeHandle(ymuint id,
		bool inv);

  /// @brief デストラクタ
  ~GbmNodeHandle();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を設定する．
  void
  set(ymuint id,
      bool inv);

  /// @brief 反転属性を反転させる．
  const GbmNodeHandle&
  negate();

  /// @brief 反転したハンドルを返す．
  GbmNodeHandle
  operator~() const;

  /// @brief ID番号を返す．
  ymuint
  id() const;

  /// @brief 反転属性を返す．
  bool
  inv() const;

  /// @brief パックした値を返す．
  ymuint
  data() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号と反転属性をパックしたもの
  ymuint32 mData;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
inline
GbmNodeHandle::GbmNodeHandle() :
  mData(0U)
{
}

// @brief 内容を指定したコンストラクタ
inline
GbmNodeHandle::GbmNodeHandle(ymuint id,
			     bool inv)
{
  set(id, inv);
}


// @brief デストラクタ
inline
GbmNodeHandle::~GbmNodeHandle()
{
}

// @brief 内容を設定する．
inline
void
GbmNodeHandle::set(ymuint id,
		   bool inv)
{
  mData = (id << 1) | static_cast<ymuint32>(inv);
}

// @brief 反転属性を反転する
inline
const GbmNodeHandle&
GbmNodeHandle::negate()
{
  mData ^= 1U;

  return *this;
}

// @brief 反転したハンドルを返す．
inline
GbmNodeHandle
GbmNodeHandle::operator~() const
{
  return GbmNodeHandle(*this).negate();
}

// @brief ID番号を返す．
inline
ymuint
GbmNodeHandle::id() const
{
  return mData >> 1;
}

// @brief 反転属性を返す．
inline
bool
GbmNodeHandle::inv() const
{
  return static_cast<bool>(mData & 1U);
}

// @brief パックした値を返す．
inline
ymuint
GbmNodeHandle::data() const
{
  return mData;
}

END_NAMESPACE_YM_GBM

#endif // YM_GBM_GBMNODEHANDLE_H
