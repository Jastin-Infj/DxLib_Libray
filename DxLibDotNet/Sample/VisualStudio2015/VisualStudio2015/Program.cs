using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;
using DxLibDLL;

namespace VisualStudio2015
{
    static class Program
    {
        /// <summary>
        /// アプリケーションのメイン エントリ ポイントです。
        /// </summary>
        [STAThread]
        static void Main()
        {
            // ウインドウモードで起動
            DX.ChangeWindowMode(DX.TRUE);

            // ＤＸライブラリの初期化
            if (DX.DxLib_Init() < 0)
            {
                return;
            }

            // 画像の読み込み
            int GraphHandle = DX.LoadGraph("Src1.bmp");

            // 描画先を裏画面に変更
            DX.SetDrawScreen(DX.DX_SCREEN_BACK);

            // 画像を左右に動かす処理のための変数を初期化
            int x = 0, xadd = 8;

            // メインループ
            while (DX.ProcessMessage() != -1)
            {
                // 画面をクリア
                DX.ClearDrawScreen();

                // 画像を描画する座標を更新
                x += xadd;
                if (x < 0 || x > 640)
                {
                    xadd = -xadd;
                }

                // 画像を描画
                DX.DrawGraph(x, 0, GraphHandle, DX.TRUE);

                // 裏画面の内容を表画面に反映する
                DX.ScreenFlip();
            }

            // ＤＸライブラリの後始末
            DX.DxLib_End();
        }
    }
}
