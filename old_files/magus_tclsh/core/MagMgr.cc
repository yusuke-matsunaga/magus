
/// @file MagMgr.cc
/// @brief MagMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "MagMgr.h"
#include "NetHandle.h"
#include "BNetHandle.h"
#include "MvnHandle.h"
#include "ym/ClibCellLibrary.h"


BEGIN_NAMESPACE_MAGUS

// カレントネットワークを常に表す文字列．
string MagMgr::kCurNetwork("_network");


// コンストラクタ
MagMgr::MagMgr()
{
  // カレントネットワークは nullptr
  mCurNet = nullptr;
}

// デストラクタ
// 全てのネットワークを破壊する．
MagMgr::~MagMgr()
{
  // このオブジェクトの管理しているネットワークを全て破棄する．
  // といってもこのループでは name_list にネットワーク名を入れている
  // だけ．
  auto name_list = get_network_names();

  // delete_net() 中で mNetHash を変更するので上のループ内
  // では削除できない．
  for ( auto name: name_list ) {
    bool stat = delete_nethandle(name);
    ASSERT_COND(stat );
  }
}

// @brief カレントセルライブラリの取得
ClibCellLibrary&
MagMgr::cur_cell_library()
{
  return mCellLibrary;
}

// @brief 名前が適切かどうか調べる関数
// @param[in] name 調べる名前
// @param[in] err_out エラー出力
// @return name が ^@?[a-zA-Z_]*$ で表される正規表現なら true を返す．
bool
MagMgr::check_name(const string& name,
		   ostream* err_out)
{
  // 今は @?[a-zA-Z0-9_]* となっているかをチェックする．
  ymuint pos = 0;
  if ( name[pos] == '@' ) {
    ++ pos;
  }
  for (; pos < name.size(); ++ pos) {
    int c = name[pos];
    if ( !isalnum(c) && c != '_' ) {
      // 名前が不正
      if ( err_out ) {
	*err_out << name << " : illegal name";
      }
      return false;
    }
  }
  return true;
}

// @brief 新たな BNetwork を作成して登録する．
// @param[in] name 名前
// @param[in] err_out エラー出力
// @return 作成したネットハンドル
// @note 同名のネットワークが既に存在していた場合にはエラーとなる．
// @note また，名前が不適切な場合にもエラーとなる．
// @note エラーが起きた場合には nullptr を返す．
NetHandle*
MagMgr::new_bnethandle(const string& name,
		       ostream* err_out)
{
  NetHandle* neth = new BNetHandle(name);
  bool stat = reg_nethandle(neth, err_out);
  if ( !stat ) {
    delete neth;
    neth = nullptr;
  }
  return neth;
}

// @brief 新たな MvNetwork を作成して登録する．
// @param[in] name 名前
// @param[in] err_out エラー出力
// @return 作成したネットハンドル
// @note 同名のネットワークが既に存在していた場合にはエラーとなる．
// @note また，名前が不適切な場合にもエラーとなる．
// @note エラーが起きた場合には nullptr を返す．
NetHandle*
MagMgr::new_mvnhandle(const string& name,
		      ostream* err_out)
{
  NetHandle* neth = new MvnHandle(name);
  bool stat = reg_nethandle(neth, err_out);
  if ( !stat ) {
    delete neth;
    neth = nullptr;
  }
  return neth;
}

// @brief ネットワークの検索
// @param[in] name ネットワーク名
// @param[in] err_out エラー出力
// @return name という名のネットワークを返す．
// @note 名前が不適切な場合やその名前のネットワークが存在しない場合
// にはエラーとなり，nullptr を返す．
NetHandle*
MagMgr::find_nethandle(const string& name,
		       ostream* err_out)
{
  if ( name == kCurNetwork ) {
    // カレントネットワークを返す．
    return mCurNet;
  }

  // そうでなければハッシュを探す．
  NetHandle* neth = _find_nethandle(name);
  if ( neth == nullptr ) {
    // 存在していなければエラー
    if ( err_out ) {
      *err_out << name << " : No such network.";
    }
  }
  return neth;
}

// @brief ネットワークの削除
// @param[in] name ネットワーク名
// @param[int] err_out エラー出力
// @retrun name という名のネットワークが存在したら削除して true を返す．
// @note 名前が不適切な場合や存在しない場合にはエラーとなり，false を返す．
bool
MagMgr::delete_nethandle(const string& name,
			 ostream* err_out)
{
  NetHandle* neth = _find_nethandle(name);
  if ( neth == nullptr ) {
    // 存在していなければエラー
    if ( err_out ) {
      *err_out << name << " : No such network.";
    }
    return false;
  }

  if ( neth->mCount > 0 ) {
    // カレントネットワークは削除できない．
    if ( err_out ) {
      *err_out << "Can't delete. The network is in use.";
    }
    return false;
  }

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


  // ハッシュから抜き，neth を削除する．
  _delete_nethandle(neth);

  // 正常終了
  return true;
}

