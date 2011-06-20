
/// @file libym_verilog/pt_simple/SptModule.cc
/// @brief SptModule の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SptModule.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "SptModule.h"
#include "SptFactory.h"

#include "ym_verilog/pt/PtItem.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// module を表すノード
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
// @param name 名前
// @param macro macromodule の時に true にするフラグ
// @param is_cell セル記述の時に true にするフラグ
// @param is_protected プロテクトされている時に true にするフラグ
// @param time_unit 時間の単位
// @param time_precision 時間の精度
// @param net_type default net type の値
// @param unconn unconnected drive の値
// @param delay default delay mode の値
// @param decay default decay time の値
// @param explicit_name 全てのポートが外向きの名前を持っている時に true にするフラグ
// @param portfaults portfauls 情報
// @param suppress_faults suppress_faults 情報
// @param config config 情報
// @param library library 情報
// @param cell cell 情報
// @param paramport_array パラメータポート宣言のリスト
// @param port_array ポートのリスト
// @param iohead_array IO宣言のリスト
// @param declhead_array 宣言のリスト
// @param item_array 要素のリスト
SptModule::SptModule(const FileRegion& file_region,
		     const char* name,
		     bool macro,
		     bool is_cell,
		     bool is_protected,
		     int time_unit,
		     int time_precision,
		     tVpiNetType net_type,
		     tVpiUnconnDrive unconn,
		     tVpiDefDelayMode delay,
		     int decay,
		     bool explicit_name,
		     bool portfaults,
		     bool suppress_faults,
		     const string& config,
		     const string& library,
		     const string& cell,
		     PtDeclHeadArray paramport_array,
		     PtiPortArray port_array,
		     PtIOHeadArray iohead_array,
		     PtDeclHeadArray decl_array,
		     PtItemArray item_array) :
  mFileRegion(file_region),
  mName(name),
  mDefDecayTime(decay),
  mConfig(config),
  mLibrary(library),
  mCell(cell),
  mParamPortArray(paramport_array),
  mPortArray(port_array),
  mIOHeadArray(iohead_array),
  mDeclHeadArray(decl_array),
  mItemArray(item_array)
{
  mFlags =
    static_cast<ymuint32>(is_cell)
    | (static_cast<ymuint32>(is_protected) << 1)
    | (static_cast<ymuint32>(time_precision + 16) << 2)
    | (static_cast<ymuint32>(time_unit + 16) << 7)
    | (static_cast<ymuint32>(net_type) << 12)
    | (static_cast<ymuint32>(unconn) << 16)
    | (static_cast<ymuint32>(delay) << 18)
    | (static_cast<ymuint32>(macro) << 21)
    | (static_cast<ymuint32>(explicit_name) << 22)
    | (1U << 23) // top_module
    | (static_cast<ymuint32>(portfaults) << 25)
    | (static_cast<ymuint32>(suppress_faults) << 26)
    ;

  ymuint n = 0;
  for (ymuint i = 0; i < mIOHeadArray.size(); ++ i) {
    n += mIOHeadArray[i]->item_num();
  }
  mIODeclNum = n;

  for (ymuint i = 0; i < mItemArray.size(); ++ i) {
    const PtItem* item = mItemArray[i];
    if ( item->type() == kPtItem_Func ) {
      mFuncDic.insert(make_pair(item->name(), item));
    }
  }
}

// デストラクタ
SptModule::~SptModule()
{
}

// ファイル位置の取得
// @return ファイル位置
FileRegion
SptModule::file_region() const
{
  return mFileRegion;
}

// 名前の取得
// @return name 名前
const char*
SptModule::name() const
{
  return mName;
}

// @brief パラメータポート宣言配列の取得
PtDeclHeadArray
SptModule::paramport_array() const
{
  return mParamPortArray;
}

// @brief ポート数の取得
// @return ポート数
ymuint
SptModule::port_num() const
{
  return mPortArray.size();
}

// ポートの取得
const PtPort*
SptModule::port(ymuint pos) const
{
  return mPortArray[pos];
}

// @brief 入出力宣言ヘッダ配列の取得
PtIOHeadArray
SptModule::iohead_array() const
{
  return mIOHeadArray;
}

// @brief 入出力宣言の要素数の取得
ymuint
SptModule::iodecl_num() const
{
  return mIODeclNum;
}

// @brief 宣言ヘッダ配列の取得
PtDeclHeadArray
SptModule::declhead_array() const
{
  return mDeclHeadArray;
}

// @brief item 配列の取得
PtItemArray
SptModule::item_array() const
{
  return mItemArray;
}

// macromodule 情報の取得
// @retval true macromodule の場合
// @retval false module の場合
bool
SptModule::is_macromodule() const
{
  return (mFlags >> 21) & 1;
}

// cell 情報の取得
// @retval true `celldefine --- `endcelldefine に挟まれたモジュールの場合
// @retval false 上記以外
bool
SptModule::is_cell() const
{
  return mFlags & 1;
}

