#pragma once


namespace Tank::Editor::Schema
{
	template <class T>
	using OnModified = std::function<void(const T&)>;
}