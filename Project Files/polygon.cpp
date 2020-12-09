
#include "main.h"
#include "renderer.h"
#include "polygon.h"





void CPolygon::Init()
{
	VERTEX_3D vertex[4];

	vertex[0].Position = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	vertex[0].Normal   = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	vertex[0].Diffuse  = D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f );
	vertex[0].TexCoord = D3DXVECTOR2( 0.0f, 0.0f );

	vertex[1].Position = D3DXVECTOR3( 200.0f, 0.0f, 0.0f );
	vertex[1].Normal   = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	vertex[1].Diffuse  = D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f );
	vertex[1].TexCoord = D3DXVECTOR2( 1.0f, 0.0f );

	vertex[2].Position = D3DXVECTOR3( 0.0f, 200.0f, 0.0f );
	vertex[2].Normal   = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	vertex[2].Diffuse  = D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f );
	vertex[2].TexCoord = D3DXVECTOR2( 0.0f, 1.0f );

	vertex[3].Position = D3DXVECTOR3( 200.0f, 200.0f, 0.0f );
	vertex[3].Normal   = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	vertex[3].Diffuse  = D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f );
	vertex[3].TexCoord = D3DXVECTOR2( 1.0f, 1.0f );


	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof(bd) );
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( VERTEX_3D ) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory( &sd, sizeof(sd) );
	sd.pSysMem = vertex;

	CRenderer::GetDevice()->CreateBuffer( &bd, &sd, &m_VertexBuffer );


	// �e�N�X�`?�ǂ�?��
	D3DX11CreateShaderResourceViewFromFile( CRenderer::GetDevice(),
											"asset/texture/number.png",
											NULL,
											NULL,
											&m_Texture,
											NULL );
	assert(m_Texture);

}


void CPolygon::Uninit()
{

	m_VertexBuffer->Release();
	m_Texture->Release();

}


void CPolygon::Update()
{


}


void CPolygon::Draw()
{
	// ?�C�g����
	LIGHT light;
	light.Enable = false;
	CRenderer::SetLight(light);


	// �}�g?�N�X�ݒ�
	CRenderer::SetWorldViewProjection2D();


	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof( VERTEX_3D );
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers( 0, 1, &m_VertexBuffer, &stride, &offset );

	// �}�e?�A?�ݒ�
	MATERIAL material;
	ZeroMemory( &material, sizeof(material) );
	material.Diffuse = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
	CRenderer::SetMaterial( material );

	// �e�N�X�`?�ݒ�
	CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	// �v?�~�e�B�u�g�|?�W�ݒ�
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

	// �|?�S?�`��
	CRenderer::GetDeviceContext()->Draw( 4, 0 );

}