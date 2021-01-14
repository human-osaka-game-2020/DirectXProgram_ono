#include "DirectGraphics.h"
#include "DirectInput.h"
#include <stdlib.h>

// XFileデータ保存用の構造体
/*
	なぜ、ヘッダファイルではなく、ソースファイルに構造体を宣言したか？
		->XFile構造体を使用するファイルがDirectGraphics.cppだけだから
*/
struct XFile
{
	DWORD MaterialNum;				// !< メッシュ数
	LPD3DXMESH Meshes;				// !< メッシュ
	LPD3DXBUFFER Materials;			// !< メッシュマテリアル
	LPDIRECT3DTEXTURE9* Textures;	// !< テクスチャデータ
};
XFile g_XFile;

LPDIRECT3D9 g_Interface    = nullptr;
LPDIRECT3DDEVICE9 g_Device = nullptr;
// テクスチャデータ
LPDIRECT3DTEXTURE9 g_Textures[TextureID::TexIDMax];

LPCWSTR g_TextureNameList[] =
{
	TEXT("res/gazou.png"),
	TEXT("res/gazou.png"),
	TEXT("res/gazou.png"),
};

bool InitDirectGraphics(HWND window_handle)
{
	/*
		Direct3DCreate9
			IDirect3D9生成関数

			引数：
				DirectXのバージョン(D3D_SDK_VERSION固定)

			戻り値：
				成功：
					IDirect3D9*
				失敗：
					nullptr	
	*/
	g_Interface = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_Interface == nullptr)
	{
		// 初期化失敗
		return false;
	}

	// Deviceの情報を設定するための構造体
	D3DPRESENT_PARAMETERS parameters;

	// 指定されたデータをサイズ分だけ0で初期化する
	ZeroMemory(&parameters, sizeof(D3DPRESENT_PARAMETERS));

	// バックバッファーの数
	parameters.BackBufferCount = 1;

	// バックバッファーのフォーマット
	// D3DFMT＿UNKNOWN => 知りません
	parameters.BackBufferFormat = D3DFMT_UNKNOWN;

	// ウィンドウモード指定
	/*
		true  => ウィンドウモード

		false => フルスクリーンモード
					※フルスクリーンは別途情報を指定する必要あり
	*/
	parameters.Windowed = true;

	/*
		バックバッファとフロントバッファの切り替え方法

		スワップエフェクト => D3DSWAPEFFECT_DISCARD(自動設定)

		スワップエフェクトとは：
			バックバッファとフロントバッファへの切り替え方法
	*/
	parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;

	/*
		Zバッファ法を使う場合PresentParameterで
		使うことを通知する必要有
	*/
	// ZバッファON
	// EnableAutoDepthStencil => デプス、ステンシルバッファの仕様フラグ
	parameters.EnableAutoDepthStencil = TRUE;
	// AutoDepthStencilFormat => デプス、ステンシルバッファのフォーマット
	// D3DFMT_D16 => デプスバッファの1データを16ビットで作成する
	parameters.AutoDepthStencilFormat = D3DFMT_D16;

	HRESULT result = g_Interface->CreateDevice(
		// ディスプレイアダプタの種類
		D3DADAPTER_DEFAULT,
		// デバイスの種類
		D3DDEVTYPE_HAL,
		// デバイスが使用するウィンドウのハンドル
		window_handle,
		// デバイスの制御の方法
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
		// PRESENT_PARAMETERS
		&parameters,
		// Deviceの保存変数指定
		&g_Device
	);

	/*
		FAILED => HRESULTの結果を判定してくれるマクロ
					trueなら失敗
					falseなら成功
	*/
	if (FAILED(result))
	{
		return false;
	}

	// プロジェクションの設定
	/*
		プロジェクションの設定をゲーム中に変更しない場合一度だけ設定をすればOK
	*/
	SetUpProjection();

	/*
		// 成功判定
		If(SUCCEEDED(result))
		{

		}
	*/
	return true;
}

