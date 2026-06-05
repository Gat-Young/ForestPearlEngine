#include "Triangle.h"

Triangle::Triangle()
{
	//»ï°¢ÇüÀ» À§ÇÑ 3°³ÀÇ Á¤Á¡ ¼±¾ð. : Á¤Á¡ÀÇ ÁÂÇ¥°ªÀº È­¸é(Screen)ÁÂÇ¥.
	//Face 0 : Á¤»ï°¢Çü.(CW) 
	// ÁÂÇ¥ (x, y)    »ö»ó( a, r, g, b)   a, °ð Alpha ´Â ±âº»°ª 255 (1.0f)
	Mesh.push_back({ 50.0f, 250.0f, 0.5f, 1.0f, 0xffff0000 });		//v0, Red.	¡Ú
	Mesh.push_back({ 150.0f,  50.0f, 0.5f, 1.0f, 0xff00ff00 });		//v1, Green ¡Ú
	Mesh.push_back({ 250.0f, 250.0f, 0.5f, 1.0f, 0xff00ffff });		//v2, Light-Blue ¡Ú

	//Face 1 : ¿ª»ï°¢Çü.(CCW) 
	Mesh.push_back({ 50.0f, 250.0f, 0.5f, 1.0f, 0xffff0000 });
	Mesh.push_back({ 150.0f, 450.0f, 0.5f, 1.0f, 0xff00ff00 });
	Mesh.push_back({ 250.0f, 250.0f, 0.5f, 1.0f, 0xff00ffff });

	//Face 2: ºø°¢ »ï°¢Çü (CW) Å×½ºÆ® »ï°¢Çü 
	Mesh.push_back({ 300.0f, 500.0f, 0.5f, 1.0f, 0xffff0000 });
	Mesh.push_back({ 400.0f, 300.0f, 0.5f, 1.0f, 0xff00ff00 });
	Mesh.push_back({ 480.0f, 430.0f, 0.5f, 1.0f, 0xff00ffff });

	//Face 3: ºø°¢ »ï°¢Çü (CCW) Å×½ºÆ® »ï°¢Çü 
	Mesh.push_back({ 500.0f, 430.0f, 0.5f, 1.0f, 0xffff0000 });
	Mesh.push_back({ 680.0f, 500.0f, 0.5f, 1.0f, 0xff00ff00 });
	Mesh.push_back({ 600.0f, 300.0f, 0.5f, 1.0f, 0xff00ffff });

	//Face 4 : Á÷°¢ »ï°¢2 (CW)
	Mesh.push_back({ 40.0f,  30.0f, 0.5f, 1.0f, 0xffff0000 });
	Mesh.push_back({ 90.0f,  30.0f, 0.5f, 1.0f, 0xff00ff00 });
	Mesh.push_back({ 90.0f, 100.0f, 0.5f, 1.0f, 0xff00ffff });

	//Face 5 : Á÷°¢ »ï°¢1 (CCW)
	Mesh.push_back({ 10.0f,  30.0f, 0.5f, 1.0f, 0xffff0000 });
	Mesh.push_back({ 10.0f, 100.0f, 0.5f, 1.0f, 0xff00ff00 });
	Mesh.push_back({ 60.0f, 100.0f, 0.5f, 1.0f, 0xff00ffff });
}

void Triangle::BeginPlay()
{
}

void Triangle::Tick()
{
}
