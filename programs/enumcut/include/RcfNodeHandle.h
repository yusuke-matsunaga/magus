#ifndef RCFNODEHANDLE_H
#define RCFNODEHANDLE_H

/// @file RcfNodeHandle.h
/// @brief RcfNodeHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "YmTools.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class RcfNodeHandle RcfNodeHandle.h "RcfNodeHandle.h"
/// @brief RcfNode のID番号と反転属性のペアを表すクラス
///
/// ただし定数0/1に縮退することもあるので，定数も表せるようになっている．
//////////////////////////////////////////////////////////////////////
class RcfNodeHandle
{
public:

  /// @brief 空のコンストラクタ
  RcfNodeHandle();

  /// @brief 内容を指定したコンストラクタ
  RcfNodeHandle(ymuint id,
		bool inv);

  /// @brief 定数0を返す．
  static
  RcfNodeHandle
  make_zero();

  /// @brief 定数1を返す．
  static
  RcfNodeHandle
  make_one();

  /// @brief デストラクタ
  ~RcfNodeHandle();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を設定する．
  void
  set(ymuint id,
      bool inv);

  /// @brief 反転属性を反転させる．
  const RcfNodeHandle&
  negate();

  /// @brief 反転したハンドルを返す．
  RcfNodeHandle
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
  RcfNodeHandle(ymuint data);


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
RcfNodeHandle::RcfNodeHandle() :
  mData(0U)
{
}

// @brief 内容を指定したコンストラクタ
inline
RcfNodeHandle::RcfNodeHandle(ymuint id,
			     bool inv)
{
  set(id, inv);
}

// @brief 内容を直接指定したコンストラクタ
// @param[in] data 設定するデータ
inline
RcfNodeHandle::RcfNodeHandle(ymuint data) :
  mData(data)
{
}

// @brief 定数0を返す．
inline
RcfNodeHandle
RcfNodeHandle::make_zero()
{
  return RcfNodeHandle(0U);
}

// @brief 定数1を返す．
inline
RcfNodeHandle
RcfNodeHandle::make_one()
{
  return RcfNodeHandle(1U);
}

// @brief デストラクタ
inline
RcfNodeHandle::~RcfNodeHandle()
{
}

// @brief 内容を設定する．
inline
void
RcfNodeHandle::set(ymuint id,
		   bool inv)
{
  mData = ((id << 1) + 2) | static_cast<ymuint32>(inv);
}

// @brief 反転属性を反転する
inline
const RcfNodeHandle&
RcfNodeHandle::negate()
{
  mData ^= 1U;

  return *this;
}

// @brief 反転したハンドルを返す．
inline
RcfNodeHandle
RcfNodeHandle::operator~() const
{
  return RcfNodeHandle(*this).negate();
}

// @brief 定数の時に true を返す．
inline
bool
RcfNodeHandle::is_const() const
{
  return mData < 2;
}

// @brief 定数0の時に true を返す．
inline
bool
RcfNodeHandle::is_zero() const
{
  return mData == 0;
}

// @brief 定数1の時に true を返す．
inline
bool
RcfNodeHandle::is_one() const
{
  return mData == 1;
}

// @brief ID番号を返す．
inline
ymuint
RcfNodeHandle::id() const
{
  assert_cond( !is_const(), __FILE__, __LINE__);
  return (mData - 2) >> 1;
}

// @brief 反転属性を返す．
inline
bool
RcfNodeHandle::inv() const
{
  return static_cast<bool>(mData & 1U);
}

// @brief パックした値を返す．
inline
ymuint
RcfNodeHandle::data() const
{
  return mData;
}

END_NAMESPACE_YM

#endif // RCFNODEHANDLE_H
