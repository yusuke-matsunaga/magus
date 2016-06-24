#ifndef MAGUS_NETHANDLE_H
#define MAGUS_NETHANDLE_H

/// @file include/NetHandle.h
/// @brief NetHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2018 Yusuke Matsunaga
/// All rights reserved.

#include "magus.h"
#include "ym/bnet.h"
#include "ym/mvn.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class NetHandle NetHandle.h "NetHandle.h"
/// @brief ネットワークを保持するハンドル
//////////////////////////////////////////////////////////////////////
class NetHandle
{
  friend class MagMgr;

public:
  //////////////////////////////////////////////////////////////////////
  // 型の定義
  //////////////////////////////////////////////////////////////////////

  /// @brief NetHandle の表す型
  enum Type {
    /// @brief BnNetwork
    kMagBn,
    /// @brief MvNetwork
    kMagMvn
  };


protected:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  // MagMgr のみが生成/破壊を行う．
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] name 名前
  NetHandle(const string& name);

  /// @brief デストラクタ
  virtual
  ~NetHandle();


public:

  /// @brief ネットワーク名を得る．
  const string&
  name() const;

  /// @brief 型を返す．
  virtual
  Type
  type() const = 0;

  /// @brief BnNetwork を得る．
  virtual
  const BnNetwork*
  bnetwork() const;

  /// @brief BnNetwork を得る．
  virtual
  BnNetwork*
  _bnetwork();

  /// @brief MvNetwork を得る．
  virtual
  const MvnMgr*
  mvn() const;

  /// @brief MvNetwork を得る．
  virtual
  MvnMgr*
  _mvn();

  /// @brief ネットワークをクリアする．
  virtual
  void
  clear() = 0;

  /// @brief ネットワークをコピーする．
  /// @param[in] src コピー元のネットハンドル
  /// @param[in] allow_conv true ならタイプが異なる時に変換する．
  /// @return コピーが成功したら true を返す．
  virtual
  bool
  copy(const NetHandle* src,
       bool allow_conv = false) = 0;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ネットワーク名
  string mName;

  // カレントネットワークスタックに積まれた回数
  int mCount;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ネットワーク名を得る．
inline
const string&
NetHandle::name() const
{
  return mName;
}

END_NAMESPACE_MAGUS

#endif // MAGUS_NETHANDLE_H
