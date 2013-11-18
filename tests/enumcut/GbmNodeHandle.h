#ifndef GBMNODEHANDLE_H
#define GBMNODEHANDLE_H

/// @file GbmNodeHandle.h
/// @brief GbmNodeHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class GbmNodeHandle GbmNodeHandle.h "GbmNodeHandle.h"
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

  /// @brief 定数0を返す．
  static
  GbmNodeHandle
  make_zero();

  /// @brief 定数1を返す．
  static
  GbmNodeHandle
  make_one();

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

  /// @brief 定数の時に true を返す．
  bool
  is_const() const;

  /// @brief 定数0の時に true を返す．
  bool
  is_zero() const;

  /// @brief 定数1の時に true を返す．
  bool
  is_one() const;

  /// @brief ID番号を返す．
  /// @note is_const() == true の時は意味を持たない．
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
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を直接指定したコンストラクタ
  /// @param[in] data 設定するデータ
  GbmNodeHandle(ymuint data);


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

// @brief 内容を直接指定したコンストラクタ
// @param[in] data 設定するデータ
inline
GbmNodeHandle::GbmNodeHandle(ymuint data) :
  mData(data)
{
}

// @brief 定数0を返す．
inline
GbmNodeHandle
GbmNodeHandle::make_zero()
{
  return GbmNodeHandle(0U);
}

// @brief 定数1を返す．
inline
GbmNodeHandle
GbmNodeHandle::make_one()
{
  return GbmNodeHandle(1U);
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
  mData = ((id << 1) + 2) | static_cast<ymuint32>(inv);
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

// @brief 定数の時に true を返す．
inline
bool
GbmNodeHandle::is_const() const
{
  return mData < 2;
}

// @brief 定数0の時に true を返す．
inline
bool
GbmNodeHandle::is_zero() const
{
  return mData == 0;
}

// @brief 定数1の時に true を返す．
inline
bool
GbmNodeHandle::is_one() const
{
  return mData == 1;
}

// @brief ID番号を返す．
inline
ymuint
GbmNodeHandle::id() const
{
  assert_cond( !is_const(), __FILE__, __LINE__);
  return (mData - 2) >> 1;
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

END_NAMESPACE_YM

#endif // GBMNODEHANDLE_H
