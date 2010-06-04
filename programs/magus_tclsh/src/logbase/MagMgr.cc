
/// @file magus/logbase/MagMgr.cc
/// @brief MagMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MagMgr.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_bnet/BNetwork.h"
#include "ym_bnet/BNetManip.h"
#include "MagMgr.h"
#include "EventHandler.h"
#include "Trace.h"
#include "NetTrace.h"
#include "NetStub.h"


#if 0
BEGIN_NAMESPACE_HASH

// Tcl_Interp へのポインタをキーにしたハッシュ関数クラスの定義
template <>
struct hash<Tcl_Interp*>
{
  size_t
  operator()(Tcl_Interp* obj) const
  {
    return reinterpret_cast<size_t>(obj)/sizeof(void*);
  }
};

END_NAMESPACE_HASH


BEGIN_NAMESPACE_MAGUS

// Tcl_Interp* をキーとして MagMgr を納める連想配列
hash_map<Tcl_Interp*, MagMgr*>* lMagMgrArray = NULL;

// Tcl_Interp* をキーとして MagMgr を割り当てる．
MagMgr*
MagMgr::get_mgr(Tcl_Interp* interp)
{
  if ( !lMagMgrArray ) {
    lMagMgrArray = new hash_map<Tcl_Interp*, MagMgr*>;
  }
  hash_map<Tcl_Interp*, MagMgr*>::iterator p = lMagMgrArray->find(interp);
  if ( p == lMagMgrArray->end() ) {
    MagMgr* mgr = new MagMgr(interp);
    lMagMgrArray->insert(pair<Tcl_Interp*, MagMgr*>(interp, mgr));
    return mgr;
  }
  return p->second;
}

// interp が削除される時に関連した MagMgr も削除する
// ためのクラスメソッド
void
MagMgr::delete_mgr(Tcl_Interp* interp)
{
  if ( lMagMgrArray ) {
    hash_map<Tcl_Interp*, MagMgr*>::iterator p = lMagMgrArray->find(interp);
    if ( p != lMagMgrArray->end() ) {
      MagMgr* mgr = p->second;
      lMagMgrArray->erase(p);
      delete mgr;
    }
  }
}
#endif

// コンストラクタ
MagMgr::MagMgr(Tcl_Interp* interp)
{
  // カレントネットワークは NULL
  mCurNet = NULL;

  // インタプリタをセットする．
  set_interp(interp);
}

// デストラクタ
// 全てのネットワークを破壊する．
MagMgr::~MagMgr()
{
  // このオブジェクトの管理しているネットワークを全て破棄する．
  // といってもこのループでは name_list にネットワーク名を入れている
  // だけ．
  list<string> name_list;
  get_network_names(name_list);

  // delete_net() 中で mNetHash を変更するので上のループ内
  // では削除できない．
  for (list<string>::iterator p = name_list.begin();
       p != name_list.end(); ++ p) {
    string name = *p;
    bool stat = delete_network(name);
    assert_cond(stat, __FILE__, __LINE__);
  }
}

// name がネットワーク名，セルライブラリ名として適切かどうか
// 調べる．
// エラーの場合には false を返す．
// leave_errmsg が true の場合にはインタプリタに
// エラーメッセージを残す．
bool
MagMgr::check_name(const string& name,
		   bool leave_errmsg) const
{
  // 今は @?[a-zA-Z0-9_]* となっているかをチェックする．
  const char* pat = "^@?[a-zA-Z0-9_]*$";
  int code = regexp_match(name, pat);
  if ( code == 1 ) {
    return true;
  }
  if ( code == -1 ) {
    TclObj emsg;
    emsg << "Sorry, internal error : " << pat << " is wrong";
    set_result(emsg);
    return false;
  }
  if ( leave_errmsg ) {
    TclObj emsg;
    emsg << name << " : illegal name";
    set_result(emsg);
  }
  return false;
}

// name という名の新たなネットワークを作成して登録する．
// 同名のネットワークが既に存在していた場合や名前が不適切な場合には
// エラーとなり，NULL を返す．
// trace_enable が true の場合にはトレースオブジェクトを付加する．
// エラーが起きた場合に leave_errmsg が true ならばインタプリタに
// メッセージを設定する．
BNetwork*
MagMgr::new_network(const string& name,
		    bool trace_enable,
		    bool leave_errmsg)
{
  NetStub* netstub = new_net(name, trace_enable, leave_errmsg);
  return netstub ? netstub->network() : 0;
}

