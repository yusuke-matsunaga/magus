#ifndef INCLUDE_VAR_H
#define INCLUDE_VAR_H

/// @file include/Var.h
/// @brief Var のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "bb_nsdef.h"


BEGIN_NAMESPACE_YM_BB

//////////////////////////////////////////////////////////////////////
/// @class Var Var.h "Var.h"
/// @brief 変数定義を表すクラス
//////////////////////////////////////////////////////////////////////
class Var
{
public:

  /// @brief コンストラクタ
  Var(const FileRegion& file_region,
      ymuint id,
      int start,
      int end,
      int delta);

  /// @brief デストラクタ
  ~Var();


public:

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const;

  /// @breif ID 番号を得る．
  ymuint
  id() const;

  /// @brief 開始値を得る．
  int
  start() const;

  /// @brief 終了値を得る．
  int
  end() const;

  /// @brief 増分を得る．
  int
  delta() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル上の位置
  FileRegion mFileRegion;

  // ID 番号
  ymuint32 mId;

  // 開始値
  int mStart;

  // 終了値
  int mEnd;

  // 増分
  int mDelta;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ファイル位置を返す．
inline
FileRegion
Var::file_region() const
{
  return mFileRegion;
}

// @breif ID 番号を得る．
inline
ymuint
Var::id() const
{
  return mId;
}

// @brief 開始値を得る．
inline
int
Var::start() const
{
  return mStart;
}

// @brief 終了値を得る．
inline
int
Var::end() const
{
  return mEnd;
}

// @brief 増分を得る．
inline
int
Var::delta() const
{
  return mDelta;
}

END_NAMESPACE_YM_BB

#endif // INCLUDE_VAR_H
