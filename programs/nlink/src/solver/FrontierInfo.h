#ifndef FRONTIERINFO_H
#define FRONTIERINFO_H

/// @file FrontierInfo.h
/// @brief FrontierInfo のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "nlink_nsdef.h"


BEGIN_NAMESPACE_YM_NLINK

//////////////////////////////////////////////////////////////////////
/// @class FrontierInfo FrontierInfo.h "FrontierInfo.h"
/// @brief フロンティア情報
//////////////////////////////////////////////////////////////////////
class FrontierInfo
{
public:

  /// @brief コンストラクタ
  FrontierInfo();

  /// @brief コンストラクタ
  /// @param[in] src コピー元のソース
  FrontierInfo(const FrontierInfo& src);

  /// @brief デストラクタ
  ~FrontierInfo();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード数を返す．
  ymuint
  node_num() const;

  /// @brief ノード番号を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < node_num() )
  ymuint
  node_id(ymuint pos) const;

  /// @brief 次数を返す．
  /// @param[in] pos ノードの位置番号
  ///
  /// pos はこのフロンティア内の相対的な位置
  /// ノード番号ではない．
  ymuint
  deg(ymuint pos) const;

  /// @brief 次数を1つ増やす．
  /// @param[in] pos ノードの位置番号
  ///
  /// pos はこのフロンティア内の相対的な位置
  /// ノード番号ではない．
  void
  inc_deg(ymuint pos);

  /// @brief 連結成分番号を返す．
  /// @param[in] pos ノードの位置番号
  ///
  /// pos はこのフロンティア内の相対的な位置
  /// ノード番号ではない．
  /// 端子につながっている場合は - 端子番号を
  /// を返す．
  int
  comp(ymuint pos) const;

  /// @brief 連結成分番号の書き換えを行う．
  /// @param[in] old_comp 旧番号
  /// @param[in] new_comp 新番号
  void
  replace_comp(int old_comp,
	       int new_comp);

  /// @brief ノードを削除する．
  /// @param[in] pos ノードの位置番号
  void
  delete_node(ymuint pos);

  /// @brief ノードを削除する．
  /// @param[in] del_list 削除するノード番号のリスト
  void
  delete_nodes(const vector<ymuint>& del_list);

  /// @brief ノードを追加する．
  /// @param[in] node_id ノード番号
  /// @param[in] deg 次数
  /// @param[in] comp_id 連結成分番号
  ///
  /// deg は 1 となる．
  void
  add_node(ymuint node_id,
	   ymuint deg,
	   int comp_id);

  /// @brief シグネチャを求める．
  string
  signature() const;

  /// @brief 内容を出力する．
  void
  dump(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  // ノード一つ分の情報
  struct Node
  {

    Node(ymuint id,
	 ymuint deg,
	 int comp) :
      mId(id),
      mDeg(deg),
      mComp(comp)
    {
    }

    // ノード番号
    ymuint mId;

    // 次数
    ymuint mDeg;

    // 連結成分番号
    int mComp;

  };


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード情報のリスト
  vector<Node> mNodeList;

};

END_NAMESPACE_YM_NLINK

#endif // FRONTIERINFO_H
