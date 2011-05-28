#ifndef SRC_LOGBASE_MVNHANDLE_H
#define SRC_LOGBASE_MVNHANDLE_H

/// @file src/logbase/MvnHandle.h
/// @brief MvnHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "NetHandle.h"
#include "ym_networks/MvnMgr.h"


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
  tType
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

#endif // SRC_LOGBASE_MVNHANDLE_H
