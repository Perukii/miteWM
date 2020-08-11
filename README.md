# MiteWM

![doga-mite3](https://user-images.githubusercontent.com/57752033/89909409-004ced00-dc2a-11ea-980a-7ff7d0de59aa.gif)


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

1.`libX11` `libcairo` をインストールします。<br>
2.`$ make`を実行します。 <br>
3.正常なコンパイルが完了した場合、`$ make set_xinitrc` を行います。<br>
**なお実行前に、`~/.xinitrc`がまだファイルシステム上に存在していないこと確認してください。既に存在する場合は、上書きされても構わない内容であるかどうかを確認して実行するか、Makefileを参考に自分で書いて下さい。** <br> 
自分で書く場合は、WM実行前に`xcompmgr`を、実行後すぐに`xterm`等ターミナルが実行される必要があることに注意してください。

ここで、X11のセッションが無効である(または無効となっているttyに移動している)ことを確認してください。まだ出来ていない場合は、直ちに行って下さい。<br>
4.`$ startx`を実行します。
