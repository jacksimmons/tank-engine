#pragma once


namespace Tank
{
	struct UINodeComponent
	{
		virtual ~UINodeComponent() = default;
		virtual void drawUI() = 0;
	};
}
