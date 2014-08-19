#ifndef ZDDBINOP_H
#define ZDDBINOP_H

/// @file ZddBinOp.h
/// @brief ZddBinOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ZddOp.h"
#include "CompTbl.h"


BEGIN_NAMESPACE_YM_ZDD

//////////////////////////////////////////////////////////////////////
/// @class ZddBinOpbase ZddBinOp.h "ZddbinOp.h"
/// @brief ZDD の二項演算を行うクラスの基底クラス
//////////////////////////////////////////////////////////////////////
class ZddBinOpBase :
  public ZddOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  ZddBinOpBase(ZddMgrImpl* mgr);

  /// @brief デストラクタ
  virtual
  ~ZddBinOpBase();


protected:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 根のレベルにしたがって子供のノードを求める．
  /// @param[in] f, g オペランド
  /// @param[out] f_0, f_1 f の子供
  /// @param[out] g_0, g_1 g の子供
  /// @return 根のレベルを返す．
  static
  ymuint
  split(ZddEdge f,
	ZddEdge g,
	ZddEdge& f_0,
	ZddEdge& f_1,
	ZddEdge& g_0,
	ZddEdge& g_1);

};


//////////////////////////////////////////////////////////////////////
/// @class ZddBinOp ZddBinOp.h "ZddbinOp.h"
/// @brief ZDD の二項演算を行うクラス
//////////////////////////////////////////////////////////////////////
class ZddBinOp :
  public ZddBinOpBase
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  /// @param[in] name テーブル名
  ZddBinOp(ZddMgrImpl* mgr,
	   const char* name);

  /// @brief デストラクタ
  virtual
  ~ZddBinOp();


public:
  //////////////////////////////////////////////////////////////////////
  // メインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算を行う関数
  /// @param[in] left, right オペランド
  virtual
  ZddEdge
  apply(ZddEdge left,
	ZddEdge right) = 0;

  /// @brief 次の GC で回収されるノードに関連した情報を削除する．
  virtual
  void
  sweep();


protected:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief id1, id2をキーとして検索を行なう
  ZddEdge
  get(ZddEdge id1,
      ZddEdge id2);

  /// @brief 結果を登録する
  void
  put(ZddEdge id1,
      ZddEdge id2,
      ZddEdge ans);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 演算結果テーブル
  CompTbl2 mCompTbl;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 根のレベルにしたがって子供のノードを求める．
// @param[in] f, g オペランド
// @param[out] f_0, f_1 f の子供
// @param[out] g_0, g_1 g の子供
// @return 根のレベルを返す．
inline
ymuint
ZddBinOpBase::split(ZddEdge f,
		    ZddEdge g,
		    ZddEdge& f_0,
		    ZddEdge& f_1,
		    ZddEdge& g_0,
		    ZddEdge& g_1)
{
  ZddNode* f_vp = f.get_node();
  ZddNode* g_vp = g.get_node();
  ymuint f_level = f_vp->level();
  ymuint g_level = g_vp->level();
  ymuint level = f_level;
  if ( level > g_level ) {
    level = g_level;
  }
  split1(level, f_level, f, f_vp, f_0, f_1);
  split1(level, g_level, g, g_vp, g_0, g_1);

  return level;
}

// @brief id1, id2をキーとして検索を行なう
inline
ZddEdge
ZddBinOp::get(ZddEdge id1,
	      ZddEdge id2)
{
  return mCompTbl.get(id1, id2);
}

// @brief 結果を登録する
inline
void
ZddBinOp::put(ZddEdge id1,
	      ZddEdge id2,
	      ZddEdge ans)
{
  return mCompTbl.put(id1, id2, ans);
}

END_NAMESPACE_YM_ZDD

#endif // ZDDOP_H