// name という名のネットワークを取って来る．
// 名前が不適切な場合やその名前のネットワークが存在しない場合には
// エラーとなり，NULL を返す．
// エラーが起きた場合に leave_errmsg が true ならばインタプリタに
// メッセージを設定する．
BNetwork*
MagMgr::find_network(const string& name,
		     bool leave_errmsg)
{
  NetStub* netstub = find_net(name, leave_errmsg);
  return netstub ? netstub->network() : 0;
}

// name という名のネットワークを削除する．
// 名前が不適切な場合や存在しない場合にもエラーとなり，false を返す．
// エラーが起きた場合に leave_errmsg が true ならばはインタプリタ
// にメッセージをセットする．
bool
MagMgr::delete_network(const string& name,
		       bool leave_errmsg)
{
  hash_map<string, NetStub*>::const_iterator p = mNetHash.find(name);
  if ( p == mNetHash.end() ) {
    // 存在していなければエラー
    if ( leave_errmsg ) {
      TclObj emsg;
      emsg << name << " : No such network.";
      set_result(emsg);
    }
    return false;
  }

  NetStub* netstub = p->second;

  if ( netstub->mCount > 0 ) {
    // カレントネットワークもしくはネットワークスタックに積まれている
    // ネットワークは削除できない．
    if ( leave_errmsg ) {
      set_result("Can't delete. The network is in use.");
    }
    return false;
  }

  if ( netstub->is_trace_enable() ) {
    // イベントハンドラに通知する．
    prop_event(kNtwkDel, name);
  }

  // ハッシュから抜く．
  mNetHash.erase(name);

  // temporary name なら mTempNameSet に戻す
  if ( name[0] == '@' ) {
    size_t n = name.size();
    int v = 0;
    for (size_t i = 1; i < n; i ++) {
      char c = name[i];
      v = v * 10 + (c - '0');
    }
    mTempNameSet.add(v);
  }

  // netstub を削除する．
  // デストラクタ中でトレースとネットワークも削除される．
  delete netstub;

  // 正常終了
  return true;
}

// 操作対象のネットワークをスタックに追加する．
bool
MagMgr::push_cur_network(const string& name,
			 bool leave_errmsg)
{
  NetStub* netstub = find_net(name, leave_errmsg);
  if ( netstub == NULL ) {
    // エラー，たぶん名前が不正
    return false;
  }

  if ( mCurNet != NULL ) {
    // 昔のカレントネットワークをスタックに追加する．
    mNetStack.push_back(mCurNet);
  }

  // 新しいネットワークをカレントネットワークにする．
  mCurNet = netstub;
  netstub->mCount ++;

  return true;
}

// カレントネットワークを設定する．
// スタックは変更しない．
bool
MagMgr::change_cur_network(const string& name,
			   bool leave_errmsg)
{
  NetStub* netstub = find_net(name, leave_errmsg);
  if ( netstub == NULL ) {
    // エラー，たぶん名前が不正
    return false;
  }

  if ( mCurNet != netstub ) {
    mCurNet->mCount --;
    netstub->mCount ++;
    mCurNet = netstub;
  }

  return true;
}

// 最新のネットワークをスタックから取り出す．
// スタックが空の場合には false を返す．
// その場合にはカレントネットワークは不変
// エラーが起きた場合に leave_errmsg が true ならばはインタプリタ
// にメッセージをセットする．
bool
MagMgr::pop_cur_network(bool leave_errmsg)
{
  if ( mNetStack.empty() ) {
    // スタックが空だった．
    if ( leave_errmsg ) {
      set_result("network stack is empty.");
    }
    return false;
  }

  // スタックの頭からネットワークを取り出す．
  NetStub* netstub = mNetStack.back();
  mNetStack.pop_back();

  // 取り出したネットワークをカレントネットワークにする．
  mCurNet->mCount --;
  mCurNet = netstub;

  return true;
}

// 現在，操作対象となっているネットワークを返す．
// この関数はエラーとなることはない．
BNetwork*
MagMgr::cur_network() const
{
  return curnet()->network();
}

