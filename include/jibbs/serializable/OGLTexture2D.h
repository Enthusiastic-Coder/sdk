#pragma once

#include "Serializable.h"
#include <comdef.h>
#include <comutil.h>

#define max(a,b) ((a) > (b) ? a : b)
#define min(a,b) ((a) < (b) ? a : b)

#include <gdiplus.h>
#pragma comment (lib, "gdiplus.lib")

#include <string>


class AutoGraphics
{
public:
	AutoGraphics()
	{
		Gdiplus::GdiplusStartupInput gdiplusStartupInput;

		Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, 0);
	}

	~AutoGraphics()
	{
		Gdiplus::GdiplusShutdown(gdiplusToken);
	}

private:

	ULONG_PTR gdiplusToken = 0;
};

inline int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;

	Gdiplus::GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;  // Failure

	pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(num * sizeof(Gdiplus::ImageCodecInfo)));
	if (pImageCodecInfo == NULL)
		return -1;  // Failure

	Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}
	}

	free(pImageCodecInfo);
	return -1;  // Failure
}

class OpenGLTexture2D : public ISerializable
{
public:
	SERIALIZE_WRITE_BEGIN(1, 0)
		SERIALIZE_WRITE_ENTRY(_filename)
		SERIALIZE_WRITE_ENTRY(_minification)
		SERIALIZE_WRITE_ENTRY(_wrapMode)
		SERIALIZE_WRITE_END()

		SERIALIZE_READ_BEGIN(1, 0)
		std::string sFilename;
	SERIALIZE_READ_ENTRY(sFilename)
		SERIALIZE_READ_ENTRY(_minification)
		SERIALIZE_READ_ENTRY(_wrapMode)
		Load(sFilename);
	SERIALIZE_READ_END()

	~OpenGLTexture2D(void)
	{
		glDeleteTextures(1, &_texture_id);
		_texture_id = 0;
		_filename = "";
	}

	void setMinification(int i)
	{
		_minification = i;
	}

	void setMagnification(int i)
	{
		_magnification = i;
	}

	void setWrapMode(int i)
	{
		_wrapMode = i;
	}

	bool generate(int width, int height, bool isDepth)
	{
		assert(_texture_id == 0);
		
		glGenTextures(1, &_texture_id);

		_width = width;
		_height = height;

		bind();

		if (isDepth)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _magnification);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _minification);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _wrapMode);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _wrapMode);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_FLOAT, 0);
		}

		glBindTexture(GL_TEXTURE_2D, 0);

		return glGetError() == GL_NO_ERROR;
	}

	bool Load(std::string sfilename)
	{
		_filename = sfilename;
		_texture_id = LoadGLTexture(_filename.c_str());
		return _texture_id !=0;
	}

	GLuint id() const
	{
		return _texture_id;
	}

	operator GLuint()
	{
		return _texture_id;
	}

	operator bool()
	{
		return _texture_id != 0;
	}

	void bind() const
	{
		glBindTexture(GL_TEXTURE_2D, _texture_id);
	}

	const std::string& filename() const
	{
		return _filename;
	}
	
	int width()
	{
		return _width;
	}

	int height()
	{
		return _height;
	}

protected:

	GLuint LoadGLTexture(const char *pszFilename)
	{
		AutoGraphics _gdi;
		// GDI+ requires unicode strings.
		// This trick only works for strings containing English ASCII text.
		std::string asciiFilename(pszFilename);
		std::wstring unicodeFilename(asciiFilename.length(), L' ');
		std::copy(asciiFilename.begin(), asciiFilename.end(), unicodeFilename.begin());

		std::shared_ptr<Gdiplus::Bitmap> pBitmap(Gdiplus::Bitmap::FromFile(unicodeFilename.c_str(), false));

		if (pBitmap.get() == 0)
			return -1;

		// GDI+ orients bitmap images top-bottom.
		// OpenGL expects bitmap images to be oriented bottom-top by default.
		pBitmap->RotateFlip(Gdiplus::RotateNoneFlipY);

		// GDI+ pads each scanline of the loaded bitmap image to 4-byte memory
		// boundaries. Fortunately OpenGL also aligns bitmap images to 4-byte
		// memory boundaries by default.
		int width = pBitmap->GetWidth();
		int height = pBitmap->GetHeight();
		int pitch = ((width * 32 + 31) & ~31) >> 3;

		std::vector<unsigned char> pixels(pitch * height);
		Gdiplus::BitmapData data;
		Gdiplus::Rect rect(0, 0, width, height);

		// Convert to 32-bit BGRA pixel format and fetch the pixel data.

		Gdiplus::Status status = pBitmap->LockBits(&rect, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &data);

		if (status != Gdiplus::Ok)
			return false;

		if (data.Stride == pitch)
		{
			memcpy(&pixels[0], data.Scan0, pitch * height);
		}
		else
		{
			unsigned char *pSrcPixels = static_cast<unsigned char *>(data.Scan0);

			for (int i = 0; i < height; ++i)
				memcpy(&pixels[i * pitch], &pSrcPixels[i * data.Stride], pitch);
		}

		pBitmap->UnlockBits(&data);

		// Create an OpenGL texture object to store the loaded bitmap image.

		if (!generate(width, height, false))
			return 0;

		glBindTexture(GL_TEXTURE_2D, _texture_id);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, &pixels[0]);
#ifdef glGenerateMipmap
		glGenerateMipmap(GL_TEXTURE_2D);
#endif
		glBindTexture(GL_TEXTURE_2D, 0);
		return _texture_id;
	}

private:
	GLuint _texture_id = 0;
	std::string _filename;
	int _wrapMode = GL_REPEAT;
	int _minification = GL_LINEAR;
	int _magnification = GL_LINEAR;
	int _width = 0;
	int _height = 0;
};

#undef max
#undef min