// @brief カレントネットワークの変更
// @param[in] name ネットワーク名
// @param[in] err_out エラー出力
// @return 正常終了したら true を返す．
// @note name という名のネットワークをカレントネットワークに設定する．
// @note 名前が不適切な場合や存在しない場合にはエラーとなり，false を返す．
bool
MagMgr::change_cur_nethandle(const string& name,
			     ostream* err_out)
{
  NetHandle* neth = find_nethandle(name, err_out);
  if ( neth == nullptr ) {
    // エラー，たぶん名前が不正
    return false;
  }

  // 以下のコードは mCurNet と neth が等しくても安全に実行できる
  -- mCurNet->mCount;
  ++ neth->mCount;
  mCurNet = neth;

  return true;
}

// 操作対象のネットワークをスタックに追加する．
bool
MagMgr::push_cur_nethandle(const string& name,
			   ostream* err_out)
{
  NetHandle* neth = find_nethandle(name, err_out);
  if ( neth == nullptr ) {
    // エラー，たぶん名前が不正
    return false;
  }

  if ( mCurNet != nullptr ) {
    // 昔のカレントネットワークをスタックに追加する．
    mNetStack.push_back(mCurNet);
  }

  // 新しいネットワークをカレントネットワークにする．
  mCurNet = neth;
  ++ neth->mCount;

  return true;
}

// 最新のネットワークをスタックから取り出す．
// スタックが空の場合には false を返す．
// その場合にはカレントネットワークは不変
// エラーが起きた場合に leave_errmsg が true ならばはインタプリタ
// にメッセージをセットする．
bool
MagMgr::pop_cur_nethandle(ostream* ostream)
{
  if ( mNetStack.empty() ) {
    // スタックが空だった．
    if ( ostream ) {
      *ostream << "network stack is empty.";
    }
    return false;
  }

  // スタックの頭からネットワークを取り出す．
  NetHandle* neth = mNetStack.back();
  mNetStack.pop_back();

  // 取り出したネットワークをカレントネットワークにする．
  -- mCurNet->mCount;
  mCurNet = neth;

  return true;
}

// @brief カレントネットワークの取得
// @note この関数はエラーとなることはない．
NetHandle*
MagMgr::cur_nethandle() const
{
  ASSERT_COND( mCurNet != nullptr );
  return mCurNet;
}

// @brief ネットワーク名のリストの作成
// @return 作成されたネットワーク名を収めるリストを返す．
vector<string>
MagMgr::get_network_names() const
{
  vector<string> name_list;
  name_list.reserve(mNetHandleMap.size());
  for ( auto p: mNetHandleMap ) {
    auto name = p.first;
    name_list.push_back(name);
  }
  return name_list;
}

// @brief テンポラリ名の生成
// @note temporary name は '@' + 数字
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

// @brief ネットワークの登録
// @param[in] handle 登録するネットワークハンドル
// @param[in] err_out エラー出力
// @return 実行結果
// @note 同名のネットワークが既に存在していた場合や名前が不適切な
// 場合にはエラーとなり，false を返す．
bool
MagMgr::reg_nethandle(NetHandle* handle,
		      ostream* err_out)
{
  string name = handle->name();

  // 名前が適切か調べる
  if ( !check_name(name, err_out) ) {
    // 名前が不適切だった．
    return false;
  }

  // ハッシュを調べる．
  if ( name == kCurNetwork || _find_nethandle(name) != nullptr ) {
    // 同名のネットワークが存在していた．
    if ( err_out ) {
      *err_out << name << " : Already exists.";
    }
    return false;
  }

  // 存在していないので登録する．
  mNetHandleMap.emplace(handle->name(), handle);

  return true;
}

// @brief ネットワークを検索する．
NetHandle*
MagMgr::_find_nethandle(const string& name)
{
  if ( mNetHandleMap.count(name) > 0 ) {
    return mNetHandleMap.at(name);
  }
  return nullptr;
}

// @brief ネットワークを削除する．
void
MagMgr::_delete_nethandle(NetHandle* net_handle)
{
  mNetHandleMap.erase(net_handle->name());
}

END_NAMESPACE_MAGUS
