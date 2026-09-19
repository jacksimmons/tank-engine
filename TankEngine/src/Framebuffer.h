#pragma once


namespace Tank
{
	class TANK_API Framebuffer
	{
	private:
		/// <summary>
		/// The viewport size when rendering to the view framebuffer.
		/// </summary>
		int m_w, m_h;

		unsigned m_fbo;
		unsigned m_rbo;
		unsigned m_texColBuf;
	public:
		Framebuffer(int w, int h);
		~Framebuffer();
		void checkStatus() const;
		constexpr unsigned getTexColBuf() noexcept { return m_texColBuf; }
		constexpr int getW() noexcept { return m_w; }
		constexpr int getH() noexcept { return m_h; }
		void update() const;
		void rescale(int w, int h);
	};
}