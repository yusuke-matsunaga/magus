# magus のビルドについて

<div style="text-align:right;">
松永　裕介
2015.07.23 Ver.1
</div>

## はじめに
magus は ymtools と同様に cmake を用いて Makefile の生成を行います．
そのため外部ライブラリが標準的な場所にインストールされている場合，
手順は非常に簡単です．
ただし，ymtools を /usr/lib や /usr/local/lib にインストールする人は
いないと思いますので，ymtools のインストール場所を指示する必要があります．
この点は ymtools よりも少しめんどうなことです．

大まかな流れは以下のとおりです．

 1. [ビルドに必要なツールのインストール](#1-ビルドに必要なツールのインストール)
 2. [cmake の実行](#2-cmake-の実行)
 3. [mk_builddir.sh](#3-mk_builddirsh)
 4. [do_cmake.sh](#4-do_cmakesh)
 5. [make の実行](#5-make-の実行)
 6. [make test の実行](#6-make-test-の実行)
 7. [make install の実行](#7-make-install-の実行)

このうち 1. は PC 上のソフトウェア開発環境を一回インストールすれば以降
は必要ありません．たまにソフトウェアのバージョンアップが必要になるかも
しれません．
2. も通常は最初に一回だけ行えばよいですが，新たにソースファイルを追加
した時などにも実行する必要があります．
3. はソースファイルに変更があった時には毎回実行します．
2. の処理を行うための補助スクリプト mk_builddir.sh と do_cmake.sh が
用意されています．


##1. ビルドに必要なツールのインストール

magus のビルドに必要なツールは以下のとおりです．

- cmake
- gcc/g++ もしくは clang/clang++
- YmTools
- YmTclpp

あったほうが良いパッケージは以下の通りです．

- tclreadline
- google-perftools

以下に個々のツールについて補足します．

- cmake
 現在，FreeBSD-10.1Release の cmake-3.1.3 を用いています．たぶん，
 cmake 3.X なら大丈夫だと思います．

- gcc/g++ もしくは clang/clang++
 現在，FreeBSD-10.1Release の clang-3.4.1 を用いています．
 たぶん gcc-4.7 以降でも動くと思います．

- YmTools, YmTclpp
  それぞれ [https://github.com/yusuke-matsunaga/ymtools.git](https://github.com/yusuke-matsunaga/ymtools.git)，
  [https://github.com/yusuke-matsunaga/ymtclpp.git](https://github.com/yusuke-matsunaga/ymtclpp.git)
  に有ります．
  それぞれ最新の安定バージョンを用いれば不整合は起きないと思います．


###1.1 上記ツールがインストールされていない場合の対処

一番簡単なのは，計算機の管理者にいって，上記のツールの最新バージョンを
インストールしてもらうことですが，それが難しい場合には自分でソースをもっ
てきて，configure; make; make install する必要があります．
その場合，ymtools 用のディレクトリを用意してそこにこれら一連のツールを
インストールすることをおすすめします．YmTools の BUILDING.md も参考にしてください．

ymtools, ymtclpp は必ず自分でインストールしなければなりません．
これはそれぞれの BUILDING.md を参考にしてコンパイルとインストールを
行ってください．
magus は ymtools や ymtclpp のライブラリを使う(import)ためにymtools
やymtclppのexport定義ファイルを読み込む必要が有ります．
もっと簡単な方法は ymtools のインストール先ディレクトリ(`CMAKE_INSTALL_PREFIX`)
と magus のインストール先ディレクトリを同じにすることです．
この場合，なにもしなくても ymtools のexport定義ファイル(`YmToolsConfig.cmake`)
を見つけてくれます．
別の場所にインストールする場合には cmake 変数`YmTools_DIR` に `YmToolsConfig.cmake`
の場所をセットしてください．


###1.1.1 cmake のインストール

Linux 系で一番問題なのが多くのディストリビューションで用意されている cmake
が未だに 2.8 なことです．2015年8月8日時点で最新は 3.3 です．
cmake のダウンロードサイト(http://www.cmake.org/download/) からソース一式
(例えば cmake-3.3.0.tar.gz)を取ってきて，適当な場所に展開し，
```shell
$ ./bootstrap
$ gmake
$ sudo gmake install
```
を実行すれば(たぶん/usr/localの下に)cmake一式がインストールされます．
インストール先を変えたい場合は README.rst を見てください(--prefix オプションを使う)．
ただし，もともとのディストリビューションに古い cmake が含まれていた場合，
新しい cmake をインストールしてだけでは意味がありません．
古い cmake は削除してもかまわないのですが，パッケージの依存関係がいろいろと
あるのでそれもできません．実際に cmake を実行するときに新しい cmake
を呼ぶようにしてください．詳細は次に述べます．


##2. cmake の実行

cmake の仕事は Cコンパイラなどのビルドツールの確認や必要なライブラリの
検索などを行って Makefile を生成することです．
ですのでソースファイルを追加したり，リンクするライブラリを追加したりす
るような Makefile に変更される場合には cmake を実行する必要があります．
逆に，ソースファイルの内容だけが変更された場合には Makefile の変更はあ
りませんので cmake を実行する必要はありません．

cmake はコンパイラやライブラリの場所や名前を巧妙な方法で探します．ライ
ブラリなどをデフォルトの位置にインストールしている場合にはほとんど何の
指定もせずに見つけてくれますが，自分のホームディレクトリの下にインストー
ルした場合などには cmake に位置を教える必要があります．
具体的な方法は cmake のマニュアルで調べてください．
通常，これらの指定は最初の一回だけ行えば，2回め以降は省略可能です．

[***重要***]
cmake はソースディレクトリ上で実行することも可能ですが，ソースディレク
トリはバージョン管理されたファイルが存在しますので，ビルド用の一時ファ
イルやコンパイル結果のファイルなどでソースディレクトリが汚れてしまうこ
とはあまり好ましいことではありません．たとえば git add ./* などとして
しまうと .gitignore で明示的に除外しておかない限りそのディレクトリにあ
るファイルがすべてバージョン管理の対象となってしまいます．
そこで，ビルド用に別のディレクトリを用意し，そこにコンパイル結果のファ
イルを置くことが一般的です．これは automake/autoconf でも同様です．
やりかたは極めて簡単です．今，ソースディレクトリを $(SRC_DIR) ビルドディ
レクトリを $(BUILD_DIR) とします．すると cmake の実行は以下のような手
順で行います．
```shell
$ mkdir -p $(BUILD_DIR)
$ cd $(BUILD_DIR)
$ cmake $(SRC_DIR)
```
これで $(BUILD_DIR) 以下に Makefile といくつかの作業用ディレクトリが生
成されます．

cmake に関してはまだ使いこなせていないのでいろいろ変わる可能性がありま
す．


##3. mk_builddir.sh

上記の様に通常，ソースディレクトリとコンパイルディレクトリは別々に用意します．
また，cmake を実行するときにインストール先のディレクトリの指定をする必要が
あります(`/usr/local/`以下にインストールしない場合)．
これらの指示を cmake を実行するたびに手で打ち込むのは面倒だし，
間違いのもとなので，最初に cmake 起動用のスクリプトを作るスクリプトを用意しました．
今，コンパイルディレクトリを `compile_dir` インストール先のディレクトリを
`install_dir` とすると，ソースディレクトリ上で
```shell
$ ./mk_builddir compile_dir install_dir
```
と実行すると
~~~
****
source  directory: XXX
build   directory: XXX
install directory: XXX
****
continue with above configuration ? (yes/no):
~~~
という表示が出ますので問題なければ yes と入力してください．
その後，`compile_dir`直下に do_cmake.sh というシェルスクリプト
が生成されます．


##4. do_cmake.sh

do_cmake.sh はその名の通り cmake を実行するだけのシェルスクリプトです．

上に書きましたが，古い cmake と新しい cmake が存在している場合，
ここで新しい cmake を起動しなければなりません．
そのためには `CMAKE` という環境変数に新しい cmake のパスを入れておけばOKです．
たとえば `/usr/local/bin/cmake` に cmake-3.3.0 がインストールされているとすると，
```shell
$ env CMAKE=/usr/local/bin/cmake ./do_cmake.sh
```
で新しい cmake を使ってくれます．
もしくは sh(bash) 系のシェルの場合は
```shell
$ export CMAKE=/usr/local/bin/cmake
```
を実行しておけば `env CMAKE=...` の部分は不要になります．
csh(tcsh) 系のシェルの場合は
```csh
% setenv CMAKE /usr/local/bin/cmake
```
です．

また，cmake 実行時(do_cmake.sh実行時)に環境変数 `CC` および `CXX` をセットしておくと
Cコンパイラ/C++コンパイラを指定することができます．
例えば `/usr/local/bin/clagn34`, `/usr/local/bin/clang++34`をそれぞれCコンパイラ/C++コンパイラ
として使いたい場合(実はYmToolsではC++のみを使う)には，
```shell
$ env CC=/usr/local/bin/clang34 CXX=/usr/local/bin/clang++34 ./do_cmake.sh
```
を実行すればOKです．環境変数の設定は上と同様に `export` 文で一回設定してしまってもOKです．
`env` 文はその行のコマンド実行のみに環境変数が設定されます．

通常は mk_builddir.sh でコンパイルディレクトリを用意した直後に一回だけ
このスクリプトを実行します．
ただし，ソースファイルを追加したり，リンクするライブラリを変更した場合には
再度 cmake を実行する必要が有ります．
特にスクリプトを使う必要もありませんが，ソースディレクトリやインストール
ディレクトリなど，一回決めてしまえば変わらないものなので毎回，
手で打ち込まずにこのスクリプトを実行するほうが簡単です．
逆に，このスクリプトはたいしたことはしていませんので
mk_builddir.sh で生成されたあとで中身を変更しても構いません．


##5. make の実行

cmake で問題が起こっていない限りあとは make とタイプするだけですべての
ビルドが完了します．

##6. make test の実行

まだすごく不十分ですが，テストコードを作っています．
make が終わったら
~~~shell
make test
~~~
で各ライブラリのテストを行ってください．
現在は CTest (cmake のテストツール)と google-test ライブラリを用いて
います．このような枠組みは CppUnit とか CppUtest とかいろいろあって
いろいろ試しているのですが，現在は CTest と google-test になっています．
具体的には個々のライブラリのディレクトリ直下に gtest というディレクトリ
があってその下にテスト用のバイナリがあります．
それを直接実行すると詳細なテスト出力が読めます．

##7. make install の実行

エラーがなければ最後に

~~~
$ make install
~~~

を実行すればインストールディレクトリにファイルがインストールされます．
インストールディレクトリが存在しない場合，作成されますのでスペルミスする
とエラーとならずに新しいディレクトリを作ってしまうので注意してください．
