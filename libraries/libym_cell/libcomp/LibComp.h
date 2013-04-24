#ifndef LIBCOMP_H
#define LIBCOMP_H

/// @file LibComp.h
/// @brief LibComp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "libcomp_nsdef.h"
#include "LcLogicMgr.h"
#include "LcFFMgr.h"
#include "LcLatchMgr.h"
#include "LcPatMgr.h"
#include "LcPat2Mgr.h"


BEGIN_NAMESPACE_YM_CELL_LIBCOMP

//////////////////////////////////////////////////////////////////////
/// @class LibComp LibComp.h "LibComp.h"
/// @brief パタングラフの情報をダンプするためのクラス
//////////////////////////////////////////////////////////////////////
class LibComp
{
  friend class LcGroupMgr;

public:

  /// @brief コンストラクタ
  LibComp();

  /// @brief デストラクタ
  ~LibComp();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief セルのグループ化，クラス化を行う．
  void
  compile(const CellLibrary& library);

  /// @brief 論理セルグループの情報を取り出す．
  const LcGroupMgr&
  logic_group_mgr() const;

  /// @brief FFセルグループの情報を取り出す．
  const LcGroupMgr&
  ff_group_mgr() const;

  /// @brief ラッチセルグループの情報を取り出す．
  const LcGroupMgr&
  latch_group_mgr() const;

  /// @brief パタングラフの情報を取り出す．
  const LcPatMgr&
  pat_mgr() const;

  /// @brief パタングラフの情報を取り出す．
  const LcPat2Mgr&
  pat2_mgr() const;

  /// @brief グラフ構造全体の内容を表示する．
  /// @param[in] s 出力先のストリーム
  void
  display(ostream& s) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 内部の情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief セルグループの数を返す．
  ymuint
  group_num() const;

  /// @brief セルグループを返す．
  /// @param[in] id セルグループ番号 ( 0 <= id < group_num() )
  LcGroup*
  group(ymuint id) const;

  /// @brief NPN同値クラスの数を返す．
  ymuint
  npn_class_num() const;

  /// @brief NPN同値クラスを返す．
  /// @param[in] id NPN同値クラス番号 ( 0 <= id < npn_class_num() )
  LcClass*
  npn_class(ymuint id) const;

  /// @brief 定義済みの論理グループ番号を返す．
  /// @param[in] id 番号
  /// - 0: 定数0
  /// - 1: 定数1
  /// - 2: バッファ
  /// - 3: インバーター
  ymuint
  logic_group(ymuint id) const;

  /// @brief 定義済みのFFクラス番号を返す．
  /// @param[in] id 番号
  /// - 0: クリアなし，プリセットなし
  /// - 1: クリアあり，プリセットなし
  /// - 2: クリアなし，プリセットあり
  /// - 3: クリアあり，プリセットあり
  ymuint
  ff_class(ymuint id) const;

  /// @brief 定義済みのラッチクラス番号を返す．
  /// @param[in] id 番号
  /// - 0: クリアなし，プリセットなし
  /// - 1: クリアあり，プリセットなし
  /// - 2: クリアなし，プリセットあり
  /// - 3: クリアあり，プリセットあり
  ymuint
  latch_class(ymuint id) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief expr から生成されるパタンを登録する．
  /// @param[in] expr 論理式
  /// @param[in] builtin 組み込みクラスの時 true にするフラグ
  void
  reg_expr(const LogExpr& expr,
	   bool builtin);

  /// @brief 新しいグループを作る．
  LcGroup*
  new_group();

  /// @brief 新しいクラスを作る．
  /// @param[in] repfunc 代表関数
  /// @param[in] builtin 組み込みクラスの時 true にするフラグ
  LcClass*
  new_class(const TvFuncM& repfunc,
	    bool builtin);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // セルグループのリスト
  // この配列上の位置とグループ番号は一致している．
  vector<LcGroup*> mGroupList;

  // NPN同値クラスのリスト
  // この配列上の位置とクラス番号は一致している．
  vector<LcClass*> mClassList;

  // 関数情報を管理するオブジェクト
  LcLogicMgr mLogicMgr;

  // FFの情報を管理するオブジェクト
  LcFFMgr mFFMgr;

  // ラッチの情報を管理するオブジェクト
  LcLatchMgr mLatchMgr;

  // パタングラフを管理するオブジェクト
  LcPatMgr mPatMgr;

  LcPat2Mgr mPat2Mgr;

  // 組み込みパタンのクラス番号



};

END_NAMESPACE_YM_CELL_LIBCOMP

#endif // LIBCOMP_H
