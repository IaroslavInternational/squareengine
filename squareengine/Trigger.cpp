#include "Trigger.h"
#include "imgui\imgui.h"
#include "EngineXM.h"
#include "Channels.h"

#include <sstream>

#if IS_ENGINE_MODE

Trigger::Trigger(std::string	   name,
				 Graphics&		   gfx,
				 DirectX::XMFLOAT2 size,
				 DirectX::XMFLOAT3 position,
				 DirectX::XMFLOAT3 orientation,
				 bool			   visibility)
	:
	VisibleObject(name, position, orientation, visibility),
	size(size),
	platform(gfx, size, position, orientation)
{
	full_position.UpdatePoints(position, size);
}
#else

Trigger::Trigger(TriggerStruct& trs)
	:
	Trigger(trs.PosTopLeft, trs.PosTopRight, trs.PosBottomLeft, trs.PosBottomRight, trs.Roll, trs.Pitch, trs.Yaw)
{}

Trigger::Trigger(DirectX::XMFLOAT3 PosTopLeft, DirectX::XMFLOAT3 PosTopRight,
				 DirectX::XMFLOAT3 PosBottomLeft, DirectX::XMFLOAT3 PosBottomRight, 
				 float roll, float pitch, float yaw)
{
	position.PosTopLeft = PosTopLeft;
	position.PosTopRight = PosTopRight;
	position.PosBottomLeft = PosBottomLeft;
	position.PosBottomRight = PosBottomRight;

	triggerOrien.triggerRoll = roll;
	triggerOrien.triggerPitch = pitch;
	triggerOrien.triggerYaw = yaw;
}
#endif // IS_ENGINE_MODE

#if IS_ENGINE_MODE
void Trigger::LinkTechniques(Rgph::RenderGraph& rg)
{
	platform.LinkTechniques(rg);
}

void Trigger::Submit(size_t channels)
{
	platform.Submit(channels);
}

void Trigger::SpawnControl()
{
	bool rotDirty = false;
	bool posDirty = false;
	bool szDirty = false;
	bool dDirty = false;

	const auto dcheck = [](bool d, bool& carry) { carry = carry || d; };

	if (ImGui::BeginChild("Триггер"))
	{
		ImGui::Text("Позиция");
		dcheck(ImGui::SliderFloat("X", &position.x, -80.0f, 80.0f, "%.2f"), posDirty);
		dcheck(ImGui::SliderFloat("Y", &position.y, -80.0f, 80.0f, "%.2f"), posDirty);
		dcheck(ImGui::SliderFloat("Z", &position.z, -80.0f, 80.0f, "%.2f"), posDirty);

		ImGui::Text("Размер");
		dcheck(ImGui::SliderFloat("Ширина", &size.x, 1.0f, 80.0f, "%.2f"), szDirty);
		dcheck(ImGui::SliderFloat("Высота", &size.y, 1.0f, 80.0f, "%.2f"), szDirty);

		ImGui::Text("Глубина:");
		dcheck(ImGui::SliderFloat("", &deep, 1.0f, 5.0f, "%.2f"), dDirty);

		platform.UpdateSize(size);

		full_position.UpdatePoints(position, platform.GetSize());
		platform.SetPosition(position);
	}
}

void Trigger::SetPosition(DirectX::XMFLOAT3 position)
{
	platform.SetPosition(position);
}

void Trigger::SetOrientation(DirectX::XMFLOAT3 orientation )
{
	platform.SetRotation(orientation);
}

void Trigger::SetDefault()
{
	full_position.UpdatePoints(position, platform.GetSize());

	platform.SetPosition(position);
	platform.SetRotation(orientation);
}
#endif // IS_ENGINE_MODE

bool Trigger::Check(DirectX::XMFLOAT3 ObjPos)
{
	objpos = ObjPos;
	return (ObjPos.x >=	   this->full_position.PosTopLeft.x
			&& ObjPos.x <= this->full_position.PosTopLeft.x + deep) &&
			ObjPos.y <=	   this->full_position.PosTopLeft.y &&
			ObjPos.z <=	   this->full_position.PosTopLeft.z &&

			(ObjPos.x >=   this->full_position.PosTopRight.x
			&& ObjPos.x <= this->full_position.PosTopRight.x + deep) &&
			ObjPos.y <=    this->full_position.PosTopRight.y &&
			ObjPos.z >=    this->full_position.PosTopRight.z &&

			(ObjPos.x >=   this->full_position.PosBottomLeft.x
			&& ObjPos.x <= this->full_position.PosBottomLeft.x + deep) &&
			ObjPos.y >=    this->full_position.PosBottomLeft.y &&
			ObjPos.z <=    this->full_position.PosBottomLeft.z &&

			(ObjPos.x >=   this->full_position.PosBottomRight.x
			&& ObjPos.x <= this->full_position.PosBottomRight.x + deep) &&
			ObjPos.y >=	   this->full_position.PosBottomRight.y &&
			ObjPos.z >=	   this->full_position.PosBottomRight.z;
}

void Trigger::SetDeep(float TriggerDeep)
{
	deep = TriggerDeep;
}

const DirectX::XMFLOAT3* Trigger::GetPosition() const
{
	DirectX::XMFLOAT3 arr[] = 
	{ 
		full_position.PosTopLeft,	 full_position.PosTopRight,
		full_position.PosBottomLeft, full_position.PosBottomRight 
	};

	return arr;
}
