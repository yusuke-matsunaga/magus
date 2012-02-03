#ifndef IMPBASE_H
#define IMPBASE_H

/// @file ImpBase.h
/// @brief ImpBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/bdn.h"


BEGIN_NAMESPACE_YM_NETWORKS


//////////////////////////////////////////////////////////////////////
/// @class ImpInfo ImpBase.h "ImpBase.h"
/// @brief 含意情報を表すクラス
//////////////////////////////////////////////////////////////////////
class ImpInfo
{
public:

  /// @brief 空のコンストラクタ
  ImpInfo();

  /// @brief 内容を指定したコンストラクタ
  /// @param[in] src_node 含意元のノードのID
  /// @param[in] src_val  含意元の値
  /// @param[in] dst_node 含意先のノードのID
  /// @param[in] dst_val  含意先の値
  ImpInfo(ymuint src_node,
	  bool src_val,
	  ymuint dst_node,
	  bool dst_val);

  /// @brief 値を設定する．
  /// @param[in] src_node 含意元のノードのID
  /// @param[in] src_val  含意元の値
  /// @param[in] dst_node 含意先のノードのID
  /// @param[in] dst_val  含意先の値
  void
  set(ymuint src_node,
      bool src_val,
      ymuint dst_node,
      bool dst_val);


public:
  //////////////////////////////////////////////////////////////////////
  // 値を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 含意元のノードのIDを返す．
  ymuint
  src_node() const;

  /// @brief 含意元の値を返す．
  bool
  src_val() const;

  /// @brief 含意先のノードのIDを返す．
  ymuint
  dst_node() const;

  /// @brief 含意先の値を返す．
  bool
  dst_val() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 含意元の情報
  ymuint32 mSrcInfo;

  // 含意先の情報
  ymuint32 mDstInfo;

};


//////////////////////////////////////////////////////////////////////
/// @class ImpBase ImpBase.h "ImpBase.h"
/// @brief 含意を求める基底クラス
//////////////////////////////////////////////////////////////////////
class ImpBase
{
public:

  /// @brief デストラクタ
  virtual
  ~ImpBase() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイスの宣言
  //////////////////////////////////////////////////////////////////////

  /// @brief ネットワーク中の間接含意を求める．
  /// @param[in] network 対象のネットワーク
  /// @param[in] imp_list 間接含意のリスト
  virtual
  void
  learning(const BdnMgr& network,
	   vector<ImpInfo>& imp_list) = 0;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
inline
ImpInfo::ImpInfo()
{
  mSrcInfo = 0;
  mDstInfo = 0;
}

// @brief 内容を指定したコンストラクタ
// @param[in] src_node 含意元のノードのID
// @param[in] src_val  含意元の値
// @param[in] dst_node 含意先のノードのID
// @param[in] dst_val  含意先の値
inline
ImpInfo::ImpInfo(ymuint src_node,
		 bool src_val,
		 ymuint dst_node,
		 bool dst_val)
{
  set(src_node, src_val, dst_node, dst_val);
}

// @brief 値を設定する．
// @param[in] src_node 含意元のノードのID
// @param[in] src_val  含意元の値
// @param[in] dst_node 含意先のノードのID
// @param[in] dst_val  含意先の値
inline
void
ImpInfo::set(ymuint src_node,
	     bool src_val,
	     ymuint dst_node,
	     bool dst_val)
{
  mSrcInfo = (src_node << 1) | static_cast<ymuint32>(src_val);
  mDstInfo = (dst_node << 1) | static_cast<ymuint32>(dst_val);
}

// @brief 含意元のノードのIDを返す．
inline
ymuint
ImpInfo::src_node() const
{
  return mSrcInfo >> 1;
}

// @brief 含意元の値を返す．
inline
bool
ImpInfo::src_val() const
{
  return static_cast<bool>(mSrcInfo & 1UL);
}

// @brief 含意先のノードのIDを返す．
inline
ymuint
ImpInfo::dst_node() const
{
  return mDstInfo >> 1;
}

// @brief 含意先の値を返す．
inline
bool
ImpInfo::dst_val() const
{
  return static_cast<bool>(mDstInfo & 1UL);
}

END_NAMESPACE_YM_NETWORKS

#endif // IMPBASE_H
