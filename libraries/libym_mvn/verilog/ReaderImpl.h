#ifndef LIBYM_MVN_VERILOG_READERIMPL_H
#define LIBYM_MVN_VERILOG_READERIMPL_H

/// @file libym_mvn/verilog/ReaderImpl.h
/// @brief ReaderImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/mvn_nsdef.h"
#include "ym_verilog/VlMgr.h"
#include "ym_verilog/vl/VlFwd.h"
#include "ym_utils/MsgHandler.h"


BEGIN_NAMESPACE_YM_MVN_VERILOG

using namespace nsYm::nsVerilog;

#if 0
//////////////////////////////////////////////////////////////////////
/// @calss TmpPortRef ReaderImpl.h "ReaderImpl.h"
/// @brief 一時的に port ref の情報を保持しておく構造体
//////////////////////////////////////////////////////////////////////
class TmpPortRef
{
public:

  /// @brief 空のコンストラクタ
  TmpPortRef();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードを取り出す．
  MvNode*
  node() const;

  /// @brief 単純な参照の時 true を返す．
  bool
  is_simple() const;

  /// @brief ビット指定の時 true を返す．
  bool
  is_bit_select() const;

  /// @brief 範囲指定の時の true を返す．
  bool
  is_part_select() const;

  /// @brief ビット指定位置を返す．
  ymuint
  index() const;

  /// @brief 範囲指定の MSB を返す．
  ymuint
  msb() const;

  /// @brief 範囲指定の LSB を返す．
  ymuint
  lsb() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 単純な参照の場合の設定
  void
  set(MvNode* node);

  /// @brief ビット指定の場合の設定
  void
  set(MvNode* node,
      ymuint index);

  /// @brief 範囲指定の場合の設定
  void
  set(MvNode* node,
      ymuint msb,
      ymuint lsb);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード
  MvNode* mNode;

  /// @brief ビット選択の index または範囲選択の MSB
  ymuint32 mMsb;

  /// @brief 範囲選択の LSB
  ymuint32 mLsb;
  
};
#endif


//////////////////////////////////////////////////////////////////////
/// @class ReaderImpl ReaderImpl.h "ReaderImpl.h"
/// @brief 合成可能な Verilog 記述を読み込んで MVN に設定するクラス
//////////////////////////////////////////////////////////////////////
class ReaderImpl
{
public:

  /// @brief コンストラクタ
  ReaderImpl();

  /// @brief デストラクタ
  ~ReaderImpl();


public:

  /// @brief 内部のデータをクリアする．
  void
  clear();
  
  /// @brief verilog 形式のファイルを読み込む．
  /// @param[in] filename ファイル名
  /// @retval true 正常に読み込めた．
  /// @retval false 読込中にエラーが起こった．
  bool
  read(const string& filename,
       const SearchPathList& searchpath = SearchPathList(),
       const list<VlLineWatcher*> watcher_list = list<VlLineWatcher*>());

  /// @brief 今まで読み込んだ情報からネットワークを生成する．
  /// @param[in] mgr ネットワーク生成用のマネージャ
  /// @param[in] searchpath サーチパス
  /// @param[in] watcher_list 行番号ウォッチャーのリスト
  /// @retval true 正常に処理を行った．
  /// @retval false 生成中にエラーが起こった．
  bool
  gen_network(MvMgr& mgr);
  
  /// @brief メッセージハンドラを付加する．
  /// @param[in] msg_handler 登録するハンドラ
  /// @note このハンドラはこのオブジェクトの破壊に伴って破壊される．
  void
  add_msg_handler(MsgHandler* msg_handler);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief module を生成する．
  /// @param[in] vl_module 対象のモジュール
  MvModule*
  gen_module(const VlModule* vl_module);

  /// @brief scope item を生成する．
  /// @param[in] module モジュール
  /// @param[in] vl_scope 対象のスコープ
  /// @retval true 成功した．
  /// @retval false エラーが起こった．
  bool
  gen_scopeitem(MvModule* module,
		const VlNamedObj* vl_scope);

  /// @brief scope item 間の接続を行う．
  /// @param[in] vl_scope 対象のスコープ
  /// @retval true 成功した．
  /// @retval false エラーが起こった．
  bool
  link_scopeitem(const VlNamedObj* vl_scope);

  /// @brief portref の実体化を行う．
  /// @param[in] expr 対象の式
  /// @param[in] iomap 入出力ノードの対応表
  /// @param[out] node 対応するノードを格納する変数
  /// @param[out] msb ビット指定位置か範囲指定の MSB を格納する変数
  /// @param[out] lsb 範囲指定の LSB を格納する変数
  /// @retval 0 単純な形式だった．
  /// @retval 1 ビット指定形式だった．
  /// @retval 2 範囲指定形式だった．
  int
  gen_portref(const VlExpr* expr,
	      const hash_map<ympuint, MvNode*>& iomap,
	      MvNode*& node,
	      ymuint& msb,
	      ymuint& lsb);
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // メッセージハンドラの管理者
  MsgMgr mMsgMgr;

  // Verilog を扱うマネージャ
  VlMgr mVlMgr;

  // MvNetwork を扱うマネージャ
  MvMgr* mMvMgr;
  
};

#if 0

//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
inline
TmpPortRef::TmpPortRef()
{
}

// @brief ノードを取り出す．
inline
MvNode*
TmpPortRef::node() const
{
  return mNode;
}

// @brief 単純な参照の時 true を返す．
inline
bool
TmpPortRef::is_simple() const
{
  return mMsb == 0U && mLsb == 0U;
}

// @brief ビット指定の時 true を返す．
inline
bool
TmpPortRef::is_bit_select() const
{
  return static_cast<bool>(mMsb & 1U);
}

// @brief 範囲指定の時の true を返す．
inline
bool
TmpPortRef::is_part_select() const
{
  return static_cast<bool>(mLsb & 1U);
}

// @brief ビット指定位置を返す．
inline
ymuint
TmpPortRef::index() const
{
  return mMsb >> 1;
}

// @brief 範囲指定の MSB を返す．
inline
ymuint
TmpPortRef::msb() const
{
  return mMsb >> 1;
}

// @brief 範囲指定の LSB を返す．
inline
ymuint
TmpPortRef::lsb() const
{
  return mLsb >> 1;
}

// @brief 単純な参照の場合の設定
inline
void
TmpPortRef::set(MvNode* node)
{
  mNode = node;
  mMsb = 0U;
  mLsb = 0U;
}

// @brief ビット指定の場合の設定
inline
void
TmpPortRef::set(MvNode* node,
		ymuint index)
{
  mNode = node;
  mMsb = (index << 1) | 1U;
  mLsb = 0U;
}

// @brief 範囲指定の場合の設定
inline
void
TmpPortRef::set(MvNode* node,
		ymuint msb,
		ymuint lsb)
{
  mNode = node;
  mMsb = (msb << 1);
  mLsb = (lsb << 1) | 1U;
}
#endif
END_NAMESPACE_YM_MVN_VERILOG

#endif // LIBYM_MVN_VERILOG_READERIMPL_H
