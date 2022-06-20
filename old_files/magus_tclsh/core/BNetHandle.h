#ifndef BNETHANDLE_H
#define BNETHANDLE_H

/// @file BNetHandle.h
/// @brief BNetHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "NetHandle.h"
#include "ym/BnNetwork.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class BNetHandle BNetHandle.h "BNetHandle.h"
/// @brief BnNetwork を持つネットハンドル
//////////////////////////////////////////////////////////////////////
class BNetHandle :
  public NetHandle
{
public:

  /// @brief コンストラクタ
  /// @param[in] name 名前
  BNetHandle(const string& name);

  /// @brief デストラクタ
  virtual
  ~BNetHandle();


public:

  /// @brief 型を返す．
  virtual
  Type
  type() const;

  /// @brief BnNetwork を得る．
  virtual
  const BnNetwork*
  bnetwork() const;

  /// @brief BnNetwork を得る．
  virtual
  BnNetwork*
  _bnetwork();

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
  BnNetwork mNetwork;

};

END_NAMESPACE_MAGUS

#endif // BNETHANDLE_H