// protect 情報の取得
// @return プロテクトされていたら true を返す．
bool
SptModule::is_protected() const
{
  return (mFlags >> 1) & 1;
}

// time unit の取得
// @return 時間の単位を表す 2 〜 -15 の整数
// もしくは未定義を表す -16
int
SptModule::time_unit() const
{
  return ((mFlags >> 7) & 0x1f) - 16;
}

// time precision の取得
// @return 時間の精度を表す 2 〜 -15 の整数
// もしくは未定義を表す -16
int
SptModule::time_precision() const
{
  return ((mFlags >> 2) & 0x1f) - 16;
}

// default net type の取得
// @return default net type
tVpiNetType
SptModule::nettype() const
{
  return static_cast<tVpiNetType>((mFlags >> 12) & 0xf);
}

// unconnected drive の取得
// @return unconnected drive
tVpiUnconnDrive
SptModule::unconn_drive() const
{
  return static_cast<tVpiUnconnDrive>((mFlags >> 16) & 0x3);
}

// default delay mode の取得
// @return default delay mode
tVpiDefDelayMode
SptModule::delay_mode() const
{
  return static_cast<tVpiDefDelayMode>((mFlags >> 18) & 0x7);
}

// default decay time の取得
// @return default decay time
int
SptModule::decay_time() const
{
  return mDefDecayTime;
}

// top module のチェック
// @return top module の場合に true を返す．
bool
SptModule::is_topmodule() const
{
  return static_cast<bool>((mFlags >> 23) & 1);
}

// top_module フラグを下ろす．
void
SptModule::clear_topmodule() const
{
  mFlags &= ~(1 << 23);
}

// @brief in_use フラグの設定
void
SptModule::set_in_use() const
{
  mFlags |= (1 << 24);
}

// @brief in_use フラグの解除
void
SptModule::reset_in_use() const
{
  mFlags &= ~(1 << 24);
}

// @brief in_use フラグの取得
bool
SptModule::is_in_use() const
{
  return static_cast<bool>((mFlags >> 24) & 1);
}

// portfaults 情報の取得
// @return true で enable_portfaults を表す．
bool
SptModule::portfaults() const
{
  return static_cast<bool>((mFlags >> 25) & 1);
}

// suppress_faults 情報の取得
// @return true で suppress_faults が効いていることを表す．
bool
SptModule::suppress_faults() const
{
  return static_cast<bool>((mFlags >> 26) & 1);
}

// config 情報の取得
// @return config 情報
const string&
SptModule::config() const
{
  return mConfig;
}

// library 情報の取得
// @return library 情報
const string&
SptModule::library() const
{
  return mLibrary;
}

// cell 情報の取得
// @return cell 情報
const string&
SptModule::cell() const
{
  return mCell;
}

// 関数名から関数の検索
// @param name 検索対象の関数名
// @return 該当する関数
// @return なければ NULL を返す．
const PtItem*
SptModule::find_function(const char* name) const
{
  hash_map<string, const PtItem*>::const_iterator p
    = mFuncDic.find(name);
  if ( p == mFuncDic.end() ) {
    return NULL;
  }
  return p->second;
}

// 名無しのポートを持つことを記録する．
void
SptModule::set_named_port()
{
  mFlags |= (1 << 22);
}

// すべてのポートが外部名を持っているときに true を返す．
//  { a, b } のような名無しポートがあると false となる．
//  true の時しか名前による結合は行えない．
bool
SptModule::explicit_name() const
{
  return (mFlags >> 22) & 1;
}


//////////////////////////////////////////////////////////////////////
// port を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param file_region ファイル位置の情報
// @param portref_array ポートの参照式の配列
// @param ext_name 外向きの名前
SptPort::SptPort(const FileRegion& file_region,
		 const PtExpr* portref,
		 PtExprArray portref_array,
		 const char* ext_name) :
  mFileRegion(file_region),
  mExtName(ext_name),
  mPortRef(portref),
  mPortRefArray(portref_array)
{
}

// デストラクタ
SptPort::~SptPort()
{
}

// ファイル位置の取得
// @return ファイル位置
FileRegion
SptPort::file_region() const
{
  return mFileRegion;
}

// 外向の名前の取得
// @return 外向の名前(本当のポート名)
// 無い場合は NULL を返す
const char*
SptPort::ext_name() const
{
  return mExtName;
}

// 内部のポート結線リストのサイズの取得
// @return 内部のポート結線リストのサイズ
ymuint
SptPort::portref_size() const
{
  return mPortRefArray.size();
}

// 内部のポート結線の取得
// @return 先頭のポート結線
const PtExpr*
SptPort::portref_elem(ymuint pos) const
{
  return mPortRefArray[pos];
}

// @brief 内部のポート結線の報告の取得
tVpiDirection
SptPort::portref_dir(ymuint pos) const
{
  return mDirArray[pos];
}

// @brief portref の方向を設定する．
// @param[in] pos 位置番号 ( 0 <= pos < portref_num() )
// @param[in] dir 方向
void
SptPort::_set_portref_dir(ymuint pos,
			  tVpiDirection dir)
{
  mDirArray[pos] = dir;
}


