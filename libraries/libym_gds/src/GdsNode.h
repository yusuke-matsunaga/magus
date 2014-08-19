#ifndef GDSNODE_H
#define GDSNODE_H

/// @file GdsNode.h
/// @brief GdsNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "gds/GdsElement.h"


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
/// @class GdsNode GdsNode.h "GdsNode.h"
/// @brief NODE を表すクラス
//////////////////////////////////////////////////////////////////////
class GdsNode :
  public GdsElement
{
  friend class GdsParser;

private:

  /// @brief コンストラクタ
  /// @param[in] elflags ELFLAGS の値
  /// @param[in] plex PLEX の値
  /// @param[in] layer LAYER の値
  /// @param[in] datatype DATATYPE の値
  /// @param[in] xy XY の値
  GdsNode(ymuint16 elflags,
	  ymint32 plex,
	  ymint16 layer,
	  ymint16 nodetype,
	  GdsXY* xy);

  /// @briefデストラクタ
  virtual
  ~GdsNode();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief層番号を返す．
  virtual
  int
  layer() const;

  /// @brief ノード型を返す．
  virtual
  int
  nodetype() const;

  /// @brief 座標のリストを返す．
  virtual
  GdsXY*
  xy() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 層番号
  ymuint8 mLayer;

  // ノード型
  ymuint8 mNodeType;

  // 座標のリスト
  GdsXY* mXY;

};

END_NAMESPACE_YM_GDS

#endif // GDSNODE_H
