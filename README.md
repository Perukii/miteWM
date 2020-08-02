# MiteWM

**MiteWM**は、X11ベースのLinux向け軽量ウインドウマネージャ(WM)です。 <br>
[sanmitsu-wm](https://github.com/Perukii/sanmitsu-wm)の機能と見た目を受け継いだ上で(もちろんバグは除く...)、随時新機能を追加することでさらなる機能性性を持ったWMを目指しています。

プロジェクトに関するやり取りについては基本的に日本語を使いますが、英語も許可しています。

**MiteWM** is X11-based WM(window manager) provides **thoroughly lightweight** desktop envirounment for Linux. <br>
This WM basically inherits [sanmitsu-wm](https://github.com/Perukii/sanmitsu-wm)'s functions and appearance (except bugs...), and additionally provides some original functions.  

**We fundamentally use Japanese(language)** for conducting this project, **but you can use English** for commiting and sending issues/pull requests alternatively.

### 使用ライブラリ・アプリケーション

**ライブラリ**
 - libX11
 - libcairo
 
**アプリケーション**
  - xterm(他のターミナルソフトでも可だが、make set_xinitrcを使用する場合はxtermである必要がある)
  - xcompmgr
 
### ビルド

ビルドにはX11が利用可能な環境が必要です。<br>

1.上記の開発用ライブラリをインストールします。<br>
2.makeを実行します。 <br>
3.正常なコンパイルが完了した場合、make set_xinitrc を行います。<br>
**なお実行前に、~/.xinitrcがまだファイルシステム上に存在していないこと確認してください。既に存在する場合は、上書きされても構わない内容であるかどうかを確認して実行するか、Makefileを参考に自分で書いて下さい。** <br> 
自分で書く場合は、WM実行前にxcompmgrを、実行後すぐにxterm等ターミナルが実行される必要があることに注意してください。

ここで、X11のセッションが無効である(または無効となっているttyに移動している)ことを確認してください。まだ出来ていない場合は、直ちに行って下さい。<br>
4.startxを実行します。


### Licence

GNU General Public License (version 3)

MiteWM
Copyright (C) 2020  Tada Perukii (Tada Teruki, 多田 瑛貴)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

