# game-libraries

ゲーム開発に利用できるライブラリ

## C++17 Header-Only libraries

VisualStudio2022の場合、Incまでのパスを通すだけで利用できます。全てHeaderOnlyなので、SolutionExplorerに追加する必要はありません。<br>
__※ExternalDependenciesフォルダ内は外部ライブラリを利用しているソースなので別途include、link等が必要です。__

| Directory                              | File                  | Description                      |
| -------------------------------------- | --------------------- | -------------------------------- |
| Inc\ExternalDependencies\Asset\IAsset\ | IAssetData.h          | 非同期ロード対応のインターフェース                |
|                                        | IAssetManager.h       | IAssetDataを管理するクラス               |
| Inc\ExternalDependencies\Asset\Json\   | JsonData.h            | IAssetDataをnlohmann_jsonで実装したクラス |
|                                        | JsonManager.h         | JsonDataを管理するクラス                 |
| Inc\ExternalDependencies\Audio\        | AudioHelper.h         | DirectXTKAudioのヘルパー              |
|                                        | AudioManager.h        | AudioEngineやinstanceを内包した管理クラス   |
| Inc\ExternalDependencies\DirectX11\    | DirectX11.h           | デバイスやコンテキストを管理するクラス              |
|                                        | DirectX11Buffer.h     | シェーダーに送信するConstantBufferを管理するクラス |
|                                        | DirectX11Helper.h     | DirectX11のヘルパー                   |
|                                        | DirectX11Texture.h    | DirectX11のテクスチャクラス               |
| Inc\ExternalDependencies\Effekseer\    | EffekseerHelper.h     | エフェクトを再生するライブラリ用ヘルパー             |
|                                        | EffekseerManager.h    | Rendererやinstanceを内包した管理クラス      |
| Inc\ExternalDependencies\ImGui\        | ImGuiHelper.h         | ImGuiのヘルパー                       |
| Inc\ExternalDependencies\Input\        | InputHelper.h         | カーソルやキーの入力ヘルパー                   |
|                                        | InputManager.h        | 入力の管理クラス                         |
|                                        | TextBox.h             | テキストボックス                         |
| Inc\ExternalDependencies\Math\         | Camera.h              | カメラ用行列                           |
|                                        | Collider.h            | active衝突判定                       |
|                                        | Collision.h           | passive衝突判定                      |
|                                        | Transform.h           | 姿勢制御                             |
| Inc\ExternalDependencies\PhysX\        | PhysXHelper.h         | 物理エンジンのライブラリ用ヘルパー                |
|                                        | PhysXManager.h        | 物理エンジンのオブジェクト管理クラス               |
| Inc\ExternalDependencies\Socket\       | SocketHelper.h        | ソケット通信のヘルパー                      |
| Inc\ExternalDependencies\Utility\      | DateTime.h            | Win32Apiの日付を変換                   |
|                                        | String.h              | 文字列                              |
| Inc\Math\                              | Constant.h            | 定数                               |
|                                        | Convert.h             | 変換                               |
|                                        | DeltaTime.h           | 可変フレームに利用する経過時間                  |
|                                        | Easing.h              | イージング計算                          |
|                                        | MinimumMath.h         | 最小限の算術クラス                        |
|                                        | ProjectileMotion.h    | 放物運動の計算                          |
|                                        | Random.h              | ランダム                             |
|                                        | Timer.h               | 時間計測                             |
| Inc\Thread\SimpleThreadManager\        | SimpleThreadManager.h | SimpleUniqueThreadの管理クラス         |
|                                        | SimpleUniqueThread.h  | 一意のthreadインスタンスを保持するクラス          |
| Inc\Utility\                           | Assert.h              | vsoutputに警告を表示                   |
|                                        | Macro.h               | マクロを定義                           |
|                                        | Memory.h              | メモリ関連                            |
|                                        | StdC++.h              | 標準ライブラリ                          |
|                                        | Templates.h           | テンプレートクラス                        |

## Tools

環境変数にffmpeg、pythonを追加してください。またImageRgbaExtractorではPillowを使用しています。

| Directory               | File                       | Description      |
| ----------------------- | -------------------------- | ---------------- |
| ConvertToWave\          | ConvertToWave.bat          | 複数のファイルをWaveに変換  |
| EasyImageResizer\       | EasyImageResizer.bat       | 複数の画像をリサイズ       |
|                         | EasyImageResizer.py        | 画像をリサイズ          |
| ImageRgbaExtractor\     | ImageRgbaExtractor.bat     | 複数の画像からのRGBA値を抽出 |
|                         | ImageRgbaExtractor.py      | 画像からのRGBA値を抽出    |
| VideoToSequentialImage\ | VideoToSequentialImage.bat | 複数のビデオを連番画像に変換   |
|                         | VideoToSequentialImage.py  | ビデオを連番画像に変換      |
