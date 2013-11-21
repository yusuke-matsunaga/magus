#ifndef DUMPER_H
#define DUMPER_H

/// @file Dumper.h
/// @brief Dumper のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ZddMgrImpl.h"
#include "ym_utils/IDO.h"


BEGIN_NAMESPACE_YM_ZDD

//////////////////////////////////////////////////////////////////////
// ZDD の節点に番号を割り振るためのクラス
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
  has_id(ZddEdge e) const;

  /// @brief e の ID 番号を返す．
  /// @note未登録ならば新しい番号を割り振る．
  ymuint64
  id(ZddEdge e);

  /// @brief 登録されている節点数を返す．
  ymuint64
  num() const;

  /// @brief クリアする．
  void
  clear();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ZddEdge と ID 番号の対応表
  hash_map<ZddEdge, ymuint64> mMap;

  // 次に割り当てるID番号
  ymuint64 mNext;

};


//////////////////////////////////////////////////////////////////////
// ZDD の内容を出力するためのクラス
//////////////////////////////////////////////////////////////////////
class Printer
{

public:

  /// @brief コンストラクタ
  Printer(ZddMgrImpl* mgr,
	  ostream& s);

  /// @brief デストラクタ
  ~Printer();


public:

  /// @brief 登録された節点数を返す．
  ymuint64
  num() const;

  /// @brief e を根とするZDDの内容を出力する．
  void
  print_root(ZddEdge e);


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief e の ID 番号を出力する．
  void
  print_id(ZddEdge e);

  /// @brief e の内容を出力する．
  void
  print_name(ZddEdge e);

  /// @brief display_root の下請関数
  void
  print_step(ZddEdge e);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ZddMgrImpl へのポインタ
  ZddMgrImpl* mMgr;

  // 出力用のストリーム
  ostream& mStream;

  // 処理済の ZddEdge に印を付けておくためのハッシュ表
  hash_set<ZddEdge> mMark;

  // ID 番号を管理するマネージャ
  IdMgr mIdMgr;

};


//////////////////////////////////////////////////////////////////////
// ZDD の内容を保存するためのクラス
//////////////////////////////////////////////////////////////////////
class Dumper
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr ZddMgr
  /// @param[in] s 出力先のストリーム
  Dumper(ZddMgrImpl* mgr,
	 ODO& s);

  /// @brief デストラクタ
  ~Dumper();


public:

  /// @brief e を根とするZDDの内容を出力する．
  void
  dump(ZddEdge e);

  /// @brief e の内容を出力する．
  void
  dump_edge(ZddEdge e);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ZddMgrImpl へのポインタ
  ZddMgrImpl* mMgr;

  // 出力用のストリーム
  ODO& mStream;

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
  /// @param[in] mgr ZddMgr
  /// @param[in] s 入力元のストリーム
  Restorer(ZddMgrImpl* mgr,
	   IDO& s);

  /// @brief デストラクタ
  ~Restorer();


public:

  /// @brief 読み込む．
  /// @return 読み込んだZDDの根の枝の数を返す．
  /// @note エラーが起きたら 0 を返す．
  ymuint
  read();

  /// @brief pos 番目の枝を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < read() の返り値 )
  ZddEdge
  root(ymuint pos);


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内部の枝を指す文字列から枝を得る．
  /// @return 見つからなければ kEdgeError を返す．
  ZddEdge
  find_edge(const char* str) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ZddMgrImpl へのポインタ
  ZddMgrImpl* mMgr;

  // 入力用のストリーム
  IDO& mStream;

  // 根の枝を格納しておくベクタ
  vector<ZddEdge> mRootVector;

  // 内部の枝を格納しておくベクタ
  vector<ZddEdge> mEdgeVector;

};

END_NAMESPACE_YM_ZDD

#endif // DUMPER_h
