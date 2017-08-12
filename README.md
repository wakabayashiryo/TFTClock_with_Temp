#
<div style="text-align: center;">
# STM32F4ファミリー用GNU-GCCプロジェクト
</div>

---
- ## 概要
    STM32F4ファミリのプログラムをコンパイルするためのプロジェクト

- ## プロジェクトの使い方
    端末より、プロジェクトのディレクトリまで移動し,makeコマンドでコンパイルされます。コンパイル完了後、BinとDebugフォルダ内に実行ファイルとオブジェクトファイルが生成されます。以下にコンパイルのコマンドを示します。

- ## コマンド一覧
    
    | コマンド | 内容 |
    |:---------:|:------------------------------------------------------|
    | make | すべてのソースファイルがコンパイルされBinフォルダに実行ファイルが生成されます。 |
    | make clean | Debug Binフォルダが消去され、実行ファイル、オブジェクトファイルが消去されます。 |
    | make -j | コンパイル時CPUが並列で処理します。他のオプションと兼用可能 |
    | make -B | すべてのファイルが強制的に再コンパイルされます。 |
    | make OPTIMIZE=-Ox | 最適化オプションを変更します。xの値で最適化レベルを変更できます。デフォルトは-O0 |

- ## プロジェクト詳細


~~~
    STM32F4xx
    ├── Apprication
    ├── Drivers
    │   ├── CMSIS
    │   │   ├── Device
    │   │   │   ├── Inc
    │   │   │   │   ├── デバイスのヘッダーファイル
    │   │   │   │   ├── stm32f4xx.h
    │   │   │   │   └── system_stm32f4xx.h
    │   │   │   └── Src
    │   │   │       ├── gcc
    │   │   │       │   └── スタートアップファイル
    │   │   │       └── system_stm32f4xx.c
    │   │   ├── Inc
    │   │   │   ├── arm_common_tables.h
    │   │   │   ├── arm_const_structs.h
    │   │   │   ├── arm_math.h
    │   │   │   ├── cmsis_armcc.h
    │   │   │   ├── cmsis_armcc_V6.h
    │   │   │   ├── cmsis_gcc.h
    │   │   │   ├── core_cm0.h
    │   │   │   ├── core_cm0plus.h
    │   │   │   ├── core_cm3.h
    │   │   │   ├── core_cm4.h
    │   │   │   ├── core_cm7.h
    │   │   │   ├── core_cmFunc.h
    │   │   │   ├── core_cmInstr.h
    │   │   │   ├── core_cmSimd.h
    │   │   │   ├── core_sc000.h
    │   │   │   └── core_sc300.h
    │   │   └── doc
    │   ├── LinkerScript
    │   │   └── リンカースクリプトファイル
    │   └── STM32F4xx_HAL_Driver
    │       ├── Inc
    │       │   ├── Legacy
    │       │   │   └── stm32_hal_legacy.h
    │       │   ├──HALドライバーのヘッダーファイル
    │       └── Src
    │           └── HALドライバーのソースファイル
    ├── Inc
    │   ├── mxconstants.h
    │   ├── stm32f4xx_hal_conf.h
    │   └── stm32f4xx_it.h
    ├── Makefile
    ├── MidleWare
    ├── README.md
    └── Src
        ├── main.c
        ├── stm32f4xx_hal_msp.c
        └── stm32f4xx_it.c
~~~

- ## 各ディレクトリの説明
    - ./Inc・・・ユーザーが記述したヘッダーファイルを入れます。
    - ./Src・・・メインのプログラムを記述するソースファイルを入れます。
    - ./MidleWare・・・このフォルダーはHALドライバーを制御するプログラムを入れます。
      MidleWare内に各ライブラリのフォルダーを作成し、その中にInc(インクルード)Src(ソース)フォルダーを作成します。
      **Inc Srcフォルダーを作成しないとエラーが発生します**
    - ./Apprication・・・このフォルダーはデータ管理や周辺機器の管理するプログラムを入れます。
      MidleWare内に各ライブラリのフォルダーを作成し、その中にInc(インクルード)Src(ソース)フォルダーを作成します。
      **Inc Srcフォルダーを作成しないとエラーが発生します**
    - ./Bin・・・実行ファイルが出力されるフォルダーです。
    - ./Debug・・・コンパイル時に生成されるオブジェクトファイルと依存関係ファイルが出力されます。
    - ./Drivers・・・デバイスに必要なヘッダーファイルやリンカースクリプト、HALドライバーが入っています。
      基本的にユーザーはこのフォルダ内のファイルは変更しません。ドライバーの更新する場合はこのディレクトリの各ファイルを最新のものに入れ替えます

- ## ドライバーの更新方法
    1. ST社のホームページ、またはSTM32Cubeから最新のHALドライバーをダウンロードします。
    2. zipファイルを解凍します。
    3. 解凍した各ファイルは以下の表に基づいて前のバージョンのファイルと入れ替えてください。***HALドライバーのテンプレートファイルは消去してください***
    4. README.mdのバージョン情報にライブラリの更新を記述
    5. コンパイルを行い、エラーが出ないことを確認してください
    6. make cleanした後に、gitにcommitします

    | ファイルの種類| ファイル名    | ディレクトリ               |
    |:--------:|:-------------:|:---------------------------:|
    |デバイスのヘッダーファイル      | stm32f4xx**.h        | ./Drivers/CMSIS/Device/Inc    |
    |デバイスのスタートアップファイル   | startup_stm32f4xx**.s| ./Drivers/CMSIS/Device/Src/gcc|
    |リンカースクリプト | STM32F4XX**.ld| ./Drivers/LinkerScript      |
    |CMSISのヘッダーファイル | cmsis.h or core.h | ./Drivers/CMSIS/Inc|
    |HALドライバーのヘッダーファイル  | stm32_hal_*.h| ./Drivers/STM32F4xx_HAL_Driver/Inc|
    |HALドライバーのソースファイル   | stm32_hal_*.c| ./Drivers/STM32F4xx_HAL_Driver/Src|