void ReleaseDirectGraphics()
{
	if (g_Device != nullptr)
	{
		// Relase => 解放関数
		g_Device->Release();
		// 解放後の不正アクセス対応
		g_Device = nullptr;
	}

	if (g_Interface != nullptr) 
	{
		// Release => 解放関数
		g_Interface->Release();
		// 解放後の不正アクセス対応
		g_Interface = nullptr;
	}
}

void StartDrawing()
{
	// ブレンドの設定(以下の設定では頂点カラーとテクスチャのαブレンドを許可している)
	/*
		SetTextureStageState
			第一：ステージのID
			第二：設定の種類
			第三：設定の詳細
	*/

	/*
		SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE)
			第一：0
			第二：D3DTSS_ALPHAOP  <= α値の合成に関する設定をする
			第三：D3DTOP_MODULATE <= 合成は乗算を使用する
	*/
	g_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	/*
		SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			第一：0
			第二：D3DTSS_COLORARG1  <=  乗算の値の設定
			第三：D3DTOP_MODULATE   <=  テクスチャカラーを使用する
	*/
	g_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	/*
		SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
			第一：0
			第二：D3DTSS_COLORARG2 <=  乗算の値の設定
			第三：D3DTOP_DIFFUSE   <=  Stageに送られてきた合成カラー
	*/
	g_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	// α値の反映を有効にする
	/*
		SetRenderState <= 描画の設定を変更する
			第一：変更したい設定の種類
			第二：設定の詳細
	*/
	/*
		SetRenderState <= 描画の設定を変更する
			第一：D3DRS_ALPHABLENDENABLE <= αブレンドの設定を変更する
			第二：true					 <= 有効 (falseが無効)
	*/
	g_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	/*
		SetRenderState <= 描画の設定を変更する
			第一：D3DRS_SRCBLEND	<= バッファに送信される色の設定
			第二：D3DBLEND_SRCALPHA	<= アルファをそのまま使う
	*/
	g_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	/*
		SetRenderState <= 描画の設定を変更する
			第一：D3DRS_DESTBLEND		<= バッファの色の設定
			第二：D3DBLEND_INVSRCALPHA	<= SRC(バッファに送信される色)のアルファを利用する(1 - srcalpha)
	*/
	g_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// ライティングを無効にする
	g_Device->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Zバッファの仕様をONにする
	g_Device->SetRenderState(D3DRS_ZENABLE, TRUE);

	// バックバッファをクリアする
	g_Device->Clear(
		0,							  // 0固定
		nullptr,					  // nullptr固定
		// D3DCLEAR_ZBUFFER => Zバッファもクリア対象に加える
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,			  // D3DCLEAR_TARGET固定
		D3DCOLOR_XRGB(255, 255, 200), // 塗りつぶす色
		1.0f,						  // 1.0f固定
		0							  // 0固定
	);

	// DirectXで描画を開始する
	g_Device->BeginScene();
}

void FinishDrawing()
{
	// DirectXの描画を終了する
	g_Device->EndScene();

	// バックバッファをフロントバッファに転送する
	g_Device->Present(nullptr, nullptr, nullptr, nullptr);
}

void DrawPorigon()
{
	CustomVertexTex vertices[] =
	{
		{ 300.0f, 100.0f, 0.0f, 0xffffff},
		{ 500.0f, 300.0f, 0.0f, 0xffffff},
		{ 50.0f , 300.0f, 0.0f, 0xffffff},
	};

	// DirectXに頂点構造を教える必要がある
	g_Device->SetFVF(FVF_2D_VERTEX);

	g_Device->DrawPrimitiveUP(
		D3DPT_TRIANGLELIST,		// ポリゴンの作り方
		1,						// ポリゴンの数
		vertices,				// 頂点リスト
		sizeof(CustomVertex)	// 1頂点のサイズ
	);
}

