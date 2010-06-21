#ifndef YM_MVN_MVPORT_H
#define YM_MVN_MVPORT_H

/// @file ym_mvn/MvPort.h
/// @brief MvPort のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/mvn_nsdef.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvPort MvPort.h "ym_mvn/MvPort.h"
/// @brief MvModule のポートを表すクラス
//////////////////////////////////////////////////////////////////////
class MvPort
{
  friend class MvMgr;
  
public:

  /// @brief 名前を得る．
  /// @note 空(NULL)の場合もある．
  const char*
  name() const;

  /// @brief ビット幅を得る．
  virtual
  ymuint
  bit_width() const = 0;

  /// @brief port_ref 数を得る．
  virtual
  ymuint
  port_ref_num() const = 0;

  /// @brief port_ref を得る．
  /// @param[in] pos 位置 ( 0 <= pos < port_ref_num() )
  virtual
  MvNode*
  port_ref(ymuint pos) const = 0;


protected:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  MvPort();

  /// @brief デストラクタ
  virtual
  ~MvPort() { };

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  const char* mName;

#if 0
  // ビット幅
  ymuint32 mBitWidth;

  // 対応する入出力ノードのリスト
  // 部分指定のノードの場合もある．
  vector<MvNode*> mPortRef;
#endif
};


END_NAMESPACE_YM_MVN

#endif // YM_MVN_MVPORT_H