- ## 対応デバイス

    |Define|Detail|
    |:-----:|:------- |
    |  STM32F405xx|STM32F405RG, STM32F405VG and STM32F405ZG   |
    |  STM32F415xx|STM32F415RG, STM32F415VG and STM32F415ZG   |
    |  STM32F407xx|STM32F407VG, STM32F407VE, STM32F407ZG, STM32F407ZE, STM32F407IG  and STM32F407IE   |
    |  STM32F417xx|STM32F417VG, STM32F417VE, STM32F417ZG, STM32F417ZE, STM32F417IG and STM32F417IE   |
    |  STM32F427xx|STM32F427VG, STM32F427VI, STM32F427ZG, STM32F427ZI, STM32F427IG and STM32F427II   |
    |  STM32F437xx|STM32F437VG, STM32F437VI, STM32F437ZG, STM32F437ZI, STM32F437IG and STM32F437II   |
    |  STM32F429xx|STM32F429VG, STM32F429VI, STM32F429ZG, STM32F429ZI, STM32F429BG, STM32F429BI, STM32F429NG,STM32F439NI, STM32F429IG  and STM32F429II   |
    |  STM32F439xx|STM32F439VG, STM32F439VI, STM32F439ZG, STM32F439ZI, STM32F439BG, STM32F439BI, STM32F439NG,STM32F439NI, STM32F439IG and STM32F439II   |
    |  STM32F401xC|STM32F401CB, STM32F401CC, STM32F401RB, STM32F401RC, STM32F401VB and STM32F401VC   |
    |  STM32F401xE|STM32F401CD, STM32F401RD, STM32F401VD, STM32F401CE, STM32F401RE and STM32F401VE   |
    |  STM32F410Tx|STM32F410T8 and STM32F410TB   |
    |  STM32F410Cx|STM32F410C8 and STM32F410CB   |
    |  STM32F410Rx|STM32F410R8 and STM32F410RB   |
    |  STM32F411xE|STM32F411CC, STM32F411RC, STM32F411VC, STM32F411CE, STM32F411RE and STM32F411VE   |
    |  STM32F446xx|STM32F446MC, STM32F446ME, STM32F446RC, STM32F446RE, STM32F446VC, STM32F446VE, STM32F446ZC,and STM32F446ZE   |
    |  STM32F469xx|STM32F469AI, STM32F469II, STM32F469BI, STM32F469NI, STM32F469AG, STM32F469IG, STM32F469BG,STM32F469NG, STM32F469AE, STM32F469IE, STM32F469BE and STM32F469NE   |
    |  STM32F479xx|STM32F479AI, STM32F479II, STM32F479BI, STM32F479NI, STM32F479AG, STM32F479IG, STM32F479BGand STM32F479NG   |
    |  STM32F412Cx|STM32F412CEU and STM32F412CGU   |
    |  STM32F412Zx|STM32F412ZET, STM32F412ZGT, STM32F412ZEJ and STM32F412ZGJ   |
    |  STM32F412Vx|STM32F412VET, STM32F412VGT, STM32F412VEH and STM32F412VGH   |
    |  STM32F412Rx|STM32F412RET, STM32F412RGT, STM32F412REY and STM32F412RGY   |
    |  STM32F413xx|STM32F413CH, STM32F413MH, STM32F413RH, STM32F413VH, STM32F413ZH, STM32F413CG, STM32F413MG,STM32F413RG, STM32F413VG and STM32F413ZG   |
    |  STM32F423xx|STM32F423CH, STM32F423RH, STM32F423VH and STM32F423ZH   |

     - ## NUCLEO使用時の注意
        NUCLEOのST-LINKから出力されるMCO(MasterClockOutput)をメインクロックで使用する場合、必ず回路図を確認してMCOとクロック入力ピンを接続してください。
※デフォルトでは接続されていない。ショート用パッドにハンダ付けして接続する

 - # デバイスの変更方法

   >  MakeFile冒頭(参考デバイスSTM32F407VGT)

        #project configuration
        PROJECT := F4xx_Project                     ・・・出力する実行ファイル名
        OPTIMIZE := -O0 -g3                         ・・・最適化オプションとデバッグオプション
        HARDFP := 0                                 ・・・浮動小数点計算をハードウェアで行うか 0:Disable1:Enable
        DEVICENAME := STM32F407xx                   ・・・使用するデバイスを定義します。定義名は上の表より選択してください
        STARTUP_FILE := startup_stm32f407xx.s       ・・・スタートアップファイル名を記述してください
        LINKERSCRIPT_FILE := STM32F407VGTx_FLASH.ld ・・・リンカースクリプト名を記述してください


 - ### バージョン情報
    |バージョン  |日付|更新内容 |
    |:-----:|:-------:|:----------------|
    |ver1.0 |2017/3/18|READMEの記述|
    |ver1.01|2017/8/12|コンパイル時に使用しているメモリを表示させた|
 - # ライセンス情報
    ## This software is released under the MIT License, see LICENSE