void DrawPorigonWithTriangleList()
{
	CustomVertex vertices[] =
	{
		// 1つ目
		{   0.0f,   0.0f, 0.0f, 1.0f, 0xffffff},
		{ 100.0f, 100.0f, 0.0f, 1.0f, 0xffffff},
		{   0.0f, 100.0f, 0.0f, 1.0f, 0xffffff},
		// 2つ目
		{ 100.0f,   0.0f, 0.0f, 1.0f, 0xffffff},
		{ 100.0f, 100.0f, 0.0f, 1.0f, 0xffffff},
		{   0.0f,   0.0f, 0.0f, 1.0f, 0xffffff},
	};
	// DirextXに頂点構造を教える必要がある
	g_Device->SetFVF(FVF_2D_VERTEX);


	g_Device->DrawPrimitiveUP(
		D3DPT_TRIANGLELIST,		// ポリゴンの作り方
		2,						// ポリゴンの数
		vertices,				// 頂点リスト
		sizeof(CustomVertex)	// １頂点のサイズ
	);
}

void DrawPorigonWithTriangleStrip()
{
	CustomVertex vertices[] =
	{
		{ 300.0f, 100.0f, 0.0f,1.0f, 0xffdc143c},
		{ 300.0f,   0.0f, 0.0f,1.0f, 0xff000088},
		{ 400.0f, 100.0f, 0.0f,1.0f, 0xffffffff},
		{ 400.0f,   0.0f, 0.0f,1.0f, 0xffffffff},
		{ 500.0f, 100.0f, 0.0f,1.0f, 0xffffffff},
		{ 500.0f,   0.0f, 0.0f,1.0f, 0xffffffff},
	};
	// DirextXに頂点構造を教える必要がある
	g_Device->SetFVF(FVF_2D_VERTEX);

	g_Device->DrawPrimitiveUP(
		D3DPT_TRIANGLESTRIP,	// ポリゴンの作り方
		4,						// ポリゴンの数
		vertices,				// 頂点リスト
		sizeof(CustomVertex)	// １頂点のサイズ
	);
}

void DrawPorigonWithTriangleFan(TextureID tex_id)
{
	if (g_Textures[tex_id] == nullptr)
	{
		return;
	}

	CustomVertexTex vertices[] =
	{
		//							 ARGB
		{ 200.0f, 200.0f, 0.0f,1.0f, 0xffffffff,0.0f,0.0f },
		{ 300.0f, 200.0f, 0.0f,1.0f, 0xffffffff,1.0f,0.0f },
		{ 300.0f, 300.0f, 0.0f,1.0f, 0xffffffff,1.0f,1.0f },
		{ 200.0f, 300.0f, 0.0f,1.0f, 0xffffffff,0.0f,1.0f },
	};

	// DirextXに頂点構造を教える必要がある
	g_Device->SetFVF(FVF_2D_VERTEX_TEXTURE);

	// DirectX(Computer)に今回のポリゴン描画で使用するテクスチャを教える
	g_Device->SetTexture(0, g_Textures[tex_id]);

	g_Device->DrawPrimitiveUP(
		D3DPT_TRIANGLEFAN,		// ポリゴンの作り方
		2,						// ポリゴンの数
		vertices,				// 頂点リスト
		sizeof(CustomVertexTex)	// １頂点のサイズ
	);
}

void DrawXFile(float x_, float y_, float z_, float angle_)
{
	float ScaleX = 1.0f;
	float ScaleY = 1.0f;
	float ScaleZ = 1.0f;

	float RadianX = D3DXToRadian(0.0f);
	float RadianY = D3DXToRadian(angle_);
	float RadianZ = D3DXToRadian(0.0f);

	D3DXMATRIX Matrix;
	D3DXMATRIX ScaleMatrix;
	D3DXMATRIX TransMatrix;
	D3DXMATRIX RotateX_Matrix;
	D3DXMATRIX RotateY_Matrix;
	D3DXMATRIX RotateZ_Matrix;

	// D3DXMatrixIdentity => 単位行列化する
	D3DXMatrixIdentity(&Matrix);

	// 拡縮
	D3DXMatrixScaling(&ScaleMatrix, ScaleX, ScaleY, ScaleZ);

	// 回転
	D3DXMatrixRotationX(&RotateX_Matrix, RadianX);
	D3DXMatrixRotationY(&RotateY_Matrix, RadianY);
	D3DXMatrixRotationZ(&RotateZ_Matrix, RadianZ);

	// 移動
	D3DXMatrixTranslation(&TransMatrix, x_, y_, z_);

	Matrix *= ScaleMatrix;

	Matrix *= RotateX_Matrix;
	Matrix *= RotateY_Matrix;
	Matrix *= RotateZ_Matrix;

	Matrix *= TransMatrix;


	g_Device->SetTransform(D3DTS_WORLD, &Matrix);

	// 描画処理
	/*
		XFileの描画はマテリアル単位で行う
		各マテリアル単位でマテリアルの設定とそのマテリアルで使われているテクスチャの設定を行う
		その設定が完了したらメッシュを描画する
	*/
	D3DXMATERIAL* materials = (D3DXMATERIAL*)g_XFile.Materials->GetBufferPointer();
	for (DWORD i = 0; i < g_XFile.MaterialNum; i++)
	{
		// マテリアルの設定
		g_Device->SetMaterial(&materials[i].MatD3D);
		// テクスチャの設定
		g_Device->SetTexture(0, g_XFile.Textures[i]);
		// メッシュを描画
		g_XFile.Meshes->DrawSubset(i);
	}
}

