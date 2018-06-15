#include <string>
#include "XLibrary11.hpp"
using namespace std;
using namespace XLibrary11;

// ��ʂ̎��
enum Mode
{
	Title,
	Game,
};


//���C���֐�
int main()
{
	const int coinNumber = 10;			// �R�C���̐�
	const int blockNumber = 10;      //�u���b�N�̐�
	const float blockInterval = 1000.0; //�u���b�N�̊Ԋu
	const float coinInterval = 200.0;	// �R�C���̊Ԋu

	float coinAmplitude = 200.0;	// ����
	float blockAmplitude = 300.0; //�u���b�N�̌���
	 float speed = 5.0;			// �i�ޑ���
	const float gravitySpeed = 0.5;		// ������X�s�[�h
	const float jumpSpeed = 7.0;		// �W�����v��
	
	float gravity = 0.0;				// �d��
	
	float coinPosition = 0.0;
	float blockPosition = 0.0;
	float distance;						// �i�񂾋���
	int score;							// ����������
	Mode mode = Title;					// ��ʁiTitle�͏����̉�ʁj
	
	Float2 coinOffset[coinNumber];
	Float2 blockOffset[blockNumber];

	Sprite sprite(L"player.jpg");	// �v���C���[�̉摜
	sprite.scale = 0.40;

	Sprite block(L"block2.png"); //�u���b�N�̉摜
	block.scale = 0.2;

	Sprite coin(L"coin.jpg"); //�R�C���̉摜
	coin.scale = 0.4;

	Sprite BackGround(L"�w�i.jpg");//�w�i�摜

	


	Text scoreText(L"0", 10.0);
	scoreText.position.y = 200.0;
	scoreText.scale = 10.0;
	scoreText.color = Float4(1.0, 0.0, 0.0, 1.0);
	

	Text titleText(L"Fly �F�B���O�̉\���͖�����\n\n\n1�{�^���������ƒN�����I�񂾋�\n2�������Ɖ����I�񂾋�\n\n���������Ă݂Ă�", 20.0);
	titleText.position.x = 30.0;
	titleText.color = Float4(1.0, 0.0, 0.0, 1.0);

	App::SetTitle(L"Fly �F�B���O�̉\���͖�����");

	Camera camera;

	Sound flySound(L"fly.wav");		// �W�����v�̉�
	flySound.SetVolume(0.5); //��ԉ�
	Sound hitSound(L"hit.wav");		// ��Q���ɓ����������̉�
	hitSound.SetVolume(0.5);//����������
	Sound pointSound(L"point.wav");	// �X�R�A�����Z���ꂽ���̉�
	pointSound.SetVolume(0.5);//�X�R�A���Z�̉�
	Sound BGM1(L"�p���_���I�񂾃Z���X�Ȃ���.mp3");//���y��BGM
	BGM1.SetLoop(true);//���y���[�v
	Sound BGM2(L"�l�͌F����.mp3");//���y��BGM2
	BGM2.SetLoop(true);//���y2���[�v
	
	// �J��Ԃ�
	while (App::Refresh())
	{
		camera.Update();

		

		switch (mode)
		{
			// �^�C�g����ʂ̏���
		case Title:

			coin.Draw();

			//1����������F�̉�
			if (App::GetKeyDown('1'))
			{
				BGM2.Stop();
				BGM1.Play();
			}
				

			//2����������F2�̉�
			if (App::GetKeyDown('2'))
			{
				BGM1.Stop();
				BGM2.Play();
			}

			// �G���^�[�L�[����������Q�[����ʂ�
			if (App::GetKeyDown(VK_RETURN))
			{
				// �p�����[�^�����Z�b�g

				coinPosition = 0.0;
				blockPosition = 0.0;
				gravity = 0.0;
				distance = sprite.position.x;
				score = 0;
				speed = 3.0;




				for (int i = 0; i < coinNumber; i++)
				{
					// �R�C�������ɓ��Ԋu�A�c�Ƀ����_���ɕ��ׂ�
					coinOffset[i] = Float2(
						coinPosition + coinInterval,
						rand() / (float)RAND_MAX * coinAmplitude - coinAmplitude / 2
					);
					coinPosition = coinOffset[i].x;
				}

				mode = Game;


				for (int i = 0; i < blockNumber; i++)
				{
					// �u���b�N�����ɓ��Ԋu�A�c�Ƀ����_���ɕ��ׂ�
					blockOffset[i] = Float2(
						blockPosition + blockInterval,
						rand() / (float)RAND_MAX * blockAmplitude - blockAmplitude / 2
					);
					blockPosition = blockOffset[i].x;
				}

				mode = Game;
			}

			// �v���C���[�����Z�b�g
			sprite.position = Float3(-200.0, 0.0, 0.0);
			sprite.angles.z = 0.0;

			sprite.Draw();

			titleText.Draw();

			scoreText.Draw();

			break;

			// �Q�[����ʂ̏���
		case Game:

			BackGround.Draw();

			if(sprite.position.y <= -App::GetWindowSize().y)
			{
				mode = Title;
			}

			

			for (int i = 0; i < coinNumber; i++)
			{
				// �R�C�������ɂ����Č����Ȃ��Ȃ�����A�E�Ɉړ�����
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

				// �����蔻��
				if (coinOffset[i].x - coinWidth < sprite.position.x &&
					coinOffset[i].x + coinWidth > sprite.position.x)
				{
					if (coinOffset[i].y - coinHeight < sprite.position.y &&
						coinOffset[i].y + coinHeight > sprite.position.y)
					{
						// �����������̏���
						pointSound.Play();
						score++;
						coinOffset[i] = Float2(
							coinPosition + coinInterval,
							rand() / (float)RAND_MAX * coinAmplitude - coinAmplitude / 2
						);
						coinPosition = coinOffset[i].x;
					}
				}

				// �R�C���𓮂�������
				coinOffset[i].x -= speed;
					



				// �����̃R�C����`�悷�鏈��
				coin.position = coinOffset[i];

				coin.Draw();
			}
			coinPosition -= speed;


			for (int i = 0; i < blockNumber; i++)
			{
				// �u���b�N�����ɂ����Č����Ȃ��Ȃ�����A�E�Ɉړ�����
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

				// �����蔻��
				if (blockOffset[i].x - blockWidth < sprite.position.x &&
					blockOffset[i].x + blockWidth > sprite.position.x)
				{
					if (blockOffset[i].y - blockHeight < sprite.position.y &&
						blockOffset[i].y + blockHeight > sprite.position.y)
					{
						// �����������̏���
						mode = Title;
						
						blockOffset[i] = Float2(
							blockPosition + blockInterval,
							rand() / (float)RAND_MAX * blockAmplitude - blockAmplitude / 2
						);
						blockPosition = blockOffset[i].x;
					}
				}

				// �u���b�N�𓮂�������
				blockOffset[i].x -= speed;



				// �����̃u���b�N��`�悷�鏈��
				block.position = blockOffset[i];

				block.Draw();
			}
			blockPosition -= speed;
			




	
			if (App::GetKeyDown(VK_LBUTTON))
			{
				flySound.Play();
				gravity = jumpSpeed;
				speed = 0.0;
				//�����}�E�X�̏ꏊ���������N���b�N������
				if (App::GetMousePosition().x < -50.0)
				{
					speed = -0.5;
				}
				//�����}�E�X�̏ꏊ���E�����N���b�N������
				if (App::GetMousePosition().x > 50.0)
				{
					speed = 2.75;
				}
				//�����}�E�X�̏ꏊ���㑤���N���b�N������
				if (App::GetMousePosition().y > 50.0)
				{
					gravity = jumpSpeed * 2.0;
				}
				//�����}�E�X�̏ꏊ���������N���b�N������
				if (App::GetMousePosition().y < -50.0)
				{
					gravity = jumpSpeed * -0.75;
				}
			}

			// �d�͂̏���
			gravity -= gravitySpeed;
			sprite.position.y += gravity;
			sprite.angles.z = gravity * 5.0;

			// �v���C���[�̕`��
			sprite.Draw();

			// ���������Z
			distance += speed;

			//�v���C���[�����X�Ɍ��ɉ����炷
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
