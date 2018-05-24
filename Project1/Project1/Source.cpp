#include <string>
#include "XLibrary11.hpp"
using namespace std;
using namespace XLibrary11;

// 画面の種類
enum Mode
{
	Title,
	Game,
};


//メイン関数
int main()
{
	const int coinNumber = 10;			// コインの数
	const int blockNumber = 10;      //ブロックの数
	const float blockInterval = 1000.0; //ブロックの間隔
	const float coinInterval = 200.0;	// コインの間隔

	float coinAmplitude = 200.0;	// 隙間
	float blockAmplitude = 300.0; //ブロックの隙間
	 float speed = 5.0;			// 進む速さ
	const float gravitySpeed = 0.5;		// 落ちるスピード
	const float jumpSpeed = 7.0;		// ジャンプ力
	
	float gravity = 0.0;				// 重力
	
	float coinPosition = 0.0;
	float blockPosition = 0.0;
	float distance;						// 進んだ距離
	int score;							// くぐった数
	Mode mode = Title;					// 画面（Titleは初期の画面）
	
	Float2 coinOffset[coinNumber];
	Float2 blockOffset[blockNumber];

	Sprite sprite(L"player.jpg");	// プレイヤーの画像
	sprite.scale = 0.40;

	Sprite block(L"block2.png"); //ブロックの画像
	block.scale = 0.2;

	Sprite coin(L"coin.jpg"); //コインの画像
	coin.scale = 0.4;

	Sprite BackGround(L"背景.jpg");//背景画像

	


	Text scoreText(L"0", 10.0);
	scoreText.position.y = 200.0;
	scoreText.scale = 10.0;
	scoreText.color = Float4(1.0, 0.0, 0.0, 1.0);
	

	Text titleText(L"Fly 熊。お前の可能性は無限大\n\n\n1ボタンを押すと誰かが選んだ曲\n2を押すと俺が選んだ曲\n\n両方試してみてね", 20.0);
	titleText.position.x = 30.0;
	titleText.color = Float4(1.0, 0.0, 0.0, 1.0);

	App::SetTitle(L"Fly 熊。お前の可能性は無限大");

	Camera camera;

	Sound flySound(L"fly.wav");		// ジャンプの音
	flySound.SetVolume(0.5); //飛ぶ音
	Sound hitSound(L"hit.wav");		// 障害物に当たった時の音
	hitSound.SetVolume(0.5);//当たった音
	Sound pointSound(L"point.wav");	// スコアが加算された時の音
	pointSound.SetVolume(0.5);//スコア加算の音
	Sound BGM1(L"パンダが選んだセンスない歌.mp3");//音楽のBGM
	BGM1.SetLoop(true);//音楽ループ
	Sound BGM2(L"僕は熊だよ.mp3");//音楽のBGM2
	BGM2.SetLoop(true);//音楽2ループ
	
	// 繰り返し
	while (App::Refresh())
	{
		camera.Update();

		

		switch (mode)
		{
			// タイトル画面の処理
		case Title:

			coin.Draw();

			//1を押したら熊の歌
			if (App::GetKeyDown('1'))
			{
				BGM2.Stop();
				BGM1.Play();
			}
				

			//2を押したら熊2の歌
			if (App::GetKeyDown('2'))
			{
				BGM1.Stop();
				BGM2.Play();
			}

			// エンターキーを押したらゲーム画面へ
			if (App::GetKeyDown(VK_RETURN))
			{
				// パラメータをリセット

				coinPosition = 0.0;
				blockPosition = 0.0;
				gravity = 0.0;
				distance = sprite.position.x;
				score = 0;
				speed = 3.0;




				for (int i = 0; i < coinNumber; i++)
				{
					// コインを横に等間隔、縦にランダムに並べる
					coinOffset[i] = Float2(
						coinPosition + coinInterval,
						rand() / (float)RAND_MAX * coinAmplitude - coinAmplitude / 2
					);
					coinPosition = coinOffset[i].x;
				}

				mode = Game;


				for (int i = 0; i < blockNumber; i++)
				{
					// ブロックを横に等間隔、縦にランダムに並べる
					blockOffset[i] = Float2(
						blockPosition + blockInterval,
						rand() / (float)RAND_MAX * blockAmplitude - blockAmplitude / 2
					);
					blockPosition = blockOffset[i].x;
				}

				mode = Game;
			}

			// プレイヤーをリセット
			sprite.position = Float3(-200.0, 0.0, 0.0);
			sprite.angles.z = 0.0;

			sprite.Draw();

			titleText.Draw();

			scoreText.Draw();

			break;

			// ゲーム画面の処理
		case Game:

			BackGround.Draw();

			if(sprite.position.y <= -App::GetWindowSize().y)
			{
				mode = Title;
			}

			

			for (int i = 0; i < coinNumber; i++)
			{
				// コインが左にいって見えなくなったら、右に移動する
				if (coinOffset[i].x < -App::GetWindowSize().x)
				{
					coinOffset[i] = Float2(
						coinPosition + coinInterval,
						rand() / (float)RAND_MAX * coinAmplitude - coinAmplitude / 2
					);
					coinPosition = coinOffset[i].x;
				}

				float playerSize = sprite.GetSize().x * sprite.scale.x * 0.8;
				float coinWidth = (coin.GetSize().x * coin.scale.x + playerSize) / 2.0;
				float coinHeight = (coinAmplitude - playerSize) / 2.0;

				// 当たり判定
				if (coinOffset[i].x - coinWidth < sprite.position.x &&
					coinOffset[i].x + coinWidth > sprite.position.x)
				{
					if (coinOffset[i].y - coinHeight < sprite.position.y &&
						coinOffset[i].y + coinHeight > sprite.position.y)
					{
						// 当たった時の処理
						pointSound.Play();
						score++;
						coinOffset[i] = Float2(
							coinPosition + coinInterval,
							rand() / (float)RAND_MAX * coinAmplitude - coinAmplitude / 2
						);
						coinPosition = coinOffset[i].x;
					}
				}

				// コインを動かす処理
				coinOffset[i].x -= speed;
					



				// 下側のコインを描画する処理
				coin.position = coinOffset[i];

				coin.Draw();
			}
			coinPosition -= speed;


			for (int i = 0; i < blockNumber; i++)
			{
				// ブロックが左にいって見えなくなったら、右に移動する
				if (blockOffset[i].x < -App::GetWindowSize().x)
				{
					blockOffset[i] = Float2(
						blockPosition + blockInterval,
						rand() / (float) RAND_MAX *  blockAmplitude - blockAmplitude / 2
					);
					blockPosition = blockOffset[i].x;
				}

				float playerSize = sprite.GetSize().x * sprite.scale.x * 0.8;
				float blockWidth = 50;
					float blockHeight = 50;

				// 当たり判定
				if (blockOffset[i].x - blockWidth < sprite.position.x &&
					blockOffset[i].x + blockWidth > sprite.position.x)
				{
					if (blockOffset[i].y - blockHeight < sprite.position.y &&
						blockOffset[i].y + blockHeight > sprite.position.y)
					{
						// 当たった時の処理
						mode = Title;
						
						blockOffset[i] = Float2(
							blockPosition + blockInterval,
							rand() / (float)RAND_MAX * blockAmplitude - blockAmplitude / 2
						);
						blockPosition = blockOffset[i].x;
					}
				}

				// ブロックを動かす処理
				blockOffset[i].x -= speed;



				// 下側のブロックを描画する処理
				block.position = blockOffset[i];

				block.Draw();
			}
			blockPosition -= speed;
			




	
			if (App::GetKeyDown(VK_LBUTTON))
			{
				flySound.Play();
				gravity = jumpSpeed;
				speed = 0.0;
				//もしマウスの場所が左側をクリックしたら
				if (App::GetMousePosition().x < -50.0)
				{
					speed = -0.5;
				}
				//もしマウスの場所が右側をクリックしたら
				if (App::GetMousePosition().x > 50.0)
				{
					speed = 2.75;
				}
				//もしマウスの場所が上側をクリックしたら
				if (App::GetMousePosition().y > 50.0)
				{
					gravity = jumpSpeed * 2.0;
				}
				//もしマウスの場所が下側をクリックしたら
				if (App::GetMousePosition().y < -50.0)
				{
					gravity = jumpSpeed * -0.75;
				}
			}

			// 重力の処理
			gravity -= gravitySpeed;
			sprite.position.y += gravity;
			sprite.angles.z = gravity * 5.0;

			// プレイヤーの描画
			sprite.Draw();

			// 距離を加算
			distance += speed;

			//プレイヤーを徐々に後ろに下がらす
			/*sprite.position.y*/

			

			scoreText.Create(to_wstring(score), 10.0);
			scoreText.Draw();

			if (App::GetKeyDown(VK_RBUTTON))
			{
				speed += 0.5;



			}
			break;
		}
	}

	return 0;
}