void SetUpView()
{
	D3DXMATRIX Matrix; // カメラ情報を保存するための行列
	D3DXVECTOR3 CameraPos = D3DXVECTOR3(0.0f, 0.0f, -200.0f);
	D3DXVECTOR3 EyePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 UpVec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	/*
		D3DXMatrixLookAtLH
			カメラ情報をもつ行列を作成する
	*/
	// L => Left H => Hand LH => 左手系でカメラ座標を設定する
	D3DXMatrixLookAtLH(
		&Matrix,		// 結果保存用行列の指定
		&CameraPos,	// カメラ座標
		&EyePos,		// 注視点座標
		&UpVec			// ワールド座標上におけるカメラの上向きのベクトル
	);

	// SetTransform => 座標変換系の情報をDirectXに通知する
	g_Device->SetTransform(D3DTS_VIEW, &Matrix);
}

void SetUpProjection()
{
	D3DXMATRIX matrix; // プロジェクション情報を保存するための行列
	float aspect = 640.0f / 480.0f;	// アスペクト比(ウィンドウサイズ横 / 縦)

	// Perspective => 透視投影
	D3DXMatrixPerspectiveFovLH(
		&matrix,				// 結果保存用の行列の指定
		// D3DXToRadian => 度数法をラジアンに変換
		D3DXToRadian(60.0f),	// 画角(ラジアン指定)
		aspect,					// アスペクト比
		0.1f,					// 視錐台のNear値
		1000.0f					// 視錐台のFar値
	);

	// SetTransform => 座標変換系の情報をDirectXに通知する
	g_Device->SetTransform(D3DTS_PROJECTION, &matrix);
}

void Draw3DPorigon()
{
	// ポリゴン描画
	CustomVertex3D vertices[]
	{
		{ -1.0f, -1.0f, 0.0f, 0xffff0000 },
		{ 0.0f, 1.0f, 0.0f, 0xffff0000 },
		{ 1.0f, -1.0f, 0.0f, 0xffff0000 }
	};

	g_Device->SetFVF(FVF_3D_VERTEX);

	g_Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 1, vertices, sizeof(CustomVertex3D));
}

void ReleaseTexture()
{
	for (int i = 0; i < TextureID::TexIDMax; i++)
	{
		if (g_Textures[i] != nullptr)
		{
			g_Textures[i]->Release();
			g_Textures[i] = nullptr;
		}
	}
}

