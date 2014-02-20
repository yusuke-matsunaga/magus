#ifndef BINOP_H
#define BINOP_H

/// @file BinOp.h
/// @brief BinOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Op.h"
#include "CompTbl.h"


BEGIN_NAMESPACE_YM_CNFDD

//////////////////////////////////////////////////////////////////////
/// @class BinOpBase BinOp.h "BinOp.h"
/// @brief CNFDD の二項演算を行うクラスの基底クラス
//////////////////////////////////////////////////////////////////////
class BinOpBase :
  public Op
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  BinOpBase(CNFddMgrImpl& mgr);

  /// @brief デストラクタ
  virtual
  ~BinOpBase();


protected:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 根のレベルにしたがって子供のノードを求める．
  /// @param[in] f, g オペランド
  /// @param[out] f_0, f_p, f_n f の子供
  /// @param[out] g_0, g_p, g_n g の子供
  /// @return 根のレベルを返す．
  static
  ymuint
  split(CNFddEdge f,
	CNFddEdge g,
	CNFddEdge& f_0,
	CNFddEdge& f_p,
	CNFddEdge& f_n,
	CNFddEdge& g_0,
	CNFddEdge& g_p,
	CNFddEdge& g_n);

};


//////////////////////////////////////////////////////////////////////
/// @class BinOp BinOp.h "BinOp.h"
/// @brief CNFDD の二項演算を行うクラス
//////////////////////////////////////////////////////////////////////
class BinOp :
  public BinOpBase
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  /// @param[in] name テーブル名
  BinOp(CNFddMgrImpl& mgr,
	const char* name);

  /// @brief デストラクタ
  virtual
  ~BinOp();


public:
  //////////////////////////////////////////////////////////////////////
  // メインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算を行う関数
  /// @param[in] left, right オペランド
  virtual
  CNFddEdge
  apply(CNFddEdge left,
	CNFddEdge right) = 0;

  /// @brief 次の GC で回収されるノードに関連した情報を削除する．
  virtual
  void
  sweep();


protected:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief id1, id2をキーとして検索を行なう
  CNFddEdge
  get(CNFddEdge id1,
      CNFddEdge id2);

  /// @brief 結果を登録する
  void
  put(CNFddEdge id1,
      CNFddEdge id2,
      CNFddEdge ans);


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
// @param[out] f_0, f_p, f_n f の子供
// @param[out] g_0, g_p, g_n g の子供
// @return 根のレベルを返す．
inline
ymuint
BinOpBase::split(CNFddEdge f,
		 CNFddEdge g,
		 CNFddEdge& f_0,
		 CNFddEdge& f_p,
		 CNFddEdge& f_n,
		 CNFddEdge& g_0,
		 CNFddEdge& g_p,
		 CNFddEdge& g_n)
{
  CNFddNode* f_vp = f.get_node();
  CNFddNode* g_vp = g.get_node();
  ymuint f_level = f_vp->level();
  ymuint g_level = g_vp->level();
  ymuint level = f_level;
  if ( level > g_level ) {
    level = g_level;
  }
  split1(level, f_level, f, f_vp, f_0, f_p, f_n);
  split1(level, g_level, g, g_vp, g_0, g_p, g_n);

  return level;
}

// @brief id1, id2をキーとして検索を行なう
inline
CNFddEdge
BinOp::get(CNFddEdge id1,
	   CNFddEdge id2)
{
  return mCompTbl.get(id1, id2);
}

// @brief 結果を登録する
inline
void
BinOp::put(CNFddEdge id1,
	   CNFddEdge id2,
	   CNFddEdge ans)
{
  return mCompTbl.put(id1, id2, ans);
}

END_NAMESPACE_YM_CNFDD

#endif // BINOP_H
