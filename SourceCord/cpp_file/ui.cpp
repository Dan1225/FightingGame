#include "../header_file/ui.h"
#include "../Graphics/framework.h"
void UI::Init()
{
	Gauge[0] = std::make_unique<Sprite>(Framework::Instance().GetDevice(), L"Assets//UI//hp.png");

}

void UI::Update()
{

}

void UI::Draw()
{
	Framework& framework = Framework::Instance();
	ID3D11DeviceContext* context = framework.GetDeviceContext();

	ID3D11BlendState* bs = framework.GetBlendState(Blender::BS_ALPHA);
	context->OMSetBlendState(bs, nullptr, 0xFFFFFFFF);

	Gauge[0]->render(context, 10, 20, 800, 80, 0, 0, 400, 40, 0, 1, 1, 1, 1.0);
	Gauge[0]->render(context, 1100, 20, 800, 80, 0, 0, 400, 40, 0, 1, 1, 1, 1.0);

}