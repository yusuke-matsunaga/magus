#ifndef MVNHANDLE_H
#define MVNHANDLE_H

/// @file MvnHandle.h
/// @brief MvnHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2016 Yusuke Matsunaga
/// All rights reserved.


#include "NetHandle.h"
#include "ym/MvnMgr.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class MvnHandle MvnHandle.h "MvnHandle.h"
/// @brief MvnMgr を持つネットハンドル
//////////////////////////////////////////////////////////////////////
class MvnHandle :
  public NetHandle
{
public:

  /// @brief コンストラクタ
  /// @param[in] name 名前
  MvnHandle(const string& name);

  /// @brief デストラクタ
  virtual
  ~MvnHandle();


public:

  /// @brief 型を返す．
  virtual
  Type
  type() const;

  /// @brief MvnMgr を得る．
  virtual
  const MvnMgr*
  mvn() const;

  /// @brief MvnMgr を得る．
  virtual
  MvnMgr*
  _mvn();

  /// @brief ネットワークをクリアする．
  virtual
  void
  clear();

  /// @brief ネットワークをコピーする．
  /// @param[in] src コピー元のネットハンドル
  /// @param[in] allow_conv true ならタイプが異なる時に変換する．
  /// @return コピーが成功したら true を返す．
  virtual
  bool
  copy(const NetHandle* src,
       bool allow_conv = false);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ネットワークの実体
  MvnMgr mNetwork;

};

END_NAMESPACE_MAGUS

#endif // MVNHANDLE_H
