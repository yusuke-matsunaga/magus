#ifndef LIBYM_LOGIC_BDD_DUMPER_H
#define LIBYM_LOGIC_BDD_DUMPER_H

/// @file libym_logic/bdd/base/Dumper.h
/// @brief Dumper のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "BddMgrImpl.h"
#include "ym_utils/BinIO.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
// BDD の節点に番号を割り振るためのクラス
//////////////////////////////////////////////////////////////////////
class IdMgr
{
public:

  /// @brief コンストラクタ
  IdMgr();

  /// @brief デストラクタ
  ~IdMgr();


public:

  /// @brief e がすでに ID 番号を持っていたら true を返す．
  bool
  has_id(BddEdge e) const;

  /// @brief e の ID 番号を返す．
  /// @note未登録ならば新しい番号を割り振る．
  ymuint
  id(BddEdge e);

  /// @brief 登録されている節点数を返す．
  ymuint
  num() const;

  /// @brief クリアする．
  void
  clear();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // BddEdge と ID 番号の対応表
  hash_map<BddEdge, ymuint32> mMap;

  // 次に割り当てるID番号
  ymuint32 mNext;

};


//////////////////////////////////////////////////////////////////////
// BDD の内容を出力するためのクラス
//////////////////////////////////////////////////////////////////////
class Displayer
{

public:

  /// @brief コンストラクタ
  Displayer(BddMgrImpl* mgr,
	    ostream& s);

  /// @brief デストラクタ
  ~Displayer();


public:

  /// @brief 登録された節点数を返す．
  ymuint
  num() const;

  /// @brief e を根とするBDDの内容を出力する．
  void
  display_root(BddEdge e);


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief e の ID 番号を出力する．
  void
  display_id(BddEdge e);

  /// @brief e の内容を出力する．
  void
  display_name(BddEdge e);

  /// @brief display_root の下請関数
  void
  display_step(BddEdge e);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // BddMgrImpl へのポインタ
  BddMgrImpl* mMgr;

  // 出力用のストリーム
  ostream& mStream;

  // 処理済の BddEdge に印を付けておくためのハッシュ表
  hash_set<BddEdge> mMark;

  // ID 番号を管理するマネージャ
  IdMgr mIdMgr;

};


//////////////////////////////////////////////////////////////////////
// BDD の内容を保存するためのクラス
//////////////////////////////////////////////////////////////////////
class Dumper
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr BddMgr
  /// @param[in] s 出力先のストリーム
  Dumper(BddMgrImpl* mgr,
	 BinO& s);

  /// @brief デストラクタ
  ~Dumper();


public:

  /// @brief 一つのBDDをダンプする．
  void
  write(BddEdge e);

  /// @brief 複数のBDDをダンプする．
  void
  write(const vector<BddEdge>& edge_list);

private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief e で指されたBDDノードの内容を出力する．
  void
  dump_node(BddEdge e);

  /// @brief e の内容を出力する．
  void
  dump_edge(BddEdge e);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // BddMgrImpl へのポインタ
  BddMgrImpl* mMgr;

  // 出力用のストリーム
  BinO& mStream;

  // ID 番号を管理するマネージャ
  IdMgr mIdMgr;

};


//////////////////////////////////////////////////////////////////////
// ダンプされた内容を読み込むためのクラス
//////////////////////////////////////////////////////////////////////
class Restorer
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr BddMgr
  /// @param[in] s 入力元のストリーム
  Restorer(BddMgrImpl* mgr,
	   BinI& s);

  /// @brief デストラクタ
  ~Restorer();


public:

  /// @brief 読み込む．
  /// @return 読み込んだBDDの根の枝の数を返す．
  /// @note エラーが起きたら 0 を返す．
  ymuint
  read();

  /// @brief pos 番目の枝を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < read() の返り値 )
  BddEdge
  root(ymuint pos);


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief データを読んで次の枝を得る．
  /// @return 見つからなければ kEdgeError を返す．
  BddEdge
  read_edge();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // BddMgrImpl へのポインタ
  BddMgrImpl* mMgr;

  // 入力用のストリーム
  BinI& mStream;

  // 根の枝を格納しておくベクタ
  vector<BddEdge> mRootVector;

  // 内部の枝を格納しておくベクタ
  vector<BddEdge> mEdgeVector;

};

END_NAMESPACE_YM_BDD

#endif