// 登録されているネットワーク名のリストを name_list
// に入れる．
void
MagMgr::get_network_names(list<string>& name_list) const
{
  name_list.clear();
  for (hash_map<string, NetStub*>::const_iterator p = mNetHash.begin();
       p != mNetHash.end(); ++ p) {
    name_list.push_back(p->first);
  }
}

// temporary name を生成する．
// temporary name は '@' + 数字
string
MagMgr::new_tmpname()
{
  int num = mTempNameSet.avail_num();
  if ( num == -1 ) {
    // 使用可能な数字が残っていない！！
    return string();
  }

  mTempNameSet.erase(num);

  ostringstream buf;
  buf << '@' << num;
  buf.flush();
  return string(buf.str());
}

// name という名に結び付けられたネットワーク構造体をとってくる．
// 名前が不適切な場合やその名前のネットワークが存在しない場合には
// エラーとなり，NULL を返す．
NetStub*
MagMgr::find_net(const string& name,
		 bool leave_errmsg)
{
  if ( name == kCurNetwork ) {
    // カレントネットワークを返す．
    return mCurNet;
  }
  
  // そうでなければハッシュを探す．
  hash_map<string, NetStub*>::const_iterator p = mNetHash.find(name);
  if ( p == mNetHash.end() ) {
    // 存在していなければエラー
    if ( leave_errmsg ) {
      TclObj emsg;
      emsg << name << " : No such network.";
      set_result(emsg);
    }
    return NULL;
  }
  return p->second;
}

// name という名のネットワークを新たに作成して登録する．
// 存在していた場合や名前が不適切な場合にはエラーとなり，NULL を返す．
// trace_enable が true の場合にはトレースオブジェクトを付加する．
// エラーが起きた場合に leave_errmsg が true ならばはインタプリタ
// にメッセージをセットする．
NetStub*
MagMgr::new_net(const string& name,
		bool trace_enable,
		bool leave_errmsg)
{
  // 名前が適切か調べる
  if ( !check_name(name, leave_errmsg) ) {
    // 名前が不適切だった．
    return NULL;
  }

  // ハッシュを調べる．
  if ( name == kCurNetwork || mNetHash.count(name) > 0 ) {
    // 同名のネットワークが存在していた．
    if ( leave_errmsg ) {
      TclObj emsg;
      emsg << name << " : Already exists.";
      set_result(emsg);
    }
    return NULL;
  }

  // 存在していないので新たに作る．
  NetStub* netstub = new NetStub(this, name, trace_enable);

  // ハッシュに登録する．
  mNetHash.insert(pair<string, NetStub*>(name, netstub));

  if ( trace_enable ) {
    // イベントハンドラに通知する．
    prop_event(kNtwkNew, name);
  }

  return netstub;
}

// カレントネットワークを取り出す．スタックは不変．
NetStub*
MagMgr::curnet() const
{
  assert_cond( mCurNet != NULL, __FILE__, __LINE__);
  return mCurNet;
}

// イベントハンドラを追加する．
void
MagMgr::add_event_handler(EventHandler* event_handler)
{
  mEventHandlers.push_back(event_handler);
  event_handler->mItForList = -- mEventHandlers.end();
}

// イベントハンドラを削除する．
// といっても実際にはリストから抜き取るだけ．削除はしない．
void
MagMgr::delete_event_handler(EventHandler* event_handler)
{
  mEventHandlers.erase(event_handler->mItForList);
}

// イベントをすべてのイベントハンドラに通知する．
void
MagMgr::prop_event(tEvent event,
		   const string& name) const
{
  for (list<EventHandler*>::const_iterator it = mEventHandlers.begin();
       it != mEventHandlers.end(); ++ it) {
    EventHandler* event_handler = *it;
    event_handler->event_proc(event, name);
  }
}

// トレースを追加する．
void
MagMgr::add_trace(const TclObj& proc,
		  int mask)
{
  Trace* trace = new Trace(this, proc, mask);
  mTraces.push_back(trace);
}