bool LoadTexture(TextureID tex_id)
{
	HRESULT hr = D3DXCreateTextureFromFile(
		g_Device,					// DirectX9のデバイス
		g_TextureNameList[tex_id],	// ファイル名
		&g_Textures[tex_id]			// 読み込まれたテクスチャ情報が保存される変数
	);

	// FAILED => HRESULT型の変数を指定して、その値が失敗していたらtrue
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

bool LoadXFile(LPCWSTR file_name)
{
	HRESULT hr = D3DXLoadMeshFromX(
		file_name,				// ●ファイル名
		D3DXMESH_IB_SYSTEMMEM,	// メッシュ作成のオプション
		g_Device,				// DirectGraphicsのデバイス(D3DXMESH_SYSTEMMEN固定)
		nullptr,				// 現状はnullptrで良し
		&g_XFile.Materials,		// ●マテリアル保存用
		nullptr,				// 現状はnullptrで良し
		&g_XFile.MaterialNum,	// ●マテリアルの数保存用
		&g_XFile.Meshes			// ●メッシュデータ保存用
	);

	// メッシュ::モデルの形状がわかる情報
	// マテリアル::モデルの材質がわかる情報
	//		マテリアルで使われているテクスチャの読み込み
	//		マテリアルにあるテクスチャ情報 => ファイルの名前(パス付)

	if (FAILED(hr))
	{
		return false;
	}

	// マテリアルの数だけテクスチャを保存できるようにする
	g_XFile.Textures = new LPDIRECT3DTEXTURE9[g_XFile.MaterialNum];
	// バッファの先頭ポインタをD3DXMATERIALにキャストして取得
	/*
		Materialsの型を確認するとわかるが、MaterialsはLPD3DBUFFER型になっている
		Bufferという汎用的にデータを保存する型にマテリアルが保存されているため、
		Material本来の型に戻す必要がある

		GetBufferPointerを使用すればBufferの先頭ポインタが取得できる
	*/
	D3DXMATERIAL* materials = (D3DXMATERIAL*)g_XFile.Materials->GetBufferPointer();

	for (int i = 0; i < g_XFile.MaterialNum; i++)
	{
		// pTexture
		if (materials[i].pTextureFilename != nullptr)
		{
			D3DXCreateTextureFromFileA(
				g_Device,
				(LPCSTR)materials[i].pTextureFilename,
				&g_XFile.Textures[i]);
		}
		else
		{
			g_XFile.Textures[i] = nullptr;
		}
	}

	return true;
}

void UpdateTPSCamera(float x_, float y_, float z_, float angle_)
{
	// ターゲットの向きベクトルを求める
	float VecX = sinf(D3DXToRadian(angle_));
	float VecZ = cosf(D3DXToRadian(angle_));

	// ベクトルの反転
	VecX *= -1.0f;
	VecZ *= -1.0f;

	// ベクトルに距離を掛けて長さを変化させる
	float Distance = 200.0f;
	VecX *= Distance;
	VecZ *= Distance;

	// ターゲットの位置にベクトルを加算した値を新しいカメラの座標とする
	float NewCameraPosX = x_ + VecX;
	float NewCameraPosY = y_;
	float NewCameraPosZ = z_ + VecZ;

	D3DXMATRIX Matrix; // カメラ情報を保存するための行列
	D3DXVECTOR3 CameraPos = D3DXVECTOR3(NewCameraPosX, NewCameraPosY, NewCameraPosZ);
	D3DXVECTOR3 EyePos = D3DXVECTOR3( x_, y_, z_);
	D3DXVECTOR3 UpVec = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);

	/*
		D3DXMatrixLookAtLH
			カメラ情報をもつ行列を作成する
	*/
	// L => Left H => Hand LH => 左手系でカメラ座標を設定する
	D3DXMatrixLookAtLH(
		&Matrix,		// 結果保存用行列の指定
		&CameraPos,	// カメラ座標
		&EyePos,		// 注視点座標
		&UpVec			// ワールド座標上におけるカメラの上向きのベクトル
	);

	// SetTransform => 座標変換系の情報をDirectXに通知する
	g_Device->SetTransform(D3DTS_VIEW, &Matrix);
}

void ReleaseXFile()
{	
	for (int i = 0; i < g_XFile.MaterialNum; i++)
	{
		if (g_XFile.Textures[i] != nullptr)
		{
			g_XFile.Textures[i]->Release();
			g_XFile.Textures[i] = nullptr;
		}
	}

	//
	delete[] g_XFile.Textures;

	if (g_XFile.Materials != nullptr)
	{
		g_XFile.Materials->Release();
		g_XFile.Materials = nullptr;
	}
	
	if (g_XFile.Meshes != nullptr)
	{
		g_XFile.Meshes->Release();
		g_XFile.Meshes = nullptr;
	}
}
