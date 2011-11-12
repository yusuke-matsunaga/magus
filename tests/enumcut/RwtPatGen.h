#ifndef RWTPATGEN_H
#define RWTPATGEN_H

/// @file RwtPatGen.h
/// @brief RwtPatGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/bdn.h"
#include "ym_logic/TvFunc.h"


BEGIN_NAMESPACE_YM_NETWORKS

class RwtPat;
class RwtNode;

//////////////////////////////////////////////////////////////////////
/// @class RwtPatGen RwtPatGen.h "RwtPatGen.h"
/// @brief RwtMgr の元データを作るためのクラス
//////////////////////////////////////////////////////////////////////
class RwtPatGen
{
public:

  /// @brief コンストラクタ
  RwtPatGen();

  /// @brief デストラクタ
  ~RwtPatGen();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力数を得る．
  ymuint
  input_num() const;

  /// @brief ノード数を得る．
  ymuint
  node_num() const;

  /// @brief ノードを得る．
  /// @param[in] id ノード番号 ( 0 <= id < node_num() )
  const RwtNode*
  node(ymuint id) const;

  /// @brief 登録されている関数の数を返す．
  ymuint
  func_num() const;

  /// @brief 関数を得る．
  /// @param[in] pos 関数番号 ( 0 <= pos < func_num() )
  TvFunc
  func(ymuint pos) const;

  /// @brief 関数に対応するパタンのリストを得る．
  /// @param[in] pos 関数番号 ( 0 <= pos < func_num() )
  const vector<RwtPat*>&
  pat_list(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  struct FuncPat
  {
    // 関数
    TvFunc mFunc;

    // パタンのリスト
    vector<RwtPat*> mPatList;

  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力数
  ymuint32 mInputNum;

  // ノードの配列
  vector<RwtNode*> mNodeArray;

  // 関数とパタンのリスト
  vector<FuncPat> mFuncList;

};

END_NAMESPACE_YM_NETWORKS

#endif // RWTPATGEN_H