// トレースを削除する．
void
MagMgr::delete_trace(const TclObj& proc,
		     int mask)
{
  // 単純な線形探索を行っている．
  for (list<Trace*>::iterator it = mTraces.begin(); it != mTraces.end(); ) {
    list<Trace*>::iterator prev = it;
    Trace* trace = *prev;
    it ++;
    if ( trace->proc().get_string() == proc.get_string() ) {
      if ( mask == 0 || mask == trace->mask() ) {
	mTraces.erase(prev);
	delete trace;
      }
    }
  }
}

#if 0
//——————————————————————————————————————

// name で示されるライブラリをとってくる．
hf_CLibrary* MagMgr::FindMetaLibrary(const string& name, bool leave_errmsg)
{
  return mLibMgr->FindMetaLibrary( name, leave_errmsg );
}

// name で示されるセルライブラリをとってくる．
Cel_CCellMgr* MagMgr::FindCelLibrary(const string& name, bool leave_errmsg)
{
  return mLibMgr->FindCelLibrary( name, leave_errmsg );
}

// name という名のライブラリを新たに作成して登録する．
// 存在していた場合にはエラーとなる．
bool MagMgr::NewLibrary(const string& name, bool leave_errmsg)
{
  return mLibMgr->NewLibrary(name, leave_errmsg);
}

// name という名のライブラリを削除する．
// ただし，カレントライブラリは削除できないのでエラーとなる．
bool MagMgr::DeleteLibrary(const string& name, bool leave_errmsg)
{
  return mLibMgr->DeleteLibrary(name, leave_errmsg);
}

// 古いライブラリをスタックに積んで新しいカレントライブラリを設定する．
bool MagMgr::PushCurLibrary(const string& name)
{
  return mLibMgr->PushCurLibrary(name);
}

// 古いライブラリをスタックに積んで新しいカレントライブラリを設定する．
bool MagMgr::PopCurLibrary(bool leave_errmsg)
{
  return mLibMgr->PopCurLibrary(leave_errmsg);
}

// ライブラリ名nameで，ライブラリnewLibを登録する．
// すでにnameというライブラリが存在した場合，それは破棄される．
void MagMgr::SetLibrary(const string& name, hf_CLibrary* newLib)
{
  mLibMgr->SetLibrary(name, newLib);
}

// カレントライブラリの種類を取得する
hf_library_type MagMgr::CurLibraryType() const
{
  return mLibMgr->CurLibraryType();
}

// カレントライブラリを取得する．
const hf_CLibrary* MagMgr::CurrentLibrary()
{
  return mLibMgr->CurrentLibrary();
}

// カレントライブラリを，名前がnameのライブラリへ変更する．
bool
MagMgr::ChgCurLibrary(const string& name, bool leave_errmsg)
{
  return mLibMgr->ChgCurLibrary(name, leave_errmsg);
}

// ライブラリのポインタからライブラリ名を探す．
string MagMgr::FindLibraryName(const hf_CLibrary* lib_mgr) const
{
  return mLibMgr->FindLibraryName(lib_mgr);
}

#endif


//////////////////////////////////////////////////////////////////////
// クラス NetTrace
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
NetTrace::NetTrace(MagMgr* mgr,
		   NetStub* netstub) :
  mMgr(mgr),
  mNetStub(netstub)
{
}

// ネットワークが変化したときに呼ばれるコールバックルーティン
void
NetTrace::event_proc(BNetChg ev)
{
  // アイドルコールバックが登録されていなければ登録する．
  if ( !is_registered() ) {
    reg();
  }
}

// アイドルコールバックルーティン
void
NetTrace::idle_proc()
{
  if ( mNetStub->is_trace_enable() ) {
    mMgr->prop_event(kNtwkChg, mNetStub->name());
  }
}


//////////////////////////////////////////////////////////////////////
// クラス NetStub
//////////////////////////////////////////////////////////////////////

// コンストラクタ
NetStub::NetStub(MagMgr* mgr,
		 const string& name,
		 bool flag) :
  mName(name),
  mTraceEnable(flag)
{
  mNetwork = new BNetwork;
  BNetManip manip(mNetwork);
  manip.set_model_name(name);
  mTrace = new NetTrace(mgr, this);
  mTrace->bind(mNetwork);
  mCount = 0;
}

// デストラクタ
NetStub::~NetStub()
{
  delete mTrace;
  delete mNetwork;
}

END_NAMESPACE_MAGUS