//////////////////////////////////////////////////////////////////////
// モジュール関係のオブジェクトを生成する関数
//////////////////////////////////////////////////////////////////////

// @brief モジュールの生成
// @param[in] file_region ファイル位置の情報
// @param[in] name モジュール名
// @param[in] macro macromodule の時 true となるフラグ
// @param[in] is_cell `celldefine - `endcelldefine で囲まれていたときに
// true となるフラグ
// @param[in] is_protected 保護されているときに true となるフラグ．
// verilog の構文にこんな情報はない．
// @param[in] time_unit 時刻の単位を表す数値．
// @param[in] time_precision 時刻の精度を表す数値
// 有効な値は 2 - -15 で 2 の時に 100秒を表す．
// 以下，1減るごとに10分の1になる．-16 で未定義を表す．
// @param[in] net_type 未定義の識別子から暗黙のネットを生成するときのネット型．
// @param[in] unconn unconnected_drive の値．
// @param[in] delay delay_mode の値．
// @param[in] decay default_decay_time の値．
// 意味のある値は正の整数もしくは無限大をあらわす -1
// @param[in] explicit_name ポートがすべて名前付きのときに true となるフラグ
// @param[in] portfaults Verifault 用．
// @param[in] suppress_faults Verifault 用
// @param[in] config config 情報
// @param[in] library library 情報
// @param[in] cell cell 情報
// @param[in] parampoar_array パラメータポートのリスト
// @param[in] port_array ポートのリスト
// @param[in] iohead_array 入出力のリスト
// @param[in] declhead_array 宣言のリスト
// @param[in] item_array 要素のリスト
// @return 生成されたモジュール
// @note paramport_array の内容と paramhead_array の内容は重複しない．
const PtModule*
SptFactory::new_Module(const FileRegion& file_region,
		       const char* name,
		       bool macro,
		       bool is_cell,
		       bool is_protected,
		       int time_unit,
		       int time_precision,
		       tVpiNetType net_type,
		       tVpiUnconnDrive unconn,
		       tVpiDefDelayMode delay,
		       int decay,
		       bool explicit_name,
		       bool portfaults,
		       bool suppress_faults,
		       const string& config,
		       const string& library,
		       const string& cell,
		       PtDeclHeadArray paramport_array,
		       PtiPortArray port_array,
		       PtIOHeadArray iohead_array,
		       PtDeclHeadArray declhead_array,
		       PtItemArray item_array)
{
  void* p = alloc().get_memory(sizeof(SptModule));
  return new (p) SptModule(file_region, name,
			   macro, is_cell, is_protected,
			   time_unit, time_precision,
			   net_type, unconn,
			   delay, decay,
			   explicit_name,
			   portfaults, suppress_faults,
			   config, library, cell,
			   paramport_array,
			   port_array,
			   iohead_array,
			   declhead_array,
			   item_array);
}


//////////////////////////////////////////////////////////////////////
// ポート関係のオブジェクトを生成する関数
//////////////////////////////////////////////////////////////////////

// @brief ポートの生成
// @param[in] file_region ファイル位置の情報
// @param[in] ext_name ポート名 (空文字列の場合もある)
// @return 生成されたポート
PtiPort*
SptFactory::new_Port(const FileRegion& file_region,
		     const char* ext_name)
{
  void* p = alloc().get_memory(sizeof(SptPort));
  return new (p) SptPort(file_region,
			 NULL,
			 PtExprArray(),
			 ext_name);
}

// @brief ポートの生成
// @param[in] file_region ファイル位置の情報
// @param portref ポートに接続している式 (ポート参照式)
// @param[in] ext_name ポート名 (空文字列の場合もある)
// @return 生成されたポート
PtiPort*
SptFactory::new_Port(const FileRegion& file_region,
		     const PtExpr* portref,
		     const char* ext_name)
{
  ymuint portref_num = 1;
  const PtExpr** portref_array = alloc_array<const PtExpr*>(portref_num);
  portref_array[0] = portref;
  void* p = alloc().get_memory(sizeof(SptPort));
  return new (p) SptPort(file_region,
			 portref,
			 PtExprArray(portref_num, portref_array),
			 ext_name);
}

// @brief ポートの生成
// @param[in] file_region ファイル位置の情報
// @param portref ポートに接続している式 (ポート参照式)
// @param portref_array ポートに接続している式 (ポート参照式) のリスト
// @param[in] ext_name ポート名 (空文字列の場合もある)
// @return 生成されたポート
PtiPort*
SptFactory::new_Port(const FileRegion& file_region,
		     const PtExpr* portref,
		     PtExprArray portref_array,
		     const char* ext_name)
{
  void* p = alloc().get_memory(sizeof(SptPort));
  return new (p) SptPort(file_region,
			 portref,
			 portref_array,
			 ext_name);
}

END_NAMESPACE_YM_VERILOG
